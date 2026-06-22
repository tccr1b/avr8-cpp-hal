
#ifndef WATCHDOG_TIMER_HPP
#define WATCHDOG_TIMER_HPP

#include <avr/interrupt.h>
#include "registers.hpp"
#include "macros.hpp"

using namespace mcu;

enum class WatchdogTimeout : uint8_t{
    _16ms  = 0x00,                       //0b1101 1000 WDP = ..0. .000
    _32ms  = RegBits::Core::WDTCSR_WDP0, //0b1101 1000 WDP = ..0. .001
    _64ms  = RegBits::Core::WDTCSR_WDP1, //0b1101 1000 WDP = ..0. .010
    _125ms = RegBits::Core::WDTCSR_WDP0 | 
             RegBits::Core::WDTCSR_WDP1, //0b1101 1000 WDP = ..0. .011
    _250ms = RegBits::Core::WDTCSR_WDP2, //0b1101 1000 WDP = ..0. .100
    _500ms = RegBits::Core::WDTCSR_WDP0 | 
             RegBits::Core::WDTCSR_WDP2, //0b1101 1000 WDP = ..0. .101
    _1sec  = RegBits::Core::WDTCSR_WDP1 | 
             RegBits::Core::WDTCSR_WDP2, //0b1101 1000 WDP = ..0. .110
    _2sec  = RegBits::Core::WDTCSR_WDP0 | 
             RegBits::Core::WDTCSR_WDP1 | 
             RegBits::Core::WDTCSR_WDP2, //0b1101 1000 WDP = ..0. .111
    _4sec  = RegBits::Core::WDTCSR_WDP3, //0b1101 1000 WDP = ..1. .000 (WDP3=1, WDP2:0=000)
    _8sec  = RegBits::Core::WDTCSR_WDP3 | 
             RegBits::Core::WDTCSR_WDP0  //0b1101 1000 WDP = ..1. .001 (WDP3=1, WDP2:0=001)
};
enum class WatchdogMode : uint8_t{
    Stop                    = 0x00,
    Interrupt               = RegBits::Core::WDTCSR_WDIE,
    SystemReset             = RegBits::Core::WDTCSR_WDE,
    InterruptAndSystemReset = RegBits::Core::WDTCSR_WDE | 
                              RegBits::Core::WDTCSR_WDIE,
};
namespace mcu{
namespace System{
    class WatchdogTimer{
        private:
            inline static WatchdogMode currentWdtMode;
            using Callback = void(*)();
            inline static Callback cbInterruptCallback = nullptr;
            inline static WatchdogTimeout wdTimeout = WatchdogTimeout::_8sec;
            inline static WatchdogMode wdMode = WatchdogMode::SystemReset;
        public:
            struct{
                void attach(Callback cb) {cbInterruptCallback = cb;}
                void detach() {cbInterruptCallback = nullptr;}
            private:
                static void __attribute__((flatten, signal(WDT_vect_num))) irqHandler(){
                    if(cbInterruptCallback) cbInterruptCallback();
                }
            }static Interrupt;
            static void setTimeOut(WatchdogTimeout wdtTimeout){
                /* Prepare new values for writing fast as much as possible*/
                uint8_t newTimeoutValue = RegBits::Core::WDTCSR_WDE | static_cast<uint8_t>(wdtTimeout);
                uint8_t changeEnableVal = RegBits::Core::WDTCSR_WDE | RegBits::Core::WDTCSR_WDCE;

                /* Store status register content and turn off global interrupt */
                uint8_t oldSREG = Regs::Core::StatusReg;
                cli();

                /* Reset watchdog timer*/
                reset();

                /*Start timed sequence*/
                /*1. In the same operation, write a logic one to the watchdog change enable bit (WDCE) and WDE. 
                A logic one must be written to WDE regardless of the previous value of the WDE bit.*/
                Regs::Core::WatchdogTimerControlReg.setValue(changeEnableVal);
                /*2. Within the next four clock cycles, write the WDE and watchdog prescaler bits (WDP) as desired, 
                but with the WDCE bit cleared. This must be done in one operation.*/
                Regs::Core::WatchdogTimerControlReg.setValue(newTimeoutValue);

                /* Restore Status Register content (SREG_I bit is also set)*/
                Regs::Core::StatusReg.setValue(oldSREG);
            }
            static void enable(WatchdogMode wdtMode, WatchdogTimeout wdtTimeout){
                wdTimeout = wdtTimeout;
                wdMode    = wdtMode;
                setTimeOut(wdtTimeout);
                setMode(wdtMode);
            }
            static void enable(){
                setTimeOut(wdTimeout);
                setMode(wdMode);
            }
            static void setMode(WatchdogMode wdtMode){
                uint8_t modeBits = static_cast<uint8_t>(wdtMode);
                currentWdtMode = wdtMode;
                /* Save prescaler bits*/
                uint8_t prescalerBitsMask = RegBits::Core::WDTCSR_WDP0 |
                                            RegBits::Core::WDTCSR_WDP1 | 
                                            RegBits::Core::WDTCSR_WDP2 |
                                            RegBits::Core::WDTCSR_WDP3;
                /* Get current prescaler*/
                prescalerBitsMask &= Regs::Core::WatchdogTimerControlReg;
                
                /* Store status register content and disable global interrupt*/
                uint8_t oldSREG = Regs::Core::StatusReg;
                cli();
                
                /* Reset watchdog timer*/
                reset();
                
                /* Enable writing to register by preserving other bits*/
                Regs::Core::WatchdogTimerControlReg.setValue(RegBits::Core::WDTCSR_WDE | 
                                                               RegBits::Core::WDTCSR_WDCE);
                /* Set new mode bits with preserved prescaler bits*/
                Regs::Core::WatchdogTimerControlReg.setValue(modeBits | prescalerBitsMask);
                
                /* Restore Status Register content*/
                Regs::Core::StatusReg.setValue(oldSREG);
            }
            static WatchdogMode getMode(){return currentWdtMode;}
            static void disable(){
                /*Store Status register content and turn off global interrupt*/
                uint8_t oldSREG = Regs::Core::StatusReg;
                cli();
                
                /*Reset Watchdog Timer*/
                reset();
                
                /*Clear WDRF in MCUSR*/
                Regs::Core::McuStatusReg.clearBitmask(RegBits::Core::MCUSR_WDRF);
                
                /*Write logical one to WDCE and WDE
                Keep old prescaler setting to prevent unintentional time-out*/
                Regs::Core::WatchdogTimerControlReg.setValue(RegBits::Core::WDTCSR_WDE | RegBits::Core::WDTCSR_WDCE);
                /*Turn off WDT*/
                Regs::Core::WatchdogTimerControlReg.setValue(0x00);
                
                /* Restore Status Register*/
                Regs::Core::StatusReg.setValue(oldSREG);
            }
            [[gnu::always_inline]] static inline void reset(){__asm__ __volatile__("wdr");}
            static bool isActive(){return mcu::Regs::Core::WatchdogTimerControlReg.readBit(mcu::RegBits::Core::WDTCSR_WDE);}
    };
} //namespace System
} //namespace mcu

#endif //WATCHDOG_TIMER_HPP