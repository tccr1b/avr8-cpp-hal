#ifndef BOARD_HPP
#define BOARD_HPP

#define __AVR_ATmega328P__

#include "HAL/registers.hpp"
#include "HAL/gpio.hpp"

using namespace HAL;
using namespace mcu::Gpio;

/* Arduino Uno R3 built-in LED pin*/
/*
using builtInLED = HAL::GpioPin<decltype(Regs::Gpio::PortRegB),
                                decltype(Regs::Gpio::DataDirectionRegB),
                                decltype(Regs::Gpio::InputPinAddrB),
                                RegBits::Gpio::PortB::PB_5>;
*/
using builtinLed = mcu::Gpio::PinPB5;

using togglingPin = mcu::Gpio::PinPB1;

/* Nokia 5110 Display*/
//using pinSCK_Lcd5110  = mcu::Gpio::PinPB4;
//using pinMOSI_Lcd5110 = mcu::Gpio::PinPB0;
//using pinMISO_Lcd5110 = mcu::Gpio::PinPB2;
//using pinSS_Lcd5110   = mcu::Gpio::PinPB7;

/* 16x2 LCD Display Pins*/
//using pinRW_Lcd1602 = mcu::Gpio::PinPC0;
//using pinRS_Lcd1602 = mcu::Gpio::PinPB3;
//using pinEN_Lcd1602 = mcu::Gpio::PinPD3;

/* 2x16 LCD Display Pins
const HAL::VirtualPort::PinConfig pinB0 = {Regs::Gpio::DataDirectionRegB,
                                            Regs::Gpio::PortRegB,
                                            Regs::Gpio::InputPinAddrB,
                                            RegBits::Gpio::PortB::PB_0};
const HAL::VirtualPort::PinConfig pinB1 = {Regs::Gpio::DataDirectionRegB,
                                            Regs::Gpio::PortRegB,
                                            Regs::Gpio::InputPinAddrB,
                                            RegBits::Gpio::PortB::PB_1};
const HAL::VirtualPort::PinConfig pinB2 = {Regs::Gpio::DataDirectionRegB,
                                            Regs::Gpio::PortRegB,
                                            Regs::Gpio::InputPinAddrB,
                                            RegBits::Gpio::PortB::PB_2};
const HAL::VirtualPort::PinConfig pinB3 = {Regs::Gpio::DataDirectionRegB,
                                            Regs::Gpio::PortRegB,
                                            Regs::Gpio::InputPinAddrB,
                                            RegBits::Gpio::PortB::PB_3};
HAL::VirtualPort::PinConfig lcdDataPins[] = {pinB0,  //LSB   Bit 0
                                             pinB1,  //      Bit 1
                                             pinB2,  //      Bit 2
                                             pinB3}; //MSB   Bit 3
HAL::VirtualPort myVirtualPort(lcdDataPins, 4);

HAL::VirtualPort::PinConfig rsPin{Regs::Gpio::DataDirectionRegD,
                                  Regs::Gpio::PortRegD,
                                  Regs::Gpio::InputPinAddrD,
                                  RegBits::Gpio::PortD::PD_1};   
HAL::VirtualPort::PinConfig enPin{Regs::Gpio::DataDirectionRegD,
                                  Regs::Gpio::PortRegD,
                                  Regs::Gpio::InputPinAddrD,
                                  RegBits::Gpio::PortD::PD_2};   
*/
                                  
#endif //BOARD_HPP