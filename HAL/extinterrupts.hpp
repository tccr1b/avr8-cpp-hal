#ifndef HAL_EXT_INTERRUPTS_HPP
#define HAL_EXT_INTERRUPTS_HPP

#define __AVR_ATmega328P__

#include "HAL/registers.hpp"
#include "HAL/macros.hpp"

#define BITPOS_ICS0 ISC00
#define BITPOS_ICS1 ISC10

using namespace mcu;

enum class ExtInterruptSense : uint8_t{
    IRQonLowLevel         = 0x00, //Generate interrupt request on the low level of INTx
    IRQonAnyLogicalChange = 0x01, //Generate interrupt request on any logical change of INTx
    IRQonFallingEdge      = 0x02, //Generate interrupt request on falling edge of INTx
    IRQonRisingEdge       = 0x03, //Generate interrupt request on rising edge of INTx
};

template<typename T> struct ExtInterruptBase{};
template<typename regEIC, typename regEIM, typename regEIF, uint8_t flagBitmask, uint8_t maskBitmask, uint8_t icsPos>
    struct ExtInterrupt{
        using Callback = void(*)();
        Callback cbFunction = nullptr;

                 void enable (){regEIM().setBitmask(maskBitmask);}
                 void disable(){regEIM().clearBitmask(maskBitmask);}
        ExtInterrupt& attach (Callback cbFunc){cbFunction = cbFunc; return *this;}
        ExtInterrupt& detach (){cbFunction = nullptr; return *this;}
          inline void handle ()__atr_always_inline__{if(cbFunction) cbFunction();}
        ExtInterrupt& selectSensing(ExtInterruptSense int_sense){
            uint8_t sensing = static_cast<uint8_t>(int_sense);
            sensing <<= icsPos;
            uint8_t mask = ~(0x03 << icsPos);            
            regEIC().writeMasked(sensing, mask);
        }
                 void clearFlag(){regEIF().setBitmask(flagBitmask);}
                 void setFlag  (){if(!(regEIF() & flagBitmask)) regEIF().setBitmask(flagBitmask);}
};

namespace mcu{
/* External interrupt on INT0 pin*/
static ExtInterrupt<decltype(Regs::Core::ExternalInterruptControlReg),
                    decltype(Regs::Core::ExternalInterruptMaskReg),
                    decltype(Regs::Core::ExternalInterruptFlagReg),
                    RegBits::Core::EIFR_INTF0,
                    RegBits::Core::EIMSK_INT0,
                    BITPOS_ICS0>ExternalInterrupt0;

/* External interrupt on INT1 pin*/
static ExtInterrupt<decltype(Regs::Core::ExternalInterruptControlReg),
                    decltype(Regs::Core::ExternalInterruptMaskReg),
                    decltype(Regs::Core::ExternalInterruptFlagReg),
                    RegBits::Core::EIFR_INTF1,
                    RegBits::Core::EIMSK_INT1,
                    BITPOS_ICS1>ExternalInterrupt1;
} // namespace mcu

















#endif // HAL_EXT_INTERRUPTS_HPP