
#pragma once

/* Kütüphane tamamlanınca SİL*/
#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include "registers.hpp"
#include <inttypes.h>

using namespace mcu;

enum class TwiMode : uint8_t{
    Master = 0,
    Slave  = 1,
};

enum class TwiBitratePrescaler : uint8_t{
    NoDivision  = 0xF8, // Bitmasked value (0x00 -> original value)
    DividedBy4  = 0xF9, // Bitmasked value (0x01 -> original value)
    DividedBy16 = 0xFA, // Bitmasked value (0x02 -> original value)
    DividedBy64 = 0xFB, // Bitmasked value (0x03 -> original value)
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

class Twi{
public:
    static void enableGeneralCallRecognition(){
        Regs::Twi::TwiAddressReg.setBitmask(RegBits::Twi::TWAR_TWGCE);
    }
    static void disableGeneralCallRecognition(){
        Regs::Twi::TwiAddressReg.clearBitmask(RegBits::Twi::TWAR_TWGCE);
    }
    static void setPrescaler(TwiBitratePrescaler prescaler = TwiBitratePrescaler::NoDivision){
        Regs::Twi::TwiStatusReg.writeBitmask(static_cast<uint8_t>(prescaler));
    }
    static void enableInterrupt(){
        Regs::Twi::TwiControlReg.setBitmask(RegBits::Twi::TWCR_TWIE);
    }
    static void disableInterrupt(){
        Regs::Twi::TwiControlReg.clearBitmask(RegBits::Twi::TWCR_TWIE);
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
    static void clearInterruptFlag(){
        /* Clear TWINT by writing a logic one (not by setting it to zero directly)*/
        Regs::Twi::TwiControlReg.setBitmask(RegBits::Twi::TWCR_TWINT);
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

