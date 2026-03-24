
#ifndef PORT_GROUP_HPP
#define PORT_GROUP_HPP
#define __AVR_ATmega328P__
#include "registers.hpp"



using namespace mcu;

template <typename PortRegs>
class Port{
public:
    static void setData(uint8_t data){
        PortRegs::Data = value;
    }

    static void writeData(uint8_t value){PortRegs::Data = value;}
    static void setDataDirection (uint8_t dir){PortRegs::DataDirection = dir;}
    static void writeDataMasked(uint8_t data, uint8_t mask){
        uint8_t currentValue = PortRegs::Data;
        PortRegs::Data = (currentValue& ~mask) | (data & mask);
    }
};

uint8_t ports[] = {DDRB,PORTB,PINB};

void foo(){
    using ddrb =    Port<decltype(ports)>;
    ddrb::setDataDirection(1);

}



#endif //PORT_GROUP_HPP