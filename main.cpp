
#define __AVR_ATmega328P__

#include <util/delay.h>
#include <avr/interrupt.h>
#include "board.hpp"
#include "HAL/watchdog.hpp"
//#include "HAL/sysctrl.hpp"
#include "HAL/serialstream.hpp"
//#include "HAL/spi.hpp"
//#include "HAL/adc.hpp"
//#include "HAL/twi.hpp"
//#include "HAL/utils.hpp"
//#include "HAL/eeprom.hpp"
#include "HAL/timers.hpp"

using namespace mcu;
using namespace HAL;

void sysReset(){
    static uint8_t resetCount = 0;
    builtInLED::toggle();
    cstd::cout << "WatchdogTimer did not stop! " << resetCount++ << cstd::endl;
    if(resetCount >= 11){ 
        mcu::System::WatchdogTimer::setMode(WatchdogMode::InterruptAndSystemReset);
        resetCount = 0;
        return;
    }
    if(mcu::System::WatchdogTimer::getMode() != WatchdogMode::Interrupt)
        cstd::cout << "Generating system reset..." << cstd::endl;
    _delay_ms(20);
    mcu::System::WatchdogTimer::setMode(WatchdogMode::Interrupt);
    cstd::cout << "System reset disabled..." << cstd::endl;
    cstd::cout << cstd::endl;
    builtInLED::toggle();
}


int main (){    

    mcu::System::Clock::setClockPrescaler(Prescaler::NoDivision);

    _delay_ms(10);
    mcu::Peripherals::Usart::init(UsartMode::Asynchronous,
                                  UsartBaudrate::_9600bps,
                                  UsartDataSize::_8bit,
                                  UsartStopBits::One,
                                  UsartParityMode::Disabled);
    
    mcu::System::WatchdogTimer::attachInterrupt(sysReset);
    mcu::System::WatchdogTimer::enable(WatchdogMode::InterruptAndSystemReset, WatchdogTimeout::_4sec);
    sei();
    mcu::System::WatchdogTimer::disable();

    togglingPin::setPinMode(HAL::PinMode::Output);

    // FASTPWM_WITHOCR
//    using t0 = mcu::Timers::Timer0;
//    t0::init(t0::Mode::FastPWM_WithOCR, t0::Clock::NoPrescaling);
//    t0::enableOutputComparePinB();
//    t0::enableOutputComparePinA();
//    t0::setOutputModePinA(t0::OutputMode::TogglePinOnCompareMatch);
//    t0::setOutputModePinB(t0::OutputMode::ClearPinOnCompareMatch);
//    t0::setCompareValueA(200);
//    t0::setCompareValueB(0);

    // FASTPWM
//    t0::init(t0::Mode::FastPWM, t0::Clock::NoPrescaling);
//    t0::enableOutputComparePinA();
//    mcu::Timers::Timer0::setOutputModeA(Timers::Timer0::OutputMode::ClearOnCompareMatch);    
//    mcu::Timers::Timer0::setCompareValueA(32);
    
//    mcu::Timers::Timer0::setCompareValueB(32);
    uint8_t duty = 0;
    while(1){
//        mcu::System::WatchdogTimer::reset();
        /* ArduinoUnoR3Pin:9 (PB1)*/
//        togglingPin::toggle();      //2.65MHz @16MHz
//        t0::setCompareValueA(duty);
        _delay_ms(1000);
        duty++;
//        if(duty==t0::getCompareValueA())duty=0;
//        t0::setCompareValueB(duty);
    }

    return 0;
}