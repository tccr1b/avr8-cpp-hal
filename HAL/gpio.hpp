#ifndef HAL_GPIO_HPP
#define HAL_GPIO_HPP

#define __AVR_ATmega328P__

#include <assert.h>

#include "registers.hpp"
#include "utils.hpp" // custom std namespace: cstd
#include "macros.hpp"

using namespace mcu;

enum class PinMode : uint8_t{
    Input           = 0,
    Output          = 1,
    HighImpedance   = 2
};

namespace HAL{  /* Template GpioPort & GpioPin*/
    template<typename regDDRx, typename regPORTx, typename regPINx>
    struct GpioPort{
        using DDR  = regDDRx;
        using PORT = regPORTx;
        using PIN  = regPINx;

        static constexpr DDR  DataDirectionReg() {return DDR();}
        static constexpr PORT PortReg()          {return PORT();}
        static constexpr PIN  InputReg()         {return PIN();}
    };

    template<typename gpioPORT, uint8_t pinPosBitmask>
    struct GpioPin{        
        using is_gpio_pin = void; // Bu satır, bu sınıfın bir GpioPin olduğunu ispatlar
        struct{ // Internal pull-up resistors
            static bool enable(){
                /* 3 Signals (PUD, DDxn, PORTxn) control the pull-ups */
                /* Check whether corresponding pin is configured as input*/
                if(PinMode::Input != getPinMode()) return false;
                /* Enable pull-up resistors by deactivating PUD in the MCUCR*/
                mcu::Regs::Core::McuControlReg.clearBitmask(mcu::RegBits::Core::MCUCR_PUD);
                /* Write logic one to the corresponding pin which is configured as an input pin*/
                gpioPORT::PortReg().setBitmask(pinPosBitmask);
                return true;
            };
            static bool disable(){
                /* Check whether corresponding pin is configured as input*/
                if(PinMode::Input != getPinMode()) return false;
                /* Write logic zero to the corresponding pin which is configured as an input pin*/
                gpioPORT::PortReg().clearBitmask(pinPosBitmask);
                return true;
            };
            [[gnu::always_inline, nodiscard]] inline static bool isEnabled(){
                return (!mcu::Regs::Core::McuControlReg.readBit(mcu::RegBits::Core::MCUCR_PUD) &&
                        !gpioPORT::DataDirectionReg().readBit(pinPosBitmask) &&
                         gpioPORT::PortReg().readBit(pinPosBitmask));
            }
        }static InputPullUp;
        static void     setPinMode(PinMode pin_mode){
            switch (pin_mode){
            case PinMode::Input:
                /* Switching between Input with Pull-Up to Output Low (InputWithPullUp => Hi-Z => OutputLow)*/
                __asm__ __volatile__("nop");
                gpioPORT::PortReg().clearBitmask(pinPosBitmask);
                gpioPORT::DataDirectionReg().clearBitmask(pinPosBitmask);
                break;
            case PinMode::Output:
                /* Switching between Hi-Z to Output-High (Hi-Z => OutputLow => OutputHigh)*/
                gpioPORT::PortReg().clearBitmask(pinPosBitmask);
                gpioPORT::DataDirectionReg().setBitmask(pinPosBitmask);
                break;
            case PinMode::HighImpedance:
                /* High Impedance regardless value of PUD bit in MCUCR Reg.*/
                gpioPORT::DataDirectionReg().clearBitmask(pinPosBitmask);
                gpioPORT::PortReg().clearBitmask(pinPosBitmask);
                break;
            default:
                break;
            }
        }
        static PinMode  getPinMode(){
            if(gpioPORT::DataDirectionReg().readBit(pinPosBitmask)){
                return PinMode::Output;
            }else{
                return gpioPORT::PortReg().readBit(pinPosBitmask) ? PinMode::Input : PinMode::HighImpedance;
            }
        }
        static uint8_t  getPortRegAddr()   {return gpioPORT::PortReg();}
        static uint8_t  getDataDirRegAddr(){return gpioPORT::DataDirectionReg();}
        static uint8_t  getInputRegAddr()  {return gpioPORT::InputReg();}

