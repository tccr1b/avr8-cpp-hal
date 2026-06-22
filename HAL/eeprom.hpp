#ifndef EEPROM_HPP
#define EEPROM_HPP

// Sonra sil
#define __AVR_ATmega328P__

#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "HAL/registers.hpp"
#include "HAL/utils/atomicblock.hpp"
#include "HAL/macros.hpp"

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
    MasterWrite         = RegBits::Eeprom::EECR_EEMPE,
    Write               = RegBits::Eeprom::EECR_EEPE,
    Read                = RegBits::Eeprom::EECR_EERE,
};

namespace mcu{
class Eeprom{
private:
    constexpr static uint8_t bitmask_eecr_mode_sel_bits = RegBits::Eeprom::EECR_EEPM0 | RegBits::Eeprom::EECR_EEPM1;
    using Callback = void(*)();
    inline static Callback cbEepromReadyCallback = nullptr;
    static inline bool isBusy() __atr_always_inline__{
        return Regs::Eeprom::EepromControlReg.readBit(RegBits::Eeprom::EECR_EEPE);
    }
    template<typename regAddr, EepromFeature feat> struct Feature{
        void enable() {regAddr().setBitmask(static_cast<uint8_t>(feat));}
        void disable(){regAddr().clearBitmask(static_cast<uint8_t>(feat));}
        [[nodiscard]] bool isEnabled(){return regAddr().readBit(static_cast<uint8_t>(feat));}
    };

public:
    typedef struct {
        EepromMode eeprom_mode;
        EepromFeature feat;
    } config_t;

    struct{ //Interrupt
        void enable() {Regs::Eeprom::EepromControlReg.setBitmask(RegBits::Eeprom::EECR_EERIE);}
        void disable(){Regs::Eeprom::EepromControlReg.clearBitmask(RegBits::Eeprom::EECR_EERIE);}
        void attach(Callback cbFunc){cbEepromReadyCallback = cbFunc;}
        void detach() {cbEepromReadyCallback = nullptr;}
        void handle() {if(cbEepromReadyCallback) cbEepromReadyCallback();}
    }static EepromReadyInterrupt;
    static Eeprom::Feature<decltype(Regs::Eeprom::EepromControlReg), EepromFeature::MasterWrite>MasterWrite;
    static Eeprom::Feature<decltype(Regs::Eeprom::EepromControlReg), EepromFeature::Write>Write;
    static Eeprom::Feature<decltype(Regs::Eeprom::EepromControlReg), EepromFeature::Read>Read;
    static void setEepromMode(EepromMode mode){
        Regs::Eeprom::EepromControlReg.writeMasked(static_cast<uint8_t>(mode), ~bitmask_eecr_mode_sel_bits);
    }
    static EepromMode getEepromMode(){
        return static_cast<EepromMode>(Regs::Eeprom::EepromControlReg.getValue(bitmask_eecr_mode_sel_bits));
    }

    /* Overloaded write functions*/
    static bool write(uint16_t addr, uint8_t val){
        /* Disable interrupt*/
        AtomicBlock ab;
        
        /* Wait for completion of previous write*/
        while(Eeprom::isBusy());
        
        /* Set up address reg*/
        Regs::Eeprom::EepromAddressReg_HByte.setValue((uint8_t)(addr >> 8));
        Regs::Eeprom::EepromAddressReg_LByte.setValue((uint8_t)(addr));

        /* Set up data reg*/
        Regs::Eeprom::EepromDataReg.setValue(val);

        /* Start eeprom write by setting EEMPE and EEPE*/
        Regs::Eeprom::EepromControlReg.setBitmask(RegBits::Eeprom::EECR_EEMPE);
        Regs::Eeprom::EepromControlReg.setBitmask(RegBits::Eeprom::EECR_EEPE);
    }
    static bool write(uint16_t addr, uint16_t val){}
    static bool write(uint16_t addr, uint32_t val){}
    static bool write(uint16_t addr, uint64_t val){}

    /* Overloaded read functions*/
    static uint8_t read(){

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
    static void init(config_t* eepromCfg){
    
    }
};

}// namespace mcu

/* Eeprom Ready Interrupt*/
ISR(EE_READY_vect){mcu::Eeprom::EepromReadyInterrupt.handle();}

#endif //EEPROM_HPP