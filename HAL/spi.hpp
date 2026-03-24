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
#include <avr/interrupt.h>

using namespace mcu;

enum class SpiClockSpeed    : uint8_t {
    /*SPI2X ve SPR0 ve SPR1 bitleri farklı registerlerde bulunduğu için, SPI2X bitini aynı değişkende 2 bit sola kaydırdık (0x04)*/
    dividedBy2   = (RegBits::Spi::SPSR_SPI2X << 2),   // 0b0000 0001 --> 0b0000 0100(mask) 0x04
    dividedBy4   = 0x00,
    dividedBy8   = (RegBits::Spi::SPSR_SPI2X << 2) | RegBits::Spi::SPCR_SPR0, //0b0000 0101
    dividedBy16  = RegBits::Spi::SPCR_SPR0,
    dividedBy32  = (RegBits::Spi::SPSR_SPI2X << 2) | RegBits::Spi::SPCR_SPR1,
    dividedBy64  = RegBits::Spi::SPCR_SPR1,
    dividedBy128 = RegBits::Spi::SPCR_SPR0 | RegBits::Spi::SPCR_SPR1,
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
    Mode0 = 0,
    Mode1 = 1,
    Mode2 = 2,
    Mode3 = 3
};

namespace mcu{
namespace Peripherals{
class Spi{
private:
    using Callback = void(*)();
    static inline Callback interruptCallback = nullptr;
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

public:
    static void attachInterrupt(Callback cbFunc){interruptCallback = cbFunc;}
    static void detachInterrupt(){interruptCallback = nullptr;}
    /* For ISR, do not use.*/
    static void interruptHandler(){if(interruptCallback) interruptCallback();}
    static void init(SpiClockSpeed speed = SpiClockSpeed::dividedBy64, 
                     SpiMode spiMode = SpiMode::Master,
                     SpiDataMode spiDataMode = SpiDataMode::Mode0){
        switch (spiMode){
        case SpiMode::Slave:
            Regs::Gpio::DataDirectionRegB.setBitmask(RegBits::Gpio::PortB::PB_4);
            Regs::Gpio::DataDirectionRegB.clearBitmask(RegBits::Gpio::PortB::PB_3 |
                                                       RegBits::Gpio::PortB::PB_5 |
                                                       RegBits::Gpio::PortB::PB_2);
            //SPI modülünü Slave olarak ayarla
            Regs::Spi::SpiControlReg.clearBitmask(RegBits::Spi::SPCR_MSTR);
            //SPI modülünü etkinleştir
            Regs::Spi::SpiControlReg.setBitmask(RegBits::Spi::SPCR_SPE);
            break;
        case SpiMode::Master:
            Regs::Gpio::DataDirectionRegB.setBitmask(RegBits::Gpio::PortB::PB_3 |   //MOSI
                                                     RegBits::Gpio::PortB::PB_5 |   //SCK
                                                     RegBits::Gpio::PortB::PB_2);   //SS
            Regs::Gpio::DataDirectionRegB.clearBitmask(RegBits::Gpio::PortB::PB_4); //MISO

            Regs::Gpio::PortRegB.setBitmask(RegBits::Gpio::PortB::PB_2 |  //SS
                                            RegBits::Gpio::PortB::PB_4);  //MISO

            Regs::Spi::SpiControlReg.setBitmask(RegBits::Spi::SPCR_MSTR);    //SPI modülünü Master olarak ayarla

            uint8_t speedMask = static_cast<uint8_t>(speed);
            if(speedMask & 0x04){
                Regs::Spi::SpiStatusReg.setBitmask(RegBits::Spi::SPSR_SPI2X);
                speedMask &= ~0x04;
                // 0xFF (0b1111 1111) --> (0b1111 1100) | 0b0000 00xx --> 0b1111 11xx & SPCR
                Regs::Spi::SpiControlReg.writeBitmask((0xFF << 2) | speedMask);     
            }else{
                Regs::Spi::SpiStatusReg.clearBitmask(RegBits::Spi::SPSR_SPI2X);
                Regs::Spi::SpiControlReg.writeBitmask((0xFF << 2) | speedMask);
            }
            //SPI modülünü etkinleştir
            Regs::Spi::SpiControlReg.setBitmask(RegBits::Spi::SPCR_SPE);
            break;
        }
    }
    static uint8_t transfer(uint8_t data){
        Regs::Spi::SpiDataReg.setValue(data);
        while (!Regs::Spi::SpiStatusReg.readBit(RegBits::Spi::SPSR_SPIF)){}
        return Regs::Spi::SpiDataReg;
    }
    static void setDataOrder(SpiDataOrder dataOrder){
        if (static_cast<bool>(dataOrder)){
            Regs::Spi::SpiControlReg.setBitmask(RegBits::Spi::SPCR_DORD);
        }else{
            Regs::Spi::SpiControlReg.clearBitmask(RegBits::Spi::SPCR_DORD);
        }
    }
    static void setTransferSpeed(SpiClockSpeed spiSpeed){
        uint8_t speedMask = static_cast<uint8_t>(spiSpeed);
        if(speedMask & 0x04){
            Regs::Spi::SpiStatusReg.setBitmask(RegBits::Spi::SPSR_SPI2X);
            speedMask &= ~0x04;
            // 0xFF (0b1111 1111) --> (0b1111 1100) | 0b0000 00xx --> 0b1111 11xx & SPCR
            Regs::Spi::SpiControlReg.writeBitmask((0xFF << 2) | speedMask);     
        }else{
            Regs::Spi::SpiStatusReg.clearBitmask(RegBits::Spi::SPSR_SPI2X);
            Regs::Spi::SpiControlReg.writeBitmask((0xFF << 2) | speedMask);
        }
    }
    static void setSpiMode(SpiMode spiMode){
        switch (spiMode){
        case SpiMode::Slave:  init(SpiClockSpeed::dividedBy64, SpiMode::Slave); break;
        case SpiMode::Master: init(SpiClockSpeed::dividedBy64, SpiMode::Master); break;
        }
    }
    static void setDataMode(SpiDataMode dataMode){
        switch (dataMode){
        case SpiDataMode::Mode0:
            setClockPhase(SpiClockPhase::sampleOnLeadingEdge);
            setClockPolarity(SpiClockPolarity::idleLow);
            break;
        case SpiDataMode::Mode1:
            setClockPhase(SpiClockPhase::sampleOnLeadingEdge);
            setClockPolarity(SpiClockPolarity::idleHigh);
            break;
        case SpiDataMode::Mode2:
            setClockPhase(SpiClockPhase::sampleOnTrailingEdge);
            setClockPolarity(SpiClockPolarity::idleLow);
            break;
        case SpiDataMode::Mode3:
            setClockPhase(SpiClockPhase::sampleOnTrailingEdge);
            setClockPolarity(SpiClockPolarity::idleHigh);
            break;
        default:
            break;
        };
    }
    static void enableSpiInterrupt(){Regs::Spi::SpiControlReg.setBitmask(RegBits::Spi::SPCR_SPIE);}
    static void disableSpiInterrupt(){Regs::Spi::SpiControlReg.clearBitmask(RegBits::Spi::SPCR_SPIE);}

};
} // namespace Peripherals
} // namespace mcu

/* SPI Transfer Complete interrupt*/
ISR(SPI_STC_vect){mcu::Peripherals::Spi::interruptHandler();}

#endif //SPI_HPP