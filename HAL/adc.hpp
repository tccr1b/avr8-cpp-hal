#ifndef ADC_HPP
#define ADC_HPP

#define __AVR_ATmega328P__
#include "registers.hpp"
#include "sysctrl.hpp"

#define BITMASK_ADC_REF         0x3F
#define BITMASK_ADC_AUTOTRIGSRC 0xF8
#define BITMASK_ADC_CHANNELSEL  0xF0
#define BITMASK_ADC_PRESCALER   0xF8

using namespace mcu;

enum class AdcReference     : uint8_t{
    /* bitmask: 0x3F*/
    /*Internal 1.1V voltage reference with external capacitor at AREF pin*/
    Internal= RegBits::Adc::ADMUX_REFS1 | RegBits::Adc::ADMUX_REFS0,
    /*AVCC with external capacitor at AREF pin*/
    Avcc    = RegBits::Adc::ADMUX_REFS0,
    /*Voltage on AREF pin*/
    External= 0x00,
};
enum class AdcPrescaler     : uint8_t{
    DividedBy2   = 0x01,
    DividedBy4   = 0x02,
    DividedBy8   = 0x03,
    DividedBy16  = 0x04,
    DividedBy32  = 0x05,
    DividedBy64  = 0x06,
    DividedBy128 = 0x07,
};
enum class AdcTriggerSource : uint8_t{
    FreeRunning         = 0x00,
    AnalogComparator    = RegBits::Adc::ADCSRB_ADTS0,
    ExternalIRQ         = RegBits::Adc::ADCSRB_ADTS1,
    Timer0CompareMatchA = RegBits::Adc::ADCSRB_ADTS0 | RegBits::Adc::ADCSRB_ADTS1,
    Timer0Overflow      = RegBits::Adc::ADCSRB_ADTS2,
    Timer1CompareMatchB = RegBits::Adc::ADCSRB_ADTS0 | RegBits::Adc::ADCSRB_ADTS2,
    Timer1Overflow      = RegBits::Adc::ADCSRB_ADTS1 | RegBits::Adc::ADCSRB_ADTS2,
    Timer1CaptureEvent  = RegBits::Adc::ADCSRB_ADTS0 | RegBits::Adc::ADCSRB_ADTS1 | RegBits::Adc::ADCSRB_ADTS2,
};
enum class AdcChannel       : uint8_t{
    No0 = 0x00,
    No1 = 0x01,
    No2 = 0x02,
    No3 = 0x03,
    No4 = 0x04,
    No5 = 0x05,
    No6 = 0x06,
    No7 = 0x07,
    InternalTempSensor = 0x08,
    BG1v1  = 0x0E,
    Ground = 0x0F,
};