        [[gnu::always_inline]] inline static void setHigh(){gpioPORT::PortReg().setBitmask(pinPosBitmask);}
        [[gnu::always_inline]] inline static void setLow (){gpioPORT::PortReg().clearBitmask(pinPosBitmask);}
        [[gnu::always_inline]] inline static void toggle (){gpioPORT::InputReg().setValue(pinPosBitmask);}
        [[gnu::always_inline]] inline static bool readPin(){return gpioPORT::InputReg().readBit(pinPosBitmask);}
};

    template<typename gpioPORT, uint8_t combinedMask>
    struct PortGroup{
        // Gruptaki tüm pinleri aynı anda çıkış yapar
        static inline void setOutput(){
            gpioPORT::DataDirectionReg().setBitmask(combinedMask);
        }
        // Belirli bir değeri gruba yazar (diğer pinleri bozmadan)
        // Bu metod sadece gruptaki pinleri etkiler
        static inline void write(uint8_t value){
            // value içindeki bitleri maskeleyip porta yazıyoruz
            gpioPORT::PortReg().writeMasked(value, combinedMask);
        }

        static inline void setHigh(){gpioPORT::PortReg().setBitmask(combinedMask);}
        static inline void setLow() {gpioPORT::PortReg().clearBitmask(combinedMask);}
    };

    template<typename T, typename = void> struct is_valid_pin : cstd::false_type {};
    template<typename T> struct is_valid_pin<T, cstd::void_t<typename T::is_gpio_pin>> : cstd::true_type {};
    template<typename... Pins>
    struct VirtualPort{
        static_assert((is_valid_pin<Pins>::values && ...), 
            "HATA [static_assert @ VirtualPort]: VirtualPort sadece GpioPin tiplerini kabul eder!");
        static_assert(sizeof...(Pins) <= 16, 
            "HATA [static_assert @ VirtualPort]: VirtualPort şimdilik en fazla 16 pin destekliyor!");
        static_assert(sizeof...(Pins) > 0, 
            "HATA [static_assert @ VirtualPort]: VirtualPort en az 1 pin içermelidir!");
        
        static inline void setPortMode(PinMode pin_mode){(Pins::setPinMode(pin_mode), ...);}
        static void write(uint16_t data){
            uint8_t bitPos = 0;
            ([&]{
                if(data & (1 << bitPos)){
                    Pins::setHigh();
                }else{
                    Pins::setLow();
                }
                bitPos++;
            }(), ...);
        }
        static uint16_t read(){
            uint16_t result = 0x0000;
            uint8_t bitPos  = 0;

            ([&]{
                if(Pins::read()){result |= (1 << bitPos);}
                bitPos++;
            }(), ...);

            return result;
        }
        
        static inline void setHigh(){}
        static inline void setLow(){}
    };

}; // namespace HAL

