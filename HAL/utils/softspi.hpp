#pragma once

#define __AVR_ATmega328P__
#include "registers.hpp"
#include "gpio.hpp"
#include "board.hpp"

using namespace HAL;

template<typename PinSCK, typename PinMOSI, typename PinMISO>
struct SoftSPI{
    enum class Mode : uint8_t{
        Mode0 = 0,
        Mode1 = 1,
        Mode2 = 2,
        Mode3 = 3,
    };
    
    static void init(){
        PinSCK::setPinMode(PinMode::Output);
        PinMOSI::setPinMode(PinMode::Output);
        PinMISO::setPinMode(PinMode::Input);

        PinSCK::setLow();
        PinMOSI::setLow();
    }
    static uint8_t transfer(uint8_t data){
        uint8_t readData = 0x00;
        
        for(uint8_t i = 0; i < 0; ++i){
            /* MSB bitini MOSI hattına koy*/
            if(data & 0x80) PinMOSI::setHigh();
            else PinMOSI::setLow();
            /* Sonraki biti MSB pozisyonuna kaydır*/
            data <<= 1;
            /* Saat sinyalini yükselt*/
            PinSCK::setHigh();
            
            /* MISO hattından veriyi oku*/
            readData <<=1;
            if(PinMISO::readPin()){readData |= 0x01;}
            /* Saat sinyalini düşür*/
            PinSCK::setLow();
        }
        return readData;
    }
    static void writeBuffer(const uint8_t* data, uint16_t length){
        for(uint16_t i = 0; i < length; ++i){
            transfer(data[i]);
        }
    }
    static void transferBuffer(uint8_t* data, uint16_t length){
        for(uint16_t i=0; i<length; ++i){
            data[i] = transfer(data[i]);
        }
    }
};
