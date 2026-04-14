
#ifndef TIMERS_HPP
#define TIMERS_HPP

#define __AVR_ATmega328P__ // Bunu sil
/*
    1) Hassas Zaman Gecikmeleri (Delay) Oluşturma
        Belirlenen süre dolduğunda işlemciye "zaman doldu" sinyali (Interrupt) gönderilir.
    2) Darbe Genişlik Modülasyonu (PWM) Üretimi
        Timer'lar, dijital sinyallerin "açık" ve "kapalı" kalma sürelerini 
        milisaniyeler seviyesinde kontrol ederek PWM sinyalleri üretir.
    3) Zaman Aralıklı Görevler (Periodic Tasks)
        Sistemin belirli aralıklarla (örneğin her 10 milisaniyede bir) bir sensörden veri okuması veya bir durumu kontrol 
        etmesi gerekiyorsa Timer kullanılır. Bu, ana döngüden (loop veya while) bağımsız olarak tam zamanında gerçekleşen 
        "saat gibi çalışan" görevler sağlar.
    4) Giriş Sinyali Yakalama (Input Capture)
        Dışarıdan gelen bir sinyalin iki yükselen kenarı arasındaki süreyi ölçmek için kullanılır. Bu yöntemle:
            * Bir sinyalin frekansı ölçülebilir.
            * Ultrasonik mesafe sensörlerinden gelen yankı süresi hesaplanarak mesafe ölçülebilir.
            * Bir motorun devir sayısı (RPM) hesaplanabilir.
    5) Olay Sayma (External Event Counter)
        Timer, işlemcinin kendi iç saatini saymak yerine dışarıdaki bir pin üzerinden gelen sinyalleri sayacak şekilde 
        yapılandırılabilir. Örneğin, bir banttan geçen ürünlerin sayısını sayan bir sensör doğrudan Timer girişine 
        bağlanabilir.

*/

#define BITPOS_COM2A 6
#define BITPOS_COM2B 4