namespace mcu{
namespace Gpio{
/* PORTB REGS*/
using GpioPortB = HAL::GpioPort<decltype(Regs::Gpio::DataDirectionRegB), // DDRx
                                decltype(Regs::Gpio::PortRegB),          // PORTx
                                decltype(Regs::Gpio::InputPinAddrB)>;    // PINx
/* PORTC REGS*/
using GpioPortC = HAL::GpioPort<decltype(Regs::Gpio::DataDirectionRegC), // DDRx
                                decltype(Regs::Gpio::PortRegC),          // PORTx
                                decltype(Regs::Gpio::InputPinAddrC)>;    // PINx
/* PORTD REGS*/
using GpioPortD = HAL::GpioPort<decltype(Regs::Gpio::DataDirectionRegD), // DDRx
                                decltype(Regs::Gpio::PortRegD),          // PORTx
                                decltype(Regs::Gpio::InputPinAddrD)>;    // PINx
/* PORTB PINS*/
using PinPB0    = HAL::GpioPin<GpioPortB, RegBits::Gpio::PortB::PB_0>;
using PinPB1    = HAL::GpioPin<GpioPortB, RegBits::Gpio::PortB::PB_1>;
using PinPB2    = HAL::GpioPin<GpioPortB, RegBits::Gpio::PortB::PB_2>;
using PinPB3    = HAL::GpioPin<GpioPortB, RegBits::Gpio::PortB::PB_3>;
using PinPB4    = HAL::GpioPin<GpioPortB, RegBits::Gpio::PortB::PB_4>;
using PinPB5    = HAL::GpioPin<GpioPortB, RegBits::Gpio::PortB::PB_5>;
using PinPB6    = HAL::GpioPin<GpioPortB, RegBits::Gpio::PortB::PB_6>;
using PinPB7    = HAL::GpioPin<GpioPortB, RegBits::Gpio::PortB::PB_7>;
/* Alias for alternate functions of pins on port B*/
using PinCLKO   = PinPB0; using PinICP1   = PinPB0; using PinOC1A   = PinPB1;
using PinOC1B   = PinPB2; using PinSS     = PinPB2; using PinMOSI   = PinPB3;
using PinOC2A   = PinPB3; using PinMISO   = PinPB4; using PinSCK    = PinPB5;
using PinTOSC1  = PinPB6; using PinTOSC2  = PinPB7;

/* PORTC PINS*/
using PinPC0    = HAL::GpioPin<GpioPortC, RegBits::Gpio::PortC::PC_0>;
using PinPC1    = HAL::GpioPin<GpioPortC, RegBits::Gpio::PortC::PC_1>;
using PinPC2    = HAL::GpioPin<GpioPortC, RegBits::Gpio::PortC::PC_2>;
using PinPC3    = HAL::GpioPin<GpioPortC, RegBits::Gpio::PortC::PC_3>;
using PinPC4    = HAL::GpioPin<GpioPortC, RegBits::Gpio::PortC::PC_4>;
using PinPC5    = HAL::GpioPin<GpioPortC, RegBits::Gpio::PortC::PC_5>;
/* Alias for alternate functions of pins on port C*/
using PinSCL    = PinPC5; using PinSDA    = PinPC4; using PinADC0   = PinPC0;
using PinADC1   = PinPC1; using PinADC2   = PinPC2; using PinADC3   = PinPC3;
using PinADC4   = PinPC4; using PinADC5   = PinPC5;

/* PORTD PINS*/
using PinPD0    = HAL::GpioPin<GpioPortD, RegBits::Gpio::PortD::PD_0>;
using PinPD1    = HAL::GpioPin<GpioPortD, RegBits::Gpio::PortD::PD_1>;
using PinPD2    = HAL::GpioPin<GpioPortD, RegBits::Gpio::PortD::PD_2>;
using PinPD3    = HAL::GpioPin<GpioPortD, RegBits::Gpio::PortD::PD_3>;
using PinPD4    = HAL::GpioPin<GpioPortD, RegBits::Gpio::PortD::PD_4>;
using PinPD5    = HAL::GpioPin<GpioPortD, RegBits::Gpio::PortD::PD_5>;
using PinPD6    = HAL::GpioPin<GpioPortD, RegBits::Gpio::PortD::PD_6>;
using PinPD7    = HAL::GpioPin<GpioPortD, RegBits::Gpio::PortD::PD_7>;
/* Alias for alternate functions of pins on port D*/
using PinAIN0   = PinPD6; using PinAIN1   = PinPD7; using PinINT0   = PinPD2;
using PinINT1   = PinPD3; using PinOC0A   = PinPD6; using PinOC0B   = PinPD5;
using PinOC2B   = PinPD3; using PinRXD    = PinPD0; using PinT0     = PinPD4;
using PinT1     = PinPD5; using PinTXD    = PinPD1; using PinXCK    = PinPD4;

} // namespace Gpio
} // namespace mcu

#endif // HAL_GPIO_HPP