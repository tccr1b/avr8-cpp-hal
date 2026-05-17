/*
MyProject/
├── Board.hpp           (Donanım Ayarları: CS pini burada seçilecek)
├── HAL/
│   ├── Register.hpp    (Adresler)
│   ├── Gpio.hpp        (Pin Kontrolü)
│   └── Spi.hpp         (SPI İletişimi)
├── Drivers/
│   └── SdCard.hpp      (SD Kart Mantığı - YENİ)
└── main.cpp            (Uygulama)
*/

#ifndef SPI_HPP
#define SPI_HPP

#define __AVR_ATmega328P__ // İş bitince bunu sil
#include <inttypes.h>
#include "registers.hpp"
#include "gpio.hpp"
#include "utils/ring_buffer.hpp"
#include <avr/interrupt.h>

#define TX_BUFFER_SIZE 32
#define RX_BUFFER_SIZE 32

using namespace mcu;

enum class SpiClock         : uint8_t {
    /*SPI2X ve SPR0 ve SPR1 bitleri farklı registerlerde bulunduğu için, SPI2X bitini aynı değişkende 2 bit sola kaydırdık (0x04)*/
    DividedBy2   = (RegBits::Spi::SPSR_SPI2X << 2),   // 0b0000 0001 --> 0b0000 0100(mask) 0x04
    DividedBy4   = 0x00,
    DividedBy8   = (RegBits::Spi::SPSR_SPI2X << 2) | RegBits::Spi::SPCR_SPR0, //0b0000 0101
    DividedBy16  = RegBits::Spi::SPCR_SPR0,
    DividedBy32  = (RegBits::Spi::SPSR_SPI2X << 2) | RegBits::Spi::SPCR_SPR1,
    DividedBy64  = RegBits::Spi::SPCR_SPR1,
    DividedBy128 = RegBits::Spi::SPCR_SPR0 | RegBits::Spi::SPCR_SPR1,
};
enum class SpiClockPolarity : uint8_t {
    /*CPOL is set to high (Default: High)*/
    idleHigh = 1,
    /*CPOL is set to low*/
    idleLow  = 0,
};
enum class SpiClockPhase    : uint8_t {
    sampleOnLeadingEdge   = 0,
    sampleOnTrailingEdge  = 1
};
enum class SpiDataOrder     : uint8_t {
    lsbFirst    = 1,
    msbFirst    = 0,
};
enum class SpiMode          : uint8_t {
    Slave   = 0,
    Master  = 1
};
enum class SpiDataMode      : uint8_t {
    Mode0 = 0x00,
    Mode1 = RegBits::Spi::SPCR_CPHA,
    Mode2 = RegBits::Spi::SPCR_CPOL,
    Mode3 = RegBits::Spi::SPCR_CPHA | RegBits::Spi::SPCR_CPOL,
};

namespace mcu{
namespace Peripherals{
class Spi{
private:
    using Callback = void(*)();
    inline static Callback cbTransferCompletedCallback = nullptr;
    inline static HAL::RingBuffer<uint8_t, TX_BUFFER_SIZE> txBuffer;
    inline static HAL::RingBuffer<uint8_t, RX_BUFFER_SIZE> rxBuffer;

