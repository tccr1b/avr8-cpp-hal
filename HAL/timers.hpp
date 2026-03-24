
#ifndef TIMERS_HPP
#define TIMERS_HPP

#define __AVR_ATmega328P__ // Bunu sil

#define BITMASK_TIMERCLOCK   0xF8

#include "registers.hpp"
#include "gpio.hpp"
#include <avr/interrupt.h>

using namespace mcu;


namespace mcu{
namespace Timers{
    class Timer0{
    public:
        enum class Mode : uint8_t{
            Normal                  = 0,
            ClearTimerOnCompareMatch= 1,
            FastPWM                 = 2,
            FastPWM_WithOCR         = 3,
            PhaseCorrectPWM         = 4,
            PhaseCorrectPWM_WithOCR = 5,
        };
        enum class Clock : uint8_t{
            Stop                 = 0x00,                                  //0x00 ...000
            NoPrescaling         = RegBits::Timers::Timer0::TCCR0B_CS00,  //0x01 ...001
            DividedBy8          = RegBits::Timers::Timer0::TCCR0B_CS01,  //0x02 ...010
            DividedBy64         = RegBits::Timers::Timer0::TCCR0B_CS00 | 
                                RegBits::Timers::Timer0::TCCR0B_CS01,  //0x03 ...011
            DividedBy256        = RegBits::Timers::Timer0::TCCR0B_CS02,  //0x04 ...100
            DividedBy1024       = RegBits::Timers::Timer0::TCCR0B_CS00 | 
                                RegBits::Timers::Timer0::TCCR0B_CS02,  //0x05 ...101
            ExternalOnFallingEdge= RegBits::Timers::Timer0::TCCR0B_CS01 |
                                RegBits::Timers::Timer0::TCCR0B_CS02,  //0x06 ...110
            ExternalOnRisingEdge = RegBits::Timers::Timer0::TCCR0B_CS00 |
                                RegBits::Timers::Timer0::TCCR0B_CS01 |
                                RegBits::Timers::Timer0::TCCR0B_CS02,  //0x07 ...111
        };
        enum class OutputMode : uint8_t{
            Disconnected         = 0x00,
            ToggleOnCompareMatch = 0x01,
            ClearOnCompareMatch  = 0x02,
            SetOnCompareMatch    = 0x03,
        };
        enum class InterruptType : uint8_t{
            OutputCompareMatchA = RegBits::Timers::Timer0::TIMSK0_OCIE0A,
            OutputCompareMatchB = RegBits::Timers::Timer0::TIMSK0_OCIE0B,
            Overflow            = RegBits::Timers::Timer0::TIMSK0_TOIE0,
            All                 = RegBits::Timers::Timer0::TIMSK0_OCIE0A |
                                RegBits::Timers::Timer0::TIMSK0_OCIE0B |
                                RegBits::Timers::Timer0::TIMSK0_TOIE0,
        };
    private:
        // Callback için fonksiyon tipi
        using Callback = void(*)();
        inline static Callback overflowCallback      = nullptr; //cbOverflow
        inline static Callback compareMatchACallback = nullptr; //cbCompareMatchA
        inline static Callback compareMatchBCallback = nullptr; //cbCompareMatchB
        static void setMode(Timer0::Mode timerMode){
            setClock(Timer0::Clock::Stop);
            Regs::Timers::Timer0::TimerCounterControlRegA.clearBitmask(RegBits::Timers::Timer0::TCCR0A_WGM00 | 
                                                                       RegBits::Timers::Timer0::TCCR0A_WGM01);
            Regs::Timers::Timer0::TimerCounterControlRegB.clearBitmask(RegBits::Timers::Timer0::TCCR0B_WGM02);
            switch (timerMode){
            case Timer0::Mode::Normal:
                /*Bits WGM00, WGM01, WGM02 are cleared already*/
                break;
            case Timer0::Mode::ClearTimerOnCompareMatch:
                Regs::Timers::Timer0::TimerCounterControlRegA.setBitmask(RegBits::Timers::Timer0::TCCR0A_WGM01);
                break;
            case Timer0::Mode::FastPWM:
                Regs::Timers::Timer0::TimerCounterControlRegA.setBitmask(RegBits::Timers::Timer0::TCCR0A_WGM00 |
                                                                         RegBits::Timers::Timer0::TCCR0A_WGM01);
                break;
            case Timer0::Mode::FastPWM_WithOCR:
                Regs::Timers::Timer0::TimerCounterControlRegA.setBitmask(RegBits::Timers::Timer0::TCCR0A_WGM00 |
                                                                         RegBits::Timers::Timer0::TCCR0A_WGM01);
                Regs::Timers::Timer0::TimerCounterControlRegB.setBitmask(RegBits::Timers::Timer0::TCCR0B_WGM02);
                break;
            case Timer0::Mode::PhaseCorrectPWM:
                Regs::Timers::Timer0::TimerCounterControlRegA.setBitmask(RegBits::Timers::Timer0::TCCR0A_WGM00);
                break;
            case Timer0::Mode::PhaseCorrectPWM_WithOCR:
                Regs::Timers::Timer0::TimerCounterControlRegA.setBitmask(RegBits::Timers::Timer0::TCCR0A_WGM00);
                Regs::Timers::Timer0::TimerCounterControlRegB.setBitmask(RegBits::Timers::Timer0::TCCR0B_WGM02);
                break;
            }
        }
    public:
        static void setClock(Timer0::Clock tmrClock = Timer0::Clock::NoPrescaling){
            constexpr uint8_t bitmask_tmr0_clk = ~(RegBits::Timers::Timer0::TCCR0B_CS00 | 
                                                   RegBits::Timers::Timer0::TCCR0B_CS01 | 
                                                   RegBits::Timers::Timer0::TCCR0B_CS02);
            /*Save current tccrb*/
            uint8_t currentTCCRB = Regs::Timers::Timer0::TimerCounterControlRegB.getValue();
            /*Set new clock/prescaler settings*/
            currentTCCRB &= (bitmask_tmr0_clk | static_cast<uint8_t>(tmrClock));
            /*Update tccrb register*/
            Regs::Timers::Timer0::TimerCounterControlRegB.setValue(currentTCCRB);
        }
        static void init(Timer0::Mode   tmrMode = Timer0::Mode::Normal, 
                         Timer0::Clock tmrClock = Timer0::Clock::DividedBy1024){
            /* Disable power reduction for timer0 */
            mcu::Regs::Core::PowerReductionReg.clearBitmask(RegBits::Core::PRR_PRTIM0);
            /* Reset registers: TCCR0A, TCCR0B, TCNT0*/                
            Regs::Timers::Timer0::TimerCounterControlRegA.setValue(0x00);
            Regs::Timers::Timer0::TimerCounterControlRegB.setValue(0x00);
            Regs::Timers::Timer0::TimerCounterReg.setValue(0x00);
            
            setMode(tmrMode);
            setClock(tmrClock);
//            HAL::GpioPin<mcu::Gpio::GpioPortD,
//                         mcu::RegBits::Gpio::PortD::PD_OC0A>pwmPin;
            using pwmPin = mcu::Gpio::PinPD0;
            
        }
        static void resetCounter(){
            Regs::Timers::Timer0::TimerCounterReg.setValue(0x00);
        }
        static uint8_t getCounterValue(){
            return Regs::Timers::Timer0::TimerCounterReg.getValue();
        }
        static void setCompareValueA(uint8_t topValue){
            Regs::Timers::Timer0::OutputCompareRegA.setValue(topValue);
        }
        static void setCompareValueB(uint8_t topValue){
            Regs::Timers::Timer0::OutputCompareRegB.setValue(topValue);
        }
        static uint8_t getCompareValueA(){
            return Regs::Timers::Timer0::OutputCompareRegA.getValue();
        }
        static uint8_t getCompareValueB(){
            return Regs::Timers::Timer0::OutputCompareRegB.getValue();
        }
        static void setOutputModeA(Timer0::OutputMode outputMode = Timer0::OutputMode::Disconnected){
            /* Bitmask for output mode A*/
            constexpr uint8_t bitmask_tmr0_out_mode = ~(RegBits::Timers::Timer0::TCCR0A_COM0A0 | 
                                                        RegBits::Timers::Timer0::TCCR0A_COM0A1);
            /* Copy content of tccra register and make all the changes on copy of it*/
            uint8_t currentTCCRA = Regs::Timers::Timer0::TimerCounterControlRegA;
            /* Assign new output mode to copied value*/
            currentTCCRA &= (bitmask_tmr0_out_mode | (static_cast<uint8_t>(outputMode) << 6));
            /*Update tccra register*/
            Regs::Timers::Timer0::TimerCounterControlRegA.setValue(currentTCCRA);
        };
        static void setOutputModeB(Timer0::OutputMode outputMode = Timer0::OutputMode::Disconnected){
            uint8_t currentTCCRA = Regs::Timers::Timer0::TimerCounterControlRegA;
            currentTCCRA &= ~(RegBits::Timers::Timer0::TCCR0A_COM0B0 | RegBits::Timers::Timer0::TCCR0A_COM0B1);
            currentTCCRA |= (static_cast<uint8_t>(outputMode) << 4);
            Regs::Timers::Timer0::TimerCounterControlRegA.setValue(currentTCCRA);
        }
        static void attachInterrupt(InterruptType intType, Callback callbackFunc){
            switch (intType){
            case InterruptType::Overflow:
                overflowCallback = callbackFunc;
                break;
            case InterruptType::OutputCompareMatchA:
                compareMatchACallback = callbackFunc;
                break;
            case InterruptType::OutputCompareMatchB:
                compareMatchBCallback = callbackFunc;
                break;
            case InterruptType::All:
                break;
            default:
                break;
            }
            enableInterrupt(intType);
        }
        static void detachInterrupt(InterruptType intType){
            switch (intType){
            case InterruptType::Overflow:
                disableInterrupt(InterruptType::Overflow);
                overflowCallback = nullptr;
                break;
            case InterruptType::OutputCompareMatchA:
                disableInterrupt(InterruptType::OutputCompareMatchA);
                compareMatchACallback = nullptr;
                break;
            case InterruptType::OutputCompareMatchB:
                disableInterrupt(InterruptType::OutputCompareMatchB);
                compareMatchBCallback = nullptr;
                break;
            case InterruptType::All:
                break;
            default:
                break;
            }
        }
        static void interruptHandler(InterruptType intType){
            switch (intType){
            case InterruptType::Overflow: if(overflowCallback) overflowCallback(); break;
            case InterruptType::OutputCompareMatchA: if(compareMatchACallback) compareMatchACallback(); break;
            case InterruptType::OutputCompareMatchB: if(compareMatchBCallback) compareMatchBCallback(); break;
            }
        }
        static void enableOutputComparePinA(){
            mcu::Gpio::PinOC0A::setPinMode(HAL::PinMode::Output);
        }
        static void enableOutputComparePinB(){
            mcu::Gpio::PinOC0B::setPinMode(HAL::PinMode::Output);
        }
        static void enableInterrupt(InterruptType intType){
            Regs::Timers::Timer0::TimerInterruptMaskReg.setBitmask(static_cast<uint8_t>(intType));
        }
        static void disableInterrupt(InterruptType intType){
            Regs::Timers::Timer0::TimerInterruptMaskReg.clearBitmask(static_cast<uint8_t>(intType));
        }
        static void forceOutputCompareA(){
            if(Regs::Timers::Timer0::TimerCounterControlRegA.readBit(RegBits::Timers::Timer0::TCCR0A_WGM00)) return;
            Regs::Timers::Timer0::TimerCounterControlRegB.setBitmask(RegBits::Timers::Timer0::TCCR0B_FOC0A);
        }
        static void forceOutputCompareB(){
            if(Regs::Timers::Timer0::TimerCounterControlRegA.readBit(RegBits::Timers::Timer0::TCCR0A_WGM00)) return;
            Regs::Timers::Timer0::TimerCounterControlRegB.setBitmask(RegBits::Timers::Timer0::TCCR0B_FOC0B);
        }
    };
} // namespace Timers    
} // namespace mcu


/* Timer Overflow Interrupt*/
ISR(TIMER0_OVF_vect){mcu::Timers::Timer0::interruptHandler(mcu::Timers::Timer0::InterruptType::Overflow);}
/* Output compare match A interrupt*/
ISR(TIMER0_COMPA_vect){mcu::Timers::Timer0::interruptHandler(mcu::Timers::Timer0::InterruptType::OutputCompareMatchA);}
/* Output compare match B interrupt*/
ISR(TIMER0_COMPB_vect){mcu::Timers::Timer0::interruptHandler(mcu::Timers::Timer0::InterruptType::OutputCompareMatchB);}
#endif //TIMERS_HPP