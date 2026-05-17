
#pragma once

/* Kütüphane tamamlanınca SİL*/
#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/interrupt.h>
#include "registers.hpp"

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
public:
    struct{ //TwiInterrupt
        void enable()   {Regs::Twi::TwiControlReg.setBitmask(RegBits::Twi::TWCR_TWIE);}
        void disable()  {Regs::Twi::TwiControlReg.clearBitmask(RegBits::Twi::TWCR_TWIE);}
        void attach(Callback cbFunc){cbTwiCallback = cbFunc;}
        void detach()   {cbTwiCallback = nullptr;}
        void handle()   {if(cbTwiCallback) cbTwiCallback();}
        /* Clear TWINT by writing a logic one (not by setting it to zero directly)*/
        void clearFlag(){Regs::Twi::TwiControlReg.setBitmask(RegBits::Twi::TWCR_TWINT);}
    }static TwiInterrupt;
    static void enableGeneralCallRecognition(){
        Regs::Twi::TwiAddressReg.setBitmask(RegBits::Twi::TWAR_TWGCE);
    }
    static void disableGeneralCallRecognition(){
        Regs::Twi::TwiAddressReg.clearBitmask(RegBits::Twi::TWAR_TWGCE);
    }
    static void setTwiClockPrescaler(TwiClock prescaler = TwiClock::NoDivision){
        Regs::Twi::TwiStatusReg.writeBitmask(static_cast<uint8_t>(prescaler));
    }
    static void enable(){
        Regs::Twi::TwiControlReg.setBitmask(RegBits::Twi::TWCR_TWEN);
    }
    static void disable(){
        Regs::Twi::TwiControlReg.clearBitmask(RegBits::Twi::TWCR_TWEN);
    }
    static bool isEnabled(){
        return Regs::Twi::TwiControlReg.readBit(RegBits::Twi::TWCR_TWEN);
    }
    static void enableAcknowledge(){
        Regs::Twi::TwiControlReg.setBitmask(RegBits::Twi::TWCR_TWEA);
    }
    static void disableAcknowledge(){
        Regs::Twi::TwiControlReg.clearBitmask(RegBits::Twi::TWCR_TWEA);
    }
    static void enable(TwiFeature feat){
        switch (feat){
        case TwiFeature::Acknowledge:
            break;
        case TwiFeature::GeneralCallRecognition:
            break;
        case TwiFeature::Interrupt:
            break;
        case TwiFeature::StartCondition:
            break;
        case TwiFeature::StopCondition:
            break;        
        default:
            break;
        }
    }
    static void disable(TwiFeature feat){
        switch (feat){
        case TwiFeature::Acknowledge:
            break;
        case TwiFeature::GeneralCallRecognition:
            break;
        case TwiFeature::Interrupt:
            break;
        case TwiFeature::StartCondition:
            break;
        case TwiFeature::StopCondition:
            break;        
        default:
            break;
        }
    }
};



}; // namespace Peripherals
}; // namespace mcu

