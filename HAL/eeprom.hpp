#ifndef EEPROM_HPP
#define EEPROM_HPP

// Sonra sil
#define __AVR_ATmega328P__

#include "registers.hpp"
//#include <avr/eeprom.h>
#include <avr/interrupt.h>

#define BITMASK_EEPROM_MODESEL  0xCF

using namespace mcu;

enum class EepromMode : uint8_t{
    /* Erase and write in one operation (atomic operation)*/
    WriteAndErase = 0x00,
    /* Erase only*/
    EraseOnly     = RegBits::Eeprom::EECR_EEPM0,
    /* Write only*/
    WriteOnly     = RegBits::Eeprom::EECR_EEPM1,
};
enum class EepromFeature : uint8_t{
    EepromReadyInterrupt= RegBits::Eeprom::EECR_EERIE,
    MasterWrite         = RegBits::Eeprom::EECR_EEMPE,
    Write               = RegBits::Eeprom::EECR_EEPE,
    Read                = RegBits::Eeprom::EECR_EERE,
};
struct EepromConfig{
        EepromMode mode;
        EepromFeature feat;
};

enum class EepromInterruptType : uint8_t{
    EepromReady = 0,
};

namespace mcu{
class Eeprom{
private:
    using Callback = void(*)();
    inline static Callback interruptCallbackEepromReady = nullptr;
    static inline bool isReady(){
        while(Regs::Eeprom::EepromControlReg.readBit(RegBits::Eeprom::EECR_EEPE));
        return true;
    }
public:
    static void detachEepromReadyInterrupt(){
        interruptCallbackEepromReady = nullptr;
    };
    static void attachEepromReadyInterrupt(Callback cbFunc){
        interruptCallbackEepromReady = cbFunc;
    }
    static void interruptHandler(){
        if(interruptCallbackEepromReady) interruptCallbackEepromReady();
    }
    static void enable(EepromFeature feat){
        Regs::Eeprom::EepromControlReg.setBitmask(static_cast<uint8_t>(feat));
    }
    static void disable(EepromFeature feat){
        Regs::Eeprom::EepromControlReg.clearBitmask(static_cast<uint8_t>(feat));
    }
    static void setEepromMode(EepromMode mode){
        Regs::Eeprom::EepromControlReg.writeMasked(static_cast<uint8_t>(mode), BITMASK_EEPROM_MODESEL);
    }
    static EepromMode getEepromMode(){
        return static_cast<EepromMode>(~BITMASK_EEPROM_MODESEL & Regs::Eeprom::EepromControlReg.getValue());
    }

    /* Overloaded write functions*/
    static bool write(){

    }

    /* Overloaded read functions*/
    static auto read(){

    }

    /* Overloaded update functions*/
    static void update(){

    }

    static bool writeBlock(){}
    static bool readBlock(){}
    static bool updateBlock(){}

    static void writeByte(uint16_t address, uint8_t data){
        /* Wait for completion of previous write */
        while(Regs::Eeprom::EepromControlReg.readBit(RegBits::Eeprom::EECR_EEPE));
        
        /* Set up address and data registers */
        Regs::Eeprom::EepromAddressReg_HByte.setValue((address >> 8) & 0xFF);
        Regs::Eeprom::EepromAddressReg_LByte.setValue(address & 0xFF);
        Regs::Eeprom::EepromDataReg.setValue(data);
        
        /* Write logical one to EEMPE */
        Regs::Eeprom::EepromControlReg.setBitmask(RegBits::Eeprom::EECR_EEMPE);
        /* Start eeprom write by setting EEPE */
        Regs::Eeprom::EepromControlReg.setBitmask(RegBits::Eeprom::EECR_EEPE);
        
    }
    static uint8_t readByte(uint16_t address){
        /* Wait for completion of previous write */
        while(Regs::Eeprom::EepromControlReg.readBit(RegBits::Eeprom::EECR_EEPE));
        
        /* Set up address register */
        Regs::Eeprom::EepromAddressReg_HByte.setValue((address >> 8) & 0xFF);
        Regs::Eeprom::EepromAddressReg_LByte.setValue(address & 0xFF);
        
        /* Start eeprom read by writing EERE */
        Regs::Eeprom::EepromControlReg.setBitmask(RegBits::Eeprom::EECR_EERE);
        
        /* Return data from Data Register */
        return Regs::Eeprom::EepromDataReg.getValue();
    }
    static void init(EepromConfig* eepromCfg){
    
    }
};

}// namespace mcu

/* Eeprom Ready Interrupt*/
ISR(EE_READY_vect){mcu::Eeprom::interruptHandler();}

#endif //EEPROM_HPP