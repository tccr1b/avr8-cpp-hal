
#ifndef AVR_REGISTERS_HPP
#define AVR_REGISTERS_HPP

#if defined(__AVR_ATmega328P__)
    #include "regwrappers/regwr328p.hpp"
#elif defined (__AVR_ATmega168P__)
    #include "regwrappers/regwr168p.hpp"
#elif defined (__AVR_ATmega32A__)
    #include "regwrappers/regwr32a.hpp"
#elif defined (__AVR_AT90S2313__)
    #include "regwrappers/regwr2313.hpp"

#endif




#endif //AVR_REGISTERS_HPP