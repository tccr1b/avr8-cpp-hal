#ifndef ADC_HPP
#define ADC_HPP

#define __AVR_ATmega328P__
#include "macros.hpp"
#include "registers.hpp"
#include "sysctrl.hpp"
#include "HAL/utils/atomicblock.hpp"

#define BITMASK_ADC_REF         0x3F
#define BITMASK_ADC_AUTOTRIGSRC 0xF8
#define BITMASK_ADC_CHANNELSEL  0xF0
#define BITMASK_ADC_PRESCALER   0xF8
#define BITMASK_ADC_DIDR        0x3F

using namespace mcu;

enum class AdcReference        : uint8_t{
    /* bitmask: 0x3F*/
    /*Internal 1.1V voltage reference with external capacitor at AREF pin*/
    Internal_1v1 = RegBits::Adc::ADMUX_REFS1 | RegBits::Adc::ADMUX_REFS0,
    /*AVCC with external capacitor at AREF pin*/
    Internal_Avcc= RegBits::Adc::ADMUX_REFS0,
    /*Voltage on AREF pin*/
    External_Aref= 0x00,
};
enum class AdcClock            : uint8_t{
    DividedBy2   = 0x01,
    DividedBy4   = 0x02,
    DividedBy8   = 0x03,
    DividedBy16  = 0x04,
    DividedBy32  = 0x05,
    DividedBy64  = 0x06,
    DividedBy128 = 0x07,
};
enum class AdcAutoTriggerSource: uint8_t{
    FreeRunning         = 0x00,
    AnalogComparator    = RegBits::Adc::ADCSRB_ADTS0,
    ExternalIRQ0        = RegBits::Adc::ADCSRB_ADTS1,
    Timer0_CompareMatchA= RegBits::Adc::ADCSRB_ADTS1|RegBits::Adc::ADCSRB_ADTS0,
    Timer0_Overflow     = RegBits::Adc::ADCSRB_ADTS2,
    Timer1_CompareMatchB= RegBits::Adc::ADCSRB_ADTS2|RegBits::Adc::ADCSRB_ADTS0,
    Timer1_Overflow     = RegBits::Adc::ADCSRB_ADTS2|RegBits::Adc::ADCSRB_ADTS1,
    Timer1_CaptureEvent = RegBits::Adc::ADCSRB_ADTS2|RegBits::Adc::ADCSRB_ADTS1|RegBits::Adc::ADCSRB_ADTS0,
};
enum class AdcChannel          : uint8_t{
    /* ADC0*/
    No0 = 0x00, //0b0000'0000
    /* ADC1*/
    No1 = 0x01, //0b0000'0001
    /* ADC2*/
    No2 = 0x02, //0b0000'0010
    /* ADC3*/
    No3 = 0x03, //0b0000'0011
    /* ADC4*/
    No4 = 0x04, //0b0000'0100
    /* ADC5*/
    No5 = 0x05, //0b0000'0101
    /* ADC6*/
    No6 = 0x06, //0b0000'0110
    /* ADC7*/
    No7 = 0x07, //0b0000'0111
    /* Built-in temperature sensor for some packages (SPDIP, TQFP, VQFN)*/
    InternalTempSensor = 0x08,
    /* Band gap voltage*/
    Vbg_1v1= 0x0E,
    GND_0V = 0x0F,
};
enum class AdcResultAdjust     : uint8_t{
    Right=0,
    Left =1
};