namespace mcu{
namespace Peripherals{
    class Adc{
    private:
        const static uint8_t bitmask_acsra_prescaler_bits   = ~(RegBits::Adc::ADCSRA_ADPS0| 
                                                                RegBits::Adc::ADCSRA_ADPS1| 
                                                                RegBits::Adc::ADCSRA_ADPS2);
        const static uint8_t bitmask_acsrb_trigger_sel_bits = ~(RegBits::Adc::ADCSRB_ADTS0|
                                                                RegBits::Adc::ADCSRB_ADTS1|
                                                                RegBits::Adc::ADCSRB_ADTS2);
        const static uint8_t bitmask_admux_ref_sel_bits     = ~(RegBits::Adc::ADMUX_REFS0|
                                                                RegBits::Adc::ADMUX_REFS1);
        const static uint8_t bitmask_admux_channel_sel_bits = ~(RegBits::Adc::ADMUX_MUX0|
                                                                RegBits::Adc::ADMUX_MUX1|
                                                                RegBits::Adc::ADMUX_MUX2|
                                                                RegBits::Adc::ADMUX_MUX3);
    public:
        static void selectChannel(AdcChannel ch){
            mcu::Regs::Adc::AdcMultiplexerSelectionReg.writeBitmask(BITMASK_ADC_CHANNELSEL | static_cast<uint8_t>(ch));
        }
        static AdcChannel getActiveChannel(){
            return static_cast<AdcChannel>(~BITMASK_ADC_CHANNELSEL & mcu::Regs::Adc::AdcMultiplexerSelectionReg.getValue());
        }
        static void setAdcClockPrescaler(AdcPrescaler adcPresc){
            uint32_t fCpu = mcu::System::Clock::getCpuFrequency();
            mcu::Regs::Adc::AdcControlAndStatusRegA.writeBitmask(bitmask_acsra_prescaler_bits | static_cast<uint8_t>(adcPresc));
        }
        static AdcPrescaler getAdcClockPrescaler(){
            return static_cast<AdcPrescaler>(~bitmask_acsra_prescaler_bits & mcu::Regs::Adc::AdcControlAndStatusRegA.getValue());
        }
        static void selectAutoTrigSource(AdcTriggerSource trigSrc){
            mcu::Regs::Adc::AdcControlAndStatusRegB.writeBitmask(bitmask_acsrb_trigger_sel_bits | static_cast<uint8_t>(trigSrc));
        }
        static AdcTriggerSource getTriggerSource(){
            return static_cast<AdcTriggerSource>(~bitmask_acsrb_trigger_sel_bits & mcu::Regs::Adc::AdcControlAndStatusRegB.getValue());
        }
        static void setReference(AdcReference ref){
            mcu::Regs::Adc::AdcMultiplexerSelectionReg.writeBitmask(bitmask_admux_ref_sel_bits | static_cast<uint8_t>(ref));
        }
        static AdcReference getReference(){
            return static_cast<AdcReference>(~bitmask_admux_ref_sel_bits & mcu::Regs::Adc::AdcMultiplexerSelectionReg.getValue());
        }
        static void enableAdc() {Regs::Adc::AdcControlAndStatusRegA.setBitmask(RegBits::Adc::ADCSRA_ADEN);}
        static void disableAdc(){Regs::Adc::AdcControlAndStatusRegA.clearBitmask(RegBits::Adc::ADCSRA_ADEN);}
        static void autoTriggerEnable(){
            mcu::Regs::Adc::AdcControlAndStatusRegA.setBitmask(mcu::RegBits::Adc::ADCSRA_ADATE);
        }
        static void autoTriggerDisable(){
            mcu::Regs::Adc::AdcControlAndStatusRegA.clearBitmask(mcu::RegBits::Adc::ADCSRA_ADATE);
        }
        static void enableInterrupt(){
            mcu::Regs::Adc::AdcControlAndStatusRegA.setBitmask(mcu::RegBits::Adc::ADCSRA_ADIE);
        }
        static void disableInterrupt(){
            mcu::Regs::Adc::AdcControlAndStatusRegA.clearBitmask(mcu::RegBits::Adc::ADCSRA_ADIE);
        }
        static void digitalInputDisable(uint8_t inputNo){
            mcu::Regs::Adc::DigitalInputDisableReg0.writeBitmask(inputNo);
        }
        static uint16_t read(){

        }
    };
} // namespace Peripherals
} // namespace mcu


void foof(){
    mcu::Peripherals::Adc::selectAutoTrigSource(AdcTriggerSource::FreeRunning);
    mcu::Peripherals::Adc::selectAutoTrigSource(AdcTriggerSource::ExternalIRQ);
    mcu::Peripherals::Adc::selectAutoTrigSource(AdcTriggerSource::Timer1CaptureEvent);
    mcu::Peripherals::Adc::setAdcClockPrescaler(AdcPrescaler::DividedBy32);
    mcu::Peripherals::Adc::selectChannel(AdcChannel::InternalTempSensor);
    mcu::Peripherals::Adc::selectAutoTrigSource(AdcTriggerSource::FreeRunning);
    mcu::Peripherals::Adc::digitalInputDisable(0);
    mcu::Peripherals::Adc::getActiveChannel();
    mcu::Peripherals::Adc::selectChannel(AdcChannel::No7);
}

#endif // ADC_HPP