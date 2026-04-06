
#ifndef ATOMIC_BLOCK_HPP
#define ATOMIC_BLOCK_HPP

#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include "../registers.hpp"
#include <inttypes.h>
#include <avr/interrupt.h>

using namespace mcu;

class AtomicBlock{
private:
    uint8_t storedStatusReg;
    
public:
    /* Kopyalamayı ve Nesne üretimini iptal et*/
    AtomicBlock(const AtomicBlock&) = delete;
    AtomicBlock& operator=(const AtomicBlock&) = delete;

    /* Constuctor*/
    inline AtomicBlock();
    /* Destructor*/
    inline ~AtomicBlock();
};
AtomicBlock::AtomicBlock(){
    storedStatusReg = Regs::Core::StatusReg;
    cli();
}
AtomicBlock::~AtomicBlock(){
    Regs::Core::StatusReg.setValue(storedStatusReg);
}






#endif //ATOMIC_BLOCK_HPP