namespace mcu{
namespace Peripherals{
    class Adc{
    private:
        constexpr static uint8_t bitmask_acsra_prescaler_bits   = (RegBits::Adc::ADCSRA_ADPS0| 
                                                                   RegBits::Adc::ADCSRA_ADPS1| 
                                                                   RegBits::Adc::ADCSRA_ADPS2);
        constexpr static uint8_t bitmask_acsrb_trigger_sel_bits = (RegBits::Adc::ADCSRB_ADTS0|
                                                                   RegBits::Adc::ADCSRB_ADTS1|
                                                                   RegBits::Adc::ADCSRB_ADTS2);
        constexpr static uint8_t bitmask_admux_ref_sel_bits     = (RegBits::Adc::ADMUX_REFS0|
                                                                   RegBits::Adc::ADMUX_REFS1);
        constexpr static uint8_t bitmask_admux_channel_sel_bits = (RegBits::Adc::ADMUX_MUX0|
                                                                   RegBits::Adc::ADMUX_MUX1|
                                                                   RegBits::Adc::ADMUX_MUX2|
                                                                   RegBits::Adc::ADMUX_MUX3);
        using Callback = void(*)();
        inline static Callback cbAdcConversionCompletedCallback = nullptr;
        template<typename regAddr, uint8_t bitPosBitmask> struct Feature{
            void enable()   {regAddr().setBitmask(bitPosBitmask);}
            void disable()  {regAddr().clearBitmask(bitPosBitmask);}
            [[nodiscard]] bool isEnabled(){return regAddr().readBit(bitPosBitmask);}
        };
        struct AutoTrig{
            AutoTrig& selectSource(AdcAutoTriggerSource trigSrc){
                mcu::Regs::Adc::AdcControlAndStatusRegB.writeMasked(static_cast<uint8_t>(trigSrc), ~bitmask_acsrb_trigger_sel_bits);
                return *this;
            }
            AdcAutoTriggerSource getSource(){
                return static_cast<AdcAutoTriggerSource>(Regs::Adc::AdcControlAndStatusRegB.getValue(bitmask_acsrb_trigger_sel_bits));
            }
            void enable   (){mcu::Regs::Adc::AdcControlAndStatusRegA.setBitmask(mcu::RegBits::Adc::ADCSRA_ADATE);}
            void disable  (){mcu::Regs::Adc::AdcControlAndStatusRegA.clearBitmask(mcu::RegBits::Adc::ADCSRA_ADATE);}
            [[nodiscard]] bool isEnabled(){return mcu::Regs::Adc::AdcControlAndStatusRegA.readBit(mcu::RegBits::Adc::ADCSRA_ADATE);}
        };
    public:
        struct Config{
            AdcReference adc_reference;
            AdcChannel   adc_channel;
            AdcClock     adc_clock;
            AdcAutoTriggerSource adc_auto_trig_src;
            AdcResultAdjust adc_result_adjust;
            /* Enables auto trigerring when true*/
            bool adc_auto_trig_enable = false;
        };
        struct{ //Interrupt
            void enable(){mcu::Regs::Adc::AdcControlAndStatusRegA.setBitmask(mcu::RegBits::Adc::ADCSRA_ADIE);}
            void disable(){mcu::Regs::Adc::AdcControlAndStatusRegA.clearBitmask(mcu::RegBits::Adc::ADCSRA_ADIE);}
            void attach(Callback callbackFunc){cbAdcConversionCompletedCallback = callbackFunc; this->enable();}
            void detach(){cbAdcConversionCompletedCallback = nullptr; this->disable();}
            [[gnu::always_inline]] inline void handler(){if(cbAdcConversionCompletedCallback) cbAdcConversionCompletedCallback();}
        }static ConversionCompletedInterrupt;
        static AutoTrig AutoTriggering;

