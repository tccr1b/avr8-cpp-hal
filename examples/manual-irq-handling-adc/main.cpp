#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/delay.h>
#include "HAL/adc.hpp"
#include "HAL/gpio.hpp"

using builtInLed = mcu::Gpio::PinPB5;   /* LED is on pin PB5*/
using adc = mcu::Peripherals::Adc;

void adcInterruptRoutine(){
    uint8_t i = 11;
    while(i--){
        builtInLed::toggle();
        _delay_ms(100);
    }
}

int main(){
    /* enable interrupts*/
    sei();

    /* configure the pin (D13) which ArduinoUnoR3 built-in LED is connected to*/
    builtInLed::setPinMode(PinMode::Output);
    builtInLed::setLow();

    /* configure adc for reading on-chip temperature sensor (PDIP package) and initialize it*/
    adc::config_t adcConfig;
        adcConfig.adcAutoTrigEnable = false;
        adcConfig.adcChannel        = AdcChannel::InternalTempSensor;
        adcConfig.adcClockPrescaler = AdcClock::DividedBy128;   // 125kHz @ 16MHz
        adcConfig.adcReference      = AdcReference::Internal_1v1;
        adcConfig.adcResultAdjust   = AdcResultAdjust::Right;   // i.e. ADLAR setting
    adc::init(&adcConfig);

    /* attach function executed on interrupt request*/
    adc::ConversionCompletedInterrupt.attach(adcInterruptRoutine);

    /* keep reading adc*/
    while(true){
        auto raw_temp = adc::read();
        _delay_ms(2000);
    }

    return 0;
}