#include "registers.hpp"
#include "gpio.hpp"
#include <avr/interrupt.h>
#include "HAL/utils/atomicblock.hpp"

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
            Stopped              = 0x00,                                  //0x00 ...000
            NoPrescaling         = RegBits::Timers::Timer0::TCCR0B_CS00,  //0x01 ...001
            DividedBy8           = RegBits::Timers::Timer0::TCCR0B_CS01,  //0x02 ...010
            DividedBy64          = RegBits::Timers::Timer0::TCCR0B_CS00 | 
                                     RegBits::Timers::Timer0::TCCR0B_CS01,  //0x03 ...011
            DividedBy256         = RegBits::Timers::Timer0::TCCR0B_CS02,  //0x04 ...100
            DividedBy1024        = RegBits::Timers::Timer0::TCCR0B_CS00 | 
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
            setClock(Timer0::Clock::Stopped);
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
            Regs::Timers::GeneralTimerCounterControlReg.setBitmask(RegBits::Timers::Common::GTCCR_PSRSYNC);
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
            }
            enableInterrupt(intType);
        }
        static void detachInterrupt(InterruptType intType){
            switch (intType){
                case InterruptType::Overflow            :overflowCallback     = nullptr; break;
                case InterruptType::OutputCompareMatchA :compareMatchACallback= nullptr; break;
                case InterruptType::OutputCompareMatchB :compareMatchBCallback= nullptr; break;
                case InterruptType::All                 :overflowCallback     = nullptr;
                                                         compareMatchACallback= nullptr;
                                                         compareMatchBCallback= nullptr; break;
            }
            disableInterrupt(intType);
        }
        static void interruptHandler(InterruptType intType){
            switch (intType){
                case InterruptType::Overflow           : if(overflowCallback)      overflowCallback(); break;
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

    class Timer1{
    public:
        enum class Mode         : uint8_t{
            Normal                           = 0x00,
            PhaseCorrectPWM_8bit             = RegBits::Timers::Timer1::TCCR1A_WGM10,
            PhaseCorrectPWM_9bit             = RegBits::Timers::Timer1::TCCR1A_WGM11,
            PhaseCorrectPWM_10bit            = RegBits::Timers::Timer1::TCCR1A_WGM11| 
                                               RegBits::Timers::Timer1::TCCR1A_WGM10,
            ClearTimerOnCompareMatch_WithOCR = RegBits::Timers::Timer1::TCCR1B_WGM12,
            FastPWM_8bit                     = RegBits::Timers::Timer1::TCCR1B_WGM12| 
                                               RegBits::Timers::Timer1::TCCR1A_WGM10,
            FastPWM_9bit                     = RegBits::Timers::Timer1::TCCR1B_WGM12| 
                                               RegBits::Timers::Timer1::TCCR1A_WGM11,
            FastPWM_10bit                    = RegBits::Timers::Timer1::TCCR1B_WGM12| 
                                               RegBits::Timers::Timer1::TCCR1A_WGM11| 
                                               RegBits::Timers::Timer1::TCCR1A_WGM10,
            PhaseAndFrequencyCorrect_WithICR = RegBits::Timers::Timer1::TCCR1B_WGM13,
            PhaseAndFrequencyCorrect_WithOCR = RegBits::Timers::Timer1::TCCR1B_WGM13| 
                                               RegBits::Timers::Timer1::TCCR1A_WGM10,
            PhaseCorrectPWM_WithICR          = RegBits::Timers::Timer1::TCCR1B_WGM13| 
                                               RegBits::Timers::Timer1::TCCR1A_WGM11,
            PhaseCorrectPWM_WithOCR          = RegBits::Timers::Timer1::TCCR1B_WGM13| 
                                               RegBits::Timers::Timer1::TCCR1A_WGM11| 
                                               RegBits::Timers::Timer1::TCCR1A_WGM10,
            ClearTimerOnCompareMatch_WithICR = RegBits::Timers::Timer1::TCCR1B_WGM13| 
                                               RegBits::Timers::Timer1::TCCR1B_WGM12,
            FastPWM_WithICR                  = RegBits::Timers::Timer1::TCCR1B_WGM13| 
                                               RegBits::Timers::Timer1::TCCR1B_WGM12| 
                                               RegBits::Timers::Timer1::TCCR1A_WGM11,
            FastPWM_WithOCR                  = RegBits::Timers::Timer1::TCCR1B_WGM13| 
                                               RegBits::Timers::Timer1::TCCR1B_WGM12| 
                                               RegBits::Timers::Timer1::TCCR1A_WGM11| 
                                               RegBits::Timers::Timer1::TCCR1A_WGM10,
        };
        enum class OutputMode   : uint8_t{ //  Value   |   OC1A         |   OC1B
            Disconnected         = 0x00, // ******00 | 00****** (<<6) | **00**** (<<4)
            ToggleOnCompareMatch = 0x01, // ******01 | 01****** (<<6) | **01**** (<<4)
            ClearOnCompareMatch  = 0x02, // ******10 | 10****** (<<6) | **10**** (<<4)
            SetOnCompareMatch    = 0x03, // ******11 | 11****** (<<6) | **11**** (<<4)
        };
        enum class CaptureEdge  : uint8_t{
            Falling =0,
            Rising  =1,
        };
        enum class Clock        : uint8_t{
            Stopped                         = 0x00,
            NoPrescaling                    = RegBits::Timers::Timer1::TCCR1B_CS10,
            DividedBy8                      = RegBits::Timers::Timer1::TCCR1B_CS11,
            DividedBy64                     = RegBits::Timers::Timer1::TCCR1B_CS10|
                                              RegBits::Timers::Timer1::TCCR1B_CS11,
            DividedBy256                    = RegBits::Timers::Timer1::TCCR1B_CS12,
            DividedBy1024                   = RegBits::Timers::Timer1::TCCR1B_CS10|
                                              RegBits::Timers::Timer1::TCCR1B_CS12,
            ExternalSourceOnFallingEdge     = RegBits::Timers::Timer1::TCCR1B_CS11|
                                              RegBits::Timers::Timer1::TCCR1B_CS12,
            ExternalSourceOnRisingingEdge   = RegBits::Timers::Timer1::TCCR1B_CS10|
                                              RegBits::Timers::Timer1::TCCR1B_CS11|
                                              RegBits::Timers::Timer1::TCCR1B_CS12,
        };
        enum class InterruptType: uint8_t{
            InputCapture        = RegBits::Timers::Timer1::TIMSK1_ICIE1,
            OutputCompareMatchB = RegBits::Timers::Timer1::TIMSK1_OCIE1B,
            OutputCompareMatchA = RegBits::Timers::Timer1::TIMSK1_OCIE1A,
            Overflow            = RegBits::Timers::Timer1::TIMSK1_TOIE1,
            All                 = RegBits::Timers::Timer1::TIMSK1_ICIE1|
                                  RegBits::Timers::Timer1::TIMSK1_OCIE1A|
                                  RegBits::Timers::Timer1::TIMSK1_OCIE1B|
                                  RegBits::Timers::Timer1::TIMSK1_TOIE1,
        };

    private:
        using Callback = void(*)();
        inline static Callback cbOverflowCallback = nullptr;
        inline static Callback cbCompareMatchACallback = nullptr;
        inline static Callback cbCompareMatchBCallback = nullptr;
        inline static Callback cbInputCaptureCallback = nullptr;
        
    public:
        static void resetCounter(){
            {AtomicBlock ab;
                /* write high byte (first)*/
                mcu::Regs::Timers::Timer1::TimerCounterReg_HByte = 0x00;
                /* write low byte (last)*/
                mcu::Regs::Timers::Timer1::TimerCounterReg_LByte = 0x00;
            }
        }
        static void setMode(Mode mode){
            /* Bitmask for WGM bits in TCCR1A*/
            constexpr uint8_t bitmask_tccra_wgm_bits = ~(RegBits::Timers::Timer1::TCCR1A_WGM11|
                                                         RegBits::Timers::Timer1::TCCR1A_WGM10);
            /* Bitmask for WGM bits in TCCR1B*/
            constexpr uint8_t bitmask_tccrb_wgm_bits = ~(RegBits::Timers::Timer1::TCCR1B_WGM13|
                                                         RegBits::Timers::Timer1::TCCR1B_WGM12);
            /* Write WGM bits on TCCR1A*/
            Regs::Timers::Timer1::TimerCounterControlRegA.writeMasked(static_cast<uint8_t>(mode), bitmask_tccra_wgm_bits);
            /* Write WGM bits on TCCR1B*/
            Regs::Timers::Timer1::TimerCounterControlRegB.writeMasked(static_cast<uint8_t>(mode), bitmask_tccrb_wgm_bits);
        }
        static void enableNoiseCanceller(){
            mcu::Regs::Timers::Timer1::TimerCounterControlRegB.setBitmask(mcu::RegBits::Timers::Timer1::TCCR1B_ICNC1);
        }
        static void disableNoiseCanceller(){
            mcu::Regs::Timers::Timer1::TimerCounterControlRegB.clearBitmask(mcu::RegBits::Timers::Timer1::TCCR1B_ICNC1);
        }
        static void selectInputCaptureEdge(CaptureEdge edge){
            if(static_cast<bool>(edge)){
                Regs::Timers::Timer1::TimerCounterControlRegB.setBitmask(RegBits::Timers::Timer1::TCCR1B_ICES1);
            }else{
                Regs::Timers::Timer1::TimerCounterControlRegB.clearBitmask(RegBits::Timers::Timer1::TCCR1B_ICES1);
            }
        }
        static void setClock(Timer1::Clock clk){
            constexpr uint8_t bitmask_tccrb_clk_bits = ~(RegBits::Timers::Timer1::TCCR1B_CS10|
                                                         RegBits::Timers::Timer1::TCCR1B_CS11|
                                                         RegBits::Timers::Timer1::TCCR1B_CS12);
            Regs::Timers::Timer1::TimerCounterControlRegB.writeMasked(static_cast<uint8_t>(clk), bitmask_tccrb_clk_bits);
        }
        static void forceOutputCompareA(){
            Regs::Timers::Timer1::TimerCounterControlRegC.setBitmask(RegBits::Timers::Timer1::TCCR1C_FOC1A);
        }
        static void forceOutputCompareB(){
            Regs::Timers::Timer1::TimerCounterControlRegC.setBitmask(RegBits::Timers::Timer1::TCCR1C_FOC1B);
        }
        static void enableInterrupt(InterruptType intType){
            Regs::Timers::Timer1::TimerInterruptMaskReg.setBitmask(static_cast<uint8_t>(intType));
        }
        static void disableInterrupt(InterruptType intType){
            Regs::Timers::Timer1::TimerInterruptMaskReg.clearBitmask(static_cast<uint8_t>(intType));
        }
        static void attachInterrupt(InterruptType intType, Callback cbFunc){
            switch(intType){
                case InterruptType::InputCapture:       cbInputCaptureCallback  = cbFunc; break;
                case InterruptType::OutputCompareMatchA:cbCompareMatchACallback = cbFunc; break;
                case InterruptType::OutputCompareMatchB:cbCompareMatchBCallback = cbFunc; break;
                case InterruptType::Overflow:           cbOverflowCallback      = cbFunc; break;
            }
            enableInterrupt(intType);
        }
        static void detachInterrupt(InterruptType intType){
            switch(intType){
                case InterruptType::InputCapture        : cbInputCaptureCallback = nullptr; break;
                case InterruptType::OutputCompareMatchA : cbCompareMatchACallback= nullptr; break;
                case InterruptType::OutputCompareMatchB : cbCompareMatchBCallback= nullptr; break;
                case InterruptType::Overflow            : cbOverflowCallback     = nullptr; break;
                case InterruptType::All                 : cbInputCaptureCallback = nullptr;
                                                          cbCompareMatchACallback= nullptr;
                                                          cbCompareMatchBCallback= nullptr;
                                                          cbOverflowCallback     = nullptr; break;
            }
            disableInterrupt(intType);
        }
        static void interruptHandler(InterruptType intType){
            switch(intType){
                case InterruptType::InputCapture       : if(cbInputCaptureCallback) cbInputCaptureCallback(); break;
                case InterruptType::OutputCompareMatchA: if(cbCompareMatchACallback)cbCompareMatchACallback();break;
                case InterruptType::OutputCompareMatchB: if(cbCompareMatchBCallback)cbCompareMatchBCallback();break;
                case InterruptType::Overflow           : if(cbOverflowCallback)     cbOverflowCallback();     break;
            }
        }
        static void setCompareValueA(uint16_t compareVal){
            {AtomicBlock ab;
                /* write: high byte (first)*/
                mcu::Regs::Timers::Timer1::OutputCompareRegA_HByte.setValue(compareVal >> 8);
                /* write: low byte (last)*/
                mcu::Regs::Timers::Timer1::OutputCompareRegA_LByte.setValue(compareVal);
            }
        }
        static uint16_t getCompareValueA(){
            uint16_t compareVal= 0x0000;
            {AtomicBlock ab;
                /* read low byte first, 0x00*/
                uint8_t lowByte = mcu::Regs::Timers::Timer1::OutputCompareRegA_LByte;
                /* read high byte, 0x--00*/
                compareVal = mcu::Regs::Timers::Timer1::OutputCompareRegA_HByte;
                /* left shift high byte and write low byte, 0x0000*/
                compareVal = (compareVal << 8) | lowByte;
            }
            /* return value*/
            return compareVal;
        }
        static void setCompareValueB(uint16_t compareVal){
            {AtomicBlock ab;
                /* write high byte first*/
                mcu::Regs::Timers::Timer1::OutputCompareRegB_HByte.setValue(compareVal >> 8);
                /* write low byte*/
                mcu::Regs::Timers::Timer1::OutputCompareRegB_LByte.setValue(compareVal);            
            }
        }
        static uint16_t getCompareValueB(){
            uint16_t compareVal = 0x0000;
            {AtomicBlock ab;
                /* read low byte first*/
                uint8_t lowByte = mcu::Regs::Timers::Timer1::OutputCompareRegB_LByte;
                /* read high byte*/
                compareVal= mcu::Regs::Timers::Timer1::OutputCompareRegB_HByte;
                /* left shift high byte*/
                compareVal = (compareVal << 8) | lowByte;
            }
            /* return resulting 16bit uint*/
            return compareVal;
        }
        static void setCounterValue(uint16_t counterVal){
            {AtomicBlock ab;
                /* write high byte (first)*/
                mcu::Regs::Timers::Timer1::TimerCounterReg_HByte = (counterVal >> 8);
                /* write low byte (last)*/
                mcu::Regs::Timers::Timer1::TimerCounterReg_LByte = counterVal;
            }
        }
        static uint16_t getCounterValue(){
            uint16_t counterVal= 0x0000;
            {AtomicBlock ab;
                uint8_t lowByte = mcu::Regs::Timers::Timer1::TimerCounterReg_LByte;
                counterVal      = mcu::Regs::Timers::Timer1::TimerCounterReg_HByte;
                counterVal      = (counterVal << 8) | lowByte;
            }
            return counterVal;
        }
        static void pwmOut(uint32_t pwmFrequency, uint8_t pwmDutyCycle){}
    };
/*
a.Disable the Timer/Counter2 interrupts by clearing OCIE2x and TOIE2.
b.Select clock source by setting AS2 as appropriate.
c.Write new values to TCNT2, OCR2x, and TCCR2x.
d.To switch to asynchronous operation: Wait for TCN2xUB, OCR2xUB, and TCR2xUB.
e.Clear the Timer/Counter2 interrupt flags.
f.Enable interrupts, if needed.
*/
    class Timer2{
    public:
        enum class Mode         : uint8_t{
            Normal                   = 0x00,
            PhaseCorrectPWM          = RegBits::Timers::Timer2::TCCR2A_WGM20,
            ClearTimerOnCompareMatch = RegBits::Timers::Timer2::TCCR2A_WGM21,
            FastPWM                  = RegBits::Timers::Timer2::TCCR2A_WGM21|
                                       RegBits::Timers::Timer2::TCCR2A_WGM20,
            PhaseCorrectPWM_WithOCR  = RegBits::Timers::Timer2::TCCR2B_WGM22|
                                       RegBits::Timers::Timer2::TCCR2A_WGM20,
            FastPWM_WithOCR          = RegBits::Timers::Timer2::TCCR2B_WGM22|
                                       RegBits::Timers::Timer2::TCCR2A_WGM21|
                                       RegBits::Timers::Timer2::TCCR2A_WGM20,
        };
        enum class OutputMode   : uint8_t{
            DisconnectedFromPin  = 0x00,
            ToggleOnCompareMatch = 0x01,
            ClearOnCompareMatch  = 0x02,
            SetOnCompareMatch    = 0x03,
        };
        enum class Clock        : uint8_t{
            Stopped       = 0x00,
            NoPrescaling  = RegBits::Timers::Timer2::TCCR2B_CS20,
            DividedBy8    = RegBits::Timers::Timer2::TCCR2B_CS21,
            DividedBy32   = RegBits::Timers::Timer2::TCCR2B_CS21|
                            RegBits::Timers::Timer2::TCCR2B_CS20,
            DividedBy64   = RegBits::Timers::Timer2::TCCR2B_CS22,
            DividedBy128  = RegBits::Timers::Timer2::TCCR2B_CS22|
                            RegBits::Timers::Timer2::TCCR2B_CS20,
            DividedBy256  = RegBits::Timers::Timer2::TCCR2B_CS22|
                            RegBits::Timers::Timer2::TCCR2B_CS21,
            DividedBy1024 = RegBits::Timers::Timer2::TCCR2B_CS22|
                            RegBits::Timers::Timer2::TCCR2B_CS21|
                            RegBits::Timers::Timer2::TCCR2B_CS20,
        };
        enum class ClockSource  : uint8_t{
            /* Clock signal (square wave 50% duty cycle) is applied on Tosc1 pin. Tosc2 pin is freed.*/
            ExternalClock   = RegBits::Timers::Timer2::ASSR_EXCLK|
                              RegBits::Timers::Timer2::ASSR_AS2,
            /* Clock signal for Timer2 is generated by using crystal connected to Tosc1 and Tosc2 pins.*/
            ExternalCrystal = RegBits::Timers::Timer2::ASSR_AS2,
            /* Timer2 uses internal clock (main clock/system clock)*/
            SystemClock     = 0x00,
        };
        enum class InterruptType: uint8_t{
            OutputCompareMatchB = RegBits::Timers::Timer2::TIMSK2_OCIE2B,
            OutputCompareMatchA = RegBits::Timers::Timer2::TIMSK2_OCIE2A,
            Overflow            = RegBits::Timers::Timer2::TIMSK2_TOIE2,
            All                 = OutputCompareMatchA | OutputCompareMatchB | Overflow,
        };
    private:
        using Callback = void(*)();
        inline static Callback overflowCallback      = nullptr; //cbOverflow
        inline static Callback compareMatchACallback = nullptr; //cbCompareMatchA
        inline static Callback compareMatchBCallback = nullptr; //cbCompareMatchB
        inline static Clock currentClock;
        static ClockSource getClockSource(){
            constexpr static uint8_t bitmask_assr_clksrc_bits =(RegBits::Timers::Timer2::ASSR_EXCLK|
                                                                RegBits::Timers::Timer2::ASSR_AS2);
            return static_cast<ClockSource>(Regs::Timers::Timer2::AsynchronousStatusReg & bitmask_assr_clksrc_bits);
        }
    public:
        inline static void synch(){
            mcu::Regs::Timers::GeneralTimerCounterControlReg.setBitmask(mcu::RegBits::Timers::Common::GTCCR_PSRASY);
        }
        static void clearCounter(){
            mcu::Regs::Timers::Timer2::TimerCounterReg = 0x00;
            if(ClockSource::SystemClock != getClockSource()){
                while(Regs::Timers::Timer2::AsynchronousStatusReg.readBit(RegBits::Timers::Timer2::ASSR_TCN2UB));
            }
        }
        static void resetCounter(){
            clearCounter();
            synch();
        }
        static void disable(){
            setClockPrescaler(Clock::Stopped);
        }
        static void enable(){
            mcu::System::Power::activatePeripheral(Peripheral::Timer2);
            if(currentClock != Clock::Stopped) setClockPrescaler(currentClock);
        }
        static void setClockPrescaler(Clock clk){
            constexpr uint8_t bitmask_tccrb_clk_bits = ~(RegBits::Timers::Timer2::TCCR2B_CS22|
                                                         RegBits::Timers::Timer2::TCCR2B_CS21|
                                                         RegBits::Timers::Timer2::TCCR2B_CS20);
            mcu::Regs::Timers::Timer2::TimerCounterControlRegB.writeMasked(static_cast<uint8_t>(clk), bitmask_tccrb_clk_bits);
            if(ClockSource::SystemClock != getClockSource()){
                while(Regs::Timers::Timer2::AsynchronousStatusReg.readBit(RegBits::Timers::Timer2::ASSR_TCR2BUB));
            }
            currentClock = clk;
        };
        static void selectClockSource(ClockSource clkSrc){
            constexpr static uint8_t bitmask_assr_clksrc_bits =~(RegBits::Timers::Timer2::ASSR_EXCLK|
                                                                RegBits::Timers::Timer2::ASSR_AS2);
            mcu::Regs::Timers::Timer2::AsynchronousStatusReg.writeMasked(static_cast<uint8_t>(clkSrc), bitmask_assr_clksrc_bits);
            
        };
        static void setMode(Timer2::Mode timerMode){
            constexpr uint8_t bitmask_tccrb_mode_bit = ~(RegBits::Timers::Timer2::TCCR2B_CS22|
                                                         RegBits::Timers::Timer2::TCCR2B_CS21|
                                                         RegBits::Timers::Timer2::TCCR2B_CS20);
            mcu::Regs::Timers::Timer2::TimerCounterControlRegB.writeMasked(static_cast<uint8_t>(timerMode), bitmask_tccrb_mode_bit);
            if(ClockSource::SystemClock != getClockSource()){
                while(Regs::Timers::Timer2::AsynchronousStatusReg.readBit(RegBits::Timers::Timer2::ASSR_TCR2BUB));
            }
        }
        static void setCompareValueA(uint8_t compareVal){
            mcu::Regs::Timers::Timer2::TimerCounterControlRegA.setValue(compareVal);
            if(ClockSource::SystemClock != getClockSource()){
                while(Regs::Timers::Timer2::AsynchronousStatusReg.readBit(RegBits::Timers::Timer2::ASSR_OCR2AUB));
            }
        };
        static void setCompareValueB(uint8_t compareVal){
            mcu::Regs::Timers::Timer2::TimerCounterControlRegB.setValue(compareVal);
            if (ClockSource::SystemClock != getClockSource()){
                while(Regs::Timers::Timer2::AsynchronousStatusReg.readBit(RegBits::Timers::Timer2::ASSR_OCR2BUB));
            }
            
        };
        static void setCounterValue(uint8_t counterVal){
            mcu::Regs::Timers::Timer2::TimerCounterReg.setValue(counterVal);
            if(ClockSource::SystemClock != getClockSource()){
                while(Regs::Timers::Timer2::AsynchronousStatusReg.readBit(RegBits::Timers::Timer2::ASSR_TCN2UB));
            }
        };
        static void setOutputModeA(OutputMode outMode = OutputMode::DisconnectedFromPin){
            constexpr static uint8_t bitmask_tccra_cmpoutmode_bits = ~(RegBits::Timers::Timer2::TCCR2A_COM2A1|
                                                                       RegBits::Timers::Timer2::TCCR2A_COM2A0);
            mcu::Regs::Timers::Timer2::TimerCounterControlRegA.writeMasked((static_cast<uint8_t>(outMode) << BITPOS_COM2A),
                                                                           bitmask_tccra_cmpoutmode_bits);
            if(ClockSource::SystemClock != getClockSource()){
                while(Regs::Timers::Timer2::AsynchronousStatusReg.readBit(RegBits::Timers::Timer2::ASSR_TCR2AUB));
            }
        }
        static void setOutputModeB(OutputMode outMode = OutputMode::DisconnectedFromPin){
            constexpr static uint8_t bitmask_tccra_cmpoutmode_bits = ~(RegBits::Timers::Timer2::TCCR2A_COM2B1|
                                                                       RegBits::Timers::Timer2::TCCR2A_COM2B0);
            mcu::Regs::Timers::Timer2::TimerCounterControlRegA.writeMasked((static_cast<uint8_t>(outMode) << BITPOS_COM2B), 
                                                                            bitmask_tccra_cmpoutmode_bits);
            if(ClockSource::SystemClock != getClockSource()){
                while(Regs::Timers::Timer2::AsynchronousStatusReg.readBit(RegBits::Timers::Timer2::ASSR_TCR2AUB));
            }
        }
        static uint8_t getCompareValueA(){
            return mcu::Regs::Timers::Timer2::OutputCompareRegA;
        };
        static uint8_t getCompareValueB(){
            return mcu::Regs::Timers::Timer2::OutputCompareRegB;
        };
        static uint8_t getCounterValue(){
            return mcu::Regs::Timers::Timer2::TimerCounterReg;
        };
        static void enableOutputComparePinA(){
            mcu::Gpio::PinOC2A::setPinMode(PinMode::Output);
        };
        static void enableOutputComparePinB(){
            mcu::Gpio::PinOC2B::setPinMode(PinMode::Output);
        };
        static void enableInterrupt(InterruptType intType){
            mcu::Regs::Timers::Timer2::TimerInterruptMaskReg.setBitmask(static_cast<uint8_t>(intType));
        }
        static void disableInterrupt(InterruptType intType){
            mcu::Regs::Timers::Timer2::TimerInterruptMaskReg.clearBitmask(static_cast<uint8_t>(intType));
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
            }
            enableInterrupt(intType);
        }
        static void syncTimer(){

        }

        static void init(Mode mode, 
                         Clock clk = Clock::NoPrescaling, 
                         ClockSource clkSource = ClockSource::SystemClock){
            currentClock = clk;
        }
            
    };

} // namespace Timers    
} // namespace mcu


