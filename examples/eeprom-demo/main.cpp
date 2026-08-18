#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/delay.h>
#include <HAL/serialstream.hpp>
#include <HAL/eeprom.hpp>

using eeprom = mcu::Peripherals::Eeprom;

struct DeviceSettings{
    uint8_t  backlightLevel;
    uint16_t defaultEncoderValue;
    bool     enabled;
    float    criticalBatteryLevel;
}__attribute__((packed)) defaultSettings;

void setup(){
    defaultSettings.criticalBatteryLevel = 2.76; //Volts
    defaultSettings.defaultEncoderValue  = 1000;
    defaultSettings.enabled              = false;
    defaultSettings.backlightLevel       = 127;
    
    DeviceSettings userSettings;
        userSettings.backlightLevel = 50;
        userSettings.criticalBatteryLevel = 3.0;
        userSettings.defaultEncoderValue = 500;
        userSettings.enabled = true;
}

int main(){

    setup();

    cstd::cout.init();

    cstd::cout << "Serial initialized successfully." << cstd::endl;
    mcu::Gpio::PinPB5::setLow();
    cstd::cout << "endl" << cstd::endl;
    cstd::cout << "test cstd::endl" << cstd::endl;

    uint16_t max_data = 512;
    while(true){
        for (size_t i = 0; i < max_data; i++){
            cstd::cout << i << " ";
            cstd::cout.printHex(eeprom::read<uint8_t>(i));
            cstd::cout << cstd::endl;
            _delay_ms(100);
        }
    };
}