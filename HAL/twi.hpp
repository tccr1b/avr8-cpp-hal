
#pragma once

/* Kütüphane tamamlanınca SİL*/
#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/interrupt.h>

#include "registers.hpp"
#include "macros.hpp"

using namespace mcu;

enum class TwiMode : uint8_t{
    Master = 0,
    Slave  = 1,
};
enum class TwiClock : uint8_t{
    NoDivision  = 0x00,
    DividedBy4  = RegBits::Twi::TWSR_TWPS0,
    DividedBy16 = RegBits::Twi::TWSR_TWPS1,
    DividedBy64 = RegBits::Twi::TWSR_TWPS1 | RegBits::Twi::TWSR_TWPS0,
};
enum class TwiFeature : uint8_t{
    GeneralCallRecognition = 0,
    Interrupt = 1,
    Acknowledge = 2,
    StartCondition = 3,
    StopCondition = 4,
};


namespace mcu{
namespace Peripherals{
/* I2C*/
class Twi{
private:
    using Callback = void(*)();
    inline static Callback cbTwiCallback = nullptr;
    constexpr static uint8_t bitmask_twsr_bitrate_prescaler_bits = RegBits::Twi::TWSR_TWPS1 | RegBits::Twi::TWSR_TWPS0;
    template<typename regAddr, uint8_t bitPosBitmask> struct Feature{
        void enable()   {regAddr().setBitmask(bitPosBitmask);}
        void disable()  {regAddr().clearBitmask(bitPosBitmask);}
        bool isEnabled(){return regAddr().readBit(bitPosBitmask);}
    };
    struct Interrupt{ //TwiInterrupt
        void enable() {Regs::Twi::TwiControlReg.setBitmask(RegBits::Twi::TWCR_TWIE);}
        void disable(){Regs::Twi::TwiControlReg.clearBitmask(RegBits::Twi::TWCR_TWIE);}
        Interrupt& attach(Callback cbFunc){cbTwiCallback = cbFunc; return *this;}
        Interrupt& detach(){cbTwiCallback = nullptr; return *this;}
        void handle() {if(cbTwiCallback) cbTwiCallback(); this->clearFlag();}
        /* Clear TWINT by writing a logic one (not by setting it to zero directly)*/
        void clearFlag(){Regs::Twi::TwiControlReg.setBitmask(RegBits::Twi::TWCR_TWINT);}
    };
public:
    static Twi::Interrupt TwiInterrupt;
    static Twi::Feature<decltype(Regs::Twi::TwiAddressReg), RegBits::Twi::TWAR_TWGCE> GeneralCallRecognition;
    static Twi::Feature<decltype(Regs::Twi::TwiControlReg), RegBits::Twi::TWCR_TWEA>  Acknowledge;
    static Twi::Feature<decltype(Regs::Twi::TwiControlReg), RegBits::Twi::TWCR_TWSTA> StartCondition;
    static Twi::Feature<decltype(Regs::Twi::TwiControlReg), RegBits::Twi::TWCR_TWSTO> StopCondition;

    static void setTwiClockPrescaler(TwiClock prescaler = TwiClock::NoDivision){
        Regs::Twi::TwiStatusReg.writeBitmask(static_cast<uint8_t>(prescaler));
    }
    [[gnu::flatten]] static void enable(){Regs::Twi::TwiControlReg.setBitmask(RegBits::Twi::TWCR_TWEN);}
    [[gnu::flatten]] static void disable(){Regs::Twi::TwiControlReg.clearBitmask(RegBits::Twi::TWCR_TWEN);}
    [[gnu::flatten]] static bool isEnabled(){return Regs::Twi::TwiControlReg.readBit(RegBits::Twi::TWCR_TWEN);}
};



}; // namespace Peripherals
}; // namespace mcu