        static void selectChannel(AdcChannel ch){
            bool auto_trg_flag = false;
            if(AutoTriggering.isEnabled()){AutoTriggering.disable(); auto_trg_flag = true;}
            mcu::Regs::Adc::AdcMultiplexerSelectionReg.writeMasked(static_cast<uint8_t>(ch), ~bitmask_admux_channel_sel_bits);
            auto_trg_flag ? AutoTriggering.enable() : AutoTriggering.disable();
        }
        static AdcChannel getActiveChannel(){
            /* ??????????????*/
            return static_cast<AdcChannel>(Regs::Adc::AdcMultiplexerSelectionReg.getValue(bitmask_admux_channel_sel_bits));
        }
        static void setAdcClockPrescaler(AdcClock adcPresc){
            mcu::Regs::Adc::AdcControlAndStatusRegA.writeMasked(static_cast<uint8_t>(adcPresc), ~bitmask_acsra_prescaler_bits);
        }
        static AdcClock getAdcClockPrescaler(){
            return static_cast<AdcClock>(Regs::Adc::AdcControlAndStatusRegA.getValue(bitmask_acsra_prescaler_bits));
        }
        static void selectReference(AdcReference ref){
            bool auto_trg_flag = false;
            if(AutoTriggering.isEnabled()){AutoTriggering.disable(); auto_trg_flag = true;}
            mcu::Regs::Adc::AdcMultiplexerSelectionReg.writeMasked(static_cast<uint8_t>(ref), ~bitmask_admux_ref_sel_bits);
            auto_trg_flag ? AutoTriggering.enable() : AutoTriggering.disable();
        }
        static AdcReference getReference(){
            return static_cast<AdcReference>(mcu::Regs::Adc::AdcMultiplexerSelectionReg.getValue(bitmask_admux_ref_sel_bits));
        }
        static void enableAdc(){
            /* The Power Reduction ADC bit, PRADC must be disabled by writing a logical zero to enable the ADC.*/
            mcu::System::Power::activatePeripheral(Peripheral::Adc);
            Regs::Adc::AdcControlAndStatusRegA.setBitmask(RegBits::Adc::ADCSRA_ADEN);
        }
        static void disableAdc(){
            Regs::Adc::AdcControlAndStatusRegA.clearBitmask(RegBits::Adc::ADCSRA_ADEN);
        }
        static void disableDigitalInputBuffer(AdcChannel adc_channel){
            uint8_t channel_pos = 1 << static_cast<uint8_t>(adc_channel);
            if(channel_pos & BITMASK_ADC_DIDR) return;
            mcu::Regs::Adc::DigitalInputDisableReg0.setBitmask(1 << static_cast<uint8_t>(adc_channel));
        }
        static void enableDigitalInputBuffer(AdcChannel adc_channel){
            uint8_t channel_pos = 1 << static_cast<uint8_t>(adc_channel);
            if(channel_pos & BITMASK_ADC_DIDR) return;
            mcu::Regs::Adc::DigitalInputDisableReg0.clearBitmask(1 << channel_pos);
        }
        static void setResultAdjust(AdcResultAdjust adj){
            if(adj == AdcResultAdjust::Right){
                Regs::Adc::AdcMultiplexerSelectionReg.clearBitmask(RegBits::Adc::ADMUX_ADLAR);
            }else{
                Regs::Adc::AdcMultiplexerSelectionReg.setBitmask(RegBits::Adc::ADMUX_ADLAR);
            }
        }
        static uint16_t read(){
            uint16_t rawAdcVal = 0x0000;
            uint8_t  lowByte   = 0x00;
            Regs::Adc::AdcControlAndStatusRegA.setBitmask(RegBits::Adc::ADCSRA_ADSC);
            while(Regs::Adc::AdcControlAndStatusRegA.readBit(RegBits::Adc::ADCSRA_ADSC)){}
            {AtomicBlock ab;
                lowByte = Regs::Adc::AdcDataReg_LByte;  // 0bLLLL'LLLL
                rawAdcVal = Regs::Adc::AdcDataReg_HByte;// 0b0000'0000'HHHH'HHHH
                rawAdcVal <<= 8;                        // 0bHHHH'HHHH'0000'0000
                rawAdcVal |= lowByte;                   // 0bHHHH'HHHH'LLLL'LLLL
            }
            return rawAdcVal;
        }
        static void init(AdcChannel chan){
            selectChannel(chan);
            enableAdc();
        }
        static void init(Config* conf){
            if(conf->adc_auto_trig_enable) AutoTriggering.selectSource(conf->adc_auto_trig_src).enable();
            selectChannel(conf->adc_channel);
            selectReference(conf->adc_reference);
            setAdcClockPrescaler(conf->adc_clock);
            setResultAdjust(conf->adc_result_adjust);
            enableAdc();
        }
    };
} // namespace Peripherals
} // namespace mcu


void foof(){
    mcu::Peripherals::Adc::setAdcClockPrescaler(AdcClock::DividedBy32);
    mcu::Peripherals::Adc::selectChannel(AdcChannel::InternalTempSensor);
    mcu::Peripherals::Adc::disableDigitalInputBuffer(AdcChannel::No0);
    mcu::Peripherals::Adc::getActiveChannel();
    mcu::Peripherals::Adc::selectChannel(AdcChannel::No7);
    mcu::Peripherals::Adc::enableAdc();
    mcu::Peripherals::Adc::read();
}

ISR(ADC_vect){mcu::Peripherals::Adc::ConversionCompletedInterrupt.handler();}

#endif // ADC_HPP