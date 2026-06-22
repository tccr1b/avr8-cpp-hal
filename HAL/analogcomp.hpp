#ifndef ANALOG_COMP_HPP
#define ANALOG_COMP_HPP

#define __AVR_ATmega328P__

#include <avr/interrupt.h>

#include "registers.hpp"

using namespace mcu;

enum class AcA1Channel    :uint8_t{
    Ain1Pin  = 0xFF,
    AdcChNo0 = 0x00,
    AdcChNo1 = 0x01,
    AdcChNo2 = 0x02,
    AdcChNo3 = 0x03,
    AdcChNo4 = 0x04,
    AdcChNo5 = 0x05,
    AdcChNo6 = 0x06,
    AdcChNo7 = 0x07,
};
enum class AcA0Channel    :uint8_t{
    BandgapRef =1,
    Ain0Pin    =0,
};
enum class AcInterruptMode:uint8_t{
    InterruptOnOutputToggle      = 0x00,
    InterruptOnFallingOutputEdge = RegBits::Adc::ACSR_ACIS1,
    InterruptOnRisingOutputEdge  = RegBits::Adc::ACSR_ACIS1 | RegBits::Adc::ACSR_ACIS0,
};
enum class AcInput        :uint8_t{
    Ain0 = RegBits::Ac::DIDR1_AIN0D,
    Ain1 = RegBits::Ac::DIDR1_AIN1D,
};

namespace mcu{
namespace Peripherals{
    class AnalogComp{
    private:
        constexpr static uint8_t bitmask_admux_channel_sel_bits = (RegBits::Adc::ADMUX_MUX0|RegBits::Adc::ADMUX_MUX1|
                                                                   RegBits::Adc::ADMUX_MUX2|RegBits::Adc::ADMUX_MUX3);

        constexpr static uint8_t bitmask_acsr_int_mode_bits     = (RegBits::Adc::ACSR_ACIS0|RegBits::Adc::ACSR_ACIS1);
        using Callback = void(*)(void);
        inline static Callback cbAcCallback = nullptr;
        template<typename regAddr, uint8_t bitPosBitmask> struct Feature{
            void enable() {regAddr().setBitmask(bitPosBitmask);}
            void disable(){regAddr().clearBitmask(bitPosBitmask);}
            [[nodiscard]] bool isEnabled(){return regAddr().readBit(bitPosBitmask);}
        };

        struct AcInterrupt{
            void enable (){Regs::Adc::AnalogComparatorControlAndStatusReg.setBitmask(RegBits::Adc::ACSR_ACIE);}
            void disable(){Regs::Adc::AnalogComparatorControlAndStatusReg.clearBitmask(RegBits::Adc::ACSR_ACIE);}
            inline bool  isEnabled(){return Regs::Adc::AnalogComparatorControlAndStatusReg.readBit(RegBits::Adc::ACSR_ACIE);}
            AcInterrupt& attach(Callback cbFunc){cbAcCallback = cbFunc; return *this;}
            AcInterrupt& detach(){cbAcCallback = nullptr; return *this;}
            inline void  handle(){if(cbAcCallback) cbAcCallback();}
            AcInterrupt& selectMode(AcInterruptMode ac_int_mode){
                Regs::Adc::AnalogComparatorControlAndStatusReg.writeMasked(static_cast<uint8_t>(ac_int_mode), ~bitmask_acsr_int_mode_bits);
                return *this;
            }
        private:
            static void __attribute__((flatten, signal(ANALOG_COMP_vect_num))) irqHandler(){
                if(cbAcCallback) cbAcCallback();
            }
        };

    public:
        typedef struct {
            /* A0 input is positive (i.e. non-inverting) input of comparator*/
            AcA0Channel ac_a0_pos_ch;
            /* A1 input is negative (i.e. inverting) input of comparator*/
            AcA1Channel ac_a1_neg_ch;
        } config_t;
        static AcInterrupt Interrupt;
        static AnalogComp::Feature<decltype(Regs::Adc::AnalogComparatorControlAndStatusReg), RegBits::Adc::ACSR_ACIC> InputCapture;
        static void setPositiveInputA0(AcA0Channel channel){
            static_cast<uint8_t>(channel)?
                Regs::Adc::AnalogComparatorControlAndStatusReg.setBitmask(RegBits::Adc::ACSR_ACBG):
                Regs::Adc::AnalogComparatorControlAndStatusReg.clearBitmask(RegBits::Adc::ACSR_ACBG);
        }
        /* Using ADC channels for negative input of analog comparator, disables ADC peripheral*/
        static void setNegativeInputA1(AcA1Channel channel){
            if(AcA1Channel::Ain1Pin == channel){Regs::Adc::AdcControlAndStatusRegB.clearBitmask(RegBits::Adc::ADCSRB_ACME); return;}
            /* Disable ADC*/
            Regs::Adc::AdcControlAndStatusRegA.clearBitmask(RegBits::Adc::ADCSRA_ADEN);
            /* Enable analog comparator to use multiplexer of adc*/
            Regs::Adc::AdcControlAndStatusRegB.setBitmask(RegBits::Adc::ADCSRB_ACME);
            /* Select channel*/
            Regs::Adc::AdcMultiplexerSelectionReg.writeMasked(static_cast<uint8_t>(channel), ~bitmask_admux_channel_sel_bits);
        }
        static void disable(){
            bool currentIntStatus = AnalogComp::Interrupt.isEnabled();
            AnalogComp::Interrupt.disable();
            Regs::Adc::AnalogComparatorControlAndStatusReg.setBitmask(RegBits::Adc::ACSR_ACD);
            if(currentIntStatus) AnalogComp::Interrupt.enable();
        }
        static void enable(){
            bool currentIntStatus = AnalogComp::Interrupt.isEnabled();
            AnalogComp::Interrupt.disable();
            Regs::Adc::AnalogComparatorControlAndStatusReg.clearBitmask(RegBits::Adc::ACSR_ACD);
            if(currentIntStatus) AnalogComp::Interrupt.enable();
        }
        static void disableDigitalInputBuffer(AcInput ac_input){
            Regs::Adc::DigitalInputDisableReg1.setBitmask(static_cast<uint8_t>(ac_input));
        }
        static void enableDigitalInputBuffer(AcInput ac_input){
            Regs::Adc::DigitalInputDisableReg1.clearBitmask(static_cast<uint8_t>(ac_input));
        }
        static bool readOutput(){
            return Regs::Adc::AnalogComparatorControlAndStatusReg.readBit(RegBits::Adc::ACSR_ACO);
        }
        static void init(config_t* cfg){
            setPositiveInputA0(cfg->ac_a0_pos_ch);
            setNegativeInputA1(cfg->ac_a1_neg_ch);
            enable();
        }
/* On the Analog Comparator output (ACO), and this change confirms to the setting of the edge 
detector, a capture will be triggered*/
    };

} // Peripherals
} // mcu

ISR(ANALOG_COMP_vect){mcu::Peripherals::AnalogComp::Interrupt.handle();}

#endif // ANALOG_COMP_HPP