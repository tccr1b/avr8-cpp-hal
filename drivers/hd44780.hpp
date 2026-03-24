
#pragma once
/* Kütüphane tamamlanınca SİL*/
#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

/* INSTRUCTION SETS*/
#define HD44780_INS_RETURN_HOME         0x00
#define HD44780_INS_CLEAR_DISPLAY       0x01
#define HD44780_INS_ENTRYMODE           0x04
#define HD44780_INS_DISPLAYCONTROL      0x08
#define HD44780_INS_CURSORDISPLAYSHIFT  0x10
#define HD44780_INS_FUNCTIONSET         0x20
#define HD44780_SETCGRAMADDR            0x40
#define HD44780_SETDDRAMADDR            0x80
/* BIT DEFS*/
#define HD44780_BITS_ENTRYMODE_S             (1 << 0) // Accompanies Display Shift
#define HD44780_BITS_ENTRYMODE_ID            (1 << 1) // Increment/Decrement
#define HD44780_BITS_DISPLAYCONTROL_D        (1 << 2) // Display on/off
#define HD44780_BITS_DISPLAYCONTROL_C        (1 << 1) // Cursor on/off
#define HD44780_BITS_DISPLAYCONTROL_B        (1 << 0) // Blinking on/off
#define HD44780_BITS_CURSORDISPLAYSHIFT_SC   (1 << 3) // Display shift / Cursor move
#define HD44780_BITS_CURSORDISPLAYSHIFT_RL   (1 << 2) // Shift to the right / Shift to the left
#define HD44780_BITS_FUNCTIONSET_DL          (1 << 4) // Dataline 8bit/4bit
#define HD44780_BITS_FUNCTIONSET_N           (1 << 3) // Display lines 2(1)/1(0)
#define HD44780_BITS_FUNCTIONSET_F           (1 << 2) // 5x10dot(1) / 5x8dot(0)

/*
I/D = 1: Increment
I/D = 0: Decrement
S = 1: Accompanies display shift
S/C = 1: Display shift
S/C = 0: Cursor move
R/L = 1: Shift to the right
R/L = 0: Shift to the left
DL = 1: 8 bits, DL = 0: 4 bits
N = 1: 2 lines, N = 0: 1 line
F = 1: 5 × 10 dots, F = 0: 5 × 8 dots
BF = 1: Internally operating
BF = 0: Instructions acceptable
*/


#include "../HAL/gpio.hpp"
using namespace HAL;

class HD44780{
private:
    VirtualPort& dataBus;
    VirtualPort::PinConfig rsPin;
    VirtualPort::PinConfig enPin;
    void clockPulse();
public:
    HD44780(VirtualPort& bus, VirtualPort::PinConfig rs, VirtualPort::PinConfig en);
    void init();
    void sendCommand();
    void sendData();

    void clear();
    void setCursor();
    void print();

    ~HD44780();
};


void HD44780::clockPulse(){}