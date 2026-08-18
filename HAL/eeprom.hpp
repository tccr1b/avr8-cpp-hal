
#ifndef EEPROM_HPP
#define EEPROM_HPP

#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/interrupt.h>
#include <stddef.h>
#include <avr/crc16.h>

#include "HAL/registers.hpp"
#include "HAL/utils/atomicblock.hpp"

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
namespace Peripherals{
class Eeprom{
private:
    constexpr static uint8_t bitmask_eecr_mode_sel_bits = RegBits::Eeprom::EECR_EEPM0 | RegBits::Eeprom::EECR_EEPM1;
    using Callback = void(*)();
    inline static Callback cbEepromReadyCallback = nullptr;
    [[gnu::always_inline, flatten]] static inline bool isBusy(){
        return Regs::Eeprom::EepromControlReg.readBit(RegBits::Eeprom::EECR_EEPE);
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
        {AtomicBlock ab;
            Regs::Eeprom::EepromAddressReg_HByte.setValue((uint8_t)(uiAddress >> 8));
            Regs::Eeprom::EepromAddressReg_LByte.setValue((uint8_t)uiAddress);
            /* Start eeprom read by writing EERE */
            Regs::Eeprom::EepromControlReg.setBitmask(RegBits::Eeprom::EECR_EERE);
        }
        /* Return data from Data Register */
        return Regs::Eeprom::EepromDataReg.getValue();
    }
    static void     updateByte(uint16_t uiAddress, uint8_t uiData){
        if(readByte(uiAddress) != uiData){writeByte(uiAddress, uiData);}
    }

public:
    struct{ //Interrupt
        public:
            void enable() {Regs::Eeprom::EepromControlReg.setBitmask(RegBits::Eeprom::EECR_EERIE);}
            void disable(){Regs::Eeprom::EepromControlReg.clearBitmask(RegBits::Eeprom::EECR_EERIE);}
            void attach(Callback cbFunc){cbEepromReadyCallback = cbFunc;}
            void detach() {cbEepromReadyCallback = nullptr;}
        private:
            static void __attribute__((flatten, signal(EE_READY_vect_num))) irqHandler(){
                if(cbEepromReadyCallback) cbEepromReadyCallback();
            }
    }static EepromReadyInterrupt;

    static EepromMode getEepromMode(){
        return static_cast<EepromMode>(Regs::Eeprom::EepromControlReg.getValue(bitmask_eecr_mode_sel_bits));
    }

    static void setEepromMode(EepromMode mode){
        Regs::Eeprom::EepromControlReg.writeMasked(static_cast<uint8_t>(mode), bitmask_eecr_mode_sel_bits);
    }

    template<typename T> 
    static void write(uint16_t romAddress, const T& tData){
        const uint8_t* ptrData = reinterpret_cast<const uint8_t**>(&tData);
        for(size_t i = 0; i < sizeof(T); ++i){writeByte(romAddress+i, ptrData[i]);}
    }

    template<typename T> 
    static void read(uint16_t romAddress, T& tData){
        uint8_t* ptrData = reinterpret_cast<uint8_t*>(&tData);
        for (size_t i=0; i<sizeof(T); ++i){ptrData[i] = readByte(romAddress+i);}
    }
    
    template<typename T> 
    static T    read(uint16_t romAddress){
        /* temporary variable on RAM*/
        T tRomData;
        read(romAddress, tRomData);
        return tRomData;
    }
    
    template<typename T> 
    static void update(uint16_t romAddress, const T& tData){
        const uint8_t* ptrData = reinterpret_cast<const uint8_t*>(&tData);
        for (size_t i=0; i<sizeof(T); ++i){updateByte(romAddress+i, ptrData[i]);}
    }

};


/*
 * @brief Tekil EEPROM değişkenleri için akıllı sarmalayıcı (Wrapper) sınıf.
 * 
 * @details EEPROM'daki bir veriyi, sanki normal bir RAM değişkeniymiş gibi (=) atama operatörü 
 * ile okuyup yazmanızı sağlar. Arka planda 'update' mantığıyla çalıştığı için, sadece değer 
 * gerçekten değiştiğinde donanıma yazma işlemi yapar ve EEPROM (100.000) ömrünü korur.
 * 
 * @tparam T EEPROM'da tutulacak verinin tipi (Örn: uint16_t, float, ayar_struct_t)
 */
template<typename T>
class EepromVariable{
private:
    uint16_t m_RomAddress;
public:
    constexpr EepromVariable(uint16_t uiRomAddress) : m_RomAddress(uiRomAddress){}
    EepromVariable& operator=(const T& tValue){
        mcu::Eeprom::update(m_RomAddress, tValue);
        return *this;
    }
    operator T() const{
        return mcu::Eeprom::read<T>(m_RomAddress);
    }
    T operator+=(const T& tValue){
        T tCurrent = mcu::Eeprom::read<T>(m_RomAddress);
        tCurrent += tValue;
        mcu::Eeprom::update(m_RomAddress, tCurrent);
        return tCurrent;
    }
};

/**
 * @brief EEPROM üzerindeki dizileri ve metinleri (string) yönetmek için sarmalayıcı sınıf.
 * 
 * @details RAM dizilerinde olduğu gibi [] indeks operatörü ile doğrudan ilgili EEPROM adresine 
 * erişim sağlar (Proxy Pattern). Performans gerektiren durumlarda tüm diziyi tek seferde 
 * okumak/yazmak için readAll() ve updateAll() metotları içerir.
 * 
 * @tparam T Dizinin eleman veri tipi (Örn: char, float)
 * @tparam N Dizinin maksimum eleman sayısı (Derleme zamanı buffer-overflow koruması sağlar)
 */
template<typename T, size_t N>
class EepromArray {
private:
    uint16_t m_BaseAddr;

public:
    // Kurucu: Dizinin başlangıç adresini atar
    constexpr EepromArray(uint16_t addr) : m_BaseAddr(addr) {}

