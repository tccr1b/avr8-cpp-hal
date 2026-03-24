#ifndef HAL_GPIO_HPP
#define HAL_GPIO_HPP

#define __AVR_ATmega328P__

#include "registers.hpp"
#include <assert.h>
#include "utils.hpp" // custom std namespace: cstd

using namespace mcu;

namespace HAL{  /* Template GpioPort & GpioPin*/
/* Kullanım:
    // Şablon Parametreleri:
    // 1. PortReg: PORT Register'ı (Örn: Regs::PORTB)
    // 2. DdrReg : DDR Register'ı  (Örn: Regs::DDRB)
    // 3. PinReg : PIN Register'ı  (Örn: Regs::PINB)
    // 4. PinBit : Pin Numarası    (Örn: 5)
    //  HAL::GpioPin<PORTx,DDRx,PINx,pinMask>

using namespace mcu;

using anyPort= HAL::GpioPort<decltype(Regs::Gpio::PortRegX),
                             decltype(Regs::Gpio::DataDirectionRegX),
                             decltype(Regs::Gpio::InputPinAddrX)>;

using anyPin = HAL::GpioPin<anyPort, RegBits::Gpio::PortX::PB_n>;
*/
    enum class PinMode : uint8_t{
        Input           = 0,
        Output          = 1,
        HighImpedance   = 2
    };

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

        static void     setPinMode(PinMode pin_mode){
            switch (pin_mode){
            case PinMode::Input:
                /* Switching between Input with Pull-Up to Output Low (InputWithPullUp => Hi-Z => OutputLow)*/
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
        static bool     enableInputPullUp(){
            /* 3 Signals (PUD, DDxn, PORTxn) control the pull-ups */
            /* Check whether corresponding pin is configured as input*/
            if(getPinMode() != PinMode::Input) return false;
            /* Enable pull-up resistors by deactivating PUD in the MCUCR*/
            mcu::Regs::Core::McuControlReg.clearBitmask(mcu::RegBits::Core::MCUCR_PUD);
            /* Write logic one to the corresponding pin which is configured as an input pin*/
            gpioPORT::PortReg().setBitmask(pinPosBitmask);
            return true;
        }
        static bool     disableInputPullUp(){
            /* Check whether corresponding pin is configured as input*/
            if(getPinMode() != PinMode::Input) return false;
            /* Write logic zero to the corresponding pin which is configured as an input pin*/
            gpioPORT::PortReg().clearBitmask(pinPosBitmask);
            return true;
        }
        static bool     isPullUpEnabled(){
            return (!mcu::Regs::Core::McuControlReg.readBit(mcu::RegBits::Core::MCUCR_PUD) &&
                    !gpioPORT::DataDirectionReg().readBit(pinPosBitmask) &&
                    gpioPORT::PortReg().readBit(pinPosBitmask));
        }
        static uint8_t  getPortRegAddr()   {return gpioPORT::PortReg();}
        static uint8_t  getDataDirRegAddr(){return gpioPORT::DataDirectionReg();}
        static uint8_t  getInputRegAddr()  {return gpioPORT::InputReg();}

        static inline void setHigh()    {gpioPORT::PortReg().setBitmask(pinPosBitmask);}
        static inline void setLow()     {gpioPORT::PortReg().clearBitmask(pinPosBitmask);}
        static inline void toggleState(){gpioPORT::PortReg().toggleBitmask(pinPosBitmask);}
        static inline bool readPin()    {__asm__("nop"); return gpioPORT::InputReg().readBit(pinPosBitmask);}
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

            /* Lambda Function and Fold Expression (...)
            ([&](auto Pin){
                if(data & (1 << bitPos)) Pin.setHigh();
                else Pin.setLow();
                bitPos++;
            }(Pins{}), ...);
            */
        }
        static uint16_t read(){
            uint16_t result = 0x0000;
            uint8_t bitPos  = 0;

            /* Lambda Function and Fold Expression (...)
            ([&](){}(), ...);
            */
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
using PinT1     = PinPD5; using PinTXD    = PinPD0; using PinXCK    = PinPD4;

} // namespace Gpio
} // namespace mcu

namespace HAL{  /* Class VirtualPort_t */
    class VirtualPort_t{
    public:
        struct PinConfig{
            volatile uint8_t DataDirectionReg;
            volatile uint8_t PortReg;
            volatile uint8_t InputReg;
            uint8_t bit;
        };
        enum class Mode : uint8_t{
            Input           = 0,
            Output          = 1,
            HighImpedance   = 2
        };
    private:
        PinConfig* vPortPins;
        uint8_t vPortPinCount;
    public:
        VirtualPort_t(PinConfig* pinArray, uint8_t count);
        void setPortMode(Mode port_mode);
        Mode getPortMode();
        void enableInputPullUps();
        void disableInputPullUps();
        bool isPullUpsEnabled();
        uint8_t read();
        uint8_t readBit();
        void write(uint8_t data);
        void writeBit(uint8_t index, uint8_t state);
        void toggleBit(uint8_t index);

        ~VirtualPort_t();
    };
    