/* Timer0 Overflow Interrupt*/
ISR(TIMER0_OVF_vect){mcu::Timers::Timer0::interruptHandler(mcu::Timers::Timer0::InterruptType::Overflow);}
/* Output compare match A interrupt*/
ISR(TIMER0_COMPA_vect){mcu::Timers::Timer0::interruptHandler(mcu::Timers::Timer0::InterruptType::OutputCompareMatchA);}
/* Output compare match B interrupt*/
ISR(TIMER0_COMPB_vect){mcu::Timers::Timer0::interruptHandler(mcu::Timers::Timer0::InterruptType::OutputCompareMatchB);}
#endif //TIMERS_HPP

/* Timer1 capture event interrupt*/
ISR(TIMER1_CAPT_vect){mcu::Timers::Timer1::interruptHandler(mcu::Timers::Timer1::InterruptType::InputCapture);}
/* Timer1 output compare match A interrupt*/
ISR(TIMER1_COMPA_vect){mcu::Timers::Timer1::interruptHandler(mcu::Timers::Timer1::InterruptType::OutputCompareMatchA);}
/* Timer1 output compare match B interrupt*/
ISR(TIMER1_COMPB_vect){mcu::Timers::Timer1::interruptHandler(mcu::Timers::Timer1::InterruptType::OutputCompareMatchB);}
/* Timer1 overflow interrupt*/
ISR(TIMER1_OVF_vect){mcu::Timers::Timer1::interruptHandler(mcu::Timers::Timer1::InterruptType::Overflow);}

void count(){
    static uint32_t cycle = 0;
    cycle++;
}

void foof(){
    using T2 = mcu::Timers::Timer2;     
    T2::init(T2::Mode::Normal,
             T2::Clock::NoPrescaling,
             T2::ClockSource::ExternalClock);
    T2::attachInterrupt(T2::InterruptType::Overflow, count);
    T2::selectClockSource(T2::ClockSource::SystemClock);
    mcu::Gpio::PinTOSC2::setPinMode(HAL::PinMode::Output);
    mcu::Gpio::PinTOSC2::setHigh();
    
}