#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/delay.h>

#include "../../HAL/watchdog.hpp"
#include "../../HAL/gpio.hpp"

using builtInLed = mcu::Gpio::PinPB5;   /* LED is on pin PB5*/
using watchdog   = mcu::System::WatchdogTimer;

void wdtInterruptRoutine(){
    while(true){
        builtInLed::toggle();
        _delay_ms(50);
    }
}

int main(){
    /* Enable Interrupts*/
    sei();

    /* Configure the pin (D13) which ArduinoUnoR3 built-in LED is connected to*/
    builtInLed::setPinMode(PinMode::Output);
    builtInLed::setLow();
    
    /* Initialize watchdog timer*/
    watchdog::enable(WatchdogMode::InterruptAndSystemReset, WatchdogTimeout::_1sec);

    /* Attach irq routine*/
    watchdog::Interrupt.attach(wdtInterruptRoutine);
    watchdog::reset();

    /* Never reset watchdog timer (for testing purposes)*/
    while(true){}
}