    VirtualPort_t::VirtualPort_t(PinConfig* pinArray, uint8_t count){
        vPortPins = pinArray;
        vPortPinCount = count;
    }
    void VirtualPort_t::setPortMode(Mode port_mode){
        switch (port_mode){
        case Mode::Output:
            for(uint8_t i = 0; i < vPortPinCount; i++){
                vPortPins[i].PortReg &= ~(vPortPins[i].bit);
                vPortPins[i].DataDirectionReg |= (vPortPins[i].bit);
            }
            break;
        case Mode::Input:
            for(uint8_t i = 0; i < vPortPinCount; i++){
                vPortPins[i].PortReg &= ~(vPortPins[i].bit);
                vPortPins[i].DataDirectionReg &= ~(vPortPins[i].bit);
            }
            break;
        case Mode::HighImpedance:
            for(uint8_t i = 0; i < vPortPinCount; i++){
                vPortPins[i].DataDirectionReg &= ~(vPortPins[i].bit);
                vPortPins[i].PortReg &= ~(vPortPins[i].bit);
            }
            break;
        }
    }
    VirtualPort_t::Mode VirtualPort_t::getPortMode(){}
    void VirtualPort_t::enableInputPullUps(){
        /* VirtualPort'un durumunu kontrol et, giriş değilse işlem yapmadan çık.*/
        if(Mode::Input != getPortMode()) return;
        /* MCUCR'deki PUD bitini resetle*/
        Regs::Core::McuControlReg.clearBitmask(RegBits::Core::MCUCR_PUD);
        /* Tüm Pinleri ayarla*/
        for(uint8_t i = 0; i < vPortPinCount; i++){vPortPins[i].PortReg |= (vPortPins[i].bit);}
    }
    void VirtualPort_t::disableInputPullUps(){
        /* VirtualPort'un durumunu kontrol et, giriş değilse işlem yapmadan çık.*/
        if(Mode::Input != getPortMode()) return;
        /* Tüm pinleri ayarla*/
        for(uint8_t i = 0; i < vPortPinCount; i++){vPortPins[i].PortReg &= ~(vPortPins[i].bit);}
    }
    bool VirtualPort_t::isPullUpsEnabled(){}
    uint8_t VirtualPort_t::read(){}
    uint8_t VirtualPort_t::readBit(){}
    void VirtualPort_t::write(uint8_t data){}
    void VirtualPort_t::writeBit(uint8_t index, uint8_t state){}
    void VirtualPort_t::toggleBit(uint8_t index){}
    VirtualPort_t::~VirtualPort_t(){}
}; // namespace HAL

namespace HAL{  /* Struct GpioPort_t & Pin_t*/
struct GpioPort_t{
    volatile uint8_t DataDirectionReg;
    volatile uint8_t PortReg;
    volatile uint8_t InputReg;
};
struct Pin_t{
    public:
    GpioPort_t* PinPort;
    uint8_t pinPosBitmask;

    void setPinMode(PinMode pin_mode){
        switch (pin_mode){
        case HAL::PinMode::Input:
            PinPort->PortReg &= ~pinPosBitmask;
            PinPort->DataDirectionReg &= ~pinPosBitmask;
            break;
        case HAL::PinMode::Output:
            PinPort->PortReg &= ~pinPosBitmask;
            PinPort->DataDirectionReg |= pinPosBitmask;
            break;
        case HAL::PinMode::HighImpedance:
            PinPort->DataDirectionReg &= ~pinPosBitmask;
            PinPort->PortReg &= ~pinPosBitmask;
            break;
        }
    }
    PinMode getPinMode(){
        if(PinPort->DataDirectionReg & pinPosBitmask){
            return PinMode::Output;
        }else{
            if(PinPort->PortReg & pinPosBitmask){
                return PinMode::Input;
            }else{
                return PinMode::HighImpedance;
            }
        }
    }
    bool enableInputPullUp(){
        if(PinMode::Input != getPinMode()) return false;
        Regs::Core::McuControlReg.clearBitmask(RegBits::Core::MCUCR_PUD);
        PinPort->PortReg |= pinPosBitmask;
        return true;
    }
    bool disableInputPullUp(){
        if(getPinMode() != PinMode::Input) return false;
        PinPort->PortReg &= ~pinPosBitmask;
        return true;
    }
    bool isPullUpEnabled(){
        return (!Regs::Core::McuControlReg.readBit(RegBits::Core::MCUCR_PUD) && 
                !(PinPort->DataDirectionReg & pinPosBitmask) && 
                (PinPort->PortReg & pinPosBitmask));
    }
    uint8_t getDataDirRegAddr(){return PinPort->DataDirectionReg;}
    uint8_t getPortRegAddr()   {return PinPort->PortReg;}
    uint8_t getInputRegAddr()  {return PinPort->InputReg;}

    void inline low()    {if(PinMode::Output == getPinMode()) PinPort->PortReg &= ~pinPosBitmask;}
    void inline high()   {if(PinMode::Output == getPinMode()) PinPort->PortReg |= pinPosBitmask;}
    void inline toggle() {if(PinMode::Output == getPinMode()) PinPort->PortReg ^= pinPosBitmask;}
    bool inline read()   {__asm__("nop"); return PinPort->InputReg & pinPosBitmask;}

};
}; // namespace HAL

#endif // HAL_GPIO_HPP