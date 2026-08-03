#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <HAL/eeprom.hpp>

using eeprom = mcu::Eeprom;

int main(){
    EepromVariable<uint8_t >a =0;
    EepromVariable<uint16_t>b =1;
    
    while(true){};
}