
#define __AVR_ATmega328P__

#include <util/delay.h>
#include <avr/interrupt.h>
#include "board.hpp"
#include "HAL/watchdog.hpp"
//#include "HAL/sysctrl.hpp"
#include "HAL/serialstream.hpp"
#include "HAL/spi.hpp"
#include "HAL/adc.hpp"
#include "HAL/twi.hpp"
//#include "HAL/utils.hpp"
//#include "HAL/eeprom.hpp"
#include "HAL/timers.hpp"
#include "HAL/analogcomp.hpp"

using namespace mcu;
using namespace HAL;


const char myStr[] __attrib_section_progmem__ ={"asd"};
const char myEepromStr[] __attrib_section_eeprom__ ={"ThisIsEepromData"};

void sysReset(){
    static uint8_t resetCount = 0;
    builtinLed::toggle();
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
    builtinLed::toggle();
}

void timerCallback(){
    builtinLed::toggle();
}

int main (){    
//    cstd::cout << "--------------" << cstd::endl;
    mcu::System::Clock::setClockPrescaler(SysClock::NoDivision);
    
    _delay_ms(10);
    mcu::Peripherals::Usart::init(UsartMode::Asynchronous,
                                  UsartBaudrate::_9600bps,
                                  UsartDataSize::_8bit,
                                  UsartStopBits::One,
                                  UsartParityMode::Disabled);
    
    mcu::System::WatchdogTimer::Interrupt.attach(sysReset);
    mcu::System::WatchdogTimer::enable(WatchdogMode::InterruptAndSystemReset, WatchdogTimeout::_4sec);
    sei();
    mcu::System::WatchdogTimer::disable();
    togglingPin::setPinMode(PinMode::Output);

    // FASTPWM_WITHOCR
    using t0 = mcu::Timers::Timer0;
    t0::init(t0::Mode::FastPWM_WithOCR, t0::Clock::NoPrescaling);
    t0::ChannelB.enable();
    t0::ChannelA.enable();
    t0::ChannelA.setOutputMode(t0::OutputMode::TogglePinOnCompareMatch);
    t0::ChannelB.setOutputMode(t0::OutputMode::ClearPinOnCompareMatch);
    t0::setCompareValueA(200);
    t0::setCompareValueB(0);

    // FASTPWM
//    using t0 = mcu::Timers::Timer0;
//    t0::init(t0::Mode::FastPWM, t0::Clock::NoPrescaling);
//    t0::enableOutputComparePinA();
//    mcu::Timers::Timer0::setOutputModePinA(Timers::Timer0::OutputMode::ClearPinOnCompareMatch);    
//    mcu::Timers::Timer0::setCompareValueA(32);
//    mcu::Timers::Timer0::setCompareValueB(32);

// CHANNEL DEFINED
//    mcu::Timers::Timer0::init(Timers::Timer0::Mode::ClearTimerOnCompareMatch, Timers::Timer0::Clock::NoPrescaling);
//    mcu::Timers::Timer0::ChannelA.enable();
//    mcu::Timers::Timer0::ChannelA.setOutputMode(Timers::Timer0::OutputMode::TogglePinOnCompareMatch);
//    mcu::Timers::Timer0::ChannelB.setOutputMode(Timers::Timer0::OutputMode::DisconnectedFromPin);
//    mcu::Timers::Timer0::ChannelA.forceOutput();
//    mcu::Timers::Timer0::setCompareValueA(32);
//    mcu::Timers::Timer0::ChannelA.forceOutput();
//    mcu::Timers::Timer0::Interrupts.attach(Timers::Timer0::InterruptType::Overflow, sysReset);
//    mcu::Timers::Timer0::Interrupts.detach(Timers::Timer0::InterruptType::Overflow);
//    mcu::Timers::Timer0::Interrupts.disable(Timers::Timer0::InterruptType::Overflow);
//    using timer0 = mcu::Timers::Timer0;
//    timer0::ChannelA.forceOutput();
//    timer0::ChannelA.setOutputMode(timer0::OutputMode::DisconnectedFromPin);
//    timer0::Interrupts.disable(timer0::InterruptType::All);
/* TESTING TIMER0 INTERRUPTS*/
//    using timer0 = mcu::Timers::Timer0;
//    timer0::init(timer0::Mode::Normal, timer0::Clock::DividedBy1024);
//    mcu::Timers::Timer0::Interrupts.attach(mcu::Timers::Timer0::InterruptType::Overflow, timerCallback);
//    mcu::Timers::Timer0::ChannelA.setOutputMode(timer0::OutputMode::DisconnectedFromPin);
//    timer0::ChannelA.setOutputMode(timer0::OutputMode::DisconnectedFromPin);
    
    using twi = mcu::Peripherals::Twi;
    twi::TwiInterrupt.clearFlag();

    using timer0 = mcu::Timers::Timer0;
    timer0::init(timer0::Mode::FastPWM, timer0::Clock::DividedBy1024);
    timer0::setCompareValueA(127);
    timer0::ChannelA.setOutputMode(timer0::OutputMode::ClearPinOnCompareMatch).enable();
    timer0::Interrupts.attach(timer0::InterruptType::OutputCompareMatchA, timerCallback).enable();
    timer0::Interrupts.disable(timer0::InterruptType::OutputCompareMatchA);
    timer0::Interrupts.detach(timer0::InterruptType::OutputCompareMatchA).disable();
    timer0::Interrupts.enable(timer0::InterruptType::All);


    using t1 = mcu::Timers::Timer1;
    t1::init(t1::Mode::FastPWM_10bit, t1::Clock::NoPrescaling);
    t1::ChannelA.setOutputMode(t1::OutputMode::ClearOnCompareMatch).enable();

    using t2 = mcu::Timers::Timer2;
    t2::init(t2::Mode::FastPWM, t2::Clock::DividedBy8, t2::ClockSource::SystemClock);
    t2::setCompareValueA(32);
    t2::ChannelA.setOutputMode(t2::OutputMode::ClearOnCompareMatch).enable();
    t2::ChannelB.setOutputMode(t2::OutputMode::DisconnectedFromPin);

    uint8_t duty = 1;
    uint16_t duty16bit = 0;
    builtinLed::setPinMode(PinMode::Output);
//    builtinLed::enableInputPullUp();


    mcu::Timers::Timer0::Interrupts.detach(Timers::Timer0::InterruptType::All);
    mcu::Timers::Timer1::Interrupts.detach(Timers::Timer1::InterruptType::All);
    mcu::Timers::Timer2::Interrupts.detach(Timers::Timer2::InterruptType::All);

    mcu::Peripherals::Adc::init(AdcChannel::InternalTempSensor);
    mcu::Peripherals::Adc::setAdcClockPrescaler(AdcClock::DividedBy128);
    mcu::Peripherals::Adc::selectReference(AdcReference::Internal_1v1);
    mcu::Peripherals::Adc::setResultAdjust(AdcResultAdjust::Right);
    mcu::Peripherals::Adc::ConversionCompletedInterrupt.attach(timerCallback);
    mcu::Peripherals::Adc::AutoTriggering.selectSource(AdcAutoTriggerSource::ExternalIRQ0).enable();
    
    using adc = mcu::Peripherals::Adc;
    adc::init(AdcChannel::InternalTempSensor);
    adc::setAdcClockPrescaler(AdcClock::DividedBy128);
    adc::selectReference(AdcReference::Internal_1v1);
    adc::setResultAdjust(AdcResultAdjust::Right);
    adc::ConversionCompletedInterrupt.attach(timerCallback);
    adc::AutoTriggering.selectSource(AdcAutoTriggerSource::FreeRunning).enable();

    adc::AdcConfig myAdcConf;
        myAdcConf.adc_ats = AdcAutoTriggerSource::FreeRunning;
        myAdcConf.adc_ch  = AdcChannel::InternalTempSensor;
        myAdcConf.adc_clk = AdcClock::DividedBy128;
        myAdcConf.adc_ref = AdcReference::Internal_1v1;
    adc::init(&myAdcConf);
    
    mcu::Peripherals::AnalogComp::Interrupt.selectMode(AcInterruptMode::InterruptOnRisingOutputEdge);
    mcu::Peripherals::AnalogComp::Interrupt.attach(sysReset).enable();

    using Comparator = mcu::Peripherals::AnalogComp;
    Comparator::Interrupt.selectMode(AcInterruptMode::InterruptOnFallingOutputEdge).attach(timerCallback).enable();
    Comparator::disableDigitalInputBuffer(AcInput::Ain0);
    Comparator::setNegativeInputA1(AcA1Channel::AdcChNo0);
    Comparator::setPositiveInputA0(AcA0Channel::Ain0Pin);
    Comparator::enable();
    while(1){
//        mcu::System::WatchdogTimer::reset();
        /* ArduinoUnoR3Pin:9 (PB1)*/
//        togglingPin::toggle();      //2.65MHz @16MHz
//        t0::setCompareValueA(duty);
//        _delay_ms(10);
        
        cstd::cout << mcu::Peripherals::Adc::read() << cstd::endl;
        _delay_ms(200);

        duty++;
        duty16bit++;
        mcu::Timers::Timer0::setCompareValueA(duty);
        t1::setCompareValueA(duty16bit);
//        if(duty==timer0::getCompareValueA())duty=0;
        timer0::setCompareValueB(duty);
    }

    return 0;
}