    // --- VEKİL (PROXY) SINIF ---
    // operator[] kullanıldığında bellek referansı yerine bu nesne döner.
    class ElementProxy {
    private:
        uint16_t m_ElementAddr;

    public:
        constexpr ElementProxy(uint16_t addr) : m_ElementAddr(addr) {}

        // Dizi elemanına yazma yapıldığında tetiklenir
        ElementProxy& operator=(const T& value) {
            mcu::Eeprom::update(m_ElementAddr, value);
            return *this;
        }

        // Dizi elemanı okunduğunda tetiklenir
        operator T() const {
            return mcu::Eeprom::read<T>(m_ElementAddr);
        }
        
        T operator+=(const T& value) {
            T current = mcu::Eeprom::read<T>(m_ElementAddr);
            current += value;
            mcu::Eeprom::update(m_ElementAddr, current);
            return current;
        }
    };

    // --- DİZİ OPERATÖRLERİ VE METOTLARI ---

    // 1. İNDEKS OPERATÖRÜ (Örn: dizi[2])
    // İstenen indeksin EEPROM'daki fiziksel adresini hesaplar ve vekil sınıf döndürür
    ElementProxy operator[](size_t index) {
        uint16_t target_address = m_BaseAddr + (index * sizeof(T));
        return ElementProxy(target_address);
    }

    // 2. KAPASİTE BİLGİSİ
    constexpr size_t size() const {
        return N;
    }

    // 3. BLOK OKUMA (Yüksek Performans)
    // Tüm diziyi tek donanım bloğunda güvenli boyuttaki RAM dizisine aktarır
    void readAll(T (&ram_buffer)[N]) const {
        mcu::Eeprom::read(m_BaseAddr, ram_buffer);
    }

    // 4. BLOK YAZMA (Yüksek Performans)
    // RAM'deki bir diziyi tek hamlede EEPROM'a günceller (Sadece değişenleri yazar)
    void updateAll(const T (&ram_buffer)[N]) {
        mcu::Eeprom::update(m_BaseAddr, ram_buffer);
    }
};

template<typename T>
struct EepromDataBlock{
    private:
        uint16_t m_BaseAddress;
    public:
        T tRamData;
        constexpr EepromDataBlock(uint16_t addr) : m_BaseAddress(addr){}
        void load(){mcu::Eeprom::read (m_BaseAddress, tRamData);}
        void save(){mcu::Eeprom::write(m_BaseAddress, tRamData);}
}__attribute__((packed));

/**
 * @brief Elektrik kesintilerine karşı CRC korumalı güvenli EEPROM veri bloğu.
 * 
 * @details Tüm ayarları (struct) RAM'de geçici olarak tutar ve tek seferde hesaplanan CRC16 
 * (Checksum) ile birlikte EEPROM'a yazar. Cihaz açılışında EEPROM verisinin yarıda kesilip 
 * kesilmediğini (corrupt) denetler ve sistemi çökmekten kurtarır.
 * 
 * @tparam T Korunacak verilerin bulunduğu ve paketlenmiş (packed) Struct veri tipi
 */
template<typename T>
struct EepromSecureDataBlock{
private:
    uint16_t m_DataAddr;
    uint16_t m_CrcAddr;

    uint16_t calculateCRC() const{
        uint16_t crc = 0xFFFF;
        const uint8_t* ptrRamData = reinterpret_cast<const uint8_t*>(&ramData);
        for(size_t i = 0; i < sizeof(T); ++i){
            crc = _crc16_update(crc, ptrRamData[i]);
        }
        return crc;
    }
public:
    T ramData;
    constexpr EepromSecureDataBlock(uint16_t data_addr, uint16_t crc_addr) : m_DataAddr(data_addr), m_CrcAddr(crc_addr){}
    bool loadAndVerify(){
        mcu::Eeprom::read(m_DataAddr, ramData);
        uint16_t stored_crc = mcu::Eeprom::read<uint16_t>(m_CrcAddr);
        return (calculateCRC() == stored_crc) ? true : false;
    }

    void save(){
        mcu::Eeprom::update(m_DataAddr, ramData);
        uint16_t new_crc = calculateCRC();
        mcu::Eeprom::update(m_CrcAddr, new_crc);
    }
};

/**
 * ==============================================================================
 * KULLANIM REHBERI: EEPROM BELLEK HARITASI (EEPROM MEMORY MAP)
 * ==============================================================================
 * EEPROM adreslerini (0x00, 0x05 gibi) elle girmek adres çakışmalarına yol açar.
 * Bu kütüphaneyi kullanırken, projenizin (main.cpp veya config.hpp) bir yerinde 
 * aşağıdaki gibi sanal bir struct (Harita) oluşturmanız şiddetle tavsiye edilir.
 * 
 * ÖRNEK KULLANIM:
 * 
 * struct AppEepromMap {
 *     uint32_t bootCounter;       // offset: 0x00
 *     float    calibrationData;   // offset: 0x04
 *     uint16_t crcCheck;          // offset: 0x08
 * } __attribute__((packed));      // <- Padding'i kapatmak ZORUNLUDUR!
 * 
 * EepromVariable<uint32_t> eepBootCount( offsetof(AppEepromMap, bootCounter) );
 * ==============================================================================
 */
}// namespace Peripherals
}// namespace mcu

#endif //EEPROM_HPP