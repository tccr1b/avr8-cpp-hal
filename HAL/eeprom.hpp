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

/* Önemli Donanım/Derleyici Notu: 
C++ derleyicileri, işlemci hızını optimize etmek için struct içerisindeki değişkenlerin 
arasına görünmez boşluklar (memory padding) ekleyebilir. Bu boşluklar RAM'de rastgele (çöp) veriler içerir. Eğer struct'ını 
doğrudan EEPROM'a yazarsan, bu çöp padding verileri her seferinde değişebileceği için update fonksiyonu bunları farklı veri 
sanıp gereksiz yere EEPROM'a yazma yapabilir. Bunu engellemek için EEPROM'a yazılacak struct'larının sonuna her zaman 
__attribute__((packed)) ekleyerek derleyicinin boşluk bırakmasını engellemelisin.
*/

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
    [[gnu::always_inline, flatten]] static inline bool isBusy(){
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
    private:
        static void __attribute__((flatten, signal(EE_READY_vect_num))) irqHandler(){
            if(cbEepromReadyCallback) cbEepromReadyCallback();
        }
    }static EepromReadyInterrupt;
    static Eeprom::Feature<decltype(Regs::Eeprom::EepromControlReg), EepromFeature::MasterWrite>MasterWrite;
    static Eeprom::Feature<decltype(Regs::Eeprom::EepromControlReg), EepromFeature::Write>      Write;
    static Eeprom::Feature<decltype(Regs::Eeprom::EepromControlReg), EepromFeature::Read>       Read;

    static EepromMode getEepromMode(){
        return static_cast<EepromMode>(Regs::Eeprom::EepromControlReg.getValue(bitmask_eecr_mode_sel_bits));
    }

    static void     setEepromMode(EepromMode mode){
        Regs::Eeprom::EepromControlReg.writeMasked(static_cast<uint8_t>(mode), ~bitmask_eecr_mode_sel_bits);
    }

    static bool     writeByte(uint16_t uiAddress, uint8_t data){
        while(Eeprom::isBusy());
        
        {AtomicBlock ab;
            Regs::Eeprom::EepromAddressReg_HByte.setValue((uint8_t)(uiAddress >> 8));
            Regs::Eeprom::EepromAddressReg_LByte.setValue((uint8_t)uiAddress);
            Regs::Eeprom::EepromDataReg.setValue(data);

            /* Write logical one to EEMPE */
            Regs::Eeprom::EepromControlReg.setBitmask(RegBits::Eeprom::EECR_EEMPE);
            /* Start eeprom write by setting EEPE */
            Regs::Eeprom::EepromControlReg.setBitmask(RegBits::Eeprom::EECR_EEPE);
        }
    }

    static uint8_t  readByte(uint16_t uiAddress){
        /* Wait for completion of previous write */
        while(Eeprom::isBusy());
        /* Set up address register */
        Regs::Eeprom::EepromAddressReg_HByte.setValue((uint8_t)(uiAddress >> 8));
        Regs::Eeprom::EepromAddressReg_LByte.setValue((uint8_t)uiAddress);
        
        /* Start eeprom read by writing EERE */
        Regs::Eeprom::EepromControlReg.setBitmask(RegBits::Eeprom::EECR_EERE);
        
        /* Return data from Data Register */
        return Regs::Eeprom::EepromDataReg.getValue();
    }

    static void     updateByte(uint16_t uiAddress, uint8_t uiData){
        if(readByte(uiAddress) != uiData){writeByte(uiAddress, uiData);}
    }

    template<typename T> static void write(uint16_t uiAddress, const T& tData){
        const uint8_t* ptrData = reinterpret_cast<const uint8_t**>(&tData);
        for(size_t i = 0; i < sizeof(T); ++i){writeByte(uiAddress+1, ptrData[i]);}
    }

    template<typename T> static void read(uint16_t uiAddress, T& tData){
        uint8_t* ptrData = reinterpret_cast<uint8_t*>(&tData);
        for (size_t i=0; i<sizeof(T); ++i){ptrData[i] = readByte(uiAddress+1);}
    }
    
    template<typename T> static T    read(uint16_t uiRomAddress){
        /* temporary variable on RAM*/
        T tRomData;
        read(uiRomAddress, tRomData);
        return tRomData;
    }
    
    template<typename T> static void update(uint16_t uiAddress, const T& tData){
        const uint8_t* ptrData = reinterpret_cast<const uint8_t*>(&tData);
        for (size_t i=0; i<sizeof(T); ++i){updateByte(uiAddress+1, ptrData[i]);}
    }

    static void init(config_t* eepromCfg){
        setEepromMode(eepromCfg->eeprom_mode);
    }


};

template<typename T>
class EepromVariable{
private:
    uint16_t m_uiRomAddress;
public:
    constexpr EepromVariable(uint16_t uiRomAddress) : m_uiRomAddress(uiRomAddress){}
    EepromVariable& operator=(const T& tValue){
        mcu::Eeprom::update(m_uiRomAddress, tValue);
        return *this;
    }
    operator T() const{
        return mcu::Eeprom::read<T>(m_uiRomAddress);
    }
    T operator+=(const T& tValue){
        T tCurrent = mcu::Eeprom::read<T>(m_uiRomAddress);
        tCurrent += tValue;
        mcu::Eeprom::update(m_uiRomAddress, tCurrent);
        return tCurrent;
    }
};

}// namespace mcu

#endif //EEPROM_HPP

void foo(){
    EepromVariable<uint16_t> evMax(0x20);
    evMax = 12;
}