    static void setClockPolarity(SpiClockPolarity clkPolarity){
        if(static_cast<bool>(clkPolarity)){
            Regs::Spi::SpiControlReg.setBitmask(RegBits::Spi::SPCR_CPOL);
        }else{
            Regs::Spi::SpiControlReg.clearBitmask(RegBits::Spi::SPCR_CPOL);
        }
    }
    static void setClockPhase(SpiClockPhase clkPhase){
        if(static_cast<bool>(clkPhase)){
            Regs::Spi::SpiControlReg.setBitmask(RegBits::Spi::SPCR_CPHA);
        }else{
            Regs::Spi::SpiControlReg.clearBitmask(RegBits::Spi::SPCR_CPHA);
        }
    }
    inline static bool isBusy(){
        return !Regs::Spi::SpiStatusReg.readBit(RegBits::Spi::SPSR_SPIF);
    }
public:
    struct{ //SPI Interrupt
        void enable() {Regs::Spi::SpiControlReg.setBitmask(RegBits::Spi::SPCR_SPIE);}
        void disable(){Regs::Spi::SpiControlReg.clearBitmask(RegBits::Spi::SPCR_SPIE);}
        void attach(Callback cbFunc){cbTransferCompletedCallback = cbFunc; this->enable();}
        void detach() {cbTransferCompletedCallback = nullptr; this->disable();}
        void handle() {if(cbTransferCompletedCallback) cbTransferCompletedCallback();}
    }static TransferCompletedInterrupt;
    inline static bool isEnabled(){
        return Regs::Spi::SpiControlReg.readBit(RegBits::Spi::SPCR_SPE) && 
               !Regs::Core::PowerReductionReg.readBit(RegBits::Core::PRR_PRSPI);
    }
    inline static void disable(){
        Regs::Spi::SpiControlReg.clearBitmask(RegBits::Spi::SPCR_SPE);
    }
    inline static void enable(){
        /* Disable power reduction for SPI*/
        mcu::Regs::Core::PowerReductionReg.clearBitmask(RegBits::Core::PRR_PRSPI);
        /* Enable SPI*/
        Regs::Spi::SpiControlReg.setBitmask(RegBits::Spi::SPCR_SPE);
    }
    static void init(SpiMode spiMode = SpiMode::Master,
                     SpiClock speed = SpiClock::DividedBy64,
                     SpiDataMode spiDataMode = SpiDataMode::Mode0){
        switch (spiMode){
        case SpiMode::Slave:
            /* Configure SPI pins for slave mode*/
            mcu::Gpio::PinMISO::setPinMode(PinMode::Output);
            mcu::Gpio::PinMOSI::setPinMode(PinMode::Input);
            mcu::Gpio::PinSCK::setPinMode(PinMode::Input);
            mcu::Gpio::PinSS::setPinMode(PinMode::Input);
            break;
        case SpiMode::Master:
            /* Configure SPI pins for master mode. Avoid configuring SS pin as input*/
            mcu::Gpio::PinMISO::setPinMode(PinMode::Input);
            mcu::Gpio::PinMOSI::setPinMode(PinMode::Output);
            mcu::Gpio::PinSCK::setPinMode(PinMode::Output);

            /* If SS is configured as an output, the pin is a general output pin which 
            does not affect the SPI system.*/
            mcu::Gpio::PinSS::setPinMode(PinMode::Output);

            uint8_t speedMask = static_cast<uint8_t>(speed);
            if(speedMask & 0x04){
                Regs::Spi::SpiStatusReg.setBitmask(RegBits::Spi::SPSR_SPI2X);
                speedMask &= ~0x04;
                Regs::Spi::SpiControlReg.writeBitmask((0xFF << 2) | speedMask);     
            }else{
                Regs::Spi::SpiStatusReg.clearBitmask(RegBits::Spi::SPSR_SPI2X);
                Regs::Spi::SpiControlReg.writeBitmask((0xFF << 2) | speedMask);
            }
            break;
        }

        /* SPI modülünü Slave veya Master olarak ayarla*/
        setSpiMode(spiMode);

        /* SPI data modunu ayarla*/
        setDataMode(spiDataMode);

        /* SPI modülünü etkinleştir*/
        enable();
    }
    static uint8_t transfer(uint8_t data){
        Regs::Spi::SpiDataReg.setValue(data);
        while (Spi::isBusy()){}
        return Regs::Spi::SpiDataReg;
    }
    static void setDataOrder(SpiDataOrder dataOrder){
        if (static_cast<bool>(dataOrder)){
            Regs::Spi::SpiControlReg.setBitmask(RegBits::Spi::SPCR_DORD);
        }else{
            Regs::Spi::SpiControlReg.clearBitmask(RegBits::Spi::SPCR_DORD);
        }
    }
    static void setSpiClockPrescaler(SpiClock spiSpeed){
        uint8_t speedMask = static_cast<uint8_t>(spiSpeed);
        if(speedMask & 0x04){
            Regs::Spi::SpiStatusReg.setBitmask(RegBits::Spi::SPSR_SPI2X);
            speedMask &= ~0x04;
            Regs::Spi::SpiControlReg.writeBitmask((0xFF << 2) | speedMask);     
        }else{
            Regs::Spi::SpiStatusReg.clearBitmask(RegBits::Spi::SPSR_SPI2X);
            Regs::Spi::SpiControlReg.writeBitmask((0xFF << 2) | speedMask);
        }
    }
    static void setSpiMode(SpiMode spiMode){
        static_cast<uint8_t>(spiMode) ? Regs::Spi::SpiControlReg.setBitmask(RegBits::Spi::SPCR_MSTR): 
                                        Regs::Spi::SpiControlReg.clearBitmask(RegBits::Spi::SPCR_MSTR);
    }
    static void setDataMode(SpiDataMode dataMode){
        constexpr uint8_t bitmask_spcr_data_mode_bits = ~(RegBits::Spi::SPCR_CPHA | RegBits::Spi::SPCR_CPOL);
        Regs::Spi::SpiControlReg.writeMasked(static_cast<uint8_t>(dataMode), bitmask_spcr_data_mode_bits);
    }
/* interrupt driven?*/
    static inline uint8_t transferByte(uint8_t data){
//        uint8_t incomingData = mcu::Regs::Spi::SpiDataReg;
        rxBuffer.push(mcu::Regs::Spi::SpiDataReg);
        while(txBuffer.isFull()){}
        txBuffer.push(data);

    }
};
} // namespace Peripherals
} // namespace mcu

/* SPI Transfer Complete interrupt*/
ISR(SPI_STC_vect){mcu::Peripherals::Spi::TransferCompletedInterrupt.handle();}

#endif //SPI_HPP