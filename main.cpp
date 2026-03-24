
#define __AVR_ATmega328P__

#include <util/delay.h>
#include <avr/interrupt.h>
#include "board.hpp"
#include "HAL/watchdog.hpp"
#include "HAL/sysctrl.hpp"
#include "HAL/serialstream.hpp"
//#include "HAL/spi.hpp"
//#include "HAL/adc.hpp"
//#include "HAL/twi.hpp"
//#include "HAL/utils.hpp"
//#include "HAL/eeprom.hpp"

using namespace mcu;
using namespace HAL;

void sysReset(){
    static uint8_t resetCount = 0;
    builtInLED::toggleState();
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
    builtInLED::toggleState();
}


int main (){
    mcu::System::captureResetReason();
    mcu::System::WatchdogTimer::disable();
    
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
    
    cstd::cout << "#=========== SYSTEM INFO ===========#" << cstd::endl;
    cstd::cout << "Master Clock Source: " << mcu::System::Clock::getMasterClockSourceStr() << cstd::endl;    
    cstd::cout << "CPU Clock Frequency: " << mcu::System::Clock::getCpuFrequency() << "Hz" << cstd::endl;
    cstd::cout << "F_CPU Frequency    : " << (uint32_t)F_CPU << "Hz" << cstd::endl;
    cstd::cout << "Extended Fuse Bits : " << mcu::System::Fuses::getExtendedFuseBits() << cstd::endl;
    cstd::cout << "High Fuse Bits     : " << mcu::System::Fuses::getHighFuseBits() << cstd::endl;
    cstd::cout << "Low Fuse Bits      : " << mcu::System::Fuses::getLowFuseBits() << cstd::endl;
    cstd::cout << "Lock Bits          : " << mcu::System::Fuses::getLockBits() << cstd::endl;
    cstd::cout << "BOD Enabled?       : " << mcu::System::Fuses::isBodEnabled() << cstd::endl;
    cstd::cout << "Baudrate           : 9600bps" << cstd::endl;
    cstd::cout << "X2 Enabled?        : " << (uint8_t)(mcu::Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_U2X0)) << cstd::endl;
    cstd::cout << "=====================================" << cstd::endl;
    cstd::cout << cstd::endl;
        
    while(1) {
//        mcu::System::WatchdogTimer::reset();
    }

    return 0;
}