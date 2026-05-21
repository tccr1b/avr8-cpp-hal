/*
MyProject/
├── HAL/
│   ├── Register.hpp    (Adresler ve Bit İsimleri - Temel)
│   ├── Gpio.hpp        (Pin Kontrol Soyutlaması)
│   └── Spi.hpp         (İletişim Soyutlaması)
├── Board.hpp           (Donanım Tanımları - Hangi pin nereye bağlı?)
└── main.cpp            (Uygulama)
*/
#ifndef REGS_ATMEGA328P_HPP
#define REGS_ATMEGA328P_HPP

#define rPINB     0x23
#define rDDRB     0x24
#define rPORTB    0x25
#define rPINC     0x26
#define rDDRC     0x27
#define rPORTC    0x28
#define rPIND     0x29
#define rDDRD     0x2A
#define rPORTD    0x2B
#define rTIFR0    0x35
#define rTIFR1    0x36
#define rTIFR2    0x37
#define rPCIFR    0x3B
#define rEIFR     0x3C
#define rEIMSK    0x3D
#define rGPIOR0   0x3E
#define rEECR     0x3F
#define rEEDR     0x40
#define rEEARL    0x41
#define rEEARH    0x42
#define rGTCCR    0x43
#define rTCCR0A   0x44
#define rTCCR0B   0x45
#define rTCNT0    0x46
#define rOCR0A    0x47
#define rOCR0B    0x48
#define rGPIOR1   0x4A
#define rGPIOR2   0x4B
#define rSPCR     0x4C
#define rSPSR     0x4D
#define rSPDR     0x4E
#define rACSR     0x50
#define rSMCR     0x53
#define rMCUSR    0x54
#define rMCUCR    0x55
#define rSPMCSR   0x57
#define rSREG     0x5F
#define rSPH      0x5E
#define rSPL      0x5D
#define rWDTCSR   0x60
#define rCLKPR    0x61
#define rPRR      0x64
#define rOSCCAL   0x66
#define rPCICR    0x68
#define rEICRA    0x69
#define rPCMSK0   0x6B
#define rPCMSK1   0x6C
#define rPCMSK2   0x6D
#define rTIMSK0   0x6E
#define rTIMSK1   0x6F
#define rTIMSK2   0x70
#define rADC      0x78
#define rADCW     0x78
#define rADCL     0x78
#define rADCH     0x79
#define rADCSRA   0x7A
#define rADCSRB   0x7B
#define rADMUX    0x7C
#define rDIDR0    0x7E
#define rDIDR1    0x7F
#define rTCCR1A   0x80
#define rTCCR1B   0x81
#define rTCCR1C   0x82
#define rTCNT1    0x84
#define rTCNT1L   0x84
#define rTCNT1H   0x85
#define rICR1     0x86
#define rICR1L    0x86
#define rICR1H    0x87
#define rOCR1A    0x88
#define rOCR1AL   0x88
#define rOCR1AH   0x89
#define rOCR1B    0x8A
#define rOCR1BL   0x8A
#define rOCR1BH   0x8B
#define rTCCR2A   0xB0
#define rTCCR2B   0xB1
#define rTCNT2    0xB2
#define rOCR2A    0xB3
#define rOCR2B    0xB4
#define rASSR     0xB6
#define rTWBR     0xB8
#define rTWSR     0xB9
#define rTWAR     0xBA
#define rTWDR     0xBB
#define rTWCR     0xBC
#define rTWAMR    0xBD
#define rUCSR0A   0xC0
#define rUCSR0B   0xC1
#define rUCSR0C   0xC2
#define rUBRR0    0xC4
#define rUBRR0L   0xC4
#define rUBRR0H   0xC5
#define rUDR0     0xC6

#include <avr/io.h>
#include <stdint.h>
#include "../macros.hpp"

namespace mcu {
    // =============================================================
    // 1. Register Şablonu (Sıfır RAM Tüketimi)
    // =============================================================
    // Bu struct içinde hiçbir veri değişkeni yoktur, sadece fonksiyonlar vardır.
    // Bu yüzden RAM'de yer kaplamaz. Adres, şablon parametresi olarak (Addr)
    // derleme zamanında verilir.

    template<uintptr_t Addr>
        struct IoRegister{
            // Okuma Operatörü (uint8_t gibi davranır)
            operator uint8_t() const volatile __attrib_always_inline__ {return *(volatile uint8_t*)Addr;}            
            void operator=(uint8_t value)  const volatile __attrib_always_inline__{*(volatile uint8_t*)Addr =value;}
            void operator|=(uint8_t value) const volatile __attrib_always_inline__{*(volatile uint8_t*)Addr |=value;}
            void operator&=(uint8_t value) const volatile __attrib_always_inline__{*(volatile uint8_t*)Addr &=value;}
            void operator^=(uint8_t value) const volatile __attrib_always_inline__{*(volatile uint8_t*)Addr ^=value;}
            
            inline void setBitmask   (uint8_t bitMask)  const volatile __attrib_always_inline__{
                *this |= bitMask;
            }
            inline void clearBitmask (uint8_t bitMask)  const volatile __attrib_always_inline__{
                *this &= ~bitMask;
            }
            inline void writeBitmask (uint8_t uByte)    const volatile __attrib_always_inline__{
                *this &= uByte;
            }
            inline void writeMasked  (uint8_t uByte, uint8_t mask) const volatile __attrib_always_inline__{
                *this &= mask; 
                *this |= uByte;
            }
            inline void toggleBitmask(uint8_t bitMask)  const volatile __attrib_always_inline__{
                *this ^= bitMask;
            }
            inline bool readBit      (uint8_t bitMask)  const volatile __attrib_always_inline__{
                return (*this & bitMask);
            }
            inline void setValue     (uint8_t value)    const volatile __attrib_always_inline__{
                *this = value;
            }
            inline uint8_t getValue  ()                 const volatile __attrib_always_inline__{
                return *this;
            }
            inline uint8_t getValue  (uint8_t mask)     const volatile __attrib_always_inline__{
                return (*this & mask);
            }
        };

    // =============================================================
    // 2. Register Adresleri (Datasheet'ten Bellek Adresleri)
    // =============================================================
    namespace Regs{
        // UART/USART Registerları
        namespace Uart{
                /*USART I/O data register*/
                constexpr IoRegister<rUDR0> UartDataReg   = {};
                /*USART Baud Rate Register High
                This is a 12-bit register which contains the USART baud rate. The UBRRnH contains the four most significant bits, and the
                UBRRnL contains the eight least significant bits of the USART baud rate*/
                constexpr IoRegister<rUBRR0H> UartBaudrateRegHByte = {};
                /*USART Baud Rate Register Low
                This is a 12-bit register which contains the USART baud rate. The UBRRnH contains the four most significant bits, and the
                UBRRnL contains the eight least significant bits of the USART baud rate*/
                constexpr IoRegister<rUBRR0L> UartBaudrateRegLByte = {};
                /*USART Control and Status Register 0C
                | UMSEL01 | UMSEL00 | UPM01 | UPM00 | USBS0 | UCSZ01 | UCSZ00 | UCPOL0 |*/
                constexpr IoRegister<rUCSR0C> UartControlAndStatusRegC = {};
                /*UCSR0B – USART Control and Status Register 0B
                Bit Yerleşimleri:
                | RXCIE0 | TXCIE0 | UDRIE0 | RXEN0 | TXEN0 | UCSZ02 | RXB80 | TXB80 |*/
                constexpr IoRegister<rUCSR0B> UartControlAndStatusRegB = {};
                /*UCSR0A – USART Control and Status Register 0A
                Bit Yerleşimleri:
                | RXC0 | TXC0 | UDRE0 | FE0 | DOR0 | UPE0 | U2X0 | MPCM0 |*/
                constexpr IoRegister<rUCSR0A> UartControlAndStatusRegA = {};
        }
        /*TWI(I2C) TWAMR, TWCR, TWDR, TWAR, TWSR, TWBR*/
        namespace Twi {
                /*TWAMR – TWI (Slave) Address Mask Register
                | TWAM6 | TWAM5 | TWAM4 | TWAM3 | TWAM2 | TWAM1 | TWAM0 |  –  |
                The TWAMR can be loaded with a 7-bit slave address mask. Each of the bits in TWAMR can mask (disable) the
                corresponding address bits in the TWI address register (TWAR). If the mask bit is set to one then the address match logic
                ignores the compare between the incoming address bit and the corresponding bit in TWAR.*/
                constexpr IoRegister<rTWAMR> TwiAddressMaskReg= {};
                /*TWCR – TWI Control Register
                | TWINT | TWEA | TWSTA | TWSTO | TWWC | TWEN | – | TWIE | 
                The TWCR is used to control the operation of the TWI. It is used to enable the TWI, to initiate a master access by applying a
    START condition to the bus, to generate a receiver acknowledge, to generate a stop condition, and to control halting of the
    bus while the data to be written to the bus are written to the TWDR. It also indicates a write collision if data is attempted
    written to TWDR while the register is inaccessible.*/
                constexpr IoRegister<rTWCR>  TwiControlReg= {};
                /*TWDR – TWI Data Register
                | TWD7 | TWD6 | TWD5 | TWD4 | TWD3 | TWD2 | TWD1 | TWD0 |
                In transmit mode, TWDR contains the next byte to be transmitted. In receive mode, the TWDR contains the last byte
    received. It is writable while the TWI is not in the process of shifting a byte. This occurs when the TWI interrupt flag (TWINT)
    is set by hardware. Note that the data register cannot be initialized by the user before the first interrupt occurs. The data in
    TWDR remains stable as long as TWINT is set. While data is shifted out, data on the bus is simultaneously shifted in. TWDR
    always contains the last byte present on the bus, except after a wake up from a sleep mode by the TWI interrupt. In this
    case, the contents of TWDR is undefined.
    In the case of a lost bus arbitration, no data is lost in the transition from master to slave. Handling of the ACK bit is controlled
    automatically by the TWI logic, the CPU cannot access the ACK bit directly.*/
                constexpr IoRegister<rTWDR>  TwiDataReg= {};
                /*TWAR – TWI (Slave) Address Register
                | TWA6 | TWA5 | TWA4 | TWA3 | TWA2 | TWA1 | TWA0 | TWGCE |
                The TWAR should be loaded with the 7-bit slave address (in the seven most significant bits of TWAR) to which the TWI will
    respond when programmed as a slave transmitter or receiver, and not needed in the master modes. In multi master
    systems, TWAR must be set in masters which can be addressed as slaves by other masters.
    The LSB of TWAR is used to enable recognition of the general call address (0x00). There is an associated address
    comparator that looks for the slave address (or general call address if enabled) in the received serial address. If a match is
    found, an interrupt request is generated.*/
                constexpr IoRegister<rTWAR>  TwiAddressReg= {};
                /*TWSR – TWI Status Register
                | TWS7 | TWS6 | TWS5 | TWS4 | TWS3 |  –  | TWPS1 | TWPS0 |
                These 5 bits reflect the status of the TWI logic and the 2-wire serial bus. The different status codes are described later in this
    section. Note that the value read from TWSR contains both the 5-bit status value and the 2-bit prescaler value. The
    application designer should mask the prescaler bits to zero when checking the status bits. This makes status checking
    independent of prescaler setting. This approach is used in this datasheet, unless otherwise noted.*/
                constexpr IoRegister<rTWSR>  TwiStatusReg= {};
                /*TWBR – TWI Bit Rate Register
                | TWBR7 | TWBR6 | TWBR5 | TWBR4 | TWBR3 | TWBR2 | TWBR1 | TWBR0 |
                TWBR selects the division factor for the bit rate generator. The bit rate generator is a frequency divider which generates the
    SCL clock frequency in the master modes.*/
                constexpr IoRegister<rTWBR>  TwiBitrateReg= {};
        }
        namespace Core{
            /*PCMSK2 – Pin Change Mask Register 2
            | PCINT23 | PCINT22 | PCINT21 | PCINT20 | PCINT19 | PCINT18 | PCINT17 | PCINT16 |
            Each PCINT23..16-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is set
and the PCIE2 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is cleared,
pin change interrupt on the corresponding I/O pin is disabled.*/
            constexpr IoRegister<rPCMSK2> PinChangeMaskReg2= {};
            /*PCMSK1 – Pin Change Mask Register 1
            |  –  | PCINT14 | PCINT13 | PCINT12 | PCINT11 | PCINT10 | PCINT9 | PCINT8 |
            Each PCINT14..8-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is set and
the PCIE1 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is cleared, pin
change interrupt on the corresponding I/O pin is disabled.*/
            constexpr IoRegister<rPCMSK1> PinChangeMaskReg1= {};
            /*PCMSK0 – Pin Change Mask Register 0
            | PCINT7 | PCINT6 | PCINT5 | PCINT4 | PCINT3 | PCINT2 | PCINT1 | PCINT0 |
            Each PCINT7..0 bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is set and the
PCIE0 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is cleared, pin change
interrupt on the corresponding I/O pin is disabled.*/
            constexpr IoRegister<rPCMSK0> PinChangeMaskReg0= {};
            /*EICRA – External Interrupt Control Register A
            |  –  |  –  |  –  |  –  | ISC11 | ISC10 | ISC01 | ISC00 |
            The external interrupt control register A contains control bits for interrupt sense control.*/
            constexpr IoRegister<rEICRA>  ExternalInterruptControlReg= {};
            /*PCICR – Pin Change Interrupt Control Register
            | – | – | – | – | – | PCIE2 | PCIE1 | PCIE0 |
            */
            constexpr IoRegister<rPCICR>  PinChangeInterruptControlReg= {};
            /*OSCCAL – Oscillator Calibration Register
            | CAL7 | CAL6 | CAL5 | CAL4 | CAL3 | CAL2 | CAL1 | CAL0 |
            The oscillator calibration register is used to trim the calibrated internal RC oscillator to remove process variations from the
oscillator frequency. A pre-programmed calibration value is automatically written to this register during chip reset, giving the
factory calibrated frequency as specified in Table 28-1 on page 260. The application software can write this register to
change the oscillator frequency. The oscillator can be calibrated to frequencies as specified in Table 28-1 on page 260.
Calibration outside that range is not guaranteed.
            */
            constexpr IoRegister<rOSCCAL> OscillatorCalibrationReg= {};
            /*PRR – Power Reduction Register
            | PRTWI | PRTIM2 | PRTIM0 | – | PRTIM1 | PRSPI | PRUSART0 | PRADC |
            The power reduction register (PRR), see Section 9.11.3 “PRR – Power Reduction Register” on page 38, provides a method
to stop the clock to individual peripherals to reduce power consumption. The current state of the peripheral is frozen and the
I/O registers can not be read or written. Resources used by the peripheral when stopping the clock will remain occupied,
hence the peripheral should in most cases be disabled before stopping the clock. Waking up a module, which is done by
clearing the bit in PRR, puts the module in the same state as before shutdown.
Module shutdown can be used in Idle mode and active mode to significantly reduce the overall power consumption. In all
other sleep modes, the clock is already stopped.
            */
            constexpr IoRegister<rPRR>    PowerReductionReg= {};
            /*CLKPR – Clock Prescale Register
            | CLKPCE | – | – | – | CLKPS3 | CLKPS2 | CLKPS1 | CLKPS0 |
            The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the
other bits in CLKPR are simultaneously written to zero. CLKPCE is cleared by hardware four cycles after it is written or when
CLKPS bits are written. Rewriting the CLKPCE bit within this time-out period does neither extend the time-out period, nor
clear the CLKPCE bit.*/
            constexpr IoRegister<rCLKPR>  ClockPrescaleReg= {};
            /*WDTCSR – Watchdog Timer Control Register
            | WDIF | WDIE | WDP3 | WDCE | WDE | WDP2 | WDP1 | WDP0 |
            Executing the corresponding interrupt vector will clear WDIE and WDIF automatically by hardware (the watchdog goes to
system reset mode). This is useful for keeping the watchdog timer security while using the interrupt. To stay in interrupt and
system reset mode, WDIE must be set after each interrupt. This should however not be done within the interrupt service
routine itself, as this might compromise the safety-function of the watchdog system reset mode. If the interrupt is not
executed before the next time-out, a system reset will be applied.*/
            constexpr IoRegister<rWDTCSR> WatchdogTimerControlReg= {};
            /*SREG – AVR Status Register
            | I | T | H | S | V | N | Z | C |
            The status register contains information about the result of the most recently executed arithmetic instruction. This
information can be used for altering program flow in order to perform conditional operations. Note that the status register is
updated after all ALU operations, as specified in the instruction set reference. This will in many cases remove the need for
using the dedicated compare instructions, resulting in faster and more compact code.
The status register is not automatically stored when entering an interrupt routine and restored when returning from an
interrupt. This must be handled by software.*/
            constexpr IoRegister<rSREG>   StatusReg= {};
            /*SPH and SPL – Stack Pointer High and Stack Pointer Low Register
            | SP15 | SP14 | SP13 | SP12 | SP11 | SP10 | SP9 | SP8 | ===> SPH
            | SP7  | SP6  | SP5  | SP4  | SP3  | SP2  | SP1 | SP0 | ===> SPL
            The stack is mainly used for storing temporary data, for storing local variables and for storing return addresses after
interrupts and subroutine calls. Note that the stack is implemented as growing from higher to lower memory locations. The
stack pointer register always points to the top of the stack. The stack pointer points to the data SRAM stack area where the
subroutine and interrupt stacks are located. A stack PUSH command will decrease the stack pointer.
The stack in the data SRAM must be defined by the program before any subroutine calls are executed or interrupts are
enabled. initial stack pointer value equals the last address of the internal SRAM and the stack pointer must be set to point
above start of the SRAM, see Figure 7-2 on page 18.The AVR stack pointer is implemented as two 8-bit registers in the I/O space. The number of bits actually used is
implementation dependent. Note that the data space in some implementations of the AVR® architecture is so small that only
SPL is needed. In this case, the SPH register will not be present.
            */
            constexpr IoRegister<rSPH>    StackPointerRegHByte= {};
            /*SPH and SPL – Stack Pointer High and Stack Pointer Low Register
            | SP15 | SP14 | SP13 | SP12 | SP11 | SP10 | SP9 | SP8 | ===> SPH
            | SP7  | SP6  | SP5  | SP4  | SP3  | SP2  | SP1 | SP0 | ===> SPL
            The stack is mainly used for storing temporary data, for storing local variables and for storing return addresses after
interrupts and subroutine calls. Note that the stack is implemented as growing from higher to lower memory locations. The
stack pointer register always points to the top of the stack. The stack pointer points to the data SRAM stack area where the
subroutine and interrupt stacks are located. A stack PUSH command will decrease the stack pointer.
The stack in the data SRAM must be defined by the program before any subroutine calls are executed or interrupts are
enabled. initial stack pointer value equals the last address of the internal SRAM and the stack pointer must be set to point
above start of the SRAM, see Figure 7-2 on page 18.The AVR stack pointer is implemented as two 8-bit registers in the I/O space. The number of bits actually used is
implementation dependent. Note that the data space in some implementations of the AVR® architecture is so small that only
SPL is needed. In this case, the SPH register will not be present.
            */
            constexpr IoRegister<rSPL>    StackPointerRegLByte= {};
            /*SPMCSR – Store Program Memory Control and Status Register
            | SPMIE | (RWWSB) | – | (RWWSRE) | BLBSET | PGWRT | PGERS | SELFPRGN |
            The store program memory control and status register contains the control bits needed to control the boot loader operations.*/
            constexpr IoRegister<rSPMCSR> StoreProgMemControlAndStatusReg= {};
            /*MCUCR – MCU Control Register
            | – | BODS | BODSE | PUD | – | – | IVSEL | IVCE |
            The MCU control register controls the placement of the interrupt vector table.
            */
            constexpr IoRegister<rMCUCR>  McuControlReg= {};
            /*MCUSR – MCU Status Register
            | – | – | – | – | WDRF | BORF | EXTRF | PORF |
            The MCU status register provides information on which reset source caused an MCU reset.*/
            constexpr IoRegister<rMCUSR>  McuStatusReg= {};
            /*SMCR – Sleep Mode Control Register
            | – | – | – | – | SM2 | SM1 | SM0 | SE |
            The sleep mode control register contains control bits for power management.*/
            constexpr IoRegister<rSMCR>   SleepModeControlReg= {};
            /*EIMSK – External Interrupt Mask Register
            | – | – | – | – | – | – | INT1 | INT0 |
            When the INT1 bit is set (one) and the I-bit in the status register (SREG) is set (one), the external pin interrupt is enabled.
The interrupt sense control1 bits 1/0 (ISC11 and ISC10) in the external interrupt control register A (EICRA) define whether
the external interrupt is activated on rising and/or falling edge of the INT1 pin or level sensed. Activity on the pin will cause an
interrupt request even if INT1 is configured as an output. The corresponding interrupt of external interrupt request 1 is
executed from the INT1 interrupt vector.*/
            constexpr IoRegister<rEIMSK>  ExternalInterruptMaskReg= {};
            /*EIFR – External Interrupt Flag Register
            | – | – | – | – | – | – | INTF1 | INTF0 |
            When an edge or logic change on the INT1 pin triggers an interrupt request, INTF1 becomes set (one). If the I-bit in SREG
and the INT1 bit in EIMSK are set (one), the MCU will jump to the corresponding interrupt vector. The flag is cleared when
the interrupt routine is executed. Alternatively, the flag can be cleared by writing a logical one to it. This flag is always cleared
when INT1 is configured as a level interrupt.*/
            constexpr IoRegister<rEIFR>   ExternalInterruptFlagReg= {};
            /*PCIFR – Pin Change Interrupt Flag Register
            | – | – | – | – | – | PCIF2 | PCIF1 | PCIF0 |
            When a logic change on any PCINT23..16 pin triggers an interrupt request, PCIF2 becomes set (one). If the I-bit in SREG
and the PCIE2 bit in PCICR are set (one), the MCU will jump to the corresponding interrupt vector. The flag is cleared when
the interrupt routine is executed. Alternatively, the flag can be cleared by writing a logical one to it.*/
            constexpr IoRegister<rPCIFR>  PinChangeInterruptFlagReg= {};
        }
        namespace Timers{
            /*8-bit Timer/Counter2 with PWM and Asynchronous Operation
            Timer/Counter2 is a general purpose, single channel, 8-bit Timer/Counter module. A simplified block diagram of the 8-bit
Timer/Counter is shown in Figure 17-1. For the actual placement of I/O pins, refer to Section 1-1 “Pinout” on page 3. CPU
accessible I/O registers, including I/O bits and I/O pins, are shown in bold. The device-specific I/O register and bit locations
are listed in the Section 17.11 “Register Description” on page 127.
The PRTIM2 bit in Section 9.10 “Minimizing Power Consumption” on page 36 must be written to zero to enable
Timer/Counter2 module.*/
            namespace Timer2{
                /*ASSR – Asynchronous Status Register
                |    –    |  EXCLK  |  AS2  | TCN2UB | OCR2AUB | OCR2BUB | TCR2AUB | TCR2BUB |
                If a write is performed to any of the five Timer/Counter2 registers while its update busy flag is set, the updated value might
get corrupted and cause an unintentional interrupt to occur.
The mechanisms for reading TCNT2, OCR2A, OCR2B, TCCR2A and TCCR2B are different. When reading TCNT2, the
actual timer value is read. When reading OCR2A, OCR2B, TCCR2A and TCCR2B the value in the temporary storage
register is read.*/
                constexpr IoRegister<rASSR>   AsynchronousStatusReg= {};

                /*OCR2B – Output Compare Register B
                The output compare register B contains an 8-bit value that is continuously compared with the counter value (TCNT2). A
match can be used to generate an output compare interrupt, or to generate a waveform output on the OC2B pin.*/
                constexpr IoRegister<rOCR2B>  OutputCompareRegB       ={};
                /*OCR2A – Output Compare Register A
                The output compare register B contains an 8-bit value that is continuously compared with the counter value (TCNT2). A
match can be used to generate an output compare interrupt, or to generate a waveform output on the OC2B pin.*/
                constexpr IoRegister<rOCR2A>  OutputCompareRegA       ={};
                /*TCNT2 – Timer/Counter Register (8-bit)
                The Timer/Counter register gives direct access, both for read and write operations, to the Timer/Counter unit 8-bit counter.
Writing to the TCNT2 register blocks (removes) the compare match on the following timer clock. Modifying the counter
(TCNT2) while the counter is running, introduces a risk of missing a compare match between TCNT2 and the OCR2x
registers.*/
                constexpr IoRegister<rTCNT2>  TimerCounterReg         ={};
                /*TCCR2B – Timer/Counter Control Register B
                | FOC2A | FOC2B | – | – | WGM22 | CS22 | CS21 | CS20 |
                If external pin modes are used for the Timer/Counter0, transitions on the T0 pin will clock the counter even if the pin is
configured as an output. This feature allows software control of the counting.*/
                constexpr IoRegister<rTCCR2B> TimerCounterControlRegB ={};
                /*TCCR2A – Timer/Counter Control Register A
                | COM2A1 | COM2A0 | COM2B1 | COM2B0 | – | – | WGM21 | WGM20 |
                These bits control the output compare pin (OC2A) behavior. If one or both of the COM2A1:0 bits are set, the OC2A output
overrides the normal port functionality of the I/O pin it is connected to. However, note that the data direction register (DDR)
bit corresponding to the OC2A pin must be set in order to enable the output driver.*/
                constexpr IoRegister<rTCCR2A> TimerCounterControlRegA ={};
                /*TIMSK2 – Timer/Counter2 Interrupt Mask Register
                | – | – | – | – | – | OCIE2B | OCIE2A | TOIE2 |
                When the OCIE2B bit is written to one and the I-bit in the status register is set (one), the Timer/Counter2 compare match B
interrupt is enabled. The corresponding interrupt is executed if a compare match in Timer/Counter2 occurs, i.e., when the
OCF2B bit is set in the Timer/Counter 2 interrupt flag register – TIFR2.*/
                constexpr IoRegister<rTIMSK2> TimerInterruptMaskReg   ={};
                /*TIFR2 – Timer/Counter2 Interrupt Flag Register
                | – | – | – | – | – | OCF2B | OCF2A | TOV2 |
                The TOV2 bit is set (one) when an overflow occurs in Timer/Counter2. TOV2 is cleared by hardware when executing the
corresponding interrupt handling vector. Alternatively, TOV2 is cleared by writing a logic one to the flag. When the
SREG I-bit, TOIE2A (Timer/Counter2 overflow interrupt enable), and TOV2 are set (one), the Timer/Counter2 overflow
interrupt is executed. In PWM mode, this bit is set when Timer/Counter2 changes counting direction at 0x00.*/
                constexpr IoRegister<rTIFR2>  TimerInterruptFlagReg   ={};
            }
            /*16-bit Timer/Counter1 with PWM
            The 16-bit Timer/Counter unit allows accurate program execution timing (event management), wave generation, and signal
timing measurement.
Most register and bit references in this section are written in general form. A lower case “n” replaces the Timer/Counter
number, and a lower case “x” replaces the output compare unit channel. However, when using the register or bit defines in a
program, the precise form must be used, i.e., TCNT1 for accessing Timer/Counter1 counter value and so on.
A simplified block diagram of the 16-bit Timer/Counter is shown in Figure 15-1 on page 90. For the actual placement of I/O
pins, refer to Section 1-1 “Pinout” on page 3. CPU accessible I/O registers, including I/O bits and I/O pins, are shown in bold.
The device-specific I/O register and bit locations are listed in the Section 15.11 “Register Description” on page 108.
The PRTIM1 bit in Section 9.11.3 “PRR – Power Reduction Register” on page 38 must be written to zero to enable
Timer/Counter1 module.*/
            namespace Timer1{
                /*Timer/Counter1 - Output compare register B high byte
                The output compare registers contain a 16-bit value that is continuously compared with the counter value (TCNT1). A match
can be used to generate an output compare interrupt, or to generate a waveform output on the OC1x pin.
The output compare registers are 16-bit in size. To ensure that both the high and low bytes are written simultaneously when
the CPU writes to these registers, the access is performed using an 8-bit temporary high byte register (TEMP). This
temporary register is shared by all the other 16-bit registers. See Section 15.3 “Accessing 16-bit Registers” on page 91.*/
                constexpr IoRegister<rOCR1BH> OutputCompareRegB_HByte ={};
                /*Timer/Counter1 - Output compare register B low byte
                The output compare registers contain a 16-bit value that is continuously compared with the counter value (TCNT1). A match
can be used to generate an output compare interrupt, or to generate a waveform output on the OC1x pin.
The output compare registers are 16-bit in size. To ensure that both the high and low bytes are written simultaneously when
the CPU writes to these registers, the access is performed using an 8-bit temporary high byte register (TEMP). This
temporary register is shared by all the other 16-bit registers. See Section 15.3 “Accessing 16-bit Registers” on page 91.*/
                constexpr IoRegister<rOCR1BL> OutputCompareRegB_LByte ={};
                /*Timer/Counter1 - Output compare register A high byte
                The output compare registers contain a 16-bit value that is continuously compared with the counter value (TCNT1). A match
can be used to generate an output compare interrupt, or to generate a waveform output on the OC1x pin.
The output compare registers are 16-bit in size. To ensure that both the high and low bytes are written simultaneously when
the CPU writes to these registers, the access is performed using an 8-bit temporary high byte register (TEMP). This
temporary register is shared by all the other 16-bit registers. See Section 15.3 “Accessing 16-bit Registers” on page 91.*/
                constexpr IoRegister<rOCR1AH> OutputCompareRegA_HByte ={};
                /*Timer/Counter1 - Output compare register A low byte
                The output compare registers contain a 16-bit value that is continuously compared with the counter value (TCNT1). A match
can be used to generate an output compare interrupt, or to generate a waveform output on the OC1x pin.
The output compare registers are 16-bit in size. To ensure that both the high and low bytes are written simultaneously when
the CPU writes to these registers, the access is performed using an 8-bit temporary high byte register (TEMP). This
temporary register is shared by all the other 16-bit registers. See Section 15.3 “Accessing 16-bit Registers” on page 91.*/
                constexpr IoRegister<rOCR1AL> OutputCompareRegA_LByte ={};
                /*Timer/Counter1 - Input capture register high byte
                The input capture is updated with the counter (TCNT1) value each time an event occurs on the ICP1 pin (or optionally on the
analog comparator output for Timer/Counter1). The input capture can be used for defining the counter TOP value.
The input capture register is 16-bit in size. To ensure that both the high and low bytes are read simultaneously when the
CPU accesses these registers, the access is performed using an 8-bit temporary high byte register (TEMP). This temporary
register is shared by all the other 16-bit registers. See Section 15.3 “Accessing 16-bit Registers” on page 91.*/
                constexpr IoRegister<rICR1H>  InputCaptureReg_HByte   ={};
                /*Timer/Counter1 - Input capture register low byte
                The input capture is updated with the counter (TCNT1) value each time an event occurs on the ICP1 pin (or optionally on the
analog comparator output for Timer/Counter1). The input capture can be used for defining the counter TOP value.
The input capture register is 16-bit in size. To ensure that both the high and low bytes are read simultaneously when the
CPU accesses these registers, the access is performed using an 8-bit temporary high byte register (TEMP). This temporary
register is shared by all the other 16-bit registers. See Section 15.3 “Accessing 16-bit Registers” on page 91.*/
                constexpr IoRegister<rICR1L>  InputCaptureReg_LByte   ={};
                /*Timer/Counter1 - Counter register high byte
                The input capture is updated with the counter (TCNT1) value each time an event occurs on the ICP1 pin (or optionally on the
analog comparator output for Timer/Counter1). The input capture can be used for defining the counter TOP value.
The input capture register is 16-bit in size. To ensure that both the high and low bytes are read simultaneously when the
CPU accesses these registers, the access is performed using an 8-bit temporary high byte register (TEMP). This temporary
register is shared by all the other 16-bit registers. See Section 15.3 “Accessing 16-bit Registers” on page 91.*/
                constexpr IoRegister<rTCNT1H> TimerCounterReg_HByte   ={};
                /*Timer/Counter1 - Counter register low byte
                The input capture is updated with the counter (TCNT1) value each time an event occurs on the ICP1 pin (or optionally on the
analog comparator output for Timer/Counter1). The input capture can be used for defining the counter TOP value.
The input capture register is 16-bit in size. To ensure that both the high and low bytes are read simultaneously when the
CPU accesses these registers, the access is performed using an 8-bit temporary high byte register (TEMP). This temporary
register is shared by all the other 16-bit registers. See Section 15.3 “Accessing 16-bit Registers” on page 91.*/
                constexpr IoRegister<rTCNT1L> TimerCounterReg_LByte   ={};
                /*TCCR1C – Timer/Counter1 Control Register C
                | FOC1A | FOC1B | – | – | – | – | – | – |
                The FOC1A/FOC1B bits are only active when the WGM13:0 bits specifies a non-PWM mode. When writing a logical one to
the FOC1A/FOC1B bit, an immediate compare match is forced on the waveform generation unit. The OC1A/OC1B output is
changed according to its COM1x1:0 bits setting. Note that the FOC1A/FOC1B bits are implemented as strobes. Therefore it
is the value present in the COM1x1:0 bits that determine the effect of the forced compare.
A FOC1A/FOC1B strobe will not generate any interrupt nor will it clear the timer in clear timer on compare match (CTC)
mode using OCR1A as TOP. The FOC1A/FOC1B bits are always read as zero.*/
                constexpr IoRegister<rTCCR1C> TimerCounterControlRegC ={};
                /*TCCR1B – Timer/Counter1 Control Register B
                |ICNC1 | ICES1 | – | WGM13 | WGM12 | CS12 | CS11 | CS10 |
                If external pin modes are used for the Timer/Counter1, transitions on the T1 pin will clock the counter even if the pin is
configured as an output. This feature allows software control of the counting.*/
                constexpr IoRegister<rTCCR1B> TimerCounterControlRegB ={};
                /*TCCR1A – Timer/Counter1 Control Register A
                | COM1A1 | COM1A0 | COM1B1 | COM1B0 | – | – | WGM11 | WGM10 |
                The COM1A1:0 and COM1B1:0 control the output compare pins (OC1A and OC1B respectively) behavior. If one or both of
the COM1A1:0 bits are written to one, the OC1A output overrides the normal port functionality of the I/O pin it is connected
to. If one or both of the COM1B1:0 bit are written to one, the OC1B output overrides the normal port functionality of the I/O
pin it is connected to. However, note that the data direction register (DDR) bit corresponding to the OC1A or OC1B pin must
be set in order to enable the output driver.*/
                constexpr IoRegister<rTCCR1A> TimerCounterControlRegA ={};
                /*TIMSK1 – Timer/Counter1 Interrupt Mask Register
                | – | – | ICIE1 | – | – | OCIE1B | OCIE1A | TOIE1 |
                When this bit is written to one, and the I-flag in the status register is set (interrupts globally enabled), the Timer/Counter1
input capture interrupt is enabled. The corresponding interrupt vector (see Section 11. “Interrupts” on page 49) is executed
when the ICF1 flag, located in TIFR1, is set.*/
                constexpr IoRegister<rTIMSK1> TimerInterruptMaskReg   ={};
                /*TIFR1 – Timer/Counter1 Interrupt Flag Register
                | – | – | ICF1 | – | – | OCF1B | OCF1A | TOV1 |
                This flag is set when a capture event occurs on the ICP1 pin. When the input capture register (ICR1) is set by the WGM13:0
to be used as the TOP value, the ICF1 flag is set when the counter reaches the TOP value.
ICF1 is automatically cleared when the input capture interrupt vector is executed. Alternatively, ICF1 can be cleared by
writing a logic one to its bit location.*/
                constexpr IoRegister<rTIFR1>  TimerInterruptFlagReg   ={};
            }
            /*8-bit Timer/Counter0 with PWM
            Timer/Counter0 is a general purpose 8-bit Timer/Counter module, with two independent output compare units, and with
PWM support. It allows accurate program execution timing (event management) and wave generation.
A simplified block diagram of the 8-bit Timer/Counter is shown in Figure 14-1. For the actual placement of I/O pins, refer to
Section 1-1 “Pinout” on page 3. CPU accessible I/O registers, including I/O bits and I/O pins, are shown in bold.
The device-specific I/O register and bit locations are listed in the Section 14.9 “Register Description” on page 84.
The PRTIM0 bit in Section 9.10 “Minimizing Power Consumption” on page 36 must be written to zero to enable
Timer/Counter0 module.
*/
            namespace Timer0{
                /*TIMSK0 – Timer/Counter Interrupt Mask Register
                | – | – | – | – | – | OCIE0B | OCIE0A | TOIE0 |
                When the OCIE0B bit is written to one, and the I-bit in the status register is set, the Timer/Counter compare match B interrupt
is enabled. The corresponding interrupt is executed if a compare match in Timer/Counter occurs, i.e., when the OCF0B bit is
set in the Timer/Counter interrupt flag register – TIFR0.*/
                constexpr IoRegister<rTIMSK0> TimerInterruptMaskReg   ={};
                /*OCR0B – Output Compare Register B
                The output compare register B contains an 8-bit value that is continuously compared with the counter value (TCNT0). A
match can be used to generate an output compare interrupt, or to generate a waveform output on the OC0B pin.*/
                constexpr IoRegister<rOCR0B>  OutputCompareRegB       ={};
                /*OCR0A – Output Compare Register A
                The output compare register A contains an 8-bit value that is continuously compared with the counter value (TCNT0). A
match can be used to generate an output compare interrupt, or to generate a waveform output on the OC0A pin.*/
                constexpr IoRegister<rOCR0A>  OutputCompareRegA       ={};
                /*TCNT0 – Timer/Counter Register
                The Timer/Counter register gives direct access, both for read and write operations, to the Timer/Counter unit 8-bit counter.
Writing to the TCNT0 register blocks (removes) the compare match on the following timer clock. Modifying the counter
(TCNT0) while the counter is running, introduces a risk of missing a compare match between TCNT0 and the OCR0x
registers.*/
                constexpr IoRegister<rTCNT0>  TimerCounterReg         ={};
                /*TCCR0B – Timer/Counter Control Register B
                | FOC0A | FOC0B | – | – | WGM02 | CS02 | CS01 | CS00 |
                If external pin modes are used for the Timer/Counter0, transitions on the T0 pin will clock the counter even if the pin is
configured as an output. This feature allows software control of the counting.*/
                constexpr IoRegister<rTCCR0B> TimerCounterControlRegB ={};
                /*TCCR0A – Timer/Counter Control Register A
                | COM0A1 | COM0A0 | COM0B1 | COM0B0 | – | – | WGM01 | WGM00 |
                These bits control the output compare pin (OC0A) behavior. If one or both of the COM0A1:0 bits are set, the OC0A output
overrides the normal port functionality of the I/O pin it is connected to. However, note that the data direction register (DDR)
bit corresponding to the OC0A pin must be set in order to enable the output driver.
When OC0A is connected to the pin, the function of the COM0A1:0 bits depends on the WGM02:0 bit setting. Table 14-2
shows the COM0A1:0 bit functionality when the WGM02:0 bits are set to a normal or CTC mode (non-PWM).*/
                constexpr IoRegister<rTCCR0A> TimerCounterControlRegA ={};
                /*TIFR0 – Timer/Counter 0 Interrupt Flag Register
                | – | – | – | – | – | OCF0B | OCF0A | TOV0 |
                The bit TOV0 is set when an overflow occurs in Timer/Counter0. TOV0 is cleared by hardware when executing the
corresponding interrupt handling vector. Alternatively, TOV0 is cleared by writing a logic one to the flag. When the
SREG I-bit, TOIE0 (Timer/Counter0 overflow interrupt enable), and TOV0 are set, the Timer/Counter0 overflow interrupt is
executed.*/
                constexpr IoRegister<rTIFR0>  TimerInterruptFlagReg   ={};
            }
            /*GTCCR – General Timer/Counter Control Register
            | TSM | – | – | – | – | – | PSRASY | PSRSYNC |
            Writing the TSM bit to one activates the Timer/Counter synchronization mode. In this mode, the value that is written to the
PSRASY and PSRSYNC bits is kept, hence keeping the corresponding prescaler reset signals asserted. This ensures that
the corresponding Timer/Counters are halted and can be configured to the same value without the risk of one of them
advancing during configuration. When the TSM bit is written to zero, the PSRASY and PSRSYNC bits are cleared by
hardware, and the Timer/Counters start counting simultaneously.*/
            constexpr IoRegister<rGTCCR>GeneralTimerCounterControlReg= {};
        }
        namespace Adc{
            /*ADMUX – ADC Multiplexer Selection Register
            | REFS1 | REFS0 | ADLAR | – | MUX3 | MUX2 | MUX1 | MUX0 |
            */
            constexpr IoRegister<rADMUX>  AdcMultiplexerSelectionReg  = {};
            /*ADCSRB – ADC Control and Status Register B
            | – | ACME | – | – | – | ADTS2 | ADTS1 | ADTS0 |
            */
            constexpr IoRegister<rADCSRB> AdcControlAndStatusRegB= {};
            /*ADCSRA – ADC Control and Status Register A
            | ADEN | ADSC | ADATE | ADIF | ADIE | ADPS2 | ADPS1 | ADPS0 |
            */
            constexpr IoRegister<rADCSRA> AdcControlAndStatusRegA= {};
            /*ADC data register high byte*/
            constexpr IoRegister<rADCH>   AdcDataReg_HByte= {};
            /*ADC data register low byte*/
            constexpr IoRegister<rADCL>   AdcDataReg_LByte= {};
            /*ACSR – Analog Comparator Control and Status Register
            | ACD | ACBG | ACO | ACI | ACIE | ACIC | ACIS1 | ACIS0 |
            */
            constexpr IoRegister<rACSR>   AnalogComparatorControlAndStatusReg= {};
            /*DIDR1 – Digital Input Disable Register 1
            |  –  |  –  |  –  |  –  |  –  |  –  | AIN1D | AIN0D |
            When this bit is written logic one, the digital input buffer on the AIN1/0 pin is disabled. The corresponding PIN register bit will
always read as zero when this bit is set. When an analog signal is applied to the AIN1/0 pin and the digital input from this pin
is not needed, this bit should be written logic one to reduce power consumption in the digital input buffer.*/
            constexpr IoRegister<rDIDR1>  DigitalInputDisableReg1= {};
            /*DIDR0 – Digital Input Disable Register 0
            |  –  |  –  | ADC5D | ADC4D | ADC3D | ADC2D | ADC1D | ADC0D |
            When this bit is written logic one, the digital input buffer on the corresponding ADC pin is disabled. The corresponding PIN
register bit will always read as zero when this bit is set. When an analog signal is applied to the ADC5..0 pin and the digital
input from this pin is not needed, this bit should be written logic one to reduce power consumption in the digital input buffer.
Note that ADC pins ADC7 and ADC6 do not have digital input buffers, and therefore do not require digital input disable bits.*/
            constexpr IoRegister<rDIDR0>  DigitalInputDisableReg0= {};
        }
        namespace Spi{
            /*SPDR – SPI Data Register
            The SPI data register is a read/write register used for data transfer between the register file and the SPI shift register. Writing
to the register initiates data transmission. Reading the register causes the shift register Receive buffer to be read.*/
            constexpr IoRegister<rSPDR>   SpiDataReg = {};
            /*SPSR – SPI Status Register
            | SPIF | WCOL | – | – | – | – | – | SPI2X |
            When a serial transfer is complete, the SPIF Flag is set. An interrupt is generated if SPIE in SPCR is set and global
interrupts are enabled. If SS is an input and is driven low when the SPI is in master mode, this will also set the SPIF flag.
SPIF is cleared by hardware when executing the corresponding interrupt handling vector. Alternatively, the SPIF bit is
cleared by first reading the SPI status register with SPIF set, then accessing the SPI data register (SPDR).*/
            constexpr IoRegister<rSPSR>   SpiStatusReg = {};
            /*SPCR – SPI Control Register
            | SPIE | SPE | DORD | MSTR | CPOL | CPHA | SPR1 | SPR0 |
            */
            constexpr IoRegister<rSPCR>   SpiControlReg = {};
        }
        namespace Eeprom{
            /*The EEPROM Address Register High Byte*/
            constexpr IoRegister<rEEARH>  EepromAddressReg_HByte= {};
            /*The EEPROM Address Register Low Byte*/
            constexpr IoRegister<rEEARL>  EepromAddressReg_LByte= {};
            /*The EEPROM Data Register
            For the EEPROM write operation, the EEDR register contains the data to be written to the EEPROM in the address given by
the EEAR register. For the EEPROM read operation, the EEDR contains the data read out from the EEPROM at the address
given by EEAR.*/
            constexpr IoRegister<rEEDR>   EepromDataReg= {};
            /*The EEPROM Control Register*/
            constexpr IoRegister<rEECR>   EepromControlReg= {};
        }
        namespace Gpio{
            /*The Port D Data Register*/
            constexpr IoRegister<rPORTD>  PortRegD= {};
            /*The Port D Data Direction Register*/
            constexpr IoRegister<rDDRD>   DataDirectionRegD= {};
            /*The Port D Input Pins Address*/
            constexpr IoRegister<rPIND>   InputPinAddrD= {};
            /*The Port C Data Register*/
            constexpr IoRegister<rPORTC>  PortRegC= {};
            /*The Port C Data Direction Register*/
            constexpr IoRegister<rDDRC>   DataDirectionRegC= {};
            /*The Port C Input Pins Address*/
            constexpr IoRegister<rPINC>   InputPinAddrC= {};
            /*The Port B Data Register*/
            constexpr IoRegister<rPORTB>  PortRegB= {};
            /*The Port B Data Direction Register*/
            constexpr IoRegister<rDDRB>   DataDirectionRegB= {};
            /*The Port B Input Pins Address*/
            constexpr IoRegister<rPINB>   InputPinAddrB= {};
            /* General purpose I/O register 2 */
            constexpr IoRegister<rGPIOR2> GeneralPurposeIOReg2= {};
            /* General purpose I/O register 1 */
            constexpr IoRegister<rGPIOR1> GeneralPurposeIOReg1= {};
            /* General purpose I/O register 0 */
            constexpr IoRegister<rGPIOR0> GeneralPurposeIOReg0= {};
        }
        namespace Cpu{constexpr IoRegister<0xFF> rsSREG = {};}
        namespace System{}
    }

    namespace RegBits{
        namespace Uart{
            /*Bit 0 – UCPOLn: Clock Polarity
This bit is used for synchronous mode only. Write this bit to zero when asynchronous mode is used. The UCPOLn bit sets
the relationship between data output change and data input sample, and the synchronous clock (XCKn).*/
            constexpr uint8_t UCSR0C_UCPOL0 = (1 << UCPOL0);

            // USART Mode bits
            /*Bit 2:1 – UCSZn1:0: Character Size
The UCSZn1:0 bits combined with the UCSZn2 bit in UCSRnB sets the number of data bits (character size) in a frame the
receiver and transmitter use.*/
            constexpr uint8_t UCSR0C_UCSZ00 = (1 << UCSZ00);
            /*Bit 2:1 – UCSZn1:0: Character Size
The UCSZn1:0 bits combined with the UCSZn2 bit in UCSRnB sets the number of data bits (character size) in a frame the
receiver and transmitter use.*/
            constexpr uint8_t UCSR0C_UCSZ01 = (1 << UCSZ01);

            // Master SPI (MSPIM) Mode bits
            /*Bit 1 - UCPHAn: Clock Phase*/
            constexpr uint8_t UCSR0C_UCPHA0 = (1 << UCPHA0);
            /*Bit 2 - UDORDn: Data Order
When set to one the LSB of the data word is transmitted first. When set to zero the MSB of the data word is transmitted first.
Refer to the frame formats section page 4 for details.*/
            constexpr uint8_t UCSR0C_UDORD0 = (1 << UDORD0);

            /*Bit 3 – USBSn: Stop Bit Select
This bit selects the number of stop bits to be inserted by the transmitter. The receiver ignores this setting.*/
            constexpr uint8_t UCSR0C_USBS0  = (1 << USBS0);
            /*Bits 5:4 – UPMn1:0: Parity Mode
These bits enable and set type of parity generation and check. If enabled, the transmitter will automatically generate and
send the parity of the transmitted data bits within each frame. The receiver will generate a parity value for the incoming data
and compare it to the UPMn setting. If a mismatch is detected, the UPEn flag in UCSRnA will be set.*/
            constexpr uint8_t UCSR0C_UPM00  = (1 << UPM00);
            /*Bits 5:4 – UPMn1:0: Parity Mode
These bits enable and set type of parity generation and check. If enabled, the transmitter will automatically generate and
send the parity of the transmitted data bits within each frame. The receiver will generate a parity value for the incoming data
and compare it to the UPMn setting. If a mismatch is detected, the UPEn flag in UCSRnA will be set.*/
            constexpr uint8_t UCSR0C_UPM01  = (1 << UPM01);
            /*Bits 7:6 – UMSELn1:0 USART Mode Select
            These bits select the mode of operation of the USARTn as shown in Table 19-4.*/
            constexpr uint8_t UCSR0C_UMSEL00= (1 << UMSEL00);
            /*Bits 7:6 – UMSELn1:0 USART Mode Select
            These bits select the mode of operation of the USARTn as shown in Table 19-4.*/
            constexpr uint8_t UCSR0C_UMSEL01= (1 << UMSEL01);


            /*UCSR0B Bit Tanımları*/
            /*Bit 0 – TXB8n: Transmit Data Bit 8 n
TXB8n is the ninth data bit in the character to be transmitted when operating with serial frames with nine data bits. Must be
written before writing the low bits to UDRn.*/
            constexpr uint8_t UCSR0B_TXB80  = (1 << TXB80);
            /*Bit 1 – RXB8n: Receive Data Bit 8 n
RXB8n is the ninth data bit of the received character when operating with serial frames with nine data bits. Must be read
before reading the low bits from UDRn.*/
            constexpr uint8_t UCSR0B_RXB80  = (1 << RXB80);
            /*Bit 2 – UCSZn2: Character Size n
The UCSZn2 bits combined with the UCSZn1:0 bit in UCSRnC sets the number of data bits (character size) in a frame the
receiver and transmitter use.*/
            constexpr uint8_t UCSR0B_UCSZ02 = (1 << UCSZ02);
            /*Bit 3 – TXENn: Transmitter Enable n
Writing this bit to one enables the USART transmitter. The transmitter will override normal port operation for the TxDn pin
when enabled. The disabling of the transmitter (writing TXENn to zero) will not become effective until ongoing and pending
transmissions are completed, i.e., when the transmit shift register and transmit buffer register do not contain data to be
transmitted. When disabled, the transmitter will no longer override the TxDn port.*/
            constexpr uint8_t UCSR0B_TXEN0  = (1 << TXEN0);
            /*Bit 4 – RXENn: Receiver Enable n
Writing this bit to one enables the USART receiver. The receiver will override normal port operation for the RxDn pin when
enabled. Disabling the receiver will flush the receive buffer invalidating the FEn, DORn, and UPEn flags.*/
            constexpr uint8_t UCSR0B_RXEN0  = (1 << RXEN0);
            /*Bit 5 – UDRIEn: USART Data Register Empty Interrupt Enable n
Writing this bit to one enables interrupt on the UDREn flag. A data register empty interrupt will be generated only if the
UDRIEn bit is written to one, the global interrupt flag in SREG is written to one and the UDREn bit in UCSRnA is set.*/
            constexpr uint8_t UCSR0B_UDRIE0 = (1 << UDRIE0);
            /*Bit 6 – TXCIEn: TX Complete Interrupt Enable n
Writing this bit to one enables interrupt on the TXCn flag. A USART transmit complete interrupt will be generated only if the
TXCIEn bit is written to one, the global interrupt flag in SREG is written to one and the TXCn bit in UCSRnA is set.*/
            constexpr uint8_t UCSR0B_TXCIE0 = (1 << TXCIE0);
            /*Bit 7 – RXCIEn: RX Complete Interrupt Enable n
Writing this bit to one enables interrupt on the RXCn flag. A USART receive complete interrupt will be generated only if the
RXCIEn bit is written to one, the global interrupt flag in SREG is written to one and the RXCn bit in UCSRnA is set.*/
            constexpr uint8_t UCSR0B_RXCIE0 = (1 << RXCIE0);

            /*UCSR0A Bit Tanımları*/
            /*Bit 0 – MPCMn: Multi-processor Communication Mode
This bit enables the multi-processor communication mode. When the MPCMn bit is written to one, all the incoming frames
received by the USART receiver that do not contain address information will be ignored. The transmitter is unaffected by the
MPCMn setting. For more detailed information see Section 19.9 “Multi-processor Communication Mode” on page 158.*/
            constexpr uint8_t UCSR0A_MPCM0  = (1 << MPCM0);
            /*Bit 1 – U2Xn: Double the USART Transmission Speed
This bit only has effect for the asynchronous operation. Write this bit to zero when using synchronous operation.
Writing this bit to one will reduce the divisor of the baud rate divider from 16 to 8 effectively doubling the transfer rate for
asynchronous communication.*/
            constexpr uint8_t UCSR0A_U2X0   = (1 << U2X0);
            /*Bit 2 – UPEn: USART Parity Error
This bit is set if the next character in the receive buffer had a parity error when received and the parity checking was enabled
at that point (UPMn1 = 1). This bit is valid until the receive buffer (UDRn) is read. Always set this bit to zero when writing to
UCSRnA.*/
            constexpr uint8_t UCSR0A_UPE0   = (1 << UPE0);
            /*Bit 3 – DORn: Data OverRun
This bit is set if a data overrun condition is detected. A data overrun occurs when the receive buffer is full (two characters), it
is a new character waiting in the receive shift register, and a new start bit is detected. This bit is valid until the receive buffer
(UDRn) is read. Always set this bit to zero when writing to UCSRnA.*/
            constexpr uint8_t UCSR0A_DOR0   = (1 << DOR0);
            /*Bit 4 – FEn: Frame Error
This bit is set if the next character in the receive buffer had a frame error when received. I.e., when the first stop bit of the
next character in the receive buffer is zero. This bit is valid until the receive buffer (UDRn) is read. The FEn bit is zero when
the stop bit of received data is one. Always set this bit to zero when writing to UCSRnA.*/
            constexpr uint8_t UCSR0A_FE0    = (1 << FE0);
            /*Bit 5 – UDREn: USART Data Register Empty
The UDREn flag indicates if the transmit buffer (UDRn) is ready to receive new data. If UDREn is one, the buffer is empty,
and therefore ready to be written. The UDREn flag can generate a data register empty interrupt (see description of the
UDRIEn bit). UDREn is set after a reset to indicate that the transmitter is ready.*/
            constexpr uint8_t UCSR0A_UDRE0  = (1 << UDRE0);
            /*Bit 6 – TXCn: USART Transmit Complete
This flag bit is set when the entire frame in the transmit shift register has been shifted out and there are no new data currently
present in the transmit buffer (UDRn). The TXCn flag bit is automatically cleared when a transmit complete interrupt is
executed, or it can be cleared by writing a one to its bit location. The TXCn flag can generate a transmit complete interrupt
(see description of the TXCIEn bit).*/
            constexpr uint8_t UCSR0A_TXC0   = (1 << TXC0);
            /*Bit 7 – RXCn: USART Receive Complete
This flag bit is set when there are unread data in the receive buffer and cleared when the receive buffer is empty (i.e., does
not contain any unread data). If the receiver is disabled, the receive buffer will be flushed and consequently the RXCn bit will
become zero. The RXCn flag can be used to generate a receive complete interrupt (see description of the RXCIEn bit).*/
            constexpr uint8_t UCSR0A_RXC0   = (1 << RXC0);
        }
        namespace Twi{
            /* TWAMR Registeri için Bit Tanımları */
            /*Bits 7..1 – TWAM: TWI Address Mask
The TWAMR can be loaded with a 7-bit slave address mask. Each of the bits in TWAMR can mask (disable) the
corresponding address bits in the TWI address register (TWAR). If the mask bit is set to one then the address match logic
ignores the compare between the incoming address bit and the corresponding bit in TWAR. Figure 21-22 shown the address
match logic in detail.*/
            constexpr uint8_t TWAMR_TWAM0 = (1 << TWAM0);
            /*Bits 7..1 – TWAM: TWI Address Mask
The TWAMR can be loaded with a 7-bit slave address mask. Each of the bits in TWAMR can mask (disable) the
corresponding address bits in the TWI address register (TWAR). If the mask bit is set to one then the address match logic
ignores the compare between the incoming address bit and the corresponding bit in TWAR. Figure 21-22 shown the address
match logic in detail.*/
            constexpr uint8_t TWAMR_TWAM1 = (1 << TWAM1);
            /*Bits 7..1 – TWAM: TWI Address Mask
The TWAMR can be loaded with a 7-bit slave address mask. Each of the bits in TWAMR can mask (disable) the
corresponding address bits in the TWI address register (TWAR). If the mask bit is set to one then the address match logic
ignores the compare between the incoming address bit and the corresponding bit in TWAR. Figure 21-22 shown the address
match logic in detail.*/
            constexpr uint8_t TWAMR_TWAM2 = (1 << TWAM2);
            /*Bits 7..1 – TWAM: TWI Address Mask
The TWAMR can be loaded with a 7-bit slave address mask. Each of the bits in TWAMR can mask (disable) the
corresponding address bits in the TWI address register (TWAR). If the mask bit is set to one then the address match logic
ignores the compare between the incoming address bit and the corresponding bit in TWAR. Figure 21-22 shown the address
match logic in detail.*/
            constexpr uint8_t TWAMR_TWAM3 = (1 << TWAM3);
            /*Bits 7..1 – TWAM: TWI Address Mask
The TWAMR can be loaded with a 7-bit slave address mask. Each of the bits in TWAMR can mask (disable) the
corresponding address bits in the TWI address register (TWAR). If the mask bit is set to one then the address match logic
ignores the compare between the incoming address bit and the corresponding bit in TWAR. Figure 21-22 shown the address
match logic in detail.*/
            constexpr uint8_t TWAMR_TWAM4 = (1 << TWAM4);
            /*Bits 7..1 – TWAM: TWI Address Mask
The TWAMR can be loaded with a 7-bit slave address mask. Each of the bits in TWAMR can mask (disable) the
corresponding address bits in the TWI address register (TWAR). If the mask bit is set to one then the address match logic
ignores the compare between the incoming address bit and the corresponding bit in TWAR. Figure 21-22 shown the address
match logic in detail.*/
            constexpr uint8_t TWAMR_TWAM5 = (1 << TWAM5);
            /*Bits 7..1 – TWAM: TWI Address Mask
The TWAMR can be loaded with a 7-bit slave address mask. Each of the bits in TWAMR can mask (disable) the
corresponding address bits in the TWI address register (TWAR). If the mask bit is set to one then the address match logic
ignores the compare between the incoming address bit and the corresponding bit in TWAR. Figure 21-22 shown the address
match logic in detail.*/
            constexpr uint8_t TWAMR_TWAM6 = (1 << TWAM6);

            /* TWCR Registeri için Bit Tanımları */
            /*Bit 0 – TWIE: TWI Interrupt Enable
When this bit is written to one, and the I-bit in SREG is set, the TWI interrupt request will be activated for as long as the
TWINT flag is high.*/
            constexpr uint8_t TWCR_TWIE   = (1 << TWIE);
            /*Bit 2 – TWEN: TWI Enable Bit
The TWEN bit enables TWI operation and activates the TWI interface. When TWEN is written to one, the TWI takes control
over the I/O pins connected to the SCL and SDA pins, enabling the slew-rate limiters and spike filters. If this bit is written to
zero, the TWI is switched off and all TWI transmissions are terminated, regardless of any ongoing operation.*/
            constexpr uint8_t TWCR_TWEN   = (1 << TWEN);
            /*Bit 3 – TWWC: TWI Write Collision Flag
The TWWC bit is set when attempting to write to the TWI data register – TWDR when TWINT is low. This flag is cleared by
writing the TWDR register when TWINT is high.*/
            constexpr uint8_t TWCR_TWWC   = (1 << TWWC);
            /*Bit 4 – TWSTO: TWI STOP Condition Bit
Writing the TWSTO bit to one in master mode will generate a STOP condition on the 2-wire serial bus. When the STOP
condition is executed on the bus, the TWSTO bit is cleared automatically. In slave mode, setting the TWSTO bit can be used
to recover from an error condition. This will not generate a STOP condition, but the TWI returns to a well-defined
unaddressed slave mode and releases the SCL and SDA lines to a high impedance state.*/
            constexpr uint8_t TWCR_TWSTO  = (1 << TWSTO);
            /*Bit 5 – TWSTA: TWI START Condition Bit
The application writes the TWSTA bit to one when it desires to become a master on the 2-wire serial bus. The TWI hardware
checks if the bus is available, and generates a START condition on the bus if it is free. However, if the bus is not free, the
TWI waits until a STOP condition is detected, and then generates a new START condition to claim the bus master status.
TWSTA must be cleared by software when the START condition has been transmitted.*/
            constexpr uint8_t TWCR_TWSTA  = (1 << TWSTA);
            /*Bit 6 – TWEA: TWI Enable Acknowledge Bit
The TWEA bit controls the generation of the acknowledge pulse. If the TWEA bit is written to one, the ACK pulse is
generated on the TWI bus if the following conditions are met:
1. The device’s own slave address has been received.
2.A general call has been received, while the TWGCE bit in the TWAR is set.
3.A data byte has been received in master receiver or slave receiver mode.
By writing the TWEA bit to zero, the device can be virtually disconnected from the 2-wire serial bus temporarily. Address
recognition can then be resumed by writing the TWEA bit to one again.*/
            constexpr uint8_t TWCR_TWEA   = (1 << TWEA);
            /*Bit 7 – TWINT: TWI Interrupt Flag
This bit is set by hardware when the TWI has finished its current job and expects application software response. If the I-bit in
SREG and TWIE in TWCR are set, the MCU will jump to the TWI interrupt vector. While the TWINT flag is set, the SCL low
period is stretched. The TWINT flag must be cleared by software by writing a logic one to it. Note that this flag is not
automatically cleared by hardware when executing the interrupt routine. Also note that clearing this flag starts the operation
of the TWI, so all accesses to the TWI address register (TWAR), TWI status register (TWSR), and TWI data register (TWDR)
must be complete before clearing this flag.*/
            constexpr uint8_t TWCR_TWINT  = (1 << TWINT);

            /* TWAR Registeri için Bit Tanımları */
            /*Bit 0 – TWGCE: TWI General Call Recognition Enable Bit
If set, this bit enables the recognition of a general call given over the 2-wire serial bus.*/
            constexpr uint8_t TWAR_TWGCE  = (1 << TWGCE);
            /*Bits 7..1 – TWA: TWI (Slave) Address Register
These seven bits constitute the slave address of the TWI unit.*/
            constexpr uint8_t TWAR_TWA0   = (1 << TWA0);
            /*Bits 7..1 – TWA: TWI (Slave) Address Register
These seven bits constitute the slave address of the TWI unit.*/
            constexpr uint8_t TWAR_TWA1   = (1 << TWA1);
            /*Bits 7..1 – TWA: TWI (Slave) Address Register
These seven bits constitute the slave address of the TWI unit.*/
            constexpr uint8_t TWAR_TWA2   = (1 << TWA2);
            /*Bits 7..1 – TWA: TWI (Slave) Address Register
These seven bits constitute the slave address of the TWI unit.*/
            constexpr uint8_t TWAR_TWA3   = (1 << TWA3);
            /*Bits 7..1 – TWA: TWI (Slave) Address Register
These seven bits constitute the slave address of the TWI unit.*/
            constexpr uint8_t TWAR_TWA4   = (1 << TWA4);
            /*Bits 7..1 – TWA: TWI (Slave) Address Register
These seven bits constitute the slave address of the TWI unit.*/
            constexpr uint8_t TWAR_TWA5   = (1 << TWA5);
            /*Bits 7..1 – TWA: TWI (Slave) Address Register
These seven bits constitute the slave address of the TWI unit.*/
            constexpr uint8_t TWAR_TWA6   = (1 << TWA6);

            /* TWSR Registeri için Bit Tanımları */
            /*Bits 1..0 – TWPS: TWI Prescaler Bits
These bits can be read and written, and control the bit rate prescaler.*/
            constexpr uint8_t TWSR_TWPS0  = (1 << TWPS0);
            /*Bits 1..0 – TWPS: TWI Prescaler Bits
These bits can be read and written, and control the bit rate prescaler.*/
            constexpr uint8_t TWSR_TWPS1  = (1 << TWPS1);
            /*Bits 7..3 – TWS: TWI Status
These 5 bits reflect the status of the TWI logic and the 2-wire serial bus. The different status codes are described later in this
section. Note that the value read from TWSR contains both the 5-bit status value and the 2-bit prescaler value. The
application designer should mask the prescaler bits to zero when checking the status bits. This makes status checking
independent of prescaler setting. This approach is used in this datasheet, unless otherwise noted.*/
            constexpr uint8_t TWSR_TWS3   = (1 << TWS3);
            /*Bits 7..3 – TWS: TWI Status
These 5 bits reflect the status of the TWI logic and the 2-wire serial bus. The different status codes are described later in this
section. Note that the value read from TWSR contains both the 5-bit status value and the 2-bit prescaler value. The
application designer should mask the prescaler bits to zero when checking the status bits. This makes status checking
independent of prescaler setting. This approach is used in this datasheet, unless otherwise noted.*/
            constexpr uint8_t TWSR_TWS4   = (1 << TWS4);
            /*Bits 7..3 – TWS: TWI Status
These 5 bits reflect the status of the TWI logic and the 2-wire serial bus. The different status codes are described later in this
section. Note that the value read from TWSR contains both the 5-bit status value and the 2-bit prescaler value. The
application designer should mask the prescaler bits to zero when checking the status bits. This makes status checking
independent of prescaler setting. This approach is used in this datasheet, unless otherwise noted.*/
            constexpr uint8_t TWSR_TWS5   = (1 << TWS5);
            /*Bits 7..3 – TWS: TWI Status
These 5 bits reflect the status of the TWI logic and the 2-wire serial bus. The different status codes are described later in this
section. Note that the value read from TWSR contains both the 5-bit status value and the 2-bit prescaler value. The
application designer should mask the prescaler bits to zero when checking the status bits. This makes status checking
independent of prescaler setting. This approach is used in this datasheet, unless otherwise noted.*/
            constexpr uint8_t TWSR_TWS6   = (1 << TWS6);
            /*Bits 7..3 – TWS: TWI Status
These 5 bits reflect the status of the TWI logic and the 2-wire serial bus. The different status codes are described later in this
section. Note that the value read from TWSR contains both the 5-bit status value and the 2-bit prescaler value. The
application designer should mask the prescaler bits to zero when checking the status bits. This makes status checking
independent of prescaler setting. This approach is used in this datasheet, unless otherwise noted.*/
            constexpr uint8_t TWSR_TWS7   = (1 << TWS7);
        }
        namespace Core{

            /* DIDR1 REG BITS */
            /*Bit 1, 0 – AIN1D, AIN0D: AIN1, AIN0 Digital Input Disable
When this bit is written logic one, the digital input buffer on the AIN1/0 pin is disabled. The corresponding PIN register bit will
always read as zero when this bit is set. When an analog signal is applied to the AIN1/0 pin and the digital input from this pin
is not needed, this bit should be written logic one to reduce power consumption in the digital input buffer.*/
            constexpr uint8_t DIDR1_AIN0D  = (1 << AIN0D);
            /*Bit 1, 0 – AIN1D, AIN0D: AIN1, AIN0 Digital Input Disable
When this bit is written logic one, the digital input buffer on the AIN1/0 pin is disabled. The corresponding PIN register bit will
always read as zero when this bit is set. When an analog signal is applied to the AIN1/0 pin and the digital input from this pin
is not needed, this bit should be written logic one to reduce power consumption in the digital input buffer.*/
            constexpr uint8_t DIDR1_AIN1D  = (1 << AIN1D);

            /* PCMSK2 */
            /*Bit 7..0 – PCINT23..16: Pin Change Enable Mask 23..16
Each PCINT23..16-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is set
and the PCIE2 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is cleared,
pin change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK2_PCINT16 = (1 << PCINT16);
            /*Bit 7..0 – PCINT23..16: Pin Change Enable Mask 23..16
Each PCINT23..16-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is set
and the PCIE2 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is cleared,
pin change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK2_PCINT17 = (1 << PCINT17);
            /*Bit 7..0 – PCINT23..16: Pin Change Enable Mask 23..16
Each PCINT23..16-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is set
and the PCIE2 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is cleared,
pin change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK2_PCINT18 = (1 << PCINT18);
            /*Bit 7..0 – PCINT23..16: Pin Change Enable Mask 23..16
Each PCINT23..16-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is set
and the PCIE2 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is cleared,
pin change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK2_PCINT19 = (1 << PCINT19);
            /*Bit 7..0 – PCINT23..16: Pin Change Enable Mask 23..16
Each PCINT23..16-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is set
and the PCIE2 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is cleared,
pin change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK2_PCINT20 = (1 << PCINT20);
            /*Bit 7..0 – PCINT23..16: Pin Change Enable Mask 23..16
Each PCINT23..16-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is set
and the PCIE2 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is cleared,
pin change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK2_PCINT21 = (1 << PCINT21);
            /*Bit 7..0 – PCINT23..16: Pin Change Enable Mask 23..16
Each PCINT23..16-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is set
and the PCIE2 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is cleared,
pin change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK2_PCINT22 = (1 << PCINT22);
            /*Bit 7..0 – PCINT23..16: Pin Change Enable Mask 23..16
Each PCINT23..16-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is set
and the PCIE2 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is cleared,
pin change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK2_PCINT23 = (1 << PCINT23);

            /* PCMSK1 */
            /*Bit 6..0 – PCINT14..8: Pin Change Enable Mask 14..8
Each PCINT14..8-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is set and
the PCIE1 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is cleared, pin
change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK1_PCINT8  = (1 << PCINT8);
            /*Bit 6..0 – PCINT14..8: Pin Change Enable Mask 14..8
Each PCINT14..8-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is set and
the PCIE1 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is cleared, pin
change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK1_PCINT9  = (1 << PCINT9);
            /*Bit 6..0 – PCINT14..8: Pin Change Enable Mask 14..8
Each PCINT14..8-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is set and
the PCIE1 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is cleared, pin
change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK1_PCINT10 = (1 << PCINT10);
            /*Bit 6..0 – PCINT14..8: Pin Change Enable Mask 14..8
Each PCINT14..8-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is set and
the PCIE1 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is cleared, pin
change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK1_PCINT11 = (1 << PCINT11);
            /*Bit 6..0 – PCINT14..8: Pin Change Enable Mask 14..8
Each PCINT14..8-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is set and
the PCIE1 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is cleared, pin
change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK1_PCINT12 = (1 << PCINT12);
            /*Bit 6..0 – PCINT14..8: Pin Change Enable Mask 14..8
Each PCINT14..8-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is set and
the PCIE1 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is cleared, pin
change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK1_PCINT13 = (1 << PCINT13);
            /*Bit 6..0 – PCINT14..8: Pin Change Enable Mask 14..8
Each PCINT14..8-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is set and
the PCIE1 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT14..8 is cleared, pin
change interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK1_PCINT14 = (1 << PCINT14);

            /* PCMSK0 */
            /*Bit 7..0 – PCINT7..0: Pin Change Enable Mask 7..0
Each PCINT7..0 bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is set and the
PCIE0 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is cleared, pin change
interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK0_PCINT0 = (1 << PCINT0);
            /*Bit 7..0 – PCINT7..0: Pin Change Enable Mask 7..0
Each PCINT7..0 bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is set and the
PCIE0 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is cleared, pin change
interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK0_PCINT1 = (1 << PCINT1);
            /*Bit 7..0 – PCINT7..0: Pin Change Enable Mask 7..0
Each PCINT7..0 bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is set and the
PCIE0 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is cleared, pin change
interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK0_PCINT2 = (1 << PCINT2);
            /*Bit 7..0 – PCINT7..0: Pin Change Enable Mask 7..0
Each PCINT7..0 bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is set and the
PCIE0 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is cleared, pin change
interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK0_PCINT3 = (1 << PCINT3);
            /*Bit 7..0 – PCINT7..0: Pin Change Enable Mask 7..0
Each PCINT7..0 bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is set and the
PCIE0 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is cleared, pin change
interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK0_PCINT4 = (1 << PCINT4);
            /*Bit 7..0 – PCINT7..0: Pin Change Enable Mask 7..0
Each PCINT7..0 bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is set and the
PCIE0 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is cleared, pin change
interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK0_PCINT5 = (1 << PCINT5);
            /*Bit 7..0 – PCINT7..0: Pin Change Enable Mask 7..0
Each PCINT7..0 bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is set and the
PCIE0 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is cleared, pin change
interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK0_PCINT6 = (1 << PCINT6);
            /*Bit 7..0 – PCINT7..0: Pin Change Enable Mask 7..0
Each PCINT7..0 bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is set and the
PCIE0 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT7..0 is cleared, pin change
interrupt on the corresponding I/O pin is disabled.*/
            constexpr uint8_t PCMSK0_PCINT7 = (1 << PCINT7);

            /* EICRA */
            /*Bit 1, 0 – ISC01, ISC00: Interrupt Sense Control 0 Bit 1 and Bit 0
The external interrupt 0 is activated by the external pin INT0 if the SREG I-flag and the corresponding interrupt mask are set.
The level and edges on the external INT0 pin that activate the interrupt are defined in Table 12-2. The value on the INT0 pin
is sampled before detecting edges. If edge or toggle interrupt is selected, pulses that last longer than one clock period will
generate an interrupt. Shorter pulses are not guaranteed to generate an interrupt. If low level interrupt is selected, the low
level must be held until the completion of the currently executing instruction to generate an interrupt.*/
            constexpr uint8_t EICRA_ISC00 = (1 << ISC00);
            /*Bit 1, 0 – ISC01, ISC00: Interrupt Sense Control 0 Bit 1 and Bit 0
The external interrupt 0 is activated by the external pin INT0 if the SREG I-flag and the corresponding interrupt mask are set.
The level and edges on the external INT0 pin that activate the interrupt are defined in Table 12-2. The value on the INT0 pin
is sampled before detecting edges. If edge or toggle interrupt is selected, pulses that last longer than one clock period will
generate an interrupt. Shorter pulses are not guaranteed to generate an interrupt. If low level interrupt is selected, the low
level must be held until the completion of the currently executing instruction to generate an interrupt.*/
            constexpr uint8_t EICRA_ISC01 = (1 << ISC01);
            /*Bit 3, 2 – ISC11, ISC10: Interrupt Sense Control 1 Bit 1 and Bit 0
The external interrupt 1 is activated by the external pin INT1 if the SREG I-flag and the corresponding interrupt mask are set.
The level and edges on the external INT1 pin that activate the interrupt are defined in Table 12-1. The value on the INT1 pin
is sampled before detecting edges. If edge or toggle interrupt is selected, pulses that last longer than one clock period will
generate an interrupt. Shorter pulses are not guaranteed to generate an interrupt. If low level interrupt is selected, the low
level must be held until the completion of the currently executing instruction to generate an interrupt.*/
            constexpr uint8_t EICRA_ISC10 = (1 << ISC10);
            /*Bit 3, 2 – ISC11, ISC10: Interrupt Sense Control 1 Bit 1 and Bit 0
The external interrupt 1 is activated by the external pin INT1 if the SREG I-flag and the corresponding interrupt mask are set.
The level and edges on the external INT1 pin that activate the interrupt are defined in Table 12-1. The value on the INT1 pin
is sampled before detecting edges. If edge or toggle interrupt is selected, pulses that last longer than one clock period will
generate an interrupt. Shorter pulses are not guaranteed to generate an interrupt. If low level interrupt is selected, the low
level must be held until the completion of the currently executing instruction to generate an interrupt.*/
            constexpr uint8_t EICRA_ISC11 = (1 << ISC11);

            /* PCICR */
            /*Bit 0 - PCIE0: Pin Change Interrupt Enable 0
When the PCIE0 bit is set (one) and the I-bit in the status register (SREG) is set (one), pin change interrupt 0 is enabled. Any
change on any enabled PCINT7..0 pin will cause an interrupt. The corresponding interrupt of pin change interrupt request is
executed from the PCI0 interrupt vector. PCINT7..0 pins are enabled individually by the PCMSK0 register.*/
            constexpr uint8_t PCICR_PCIE0 = (1 << PCIE0);
            /*Bit 1 - PCIE1: Pin Change Interrupt Enable 1
When the PCIE1 bit is set (one) and the I-bit in the status register (SREG) is set (one), pin change interrupt 1 is enabled. Any
change on any enabled PCINT14..8 pin will cause an interrupt. The corresponding interrupt of pin change interrupt request is
executed from the PCI1 interrupt vector. PCINT14..8 pins are enabled individually by the PCMSK1 register.*/
            constexpr uint8_t PCICR_PCIE1 = (1 << PCIE1);
            /*Bit 2 - PCIE2: Pin Change Interrupt Enable 2
When the PCIE2 bit is set (one) and the I-bit in the status register (SREG) is set (one), pin change interrupt 2 is enabled. Any
change on any enabled PCINT23..16 pin will cause an interrupt. The corresponding interrupt of pin change interrupt request
is executed from the PCI2 interrupt vector. PCINT23..16 pins are enabled individually by the PCMSK2 register.*/
            constexpr uint8_t PCICR_PCIE2 = (1 << PCIE2);

            /* OSCCAL */
            /*Bits 7..0 – CAL7..0: Oscillator Calibration Value
The oscillator calibration register is used to trim the calibrated internal RC oscillator to remove process variations from the
oscillator frequency. A pre-programmed calibration value is automatically written to this register during chip reset, giving the
factory calibrated frequency as specified in Table 28-1 on page 260. The application software can write this register to
change the oscillator frequency. The oscillator can be calibrated to frequencies as specified in Table 28-1 on page 260.
Calibration outside that range is not guaranteed.
Note that this oscillator is used to time EEPROM and flash write accesses, and these write times will be affected accordingly.
If the EEPROM or flash are written, do not calibrate to more than 8.8MHz. Otherwise, the EEPROM or flash write may fail.
The CAL7 bit determines the range of operation for the oscillator. Setting this bit to 0 gives the lowest frequency range,
setting this bit to 1 gives the highest frequency range. The two frequency ranges are overlapping, in other words a setting of
OSCCAL = 0x7F gives a higher frequency than OSCCAL = 0x80.
The CAL6..0 bits are used to tune the frequency within the selected range. A setting of 0x00 gives the lowest frequency in
that range, and a setting of 0x7F gives the highest frequency in the range.*/
            constexpr uint8_t OSCCAL_CAL0 = (1 << CAL0);
            /*Bits 7..0 – CAL7..0: Oscillator Calibration Value
The oscillator calibration register is used to trim the calibrated internal RC oscillator to remove process variations from the
oscillator frequency. A pre-programmed calibration value is automatically written to this register during chip reset, giving the
factory calibrated frequency as specified in Table 28-1 on page 260. The application software can write this register to
change the oscillator frequency. The oscillator can be calibrated to frequencies as specified in Table 28-1 on page 260.
Calibration outside that range is not guaranteed.
Note that this oscillator is used to time EEPROM and flash write accesses, and these write times will be affected accordingly.
If the EEPROM or flash are written, do not calibrate to more than 8.8MHz. Otherwise, the EEPROM or flash write may fail.
The CAL7 bit determines the range of operation for the oscillator. Setting this bit to 0 gives the lowest frequency range,
setting this bit to 1 gives the highest frequency range. The two frequency ranges are overlapping, in other words a setting of
OSCCAL = 0x7F gives a higher frequency than OSCCAL = 0x80.
The CAL6..0 bits are used to tune the frequency within the selected range. A setting of 0x00 gives the lowest frequency in
that range, and a setting of 0x7F gives the highest frequency in the range.*/
            constexpr uint8_t OSCCAL_CAL1 = (1 << CAL1);
            /*Bits 7..0 – CAL7..0: Oscillator Calibration Value
The oscillator calibration register is used to trim the calibrated internal RC oscillator to remove process variations from the
oscillator frequency. A pre-programmed calibration value is automatically written to this register during chip reset, giving the
factory calibrated frequency as specified in Table 28-1 on page 260. The application software can write this register to
change the oscillator frequency. The oscillator can be calibrated to frequencies as specified in Table 28-1 on page 260.
Calibration outside that range is not guaranteed.
Note that this oscillator is used to time EEPROM and flash write accesses, and these write times will be affected accordingly.
If the EEPROM or flash are written, do not calibrate to more than 8.8MHz. Otherwise, the EEPROM or flash write may fail.
The CAL7 bit determines the range of operation for the oscillator. Setting this bit to 0 gives the lowest frequency range,
setting this bit to 1 gives the highest frequency range. The two frequency ranges are overlapping, in other words a setting of
OSCCAL = 0x7F gives a higher frequency than OSCCAL = 0x80.
The CAL6..0 bits are used to tune the frequency within the selected range. A setting of 0x00 gives the lowest frequency in
that range, and a setting of 0x7F gives the highest frequency in the range.*/
            constexpr uint8_t OSCCAL_CAL2 = (1 << CAL2);
            /*Bits 7..0 – CAL7..0: Oscillator Calibration Value
The oscillator calibration register is used to trim the calibrated internal RC oscillator to remove process variations from the
oscillator frequency. A pre-programmed calibration value is automatically written to this register during chip reset, giving the
factory calibrated frequency as specified in Table 28-1 on page 260. The application software can write this register to
change the oscillator frequency. The oscillator can be calibrated to frequencies as specified in Table 28-1 on page 260.
Calibration outside that range is not guaranteed.
Note that this oscillator is used to time EEPROM and flash write accesses, and these write times will be affected accordingly.
If the EEPROM or flash are written, do not calibrate to more than 8.8MHz. Otherwise, the EEPROM or flash write may fail.
The CAL7 bit determines the range of operation for the oscillator. Setting this bit to 0 gives the lowest frequency range,
setting this bit to 1 gives the highest frequency range. The two frequency ranges are overlapping, in other words a setting of
OSCCAL = 0x7F gives a higher frequency than OSCCAL = 0x80.
The CAL6..0 bits are used to tune the frequency within the selected range. A setting of 0x00 gives the lowest frequency in
that range, and a setting of 0x7F gives the highest frequency in the range.*/
            constexpr uint8_t OSCCAL_CAL3 = (1 << CAL3);
            /*Bits 7..0 – CAL7..0: Oscillator Calibration Value
The oscillator calibration register is used to trim the calibrated internal RC oscillator to remove process variations from the
oscillator frequency. A pre-programmed calibration value is automatically written to this register during chip reset, giving the
factory calibrated frequency as specified in Table 28-1 on page 260. The application software can write this register to
change the oscillator frequency. The oscillator can be calibrated to frequencies as specified in Table 28-1 on page 260.
Calibration outside that range is not guaranteed.
Note that this oscillator is used to time EEPROM and flash write accesses, and these write times will be affected accordingly.
If the EEPROM or flash are written, do not calibrate to more than 8.8MHz. Otherwise, the EEPROM or flash write may fail.
The CAL7 bit determines the range of operation for the oscillator. Setting this bit to 0 gives the lowest frequency range,
setting this bit to 1 gives the highest frequency range. The two frequency ranges are overlapping, in other words a setting of
OSCCAL = 0x7F gives a higher frequency than OSCCAL = 0x80.
The CAL6..0 bits are used to tune the frequency within the selected range. A setting of 0x00 gives the lowest frequency in
that range, and a setting of 0x7F gives the highest frequency in the range.*/
            constexpr uint8_t OSCCAL_CAL4 = (1 << CAL4);
            /*Bits 7..0 – CAL7..0: Oscillator Calibration Value
The oscillator calibration register is used to trim the calibrated internal RC oscillator to remove process variations from the
oscillator frequency. A pre-programmed calibration value is automatically written to this register during chip reset, giving the
factory calibrated frequency as specified in Table 28-1 on page 260. The application software can write this register to
change the oscillator frequency. The oscillator can be calibrated to frequencies as specified in Table 28-1 on page 260.
Calibration outside that range is not guaranteed.
Note that this oscillator is used to time EEPROM and flash write accesses, and these write times will be affected accordingly.
If the EEPROM or flash are written, do not calibrate to more than 8.8MHz. Otherwise, the EEPROM or flash write may fail.
The CAL7 bit determines the range of operation for the oscillator. Setting this bit to 0 gives the lowest frequency range,
setting this bit to 1 gives the highest frequency range. The two frequency ranges are overlapping, in other words a setting of
OSCCAL = 0x7F gives a higher frequency than OSCCAL = 0x80.
The CAL6..0 bits are used to tune the frequency within the selected range. A setting of 0x00 gives the lowest frequency in
that range, and a setting of 0x7F gives the highest frequency in the range.*/
            constexpr uint8_t OSCCAL_CAL5 = (1 << CAL5);
            /*Bits 7..0 – CAL7..0: Oscillator Calibration Value
The oscillator calibration register is used to trim the calibrated internal RC oscillator to remove process variations from the
oscillator frequency. A pre-programmed calibration value is automatically written to this register during chip reset, giving the
factory calibrated frequency as specified in Table 28-1 on page 260. The application software can write this register to
change the oscillator frequency. The oscillator can be calibrated to frequencies as specified in Table 28-1 on page 260.
Calibration outside that range is not guaranteed.
Note that this oscillator is used to time EEPROM and flash write accesses, and these write times will be affected accordingly.
If the EEPROM or flash are written, do not calibrate to more than 8.8MHz. Otherwise, the EEPROM or flash write may fail.
The CAL7 bit determines the range of operation for the oscillator. Setting this bit to 0 gives the lowest frequency range,
setting this bit to 1 gives the highest frequency range. The two frequency ranges are overlapping, in other words a setting of
OSCCAL = 0x7F gives a higher frequency than OSCCAL = 0x80.
The CAL6..0 bits are used to tune the frequency within the selected range. A setting of 0x00 gives the lowest frequency in
that range, and a setting of 0x7F gives the highest frequency in the range.*/
            constexpr uint8_t OSCCAL_CAL6 = (1 << CAL6);
            /*Bits 7..0 – CAL7..0: Oscillator Calibration Value
The oscillator calibration register is used to trim the calibrated internal RC oscillator to remove process variations from the
oscillator frequency. A pre-programmed calibration value is automatically written to this register during chip reset, giving the
factory calibrated frequency as specified in Table 28-1 on page 260. The application software can write this register to
change the oscillator frequency. The oscillator can be calibrated to frequencies as specified in Table 28-1 on page 260.
Calibration outside that range is not guaranteed.
Note that this oscillator is used to time EEPROM and flash write accesses, and these write times will be affected accordingly.
If the EEPROM or flash are written, do not calibrate to more than 8.8MHz. Otherwise, the EEPROM or flash write may fail.
The CAL7 bit determines the range of operation for the oscillator. Setting this bit to 0 gives the lowest frequency range,
setting this bit to 1 gives the highest frequency range. The two frequency ranges are overlapping, in other words a setting of
OSCCAL = 0x7F gives a higher frequency than OSCCAL = 0x80.
The CAL6..0 bits are used to tune the frequency within the selected range. A setting of 0x00 gives the lowest frequency in
that range, and a setting of 0x7F gives the highest frequency in the range.*/
            constexpr uint8_t OSCCAL_CAL7 = (1 << CAL7);

            /* PRR */
            /*Bit 0 - PRADC: Power Reduction ADC
Writing a logic one to this bit shuts down the ADC. The ADC must be disabled before shut down. The analog comparator
cannot use the ADC input MUX when the ADC is shut down.*/
            constexpr uint8_t PRR_PRADC   = (1 << PRADC);
            /*Bit 1 - PRUSART0: Power Reduction USART0
Writing a logic one to this bit shuts down the USART by stopping the clock to the module. When waking up the USART
again, the USART should be re initialized to ensure proper operation.*/
            constexpr uint8_t PRR_PRUSART0 = (1 << PRUSART0);
            /*Bit 2 - PRSPI: Power Reduction Serial Peripheral Interface
If using debugWIRE on-chip debug system, this bit should not be written to one.
Writing a logic one to this bit shuts down the serial peripheral interface by stopping the clock to the module. When waking up
the SPI again, the SPI should be re initialized to ensure proper operation.*/
            constexpr uint8_t PRR_PRSPI   = (1 << PRSPI);
            /*Bit 3 - PRTIM1: Power Reduction Timer/Counter1
Writing a logic one to this bit shuts down the Timer/Counter1 module. When the Timer/Counter1 is enabled, operation will
continue like before the shutdown.*/
            constexpr uint8_t PRR_PRTIM1  = (1 << PRTIM1);
            /*Bit 5 - PRTIM0: Power Reduction Timer/Counter0
Writing a logic one to this bit shuts down the Timer/Counter0 module. When the Timer/Counter0 is enabled, operation will
continue like before the shutdown.*/
            constexpr uint8_t PRR_PRTIM0  = (1 << PRTIM0);
            /*Bit 6 - PRTIM2: Power Reduction Timer/Counter2
Writing a logic one to this bit shuts down the Timer/Counter2 module in synchronous mode (AS2 is 0). When the
Timer/Counter2 is enabled, operation will continue like before the shutdown.*/
            constexpr uint8_t PRR_PRTIM2  = (1 << PRTIM2);
            /*Bit 7 - PRTWI: Power Reduction TWI
Writing a logic one to this bit shuts down the TWI by stopping the clock to the module. When waking up the TWI again, the
TWI should be re initialized to ensure proper operation.*/
            constexpr uint8_t PRR_PRTWI   = (1 << PRTWI);

            /* CLKPR */
            /*Bits 3..0 – CLKPS3..0: Clock Prescaler Select Bits 3 - 0
These bits define the division factor between the selected clock source and the internal system clock. These bits can be
written run-time to vary the clock frequency to suit the application requirements. As the divider divides the master clock input
to the MCU, the speed of all synchronous peripherals is reduced when a division factor is used. The division factors are
given in Table 8-16.
The CKDIV8 fuse determines the initial value of the CLKPS bits. If CKDIV8 is unprogrammed, the CLKPS bits will be reset to
“0000”. If CKDIV8 is programmed, CLKPS bits are reset to “0011”, giving a division factor of 8 at start up. This feature should
be used if the selected clock source has a higher frequency than the maximum frequency of the device at the present
operating conditions. Note that any value can be written to the CLKPS bits regardless of the CKDIV8 fuse setting. The
application software must ensure that a sufficient division factor is chosen if the selected clock source has a higher
frequency than the maximum frequency of the device at the present operating conditions. The device is shipped with the
CKDIV8 fuse programmed.*/
            constexpr uint8_t CLKPR_CLKPS0= (1 << CLKPS0);
            /*Bits 3..0 – CLKPS3..0: Clock Prescaler Select Bits 3 - 0
These bits define the division factor between the selected clock source and the internal system clock. These bits can be
written run-time to vary the clock frequency to suit the application requirements. As the divider divides the master clock input
to the MCU, the speed of all synchronous peripherals is reduced when a division factor is used. The division factors are
given in Table 8-16.
The CKDIV8 fuse determines the initial value of the CLKPS bits. If CKDIV8 is unprogrammed, the CLKPS bits will be reset to
“0000”. If CKDIV8 is programmed, CLKPS bits are reset to “0011”, giving a division factor of 8 at start up. This feature should
be used if the selected clock source has a higher frequency than the maximum frequency of the device at the present
operating conditions. Note that any value can be written to the CLKPS bits regardless of the CKDIV8 fuse setting. The
application software must ensure that a sufficient division factor is chosen if the selected clock source has a higher
frequency than the maximum frequency of the device at the present operating conditions. The device is shipped with the
CKDIV8 fuse programmed.*/
            constexpr uint8_t CLKPR_CLKPS1= (1 << CLKPS1);
            /*Bits 3..0 – CLKPS3..0: Clock Prescaler Select Bits 3 - 0
These bits define the division factor between the selected clock source and the internal system clock. These bits can be
written run-time to vary the clock frequency to suit the application requirements. As the divider divides the master clock input
to the MCU, the speed of all synchronous peripherals is reduced when a division factor is used. The division factors are
given in Table 8-16.
The CKDIV8 fuse determines the initial value of the CLKPS bits. If CKDIV8 is unprogrammed, the CLKPS bits will be reset to
“0000”. If CKDIV8 is programmed, CLKPS bits are reset to “0011”, giving a division factor of 8 at start up. This feature should
be used if the selected clock source has a higher frequency than the maximum frequency of the device at the present
operating conditions. Note that any value can be written to the CLKPS bits regardless of the CKDIV8 fuse setting. The
application software must ensure that a sufficient division factor is chosen if the selected clock source has a higher
frequency than the maximum frequency of the device at the present operating conditions. The device is shipped with the
CKDIV8 fuse programmed.*/
            constexpr uint8_t CLKPR_CLKPS2= (1 << CLKPS2);
            /*Bits 3..0 – CLKPS3..0: Clock Prescaler Select Bits 3 - 0
These bits define the division factor between the selected clock source and the internal system clock. These bits can be
written run-time to vary the clock frequency to suit the application requirements. As the divider divides the master clock input
to the MCU, the speed of all synchronous peripherals is reduced when a division factor is used. The division factors are
given in Table 8-16.
The CKDIV8 fuse determines the initial value of the CLKPS bits. If CKDIV8 is unprogrammed, the CLKPS bits will be reset to
“0000”. If CKDIV8 is programmed, CLKPS bits are reset to “0011”, giving a division factor of 8 at start up. This feature should
be used if the selected clock source has a higher frequency than the maximum frequency of the device at the present
operating conditions. Note that any value can be written to the CLKPS bits regardless of the CKDIV8 fuse setting. The
application software must ensure that a sufficient division factor is chosen if the selected clock source has a higher
frequency than the maximum frequency of the device at the present operating conditions. The device is shipped with the
CKDIV8 fuse programmed.*/
            constexpr uint8_t CLKPR_CLKPS3= (1 << CLKPS3);
            /*Bit 7 – CLKPCE: Clock Prescaler Change Enable
The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the
other bits in CLKPR are simultaneously written to zero. CLKPCE is cleared by hardware four cycles after it is written or when
CLKPS bits are written. Rewriting the CLKPCE bit within this time-out period does neither extend the time-out period, nor
clear the CLKPCE bit.*/
            constexpr uint8_t CLKPR_CLKPCE= (1 << CLKPCE);

            /* WDTCSR */
            /*Bit 5, 2..0 - WDP3..0: Watchdog Timer Prescaler 3, 2, 1 and 0
The WDP3..0 bits determine the watchdog timer prescaling when the watchdog timer is running. The different prescaling
values and their corresponding time-out periods are shown in Table 10-3.*/
            constexpr uint8_t WDTCSR_WDP0 = (1 << WDP0);
            /*Bit 5, 2..0 - WDP3..0: Watchdog Timer Prescaler 3, 2, 1 and 0
The WDP3..0 bits determine the watchdog timer prescaling when the watchdog timer is running. The different prescaling
values and their corresponding time-out periods are shown in Table 10-3.*/
            constexpr uint8_t WDTCSR_WDP1 = (1 << WDP1);
            /*Bit 5, 2..0 - WDP3..0: Watchdog Timer Prescaler 3, 2, 1 and 0
The WDP3..0 bits determine the watchdog timer prescaling when the watchdog timer is running. The different prescaling
values and their corresponding time-out periods are shown in Table 10-3.*/
            constexpr uint8_t WDTCSR_WDP2 = (1 << WDP2);
            /*Bit 3 - WDE: Watchdog System Reset Enable
WDE is overridden by WDRF in MCUSR. This means that WDE is always set when WDRF is set. To clear WDE, WDRF
must be cleared first. This feature ensures multiple resets during conditions causing failure, and a safe start-up after the
failure.*/
            constexpr uint8_t WDTCSR_WDE  = (1 << WDE);
            /*Bit 4 - WDCE: Watchdog Change Enable
This bit is used in timed sequences for changing WDE and prescaler bits. To clear the WDE bit, and/or change the prescaler
bits, WDCE must be set.
Once written to one, hardware will clear WDCE after four clock cycles.*/
            constexpr uint8_t WDTCSR_WDCE = (1 << WDCE);
            /*Bit 5, 2..0 - WDP3..0: Watchdog Timer Prescaler 3, 2, 1 and 0
The WDP3..0 bits determine the watchdog timer prescaling when the watchdog timer is running. The different prescaling
values and their corresponding time-out periods are shown in Table 10-3.*/
            constexpr uint8_t WDTCSR_WDP3 = (1 << WDP3);
            /*Bit 6 - WDIE: Watchdog Interrupt Enable
When this bit is written to one and the I-bit in the status register is set, the watchdog interrupt is enabled. If WDE is cleared in
combination with this setting, the watchdog timer is in interrupt mode, and the corresponding interrupt is executed if time-out
in the watchdog timer occurs. If WDE is set, the watchdog timer is in interrupt and system reset mode. The first time-out in
the watchdog timer will set WDIF.
Executing the corresponding interrupt vector will clear WDIE and WDIF automatically by hardware (the watchdog goes to
system reset mode). This is useful for keeping the watchdog timer security while using the interrupt. To stay in interrupt and
system reset mode, WDIE must be set after each interrupt. This should however not be done within the interrupt service
routine itself, as this might compromise the safety-function of the watchdog system reset mode. If the interrupt is not
executed before the next time-out, a system reset will be applied.*/
            constexpr uint8_t WDTCSR_WDIE = (1 << WDIE);
            /*Bit 7 - WDIF: Watchdog Interrupt Flag
This bit is set when a time-out occurs in the watchdog timer and the watchdog timer is configured for interrupt. WDIF is
cleared by hardware when executing the corresponding interrupt handling vector. Alternatively, WDIF is cleared by writing a
logic one to the flag. When the I-bit in SREG and WDIE are set, the watchdog time-out interrupt is executed.*/
            constexpr uint8_t WDTCSR_WDIF = (1 << WDIF);

            /* SREG */
            /*Bit 0 – C: Carry Flag
The carry flag C indicates a carry in an arithmetic or logic operation. See Section “” on page 281 for detailed information.*/
            constexpr uint8_t StatusReg_C = (1 << SREG_C);
            /*Bit 1 – Z: Zero Flag
The zero flag Z indicates a zero result in an arithmetic or logic operation. See Section “” on page 281 for detailed
information.*/
            constexpr uint8_t StatusReg_Z = (1 << SREG_Z);
            /*Bit 2 – N: Negative Flag
The negative flag N indicates a negative result in an arithmetic or logic operation. See Section “” on page 281 for detailed
information.*/
            constexpr uint8_t StatusReg_N = (1 << SREG_N);
            /*Bit 3 – V: Two’s Complement Overflow Flag
The two’s complement overflow flag V supports two’s complement arithmetics. See Section “” on page 281 for detailed
information.*/
            constexpr uint8_t StatusReg_V = (1 << SREG_V);
            /*Bit 4 – S: Sign Bit, S = N + V
The S-bit is always an exclusive or between the negative flag N and the two’s complement overflow flag V. See Section “”
on page 281 for detailed information.*/
            constexpr uint8_t StatusReg_S = (1 << SREG_S);
            /*Bit 5 – H: Half Carry Flag
The half carry flag H indicates a half carry in some arithmetic operations. Half carry Is useful in BCD arithmetic. See Section
“” on page 281 for detailed information.*/
            constexpr uint8_t StatusReg_H = (1 << SREG_H);
            /*Bit 6 – T: Bit Copy Storage
The bit copy instructions BLD (bit LoaD) and BST (Bit STore) use the T-bit as source or destination for the operated bit. A bit
from a register in the register file can be copied into T by the BST instruction, and a bit in T can be copied into a bit in a
register in the register file by the BLD instruction.*/
            constexpr uint8_t StatusReg_T = (1 << SREG_T);
            /*Bit 7 – I: Global Interrupt Enable
The global interrupt enable bit must be set for the interrupts to be enabled. The individual interrupt enable control is then
performed in separate control registers. If the global interrupt enable register is cleared, none of the interrupts are enabled
independent of the individual interrupt enable settings. The I-bit is cleared by hardware after an interrupt has occurred, and is
set by the RETI instruction to enable subsequent interrupts. The I-bit can also be set and cleared by the application with the
SEI and CLI instructions, as described in the instruction set reference.*/
            constexpr uint8_t StatusReg_I = (1 << SREG_I);

            /* SPH */
            constexpr uint8_t SPH_SP8 = (1 << 0);
            constexpr uint8_t SPH_SP9 = (1 << 1);
            constexpr uint8_t SPH_SP10= (1 << 2);
            constexpr uint8_t SPH_SP11= (1 << 3);
            constexpr uint8_t SPH_SP12= (1 << 4);
            constexpr uint8_t SPH_SP13= (1 << 5);
            constexpr uint8_t SPH_SP14= (1 << 6);
            constexpr uint8_t SPH_SP15= (1 << 7);
            /* SPL */
            constexpr uint8_t SPL_SP0 = (1 << 0);
            constexpr uint8_t SPL_SP1 = (1 << 1);
            constexpr uint8_t SPL_SP2 = (1 << 2);
            constexpr uint8_t SPL_SP3 = (1 << 3);
            constexpr uint8_t SPL_SP4 = (1 << 4);
            constexpr uint8_t SPL_SP5 = (1 << 5);
            constexpr uint8_t SPL_SP6 = (1 << 6);
            constexpr uint8_t SPL_SP7 = (1 << 7);

            /* SPMCSR */
            /*Bit 0 – SELFPRGEN: Self Programming Enable
This bit enables the SPM instruction for the next four clock cycles. If written to one together with either RWWSRE, BLBSET,
PGWRT, or PGERS, the following SPM instruction will have a special meaning, see description above. If only SELFPRGEN
is written, the following SPM instruction will store the value in R1:R0 in the temporary page buffer addressed by the
Z-pointer. The LSB of the Z-pointer is ignored. The SELFPRGEN bit will auto-clear upon completion of an SPM instruction,
or if no SPM instruction is executed within four clock cycles. During page erase and page write, the SELFPRGEN bit remains
high until the operation is completed.
Writing any other combination than “10001”, “01001”, “00101”, “00011” or “00001” in the lower five bits will have no effect.*/
            constexpr uint8_t SPMCSR_SELFPRGEN = (1 << SELFPRGEN);
            /*Bit 1 – PGERS: Page Erase
If this bit is written to one at the same time as SELFPRGEN, the next SPM instruction within four clock cycles executes page
erase. The page address is taken from the high part of the Z-pointer. The data in R1 and R0 are ignored. The PGERS bit will
auto-clear upon completion of a page erase, or if no SPM instruction is executed within four clock cycles. The CPU is halted
during the entire page write operation.*/
            constexpr uint8_t SPMCSR_PGERS    = (1 << PGERS);
            /*Bit 2 – PGWRT: Page Write
If this bit is written to one at the same time as SELFPRGEN, the next SPM instruction within four clock cycles executes page
write, with the data stored in the temporary buffer. The page address is taken from the high part of the Z-pointer. The data in
R1 and R0 are ignored. The PGWRT bit will auto-clear upon completion of a page write, or if no SPM instruction is executed
within four clock cycles. The CPU is halted during the entire page write operation.*/
            constexpr uint8_t SPMCSR_PGWRT    = (1 << PGWRT);
            /*Bit 3 – BLBSET: Boot Lock Bit Set
An LPM instruction within three cycles after BLBSET and SELFPRGEN are set in the SPMCSR register, will read either the
lock bits or the fuse bits (depending on Z0 in the Z-pointer) into the destination register. See Section 25.2.2 “Reading the
Fuse and Lock Bits from Software” on page 225 for details.*/
            constexpr uint8_t SPMCSR_BLBSET   = (1 << BLBSET);
            /*Bit 4 – RWWSRE: Read-While-Write Section Read Enable
If the RWWSRE bit is written while filling the temporary page buffer, the temporary page buffer will be cleared and the data
will be lost.*/
            constexpr uint8_t SPMCSR_RWWSRE   = (1 << RWWSRE);
            /*Bit 6 – RWWSB: Read-While-Write Section Busy
This bit is for compatibility with devices supporting read-while-write. It will always read as zero in ATmega328P.*/
            constexpr uint8_t SPMCSR_RWWSB    = (1 << RWWSB);
            /*Bit 7 – SPMIE: SPM Interrupt Enable
When the SPMIE bit is written to one, and the I-bit in the status register is set (one), the SPM ready interrupt will be enabled.
The SPM ready interrupt will be executed as long as the SELFPRGEN bit in the SPMCSR register is cleared. The interrupt
will not be generated during EEPROM write or SPM.*/
            constexpr uint8_t SPMCSR_SPMIE    = (1 << SPMIE);

            /* MCUCR */
            /*Bit 0 – IVCE: Interrupt Vector Change Enable
The IVCE bit must be written to logic one to enable change of the IVSEL bit. IVCE is cleared by hardware four cycles after it
is written or when IVSEL is written. Setting the IVCE bit will disable interrupts, as explained in the IVSEL description above.*/
            constexpr uint8_t MCUCR_IVCE  = (1 << IVCE);
            /*Bit 1 – IVSEL: Interrupt Vector Select
When the IVSEL bit is cleared (zero), the interrupt vectors are placed at the start of the flash memory. When this bit is set
(one), the interrupt vectors are moved to the beginning of the boot loader section of the flash. The actual address of the start
of the boot flash section is determined by the BOOTSZ fuses. Refer to the Section 26. “Boot Loader Support – Read-While-
Write Self-Programming” on page 229 for details. To avoid unintentional changes of interrupt vector tables, a special write
procedure must be followed to change the IVSEL bit:
a. Write the interrupt vector change enable (IVCE) bit to one.
b.
Within four cycles, write the desired value to IVSEL while writing a zero to IVCE.
Interrupts will automatically be disabled while this sequence is executed. Interrupts are disabled in the cycle IVCE is set, and
they remain disabled until after the instruction following the write to IVSEL. If IVSEL is not written, interrupts remain disabled
for four cycles. The I-bit in the status register is unaffected by the automatic disabling.
Note:
If interrupt vectors are placed in the boot loader section and boot lock bit BLB02 is programmed, interrupts are
disabled while executing from the application section. If interrupt vectors are placed in the application section
and boot lock bit BLB12 is programed, interrupts are disabled while executing from the boot loader section.
Refer to the Section 26. “Boot Loader Support – Read-While-Write Self-Programming” on page 229 for details
on boot lock bits.
This bit is not available in Atmel® ATmega328P.*/
            constexpr uint8_t MCUCR_IVSEL = (1 << IVSEL);
            /*Bit 4 – PUD: Pull-up Disable
When this bit is written to one, the pull-ups in the I/O ports are disabled even if the DDxn and PORTxn registers are
configured to enable the pull-ups ({DDxn, PORTxn} = 0b01). See Section 13.2.1 “Configuring the Pin” on page 59 for more
details about this feature.*/
            constexpr uint8_t MCUCR_PUD   = (1 << PUD);
            /*Bit 5 – BODSE: BOD Sleep Enable
BODSE enables setting of BODS control bit, as explained in BODS bit description. BOD disable is controlled by a timed
sequence.*/
            constexpr uint8_t MCUCR_BODSE = (1 << BODSE);
            /*Bit 6 – BODS: BOD Sleep
The BODS bit must be written to logic one in order to turn off BOD during sleep, see Table 9-1 on page 34. Writing to the
BODS bit is controlled by a timed sequence and an enable bit, BODSE in MCUCR. To disable BOD in relevant sleep modes,
both BODS and BODSE must first be set to one. Then, to set the BODS bit, BODS must be set to one and BODSE must be
set to zero within four clock cycles.
The BODS bit is active three clock cycles after it is set. A sleep instruction must be executed while BODS is active in order to
turn off the BOD for the actual sleep mode. The BODS bit is automatically cleared after three clock cycles.*/
            constexpr uint8_t MCUCR_BODS  = (1 << BODS);
            
            /* MCUSR */
            /*Bit 0 – PORF: Power-on Reset Flag
This bit is set if a power-on reset occurs. The bit is reset only by writing a logic zero to the flag.
To make use of the reset flags to identify a reset condition, the user should read and then reset the MCUSR as early as
possible in the program. If the register is cleared before another reset occurs, the source of the reset can be found by
examining the reset flags.*/
            constexpr uint8_t MCUSR_PORF  = (1 << PORF);
            /*Bit 1 – EXTRF: External Reset Flag
This bit is set if an external reset occurs. The bit is reset by a power-on reset, or by writing a logic zero to the flag.*/
            constexpr uint8_t MCUSR_EXTRF = (1 << EXTRF);
            /*Bit 2 – BORF: Brown-out Reset Flag
This bit is set if a brown-out reset occurs. The bit is reset by a power-on reset, or by writing a logic zero to the flag.*/
            constexpr uint8_t MCUSR_BORF  = (1 << BORF);
            /*Bit 3 – WDRF: Watchdog System Reset Flag
This bit is set if a watchdog system reset occurs. The bit is reset by a power-on reset, or by writing a logic zero to the flag.*/
            constexpr uint8_t MCUSR_WDRF  = (1 << WDRF);

            /* SMCR */
            /*Bit 0 – SE: Sleep Enable
The SE bit must be written to logic one to make the MCU enter the sleep mode when the SLEEP instruction is executed. To
avoid the MCU entering the sleep mode unless it is the programmer’s purpose, it is recommended to write the sleep enable
(SE) bit to one just before the execution of the SLEEP instruction and to clear it immediately after waking up.*/
            constexpr uint8_t SMCR_SE  = (1 << SE);
            /*Bits 3..1 – SM2..0: Sleep Mode Select Bits 2, 1, and 0
These bits select between the five available sleep modes as shown in Table 9-2 on page 38.*/
            constexpr uint8_t SMCR_SM0 = (1 << SM0);
            /*Bits 3..1 – SM2..0: Sleep Mode Select Bits 2, 1, and 0
These bits select between the five available sleep modes as shown in Table 9-2 on page 38.*/
            constexpr uint8_t SMCR_SM1 = (1 << SM1);
            /*Bits 3..1 – SM2..0: Sleep Mode Select Bits 2, 1, and 0
These bits select between the five available sleep modes as shown in Table 9-2 on page 38.*/
            constexpr uint8_t SMCR_SM2 = (1 << SM2);

            /* EIMSK */
            /*Bit 0 – INT0: External Interrupt Request 0 Enable
When the INT0 bit is set (one) and the I-bit in the status register (SREG) is set (one), the external pin interrupt is enabled.
The interrupt sense control0 bits 1/0 (ISC01 and ISC00) in the external interrupt control register A (EICRA) define whether
the external interrupt is activated on rising and/or falling edge of the INT0 pin or level sensed. Activity on the pin will cause an
interrupt request even if INT0 is configured as an output. The corresponding interrupt of external interrupt request 0 is
executed from the INT0 interrupt vector.*/
            constexpr uint8_t EIMSK_INT0 = (1 << INT0);
            /*Bit 1 – INT1: External Interrupt Request 1 Enable
When the INT1 bit is set (one) and the I-bit in the status register (SREG) is set (one), the external pin interrupt is enabled.
The interrupt sense control1 bits 1/0 (ISC11 and ISC10) in the external interrupt control register A (EICRA) define whether
the external interrupt is activated on rising and/or falling edge of the INT1 pin or level sensed. Activity on the pin will cause an
interrupt request even if INT1 is configured as an output. The corresponding interrupt of external interrupt request 1 is
executed from the INT1 interrupt vector.*/
            constexpr uint8_t EIMSK_INT1 = (1 << INT1);
            
            /* EIFR */
            /*Bit 0 – INTF0: External Interrupt Flag 0
When an edge or logic change on the INT0 pin triggers an interrupt request, INTF0 becomes set (one). If the I-bit in SREG
and the INT0 bit in EIMSK are set (one), the MCU will jump to the corresponding interrupt vector. The flag is cleared when
the interrupt routine is executed. Alternatively, the flag can be cleared by writing a logical one to it. This flag is always cleared
when INT0 is configured as a level interrupt.*/
            constexpr uint8_t EIFR_INTF0 = (1 << INTF0);
            /*Bit 1 – INTF1: External Interrupt Flag 1
When an edge or logic change on the INT1 pin triggers an interrupt request, INTF1 becomes set (one). If the I-bit in SREG
and the INT1 bit in EIMSK are set (one), the MCU will jump to the corresponding interrupt vector. The flag is cleared when
the interrupt routine is executed. Alternatively, the flag can be cleared by writing a logical one to it. This flag is always cleared
when INT1 is configured as a level interrupt.*/
            constexpr uint8_t EIFR_INTF1 = (1 << INTF1);

            /* PCIFR */
            /*Bit 0 - PCIF0: Pin Change Interrupt Flag 0
When a logic change on any PCINT7..0 pin triggers an interrupt request, PCIF0 becomes set (one). If the I-bit in SREG and
the PCIE0 bit in PCICR are set (one), the MCU will jump to the corresponding interrupt vector. The flag is cleared when the
interrupt routine is executed. Alternatively, the flag can be cleared by writing a logical one to it.*/
            constexpr uint8_t PCIFR_PCIF0 = (1 << PCIF0);
            /*Bit 1 - PCIF1: Pin Change Interrupt Flag 1
When a logic change on any PCINT14..8 pin triggers an interrupt request, PCIF1 becomes set (one). If the I-bit in SREG and
the PCIE1 bit in PCICR are set (one), the MCU will jump to the corresponding interrupt vector. The flag is cleared when the
interrupt routine is executed. Alternatively, the flag can be cleared by writing a logical one to it.*/
            constexpr uint8_t PCIFR_PCIF1 = (1 << PCIF1);
            /*Bit 2 - PCIF2: Pin Change Interrupt Flag 2
When a logic change on any PCINT23..16 pin triggers an interrupt request, PCIF2 becomes set (one). If the I-bit in SREG
and the PCIE2 bit in PCICR are set (one), the MCU will jump to the corresponding interrupt vector. The flag is cleared when
the interrupt routine is executed. Alternatively, the flag can be cleared by writing a logical one to it.*/
            constexpr uint8_t PCIFR_PCIF2 = (1 << PCIF2);
        }
        namespace Timers{
            namespace Timer2{
            /* ASSR REG BITS */
            /*Bit 0 – TCR2BUB: Timer/Counter Control Register2 Update Busy
When Timer/Counter2 operates asynchronously and TCCR2B is written, this bit becomes set. When TCCR2B has been
updated from the temporary storage register, this bit is cleared by hardware. A logical zero in this bit indicates that TCCR2B
is ready to be updated with a new value.
If a write is performed to any of the five Timer/Counter2 registers while its update busy flag is set, the updated value might
get corrupted and cause an unintentional interrupt to occur.
The mechanisms for reading TCNT2, OCR2A, OCR2B, TCCR2A and TCCR2B are different. When reading TCNT2, the
actual timer value is read. When reading OCR2A, OCR2B, TCCR2A and TCCR2B the value in the temporary storage
register is read.*/
                constexpr uint8_t ASSR_TCR2BUB = (1 << TCR2BUB);
            /*Bit 1 – TCR2AUB: Timer/Counter Control Register2 Update Busy
When Timer/Counter2 operates asynchronously and TCCR2A is written, this bit becomes set. When TCCR2A has been
updated from the temporary storage register, this bit is cleared by hardware. A logical zero in this bit indicates that TCCR2A
is ready to be updated with a new value.*/
                constexpr uint8_t ASSR_TCR2AUB = (1 << TCR2AUB);
            /*Bit 2 – OCR2BUB: Output Compare Register2 Update Busy
When Timer/Counter2 operates asynchronously and OCR2B is written, this bit becomes set. When OCR2B has been
updated from the temporary storage register, this bit is cleared by hardware. A logical zero in this bit indicates that OCR2B is
ready to be updated with a new value.*/
                constexpr uint8_t ASSR_OCR2BUB = (1 << OCR2BUB);
            /*Bit 3 – OCR2AUB: Output Compare Register2 Update Busy
When Timer/Counter2 operates asynchronously and OCR2A is written, this bit becomes set. When OCR2A has been
updated from the temporary storage register, this bit is cleared by hardware. A logical zero in this bit indicates that OCR2A is
ready to be updated with a new value.*/
                constexpr uint8_t ASSR_OCR2AUB = (1 << OCR2AUB);
            /*Bit 4 – TCN2UB: Timer/Counter2 Update Busy
When Timer/Counter2 operates asynchronously and TCNT2 is written, this bit becomes set. When TCNT2 has been
updated from the temporary storage register, this bit is cleared by hardware. A logical zero in this bit indicates that TCNT2 is
ready to be updated with a new value.*/
                constexpr uint8_t ASSR_TCN2UB  = (1 << TCN2UB);
            /*Bit 5 – AS2: Asynchronous Timer/Counter2
When AS2 is written to zero, Timer/Counter2 is clocked from the I/O clock, clkI/O. When AS2 is written to one,
Timer/Counter2 is clocked from a crystal oscillator connected to the timer oscillator 1 (TOSC1) pin. When the value of AS2 is
changed, the contents of TCNT2, OCR2A, OCR2B, TCCR2A and TCCR2B might be corrupted.*/
                constexpr uint8_t ASSR_AS2     = (1 << AS2);
            /*Bit 6 – EXCLK: Enable External Clock Input
When EXCLK is written to one, and asynchronous clock is selected, the external clock input buffer is enabled and an
external clock can be input on timer oscillator 1 (TOSC1) pin instead of a 32kHz crystal. Writing to EXCLK should be done
before asynchronous operation is selected. Note that the crystal oscillator will only run when this bit is zero.*/
                constexpr uint8_t ASSR_EXCLK   = (1 << EXCLK);

                /* TCCR2B */
                /*Bit 2:0 – CS22:0: Clock Select
The three clock select bits select the clock source to be used by the Timer/Counter, see Table 17-9.*/
                constexpr uint8_t TCCR2B_CS20  = (1 << CS20);
                /*Bit 2:0 – CS22:0: Clock Select
The three clock select bits select the clock source to be used by the Timer/Counter, see Table 17-9.*/
                constexpr uint8_t TCCR2B_CS21  = (1 << CS21);
                /*Bit 2:0 – CS22:0: Clock Select
The three clock select bits select the clock source to be used by the Timer/Counter, see Table 17-9.*/
                constexpr uint8_t TCCR2B_CS22  = (1 << CS22);
                /*Bit 3 – WGM22: Waveform Generation Mode
See the description in the Section 17.11.1 “TCCR2A – Timer/Counter Control Register A” on page 127.*/
                constexpr uint8_t TCCR2B_WGM22 = (1 << WGM22);
                /*Bit 6 – FOC2B: Force Output Compare B
The FOC2B bit is only active when the WGM bits specify a non-PWM mode.
However, for ensuring compatibility with future devices, this bit must be set to zero when TCCR2B is written when operating
in PWM mode. When writing a logical one to the FOC2B bit, an immediate compare match is forced on the waveform
generation unit. The OC2B output is changed according to its COM2B1:0 bits setting. Note that the FOC2B bit is
implemented as a strobe. Therefore it is the value present in the COM2B1:0 bits that determines the effect of the forced
compare.
A FOC2B strobe will not generate any interrupt, nor will it clear the timer in CTC mode using OCR2B as TOP.
The FOC2B bit is always read as zero.*/
                constexpr uint8_t TCCR2B_FOC2B = (1 << FOC2B);
                /*Bit 7 – FOC2A: Force Output Compare A
The FOC2A bit is only active when the WGM bits specify a non-PWM mode.
However, for ensuring compatibility with future devices, this bit must be set to zero when TCCR2B is written when operating
in PWM mode. When writing a logical one to the FOC2A bit, an immediate compare match is forced on the waveform
generation unit. The OC2A output is changed according to its COM2A1:0 bits setting. Note that the FOC2A bit is
implemented as a strobe. Therefore it is the value present in the COM2A1:0 bits that determines the effect of the forced
compare.
A FOC2A strobe will not generate any interrupt, nor will it clear the timer in CTC mode using OCR2A as TOP.
The FOC2A bit is always read as zero.*/
                constexpr uint8_t TCCR2B_FOC2A = (1 << FOC2A);

                /* TCCR2A */
                /*Bits 1:0 – WGM21:0: Waveform Generation Mode
Combined with the WGM22 bit found in the TCCR2B register, these bits control the counting sequence of the counter, the
source for maximum (TOP) counter value, and what type of waveform generation to be used, see Table 17-8. Modes of
operation supported by the Timer/Counter unit are: Normal mode (counter), Clear timer on compare match (CTC) mode, and
two types of pulse width modulation (PWM) modes (see Section 17.7 “Modes of Operation” on page 120).*/
                constexpr uint8_t TCCR2A_WGM20  = (1 << WGM20);
                /*Bits 1:0 – WGM21:0: Waveform Generation Mode
Combined with the WGM22 bit found in the TCCR2B register, these bits control the counting sequence of the counter, the
source for maximum (TOP) counter value, and what type of waveform generation to be used, see Table 17-8. Modes of
operation supported by the Timer/Counter unit are: Normal mode (counter), Clear timer on compare match (CTC) mode, and
two types of pulse width modulation (PWM) modes (see Section 17.7 “Modes of Operation” on page 120).*/
                constexpr uint8_t TCCR2A_WGM21  = (1 << WGM21);
                /*Bits 5:4 – COM2B1:0: Compare Match Output B Mode
These bits control the output compare pin (OC2B) behavior. If one or both of the COM2B1:0 bits are set, the OC2B output
overrides the normal port functionality of the I/O pin it is connected to. However, note that the data direction register (DDR)
bit corresponding to the OC2B pin must be set in order to enable the output driver. When OC2B is connected to the pin, the function of the COM2B1:0 bits depends on the WGM22:0 bit setting. Table 17-5
shows the COM2B1:0 bit functionality when the WGM22:0 bits are set to a normal or CTC mode (non-PWM).*/
                constexpr uint8_t TCCR2A_COM2B0 = (1 << COM2B0);
                /*Bits 5:4 – COM2B1:0: Compare Match Output B Mode
These bits control the output compare pin (OC2B) behavior. If one or both of the COM2B1:0 bits are set, the OC2B output
overrides the normal port functionality of the I/O pin it is connected to. However, note that the data direction register (DDR)
bit corresponding to the OC2B pin must be set in order to enable the output driver. When OC2B is connected to the pin, the function of the COM2B1:0 bits depends on the WGM22:0 bit setting. Table 17-5
shows the COM2B1:0 bit functionality when the WGM22:0 bits are set to a normal or CTC mode (non-PWM).*/
                constexpr uint8_t TCCR2A_COM2B1 = (1 << COM2B1);
                /*Bits 7:6 – COM2A1:0: Compare Match Output A Mode
These bits control the output compare pin (OC2A) behavior. If one or both of the COM2A1:0 bits are set, the OC2A output
overrides the normal port functionality of the I/O pin it is connected to. However, note that the data direction register (DDR)
bit corresponding to the OC2A pin must be set in order to enable the output driver. When OC2A is connected to the pin, the function of the COM2A1:0 bits depends on the WGM22:0 bit setting. Table 17-2
shows the COM2A1:0 bit functionality when the WGM22:0 bits are set to a normal or CTC mode (non-PWM).*/
                constexpr uint8_t TCCR2A_COM2A0 = (1 << COM2A0);
                /*Bits 7:6 – COM2A1:0: Compare Match Output A Mode
These bits control the output compare pin (OC2A) behavior. If one or both of the COM2A1:0 bits are set, the OC2A output
overrides the normal port functionality of the I/O pin it is connected to. However, note that the data direction register (DDR)
bit corresponding to the OC2A pin must be set in order to enable the output driver. When OC2A is connected to the pin, the function of the COM2A1:0 bits depends on the WGM22:0 bit setting. Table 17-2
shows the COM2A1:0 bit functionality when the WGM22:0 bits are set to a normal or CTC mode (non-PWM).*/
                constexpr uint8_t TCCR2A_COM2A1 = (1 << COM2A1);

                /* TIMSK2 */
                /*Bit 0 – TOIE2: Timer/Counter2 Overflow Interrupt Enable
When the TOIE2 bit is written to one and the I-bit in the status register is set (one), the Timer/Counter2 overflow interrupt is
enabled. The corresponding interrupt is executed if an overflow in Timer/Counter2 occurs, i.e., when the TOV2 bit is set in
the Timer/Counter2 interrupt flag register – TIFR2.*/
                constexpr uint8_t TIMSK2_TOIE2  = (1 << TOIE2);
                /*Bit 1 – OCIE2A: Timer/Counter2 Output Compare Match A Interrupt Enable
When the OCIE2A bit is written to one and the I-bit in the status register is set (one), the Timer/Counter2 compare match A
interrupt is enabled. The corresponding interrupt is executed if a compare match in Timer/Counter2 occurs, i.e., when the
OCF2A bit is set in the Timer/Counter 2 interrupt flag register – TIFR2.*/
                constexpr uint8_t TIMSK2_OCIE2A = (1 << OCIE2A);
                /*Bit 2 – OCIE2B: Timer/Counter2 Output Compare Match B Interrupt Enable
When the OCIE2B bit is written to one and the I-bit in the status register is set (one), the Timer/Counter2 compare match B
interrupt is enabled. The corresponding interrupt is executed if a compare match in Timer/Counter2 occurs, i.e., when the
OCF2B bit is set in the Timer/Counter 2 interrupt flag register – TIFR2.*/
                constexpr uint8_t TIMSK2_OCIE2B = (1 << OCIE2B);

                /* TIFR2 */
                /*Bit 0 – TOV2: Timer/Counter2 Overflow Flag
The TOV2 bit is set (one) when an overflow occurs in Timer/Counter2. TOV2 is cleared by hardware when executing the
corresponding interrupt handling vector. Alternatively, TOV2 is cleared by writing a logic one to the flag. When the
SREG I-bit, TOIE2A (Timer/Counter2 overflow interrupt enable), and TOV2 are set (one), the Timer/Counter2 overflow
interrupt is executed. In PWM mode, this bit is set when Timer/Counter2 changes counting direction at 0x00.*/
                constexpr uint8_t TIFR2_TOV2 = (1 << TOV2);
                /*Bit 1 – OCF2A: Output Compare Flag 2 A
The OCF2A bit is set (one) when a compare match occurs between the Timer/Counter2 and the data in OCR2A – output
compare register2. OCF2A is cleared by hardware when executing the corresponding interrupt handling vector.
Alternatively, OCF2A is cleared by writing a logic one to the flag. When the I-bit in SREG, OCIE2A (Timer/Counter2 compare
match interrupt enable), and OCF2A are set (one), the Timer/Counter2 compare match interrupt is executed.*/
                constexpr uint8_t TIFR2_OCF2A = (1 << OCF2A);
                /*Bit 2 – OCF2B: Output Compare Flag 2 B
The OCF2B bit is set (one) when a compare match occurs between the Timer/Counter2 and the data in OCR2B – output
compare register2. OCF2B is cleared by hardware when executing the corresponding interrupt handling vector.
Alternatively, OCF2B is cleared by writing a logic one to the flag. When the I-bit in SREG, OCIE2B (Timer/Counter2 compare
match interrupt enable), and OCF2B are set (one), the Timer/Counter2 compare match interrupt is executed.*/
                constexpr uint8_t TIFR2_OCF2B = (1 << OCF2B);
            }
            namespace Timer1{
                /*TCCR1C*/
                /*Bit 6 – FOC1B: Force Output Compare for Channel B
The FOC1A/FOC1B bits are only active when the WGM13:0 bits specifies a non-PWM mode. When writing a logical one to
the FOC1A/FOC1B bit, an immediate compare match is forced on the waveform generation unit. The OC1A/OC1B output is
changed according to its COM1x1:0 bits setting. Note that the FOC1A/FOC1B bits are implemented as strobes. Therefore it
is the value present in the COM1x1:0 bits that determine the effect of the forced compare.
A FOC1A/FOC1B strobe will not generate any interrupt nor will it clear the timer in clear timer on compare match (CTC)
mode using OCR1A as TOP. The FOC1A/FOC1B bits are always read as zero.*/
                constexpr uint8_t TCCR1C_FOC1B = (1 << FOC1B);
                /*Bit 7 – FOC1A: Force Output Compare for Channel A*/
                constexpr uint8_t TCCR1C_FOC1A = (1 << FOC1A);

                /*TCCR1B*/
                /*Bit 2:0 – CS12:0: Clock Select
The three clock select bits select the clock source to be used by the Timer/Counter, see Figure 15-10 on page 106 and
Figure 15-11 on page 106.*/
                constexpr uint8_t TCCR1B_CS10  = (1 << CS10);
                /*Bit 2:0 – CS12:0: Clock Select
The three clock select bits select the clock source to be used by the Timer/Counter, see Figure 15-10 on page 106 and
Figure 15-11 on page 106.*/
                constexpr uint8_t TCCR1B_CS11  = (1 << CS11);
                /*Bit 2:0 – CS12:0: Clock Select
The three clock select bits select the clock source to be used by the Timer/Counter, see Figure 15-10 on page 106 and
Figure 15-11 on page 106.*/
                constexpr uint8_t TCCR1B_CS12  = (1 << CS12);
                /*Bit 4:3 – WGM13:2: Waveform Generation Mode
See TCCR1A register description.*/
                constexpr uint8_t TCCR1B_WGM12 = (1 << WGM12);
                /*Bit 4:3 – WGM13:2: Waveform Generation Mode
See TCCR1A register description.*/
                constexpr uint8_t TCCR1B_WGM13 = (1 << WGM13);
                /*Bit 6 – ICES1: Input Capture Edge Select
This bit selects which edge on the input capture pin (ICP1) that is used to trigger a capture event. When the ICES1 bit is
written to zero, a falling (negative) edge is used as trigger, and when the ICES1 bit is written to one, a rising (positive) edge
will trigger the capture.
When a capture is triggered according to the ICES1 setting, the counter value is copied into the input capture register
(ICR1). The event will also set the input capture flag (ICF1), and this can be used to cause an input capture interrupt, if this
interrupt is enabled.
When the ICR1 is used as TOP value (see description of the WGM13:0 bits located in the TCCR1A and the TCCR1B
register), the ICP1 is disconnected and consequently the input capture function is disabled.*/
                constexpr uint8_t TCCR1B_ICES1 = (1 << ICES1);
                /*Bit 7 – ICNC1: Input Capture Noise Canceler
Setting this bit (to one) activates the input capture noise canceler. When the noise canceler is activated, the input from the
input capture pin (ICP1) is filtered. The filter function requires four successive equal valued samples of the ICP1 pin for
changing its output. The input capture is therefore delayed by four oscillator cycles when the noise canceler is enabled.*/
                constexpr uint8_t TCCR1B_ICNC1 = (1 << ICNC1);

                /*TCCR1A*/
                /*Bit 1:0 – WGM11:0: Waveform Generation Mode
Combined with the WGM13:2 bits found in the TCCR1B register, these bits control the counting sequence of the counter, the
source for maximum (TOP) counter value, and what type of waveform generation to be used, see Table 15-5. Modes of
operation supported by the Timer/Counter unit are: Normal mode (counter), clear timer on compare match (CTC) mode, and
three types of pulse width modulation (PWM) modes. See (Section 15.9 “Modes of Operation” on page 100).*/
                constexpr uint8_t TCCR1A_WGM10  = (1 << WGM10);
                /*Bit 1:0 – WGM11:0: Waveform Generation Mode
Combined with the WGM13:2 bits found in the TCCR1B register, these bits control the counting sequence of the counter, the
source for maximum (TOP) counter value, and what type of waveform generation to be used, see Table 15-5. Modes of
operation supported by the Timer/Counter unit are: Normal mode (counter), clear timer on compare match (CTC) mode, and
three types of pulse width modulation (PWM) modes. See (Section 15.9 “Modes of Operation” on page 100).*/
                constexpr uint8_t TCCR1A_WGM11  = (1 << WGM11);
                /*Bit 5:4 – COM1B1:0: Compare Output Mode for Channel B
The COM1A1:0 and COM1B1:0 control the output compare pins (OC1A and OC1B respectively) behavior. If one or both of
the COM1A1:0 bits are written to one, the OC1A output overrides the normal port functionality of the I/O pin it is connected
to. If one or both of the COM1B1:0 bit are written to one, the OC1B output overrides the normal port functionality of the I/O
pin it is connected to. However, note that the data direction register (DDR) bit corresponding to the OC1A or OC1B pin must
be set in order to enable the output driver.
When the OC1A or OC1B is connected to the pin, the function of the COM1x1:0 bits is dependent of the WGM13:0 bits
setting. Table 15-2 shows the COM1x1:0 bit functionality when the WGM13:0 bits are set to a normal or a CTC mode
(non-PWM).*/
                constexpr uint8_t TCCR1A_COM1B0 = (1 << COM1B0);
                /*Bit 5:4 – COM1B1:0: Compare Output Mode for Channel B
The COM1A1:0 and COM1B1:0 control the output compare pins (OC1A and OC1B respectively) behavior. If one or both of
the COM1A1:0 bits are written to one, the OC1A output overrides the normal port functionality of the I/O pin it is connected
to. If one or both of the COM1B1:0 bit are written to one, the OC1B output overrides the normal port functionality of the I/O
pin it is connected to. However, note that the data direction register (DDR) bit corresponding to the OC1A or OC1B pin must
be set in order to enable the output driver.
When the OC1A or OC1B is connected to the pin, the function of the COM1x1:0 bits is dependent of the WGM13:0 bits
setting. Table 15-2 shows the COM1x1:0 bit functionality when the WGM13:0 bits are set to a normal or a CTC mode
(non-PWM).*/
                constexpr uint8_t TCCR1A_COM1B1 = (1 << COM1B1);
                /*Bit 7:6 – COM1A1:0: Compare Output Mode for Channel A*/
                constexpr uint8_t TCCR1A_COM1A0 = (1 << COM1A0);
                /*Bit 7:6 – COM1A1:0: Compare Output Mode for Channel A*/
                constexpr uint8_t TCCR1A_COM1A1 = (1 << COM1A1);

                /*TIMSK1*/
                /*Bit 0 – TOIE1: Timer/Counter1, Overflow Interrupt Enable
When this bit is written to one, and the I-flag in the status register is set (interrupts globally enabled), the Timer/Counter1
overflow interrupt is enabled. The corresponding interrupt vector (see Section 11. “Interrupts” on page 49) is executed when
the TOV1 flag, located in TIFR1, is set.*/
                constexpr uint8_t TIMSK1_TOIE1  = (1 << TOIE1);
                /*Bit 1 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
When this bit is written to one, and the I-flag in the status register is set (interrupts globally enabled), the Timer/Counter1
output compare A match interrupt is enabled. The corresponding interrupt vector (see Section 11. “Interrupts” on page 49) is
executed when the OCF1A flag, located in TIFR1, is set.*/
                constexpr uint8_t TIMSK1_OCIE1A = (1 << OCIE1A);
                /*Bit 2 – OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Enable
When this bit is written to one, and the I-flag in the status register is set (interrupts globally enabled), the Timer/Counter1
output compare B match interrupt is enabled. The corresponding interrupt vector (see Section 11. “Interrupts” on page 49) is
executed when the OCF1B flag, located in TIFR1, is set.*/
                constexpr uint8_t TIMSK1_OCIE1B = (1 << OCIE1B);
                /*Bit 5 – ICIE1: Timer/Counter1, Input Capture Interrupt Enable
When this bit is written to one, and the I-flag in the status register is set (interrupts globally enabled), the Timer/Counter1
input capture interrupt is enabled. The corresponding interrupt vector (see Section 11. “Interrupts” on page 49) is executed
when the ICF1 flag, located in TIFR1, is set.*/
                constexpr uint8_t TIMSK1_ICIE1   = (1 << ICIE1);
                
                /*TIFR1*/
                /*Bit 0 – TOV1: Timer/Counter1, Overflow Flag
The setting of this flag is dependent of the WGM13:0 bits setting. In normal and CTC modes, the TOV1 flag is set when the
timer overflows. Refer to Table 15-5 on page 109 for the TOV1 flag behavior when using another WGM13:0 bit setting.
TOV1 is automatically cleared when the Timer/Counter1 overflow interrupt vector is executed. Alternatively, TOV1 can be
cleared by writing a logic one to its bit location.*/
                constexpr uint8_t TIFR1_TOV1  = (1 << TOV1);
                /*Bit 1 – OCF1A: Timer/Counter1, Output Compare A Match Flag
This flag is set in the timer clock cycle after the counter (TCNT1) value matches the output compare register A (OCR1A).
Note that a forced output compare (FOC1A) strobe will not set the OCF1A flag.
OCF1A is automatically cleared when the output compare match A interrupt vector is executed. Alternatively, OCF1A can be
cleared by writing a logic one to its bit location.*/
                constexpr uint8_t TIFR1_OCF1A = (1 << OCF1A);
                /*Bit 2 – OCF1B: Timer/Counter1, Output Compare B Match Flag
This flag is set in the timer clock cycle after the counter (TCNT1) value matches the output compare register B (OCR1B).
Note that a forced output compare (FOC1B) strobe will not set the OCF1B flag.
OCF1B is automatically cleared when the output compare match B interrupt vector is executed. Alternatively, OCF1B can be
cleared by writing a logic one to its bit location.*/
                constexpr uint8_t TIFR1_OCF1B = (1 << OCF1B);
                /*Bit 5 – ICF1: Timer/Counter1, Input Capture Flag
This flag is set when a capture event occurs on the ICP1 pin. When the input capture register (ICR1) is set by the WGM13:0
to be used as the TOP value, the ICF1 flag is set when the counter reaches the TOP value.
ICF1 is automatically cleared when the input capture interrupt vector is executed. Alternatively, ICF1 can be cleared by
writing a logic one to its bit location.*/
                constexpr uint8_t TIFR1_ICF1  = (1 << ICF1);
            }
            namespace Timer0{
                /*TIMSK0*/
                /*Bit 0 – TOIE0: Timer/Counter0 Overflow Interrupt Enable
When the TOIE0 bit is written to one, and the I-bit in the status register is set, the Timer/Counter0 overflow interrupt is
enabled. The corresponding interrupt is executed if an overflow in Timer/Counter0 occurs, i.e., when the TOV0 bit is set in
the Timer/Counter 0 interrupt flag register – TIFR0.*/
                constexpr uint8_t TIMSK0_TOIE0  = (1 << TOIE0);
                /*Bit 1 – OCIE0A: Timer/Counter0 Output Compare Match A Interrupt Enable
When the OCIE0A bit is written to one, and the I-bit in the status register is set, the Timer/Counter0 compare match A
interrupt is enabled. The corresponding interrupt is executed if a compare match in Timer/Counter0 occurs, i.e., when the
OCF0A bit is set in the Timer/Counter 0 interrupt flag register – TIFR0.*/
                constexpr uint8_t TIMSK0_OCIE0A = (1 << OCIE0A);
                /*Bit 2 – OCIE0B: Timer/Counter Output Compare Match B Interrupt Enable
When the OCIE0B bit is written to one, and the I-bit in the status register is set, the Timer/Counter compare match B interrupt
is enabled. The corresponding interrupt is executed if a compare match in Timer/Counter occurs, i.e., when the OCF0B bit is
set in the Timer/Counter interrupt flag register – TIFR0.*/
                constexpr uint8_t TIMSK0_OCIE0B = (1 << OCIE0B);

                /*TCCR0B*/
                /*Bits 2:0 – CS02:0: Clock Select
The three clock select bits select the clock source to be used by the Timer/Counter.*/
                constexpr uint8_t TCCR0B_CS00   = (1 << CS00);
                /*Bits 2:0 – CS02:0: Clock Select
The three clock select bits select the clock source to be used by the Timer/Counter.*/
                constexpr uint8_t TCCR0B_CS01   = (1 << CS01);
                /*Bits 2:0 – CS02:0: Clock Select
The three clock select bits select the clock source to be used by the Timer/Counter.*/
                constexpr uint8_t TCCR0B_CS02   = (1 << CS02);
                /*Bit 3 – WGM02: Waveform Generation Mode
See the description in the Section 14.9.1 “TCCR0A – Timer/Counter Control Register A” on page 84.*/
                constexpr uint8_t TCCR0B_WGM02  = (1 << WGM02);
                /*Bit 6 – FOC0B: Force Output Compare B
The FOC0B bit is only active when the WGM bits specify a non-PWM mode.
However, for ensuring compatibility with future devices, this bit must be set to zero when TCCR0B is written when operating
in PWM mode. When writing a logical one to the FOC0B bit, an immediate compare match is forced on the waveform
generation unit. The OC0B output is changed according to its COM0B1:0 bits setting. Note that the FOC0B bit is
implemented as a strobe. Therefore it is the value present in the COM0B1:0 bits that determines the effect of the forced
compare.
A FOC0B strobe will not generate any interrupt, nor will it clear the timer in CTC mode using OCR0B as TOP.
The FOC0B bit is always read as zero.*/
                constexpr uint8_t TCCR0B_FOC0B  = (1 << FOC0B);
                /*Bit 7 – FOC0A: Force Output Compare A
The FOC0A bit is only active when the WGM bits specify a non-PWM mode.
However, for ensuring compatibility with future devices, this bit must be set to zero when TCCR0B is written when operating
in PWM mode. When writing a logical one to the FOC0A bit, an immediate compare match is forced on the waveform
generation unit. The OC0A output is changed according to its COM0A1:0 bits setting. Note that the FOC0A bit is
implemented as a strobe. Therefore it is the value present in the COM0A1:0 bits that determines the effect of the forced
compare.
A FOC0A strobe will not generate any interrupt, nor will it clear the timer in CTC mode using OCR0A as TOP.
The FOC0A bit is always read as zero.*/
                constexpr uint8_t TCCR0B_FOC0A  = (1 << FOC0A);
                
                /*TCCR0A*/
                /*Bits 1:0 – WGM01:0: Waveform Generation Mode
Combined with the WGM02 bit found in the TCCR0B register, these bits control the counting sequence of the counter, the
source for maximum (TOP) counter value, and what type of waveform generation to be used, see Table 14-8. Modes of
operation supported by the Timer/Counter unit are: Normal mode (counter), clear timer on compare match (CTC) mode, and
two types of pulse width modulation (PWM) modes (see Section 14.7 “Modes of Operation” on page 78).*/
                constexpr uint8_t TCCR0A_WGM00  = (1 << WGM00);
                /*Bits 1:0 – WGM01:0: Waveform Generation Mode
Combined with the WGM02 bit found in the TCCR0B register, these bits control the counting sequence of the counter, the
source for maximum (TOP) counter value, and what type of waveform generation to be used, see Table 14-8. Modes of
operation supported by the Timer/Counter unit are: Normal mode (counter), clear timer on compare match (CTC) mode, and
two types of pulse width modulation (PWM) modes (see Section 14.7 “Modes of Operation” on page 78).*/
                constexpr uint8_t TCCR0A_WGM01  = (1 << WGM01);
                /*Bits 5:4 – COM0B1:0: Compare Match Output B Mode
These bits control the output compare pin (OC0B) behavior. If one or both of the COM0B1:0 bits are set, the OC0B output
overrides the normal port functionality of the I/O pin it is connected to. However, note that the data direction register (DDR)
bit corresponding to the OC0B pin must be set in order to enable the output driver.
When OC0B is connected to the pin, the function of the COM0B1:0 bits depends on the WGM02:0 bit setting. Table 14-5
shows the COM0B1:0 bit functionality when the WGM02:0 bits are set to a normal or CTC mode (non-PWM).*/
                constexpr uint8_t TCCR0A_COM0B0 = (1 << COM0B0);
                /*Bits 5:4 – COM0B1:0: Compare Match Output B Mode
These bits control the output compare pin (OC0B) behavior. If one or both of the COM0B1:0 bits are set, the OC0B output
overrides the normal port functionality of the I/O pin it is connected to. However, note that the data direction register (DDR)
bit corresponding to the OC0B pin must be set in order to enable the output driver.
When OC0B is connected to the pin, the function of the COM0B1:0 bits depends on the WGM02:0 bit setting. Table 14-5
shows the COM0B1:0 bit functionality when the WGM02:0 bits are set to a normal or CTC mode (non-PWM).*/
                constexpr uint8_t TCCR0A_COM0B1 = (1 << COM0B1);
                /*Bits 7:6 – COM0A1:0: Compare Match Output A Mode
These bits control the output compare pin (OC0A) behavior. If one or both of the COM0A1:0 bits are set, the OC0A output
overrides the normal port functionality of the I/O pin it is connected to. However, note that the data direction register (DDR)
bit corresponding to the OC0A pin must be set in order to enable the output driver.
When OC0A is connected to the pin, the function of the COM0A1:0 bits depends on the WGM02:0 bit setting. Table 14-2
shows the COM0A1:0 bit functionality when the WGM02:0 bits are set to a normal or CTC mode (non-PWM).*/
                constexpr uint8_t TCCR0A_COM0A0 = (1 << COM0A0);
                /*Bits 7:6 – COM0A1:0: Compare Match Output A Mode
These bits control the output compare pin (OC0A) behavior. If one or both of the COM0A1:0 bits are set, the OC0A output
overrides the normal port functionality of the I/O pin it is connected to. However, note that the data direction register (DDR)
bit corresponding to the OC0A pin must be set in order to enable the output driver.
When OC0A is connected to the pin, the function of the COM0A1:0 bits depends on the WGM02:0 bit setting. Table 14-2
shows the COM0A1:0 bit functionality when the WGM02:0 bits are set to a normal or CTC mode (non-PWM).*/
                constexpr uint8_t TCCR0A_COM0A1 = (1 << COM0A1);

                /*TIFR0*/
                /*Bit 0 – TOV0: Timer/Counter0 Overflow Flag
The bit TOV0 is set when an overflow occurs in Timer/Counter0. TOV0 is cleared by hardware when executing the
corresponding interrupt handling vector. Alternatively, TOV0 is cleared by writing a logic one to the flag. When the
SREG I-bit, TOIE0 (Timer/Counter0 overflow interrupt enable), and TOV0 are set, the Timer/Counter0 overflow interrupt is
executed.
The setting of this flag is dependent of the WGM02:0 bit setting. Refer to Table 14-8 on page 86, Section 14-8 “Waveform
Generation Mode Bit Description” on page 86.*/
                constexpr uint8_t TIFR0_TOV0    = (1 << TOV0);
                /*Bit 1 – OCF0A: Timer/Counter 0 Output Compare A Match Flag
The OCF0A bit is set when a compare match occurs between the Timer/Counter0 and the data in OCR0A – output compare
register0. OCF0A is cleared by hardware when executing the corresponding interrupt handling vector. Alternatively, OCF0A
is cleared by writing a logic one to the flag. When the I-bit in SREG, OCIE0A (Timer/Counter0 compare match interrupt
enable), and OCF0A are set, the Timer/Counter0 compare match interrupt is executed.*/
                constexpr uint8_t TIFR0_OCF0A   = (1 << OCF0A);
                /*Bit 2 – OCF0B: Timer/Counter 0 Output Compare B Match Flag
The OCF0B bit is set when a compare match occurs between the Timer/Counter and the data in OCR0B – output compare
register0 B. OCF0B is cleared by hardware when executing the corresponding interrupt handling vector. Alternatively,
OCF0B is cleared by writing a logic one to the flag. When the I-bit in SREG, OCIE0B (Timer/Counter compare B match
interrupt enable), and OCF0B are set, the Timer/Counter compare match interrupt is executed.*/
                constexpr uint8_t TIFR0_OCF0B   = (1 << OCF0B);
            }
            namespace Common{
                /*Bit 0 – PSRSYNC: Prescaler Reset
When this bit is one, Timer/Counter1 and Timer/Counter0 prescaler will be reset. This bit is normally cleared immediately by
hardware, except if the TSM bit is set. Note that Timer/Counter1 and Timer/Counter0 share the same prescaler and a reset
of this prescaler will affect both timers.*/
                constexpr uint8_t GTCCR_PSRSYNC = (1 << PSRSYNC);
                /*Bit 0 – PSRSYNC: Prescaler Reset
When this bit is one, Timer/Counter1 and Timer/Counter0 prescaler will be reset. This bit is normally cleared immediately by
hardware, except if the TSM bit is set. Note that Timer/Counter1 and Timer/Counter0 share the same prescaler and a reset
of this prescaler will affect both timers.*/
                constexpr uint8_t GTCCR_PSRASY  = (1 << PSRASY);
                /*Bit 7 – TSM: Timer/Counter Synchronization Mode
Writing the TSM bit to one activates the Timer/Counter synchronization mode. In this mode, the value that is written to the
PSRASY and PSRSYNC bits is kept, hence keeping the corresponding prescaler reset signals asserted. This ensures that
the corresponding Timer/Counters are halted and can be configured to the same value without the risk of one of them
advancing during configuration. When the TSM bit is written to zero, the PSRASY and PSRSYNC bits are cleared by
hardware, and the Timer/Counters start counting simultaneously.*/
                constexpr uint8_t GTCCR_TSM     = (1 << TSM);
            }
        }
        namespace Adc{
            /*ADMUX*/
            /*Bits 3:0 – MUX3:0: Analog Channel Selection Bits
The value of these bits selects which analog inputs are connected to the ADC. See Table 23-4 on page 218 for details. If
these bits are changed during a conversion, the change will not go in effect until this conversion is complete (ADIF in
ADCSRA is set).*/
            constexpr uint8_t ADMUX_MUX0  = (1 << MUX0);
            /*Bits 3:0 – MUX3:0: Analog Channel Selection Bits
The value of these bits selects which analog inputs are connected to the ADC. See Table 23-4 on page 218 for details. If
these bits are changed during a conversion, the change will not go in effect until this conversion is complete (ADIF in
ADCSRA is set).*/
            constexpr uint8_t ADMUX_MUX1  = (1 << MUX1);
            /*Bits 3:0 – MUX3:0: Analog Channel Selection Bits
The value of these bits selects which analog inputs are connected to the ADC. See Table 23-4 on page 218 for details. If
these bits are changed during a conversion, the change will not go in effect until this conversion is complete (ADIF in
ADCSRA is set).*/
            constexpr uint8_t ADMUX_MUX2  = (1 << MUX2);
            /*Bits 3:0 – MUX3:0: Analog Channel Selection Bits
The value of these bits selects which analog inputs are connected to the ADC. See Table 23-4 on page 218 for details. If
these bits are changed during a conversion, the change will not go in effect until this conversion is complete (ADIF in
ADCSRA is set).*/
            constexpr uint8_t ADMUX_MUX3  = (1 << MUX3);
            /*Bit 5 – ADLAR: ADC Left Adjust Result
The ADLAR bit affects the presentation of the ADC conversion result in the ADC data register. Write one to ADLAR to left
adjust the result. Otherwise, the result is right adjusted. Changing the ADLAR bit will affect the ADC data register
immediately, regardless of any ongoing conversions. For a complete description of this bit, see Section 23.9.3 “ADCL and
ADCH – The ADC Data Register” on page 219.*/
            constexpr uint8_t ADMUX_ADLAR = (1 << ADLAR);
            /*Bit 7:6 – REFS1:0: Reference Selection Bits
These bits select the voltage reference for the ADC, as shown in Table 23-3. If these bits are changed during a conversion,
the change will not go in effect until this conversion is complete (ADIF in ADCSRA is set). The internal voltage reference
options may not be used if an external reference voltage is being applied to the AREF pin.*/
            constexpr uint8_t ADMUX_REFS0 = (1 << REFS0);
            /*Bit 7:6 – REFS1:0: Reference Selection Bits
These bits select the voltage reference for the ADC, as shown in Table 23-3. If these bits are changed during a conversion,
the change will not go in effect until this conversion is complete (ADIF in ADCSRA is set). The internal voltage reference
options may not be used if an external reference voltage is being applied to the AREF pin.*/
            constexpr uint8_t ADMUX_REFS1 = (1 << REFS1);

            /*ADCSRB*/
            /*Bit 2:0 – ADTS2:0: ADC Auto Trigger Source
If ADATE in ADCSRA is written to one, the value of these bits selects which source will trigger an ADC conversion. If ADATE
is cleared, the ADTS2:0 settings will have no effect. A conversion will be triggered by the rising edge of the selected interrupt
flag. Note that switching from a trigger source that is cleared to a trigger source that is set, will generate a positive edge on
the trigger signal. If ADEN in ADCSRA is set, this will start a conversion. Switching to free running mode (ADTS[2:0]=0) will
not cause a trigger event, even if the ADC interrupt flag is set.*/
            constexpr uint8_t ADCSRB_ADTS0 = (1 << ADTS0);
            /*Bit 2:0 – ADTS2:0: ADC Auto Trigger Source
If ADATE in ADCSRA is written to one, the value of these bits selects which source will trigger an ADC conversion. If ADATE
is cleared, the ADTS2:0 settings will have no effect. A conversion will be triggered by the rising edge of the selected interrupt
flag. Note that switching from a trigger source that is cleared to a trigger source that is set, will generate a positive edge on
the trigger signal. If ADEN in ADCSRA is set, this will start a conversion. Switching to free running mode (ADTS[2:0]=0) will
not cause a trigger event, even if the ADC interrupt flag is set.*/
            constexpr uint8_t ADCSRB_ADTS1 = (1 << ADTS1);
            /*Bit 2:0 – ADTS2:0: ADC Auto Trigger Source
If ADATE in ADCSRA is written to one, the value of these bits selects which source will trigger an ADC conversion. If ADATE
is cleared, the ADTS2:0 settings will have no effect. A conversion will be triggered by the rising edge of the selected interrupt
flag. Note that switching from a trigger source that is cleared to a trigger source that is set, will generate a positive edge on
the trigger signal. If ADEN in ADCSRA is set, this will start a conversion. Switching to free running mode (ADTS[2:0]=0) will
not cause a trigger event, even if the ADC interrupt flag is set.*/
            constexpr uint8_t ADCSRB_ADTS2 = (1 << ADTS2);
            /*Bit 6 – ACME: Analog Comparator Multiplexer Enable
When this bit is written logic one and the ADC is switched off (ADEN in ADCSRA is zero), the ADC multiplexer selects the
negative input to the Analog Comparator. When this bit is written logic zero, AIN1 is applied to the negative input of the
Analog Comparator. For a detailed description of this bit, see Section 22.2 “Analog Comparator Multiplexed Input” on page
202.*/
            constexpr uint8_t ADCSRB_ACME  = (1 << ACME);

            /*ADCSRA*/
            /*Bits 2:0 – ADPS2:0: ADC Prescaler Select Bits
These bits determine the division factor between the system clock frequency and the input clock to the ADC.*/
            constexpr uint8_t ADCSRA_ADPS0 = (1 << ADPS0);
            /*Bits 2:0 – ADPS2:0: ADC Prescaler Select Bits
These bits determine the division factor between the system clock frequency and the input clock to the ADC.*/
            constexpr uint8_t ADCSRA_ADPS1 = (1 << ADPS1);
            /*Bits 2:0 – ADPS2:0: ADC Prescaler Select Bits
These bits determine the division factor between the system clock frequency and the input clock to the ADC.*/
            constexpr uint8_t ADCSRA_ADPS2 = (1 << ADPS2);
            /*Bit 3 – ADIE: ADC Interrupt Enable
When this bit is written to one and the I-bit in SREG is set, the ADC conversion complete interrupt is activated.*/
            constexpr uint8_t ADCSRA_ADIE  = (1 << ADIE);
            /*Bit 4 – ADIF: ADC Interrupt Flag
This bit is set when an ADC conversion completes and the data registers are updated. The ADC conversion complete
interrupt is executed if the ADIE bit and the I-bit in SREG are set. ADIF is cleared by hardware when executing the
corresponding interrupt handling vector. Alternatively, ADIF is cleared by writing a logical one to the flag. Beware that if
doing a read-modify-write on ADCSRA, a pending interrupt can be disabled. This also applies if the SBI and CBI instructions
are used.*/
            constexpr uint8_t ADCSRA_ADIF  = (1 << ADIF);
            /*Bit 5 – ADATE: ADC Auto Trigger Enable
When this bit is written to one, auto triggering of the ADC is enabled. The ADC will start a conversion on a positive edge of
the selected trigger signal. The trigger source is selected by setting the ADC trigger select bits, ADTS in ADCSRB.*/
            constexpr uint8_t ADCSRA_ADATE = (1 << ADATE);
            /*Bit 6 – ADSC: ADC Start Conversion
In single conversion mode, write this bit to one to start each conversion. In free running mode, write this bit to one to start the
first conversion. The first conversion after ADSC has been written after the ADC has been enabled, or if ADSC is written at
the same time as the ADC is enabled, will take 25 ADC clock cycles instead of the normal 13. This first conversion performs
initialization of the ADC.
ADSC will read as one as long as a conversion is in progress. When the conversion is complete, it returns to zero. Writing
zero to this bit has no effect.*/
            constexpr uint8_t ADCSRA_ADSC  = (1 << ADSC);
            /*Bit 7 – ADEN: ADC Enable
Writing this bit to one enables the ADC. By writing it to zero, the ADC is turned off. Turning the ADC off while a conversion is
in progress, will terminate this conversion.*/
            constexpr uint8_t ADCSRA_ADEN  = (1 << ADEN);

            /*ACSR*/
            /*Bits 1, 0 – ACIS1, ACIS0: Analog Comparator Interrupt Mode Select
These bits determine which comparator events that trigger the analog comparator interrupt. The different settings are shown
in Table 22-2. When changing the ACIS1/ACIS0 bits, the analog comparator interrupt must be disabled by clearing its interrupt enable bit in
the ACSR register. Otherwise an interrupt can occur when the bits are changed.*/
            constexpr uint8_t ACSR_ACIS0 = (1 << ACIS0);
            /*Bits 1, 0 – ACIS1, ACIS0: Analog Comparator Interrupt Mode Select
These bits determine which comparator events that trigger the analog comparator interrupt. The different settings are shown
in Table 22-2. When changing the ACIS1/ACIS0 bits, the analog comparator interrupt must be disabled by clearing its interrupt enable bit in
the ACSR register. Otherwise an interrupt can occur when the bits are changed.*/
            constexpr uint8_t ACSR_ACIS1 = (1 << ACIS1);
            /*Bit 2 – ACIC: Analog Comparator Input Capture Enable
When written logic one, this bit enables the input capture function in Timer/Counter1 to be triggered by the analog
comparator. The comparator output is in this case directly connected to the input capture front-end logic, making the
comparator utilize the noise canceler and edge select features of the Timer/Counter1 input capture interrupt. When written
logic zero, no connection between the analog comparator and the input capture function exists. To make the comparator
trigger the Timer/Counter1 input capture interrupt, the ICIE1 bit in the timer interrupt mask register (TIMSK1) must be set.*/
            constexpr uint8_t ACSR_ACIC  = (1 << ACIC);
            /*Bit 3 – ACIE: Analog Comparator Interrupt Enable
When the ACIE bit is written logic one and the I-bit in the status register is set, the analog comparator interrupt is activated.
When written logic zero, the interrupt is disabled.*/
            constexpr uint8_t ACSR_ACIE  = (1 << ACIE);
            /*Bit 4 – ACI: Analog Comparator Interrupt Flag
This bit is set by hardware when a comparator output event triggers the interrupt mode defined by ACIS1 and ACIS0. The
analog comparator interrupt routine is executed if the ACIE bit is set and the I-bit in SREG is set. ACI is cleared by hardware
when executing the corresponding interrupt handling vector. Alternatively, ACI is cleared by writing a logic one to the flag.*/
            constexpr uint8_t ACSR_ACI   = (1 << ACI);
            /*Bit 5 – ACO: Analog Comparator Output
The output of the analog comparator is synchronized and then directly connected to ACO. The synchronization introduces a
delay of 1 - 2 clock cycles.*/
            constexpr uint8_t ACSR_ACO   = (1 << ACO);
            /*Bit 6 – ACBG: Analog Comparator Bandgap Select
When this bit is set, a fixed bandgap reference voltage replaces the positive input to the analog comparator. When this bit is
cleared, AIN0 is applied to the positive input of the analog comparator. When the bandgap referance is used as input to the
analog comparator, it will take a certain time for the voltage to stabilize. If not stabilized, the first conversion may give a
wrong value. See Section 10.7 “Internal Voltage Reference” on page 43*/
            constexpr uint8_t ACSR_ACBG  = (1 << ACBG);
            /*Bit 7 – ACD: Analog Comparator Disable
When this bit is written logic one, the power to the analog comparator is switched off. This bit can be set at any time to turn
off the analog comparator. This will reduce power consumption in active and idle mode. When changing the ACD bit, the
analog comparator interrupt must be disabled by clearing the ACIE bit in ACSR. Otherwise an interrupt can occur when the
bit is changed.*/
            constexpr uint8_t ACSR_ACD   = (1 << ACD);
            /* DIDR0 REG BITS */
            /*Bit 5:0 – ADC5D..ADC0D: ADC5..0 Digital Input Disable
When this bit is written logic one, the digital input buffer on the corresponding ADC pin is disabled. The corresponding PIN
register bit will always read as zero when this bit is set. When an analog signal is applied to the ADC5..0 pin and the digital
input from this pin is not needed, this bit should be written logic one to reduce power consumption in the digital input buffer.
Note that ADC pins ADC7 and ADC6 do not have digital input buffers, and therefore do not require digital input disable bits.*/
            constexpr uint8_t DIDR0_ADC0D = (1 << ADC0D);
            /*Bit 5:0 – ADC5D..ADC0D: ADC5..0 Digital Input Disable
When this bit is written logic one, the digital input buffer on the corresponding ADC pin is disabled. The corresponding PIN
register bit will always read as zero when this bit is set. When an analog signal is applied to the ADC5..0 pin and the digital
input from this pin is not needed, this bit should be written logic one to reduce power consumption in the digital input buffer.
Note that ADC pins ADC7 and ADC6 do not have digital input buffers, and therefore do not require digital input disable bits.*/
            constexpr uint8_t DIDR0_ADC1D = (1 << ADC1D);
            /*Bit 5:0 – ADC5D..ADC0D: ADC5..0 Digital Input Disable
When this bit is written logic one, the digital input buffer on the corresponding ADC pin is disabled. The corresponding PIN
register bit will always read as zero when this bit is set. When an analog signal is applied to the ADC5..0 pin and the digital
input from this pin is not needed, this bit should be written logic one to reduce power consumption in the digital input buffer.
Note that ADC pins ADC7 and ADC6 do not have digital input buffers, and therefore do not require digital input disable bits.*/
            constexpr uint8_t DIDR0_ADC2D = (1 << ADC2D);
            /*Bit 5:0 – ADC5D..ADC0D: ADC5..0 Digital Input Disable
When this bit is written logic one, the digital input buffer on the corresponding ADC pin is disabled. The corresponding PIN
register bit will always read as zero when this bit is set. When an analog signal is applied to the ADC5..0 pin and the digital
input from this pin is not needed, this bit should be written logic one to reduce power consumption in the digital input buffer.
Note that ADC pins ADC7 and ADC6 do not have digital input buffers, and therefore do not require digital input disable bits.*/
            constexpr uint8_t DIDR0_ADC3D = (1 << ADC3D);
            /*Bit 5:0 – ADC5D..ADC0D: ADC5..0 Digital Input Disable
When this bit is written logic one, the digital input buffer on the corresponding ADC pin is disabled. The corresponding PIN
register bit will always read as zero when this bit is set. When an analog signal is applied to the ADC5..0 pin and the digital
input from this pin is not needed, this bit should be written logic one to reduce power consumption in the digital input buffer.
Note that ADC pins ADC7 and ADC6 do not have digital input buffers, and therefore do not require digital input disable bits.*/
            constexpr uint8_t DIDR0_ADC4D = (1 << ADC4D);
            /*Bit 5:0 – ADC5D..ADC0D: ADC5..0 Digital Input Disable
When this bit is written logic one, the digital input buffer on the corresponding ADC pin is disabled. The corresponding PIN
register bit will always read as zero when this bit is set. When an analog signal is applied to the ADC5..0 pin and the digital
input from this pin is not needed, this bit should be written logic one to reduce power consumption in the digital input buffer.
Note that ADC pins ADC7 and ADC6 do not have digital input buffers, and therefore do not require digital input disable bits.*/
            constexpr uint8_t DIDR0_ADC5D = (1 << ADC5D);
        }
        namespace Ac{
            constexpr uint8_t DIDR1_AIN0D  = (1 << AIN0D);
            constexpr uint8_t DIDR1_AIN1D  = (1 << AIN1D);
        }
        namespace Spi{
            /*SPSR*/
            /*Bit 7 – SPIF: SPI Interrupt Flag
When a serial transfer is complete, the SPIF Flag is set. An interrupt is generated if SPIE in SPCR is set and global
interrupts are enabled. If SS is an input and is driven low when the SPI is in master mode, this will also set the SPIF flag.
SPIF is cleared by hardware when executing the corresponding interrupt handling vector. Alternatively, the SPIF bit is
cleared by first reading the SPI status register with SPIF set, then accessing the SPI data register (SPDR).*/
            constexpr uint8_t SPSR_SPIF = (1 << SPIF);
            /*Bit 6 – WCOL: Write COLlision Flag
The WCOL bit is set if the SPI data register (SPDR) is written during a data transfer. The WCOL bit (and the SPIF bit) are
cleared by first reading the SPI status register with WCOL set, and then accessing the SPI data register.*/
            constexpr uint8_t SPSR_WCOL = (1 << WCOL);
            /*Bit 0 – SPI2X: Double SPI Speed Bit
When this bit is written logic one the SPI speed (SCK frequency) will be doubled when the SPI is in master mode
(see Table 18-5 on page 141). This means that the minimum SCK period will be two CPU clock periods. When the SPI is
configured as slave, the SPI is only guaranteed to work at fosc/4 or lower.
The SPI interface on the Atmel ATmega328P is also used for program memory and EEPROM downloading or uploading.
See Section 27.8 “Serial Downloading” on page 254 for serial programming and verification.*/
            constexpr uint8_t SPSR_SPI2X= (1 << SPI2X);

            /*SPCR*/
            /*Bits 1, 0 – SPR1, SPR0: SPI Clock Rate Select 1 and 0
These two bits control the SCK rate of the device configured as a master. SPR1 and SPR0 have no effect on the slave.
The relationship between SCK and the oscillator clock frequency fosc is shown in Table 18-5.*/
            constexpr uint8_t SPCR_SPR0 = (1 << SPR0);
            /*Bits 1, 0 – SPR1, SPR0: SPI Clock Rate Select 1 and 0
These two bits control the SCK rate of the device configured as a master. SPR1 and SPR0 have no effect on the slave.
The relationship between SCK and the oscillator clock frequency fosc is shown in Table 18-5.*/
            constexpr uint8_t SPCR_SPR1 = (1 << SPR1);
            /*Bit 2 – CPHA: Clock Phase
The settings of the clock phase bit (CPHA) determine if data is sampled on the leading (first) or trailing (last) edge of SCK.
Refer to Figure 18-3 on page 139 and Figure 18-4 on page 140 for an example. The CPOL functionality is summarized
below:*/
            constexpr uint8_t SPCR_CPHA = (1 << CPHA);
            /*Bit 3 – CPOL: Clock Polarity
When this bit is written to one, SCK is high when idle. When CPOL is written to zero, SCK is low when idle. Refer to Figure
18-3 on page 139 and Figure 18-4 for an example. The CPOL functionality is summarized below.*/
            constexpr uint8_t SPCR_CPOL = (1 << CPOL);
            /*Bit 4 – MSTR: Master/Slave Select
This bit selects master SPI mode when written to one, and slave SPI mode when written logic zero. If SS is configured as an
input and is driven low while MSTR is set, MSTR will be cleared, and SPIF in SPSR will become set. The user will then have
to set MSTR to re-enable SPI master mode.*/
            constexpr uint8_t SPCR_MSTR = (1 << MSTR);
            /*Bit 5 – DORD: Data Order
When the DORD bit is written to one, the LSB of the data word is transmitted first.
When the DORD bit is written to zero, the MSB of the data word is transmitted first.*/
            constexpr uint8_t SPCR_DORD = (1 << DORD);
            /*Bit 6 – SPE: SPI Enable
When the SPE bit is written to one, the SPI is enabled. This bit must be set to enable any SPI operations.*/
            constexpr uint8_t SPCR_SPE  = (1 << SPE);
            /*Bit 7 – SPIE: SPI Interrupt Enable
This bit causes the SPI interrupt to be executed if SPIF bit in the SPSR register is set and the if the global interrupt enable bit
in SREG is set.*/
            constexpr uint8_t SPCR_SPIE = (1 << SPIE);
        }
        namespace Eeprom{
            /*EECR*/
            /*Bit 0 – EERE: EEPROM Read Enable
The EEPROM read enable signal EERE is the read strobe to the EEPROM. When the correct address is set up in the EEAR
register, the EERE bit must be written to a logic one to trigger the EEPROM read. The EEPROM read access takes one
instruction, and the requested data is available immediately. When the EEPROM is read, the CPU is halted for four cycles
before the next instruction is executed.
The user should poll the EEPE bit before starting the read operation. If a write operation is in progress, it is neither possible
to read the EEPROM, nor to change the EEAR register.*/
            constexpr uint8_t EECR_EERE  = (1 << EERE);
            /*Bit 1 – EEPE: EEPROM Write Enable
The EEPROM Write enable signal EEPE is the write strobe to the EEPROM. When address and data are correctly set up,
the EEPE bit must be written to one to write the value into the EEPROM. The EEMPE bit must be written to one before a
logical one is written to EEPE, otherwise no EEPROM write takes place. The following procedure should be followed when
writing the EEPROM (the order of steps 3 and 4 is not essential):
1. Wait until EEPE becomes zero.
2.Wait until SELFPRGEN in SPMCSR becomes zero.
3.Write new EEPROM address to EEAR (optional).
4.Write new EEPROM data to EEDR (optional).
5.Write a logical one to the EEMPE bit while writing a zero to EEPE in EECR.
6.Within four clock cycles after setting EEMPE, write a logical one to EEPE.
The EEPROM can not be programmed during a CPU write to the flash memory. The software must check that the flash
programming is completed before initiating a new EEPROM write. Step 2 is only relevant if the software contains a boot
loader allowing the CPU to program the flash. If the flash is never being updated by the CPU, step 2 can be omitted. See
Section 26. “Boot Loader Support – Read-While-Write Self-Programming” on page 229 for details about boot programming. When the write access time has elapsed, the EEPE bit is cleared by hardware. The user software can poll this bit and wait
for a zero before writing the next byte. When EEPE has been set, the CPU is halted for two cycles before the next instruction
is executed.*/
            constexpr uint8_t EECR_EEPE  = (1 << EEPE);
            /*Bit 2 – EEMPE: EEPROM Master Write Enable
The EEMPE bit determines whether setting EEPE to one causes the EEPROM to be written. When EEMPE is set, setting
EEPE within four clock cycles will write data to the EEPROM at the selected address If EEMPE is zero, setting EEPE will
have no effect. When EEMPE has been written to one by software, hardware clears the bit to zero after four clock cycles.
See the description of the EEPE bit for an EEPROM write procedure.*/
            constexpr uint8_t EECR_EEMPE = (1 << EEMPE);
            /*Bit 3 – EERIE: EEPROM Ready Interrupt Enable
Writing EERIE to one enables the EEPROM ready interrupt if the I bit in SREG is set. Writing EERIE to zero disables the
interrupt. The EEPROM ready interrupt generates a constant interrupt when EEPE is cleared. The interrupt will not be
generated during EEPROM write or SPM.*/
            constexpr uint8_t EECR_EERIE = (1 << EERIE);
            /*Bits 5, 4 – EEPM1 and EEPM0: EEPROM Programming Mode Bits
The EEPROM programming mode bit setting defines which programming action that will be triggered when writing EEPE. It
is possible to program data in one atomic operation (erase the old value and program the new value) or to split the erase and
write operations in two different operations. The programming times for the different modes are shown in Table 7-1. While
EEPE is set, any write to EEPMn will be ignored. During reset, the EEPMn bits will be reset to 0b00 unless the EEPROM is
busy programming.*/
            constexpr uint8_t EECR_EEPM0 = (1 << EEPM0);
            /*Bits 5, 4 – EEPM1 and EEPM0: EEPROM Programming Mode Bits
The EEPROM programming mode bit setting defines which programming action that will be triggered when writing EEPE. It
is possible to program data in one atomic operation (erase the old value and program the new value) or to split the erase and
write operations in two different operations. The programming times for the different modes are shown in Table 7-1. While
EEPE is set, any write to EEPMn will be ignored. During reset, the EEPMn bits will be reset to 0b00 unless the EEPROM is
busy programming.*/
            constexpr uint8_t EECR_EEPM1 = (1 << EEPM1);
        }
        namespace Gpio{
            /*PORT D*/
            namespace PortD{
                constexpr uint8_t PD_0      = (1 << PD0);
                constexpr uint8_t PD_RXD    = (1 << PD0);
                constexpr uint8_t PD_PCINT16= (1 << PD0);
                constexpr uint8_t PD_1      = (1 << PD1);
                constexpr uint8_t PD_TXD    = (1 << PD1);
                constexpr uint8_t PD_PCINT17= (1 << PD1);
                constexpr uint8_t PD_2      = (1 << PD2);
                constexpr uint8_t PD_INT0   = (1 << PD2);
                constexpr uint8_t PD_PCINT18= (1 << PD2);
                constexpr uint8_t PD_3      = (1 << PD3);
                constexpr uint8_t PD_INT1   = (1 << PD3);
                constexpr uint8_t PD_OC2B   = (1 << PD3);
                constexpr uint8_t PD_PCINT19= (1 << PD3);
                constexpr uint8_t PD_4      = (1 << PD4);
                constexpr uint8_t PD_T0     = (1 << PD4);
                constexpr uint8_t PD_XCK    = (1 << PD4);
                constexpr uint8_t PD_PCINT20= (1 << PD4);
                constexpr uint8_t PD_5      = (1 << PD5);
                constexpr uint8_t PD_T1     = (1 << PD5);
                constexpr uint8_t PD_OC0B   = (1 << PD5);
                constexpr uint8_t PD_PCINT21= (1 << PD5);
                constexpr uint8_t PD_6      = (1 << PD6);
                constexpr uint8_t PD_AIN0   = (1 << PD6);
                constexpr uint8_t PD_OC0A   = (1 << PD6);
                constexpr uint8_t PD_PCINT22= (1 << PD6);
                constexpr uint8_t PD_7      = (1 << PD7);
                constexpr uint8_t PD_AIN1   = (1 << PD7);
                constexpr uint8_t PD_PCINT23= (1 << PD7);
            }
            /*PORT B*/
            namespace PortB{
                /*ICP1 (Timer/Counter1 input capture input)
                  CLKO (divided system clock output)
                  PCINT0 (pin change interrupt 0)*/
                constexpr uint8_t PB_0      = (1 << PB0);
                constexpr uint8_t PB_ICP1   = (1 << PB0);
                constexpr uint8_t PB_CLKO   = (1 << PB0);
                constexpr uint8_t PB_PCINT0 = (1 << PB0);
                /*OC1A (Timer/Counter1 output compare match A output)
                  PCINT1 (pin change interrupt 1)*/
                constexpr uint8_t PB_1      = (1 << PB1);
                constexpr uint8_t PB_OC1A   = (1 << PB1);
                constexpr uint8_t PB_PCINT1 = (1 << PB1);
                /*SS (SPI bus master slave select)
                  OC1B (Timer/Counter1 output compare match B output)
                  PCINT2 (pin change interrupt 2)*/
                constexpr uint8_t PB_2      = (1 << PB2);
                constexpr uint8_t PB_SS     = (1 << PB2);
                constexpr uint8_t PB_OC1B   = (1 << PB2);
                constexpr uint8_t PB_PCINT2 = (1 << PB2);
                /*MOSI (SPI bus master output/slave input)
                  OC2A (Timer/Counter2 output compare match A output)
                  PCINT3 (pin change interrupt 3)*/
                constexpr uint8_t PB_3      = (1 << PB3);
                constexpr uint8_t PB_MOSI   = (1 << PB3);
                constexpr uint8_t PB_OC2A   = (1 << PB3);
                constexpr uint8_t PB_PCINT3 = (1 << PB3);
                /*MISO (SPI bus master input/slave output)
                  PCINT4 (pin change interrupt 4)*/
                constexpr uint8_t PB_4      = (1 << PB4);
                constexpr uint8_t PB_MISO   = (1 << PB4);
                constexpr uint8_t PB_PCINT4 = (1 << PB4);
                /*SCK (SPI bus master clock input)
                  PCINT5 (pin change interrupt 5)*/
                constexpr uint8_t PB_5      = (1 << PB5);
                constexpr uint8_t PB_SCK    = (1 << PB5);
                constexpr uint8_t PB_PCINT5 = (1 << PB5);
                /*XTAL1 (chip clock oscillator pin 1 or external clock input)
                  TOSC1 (timer oscillator pin 1)
                  PCINT6 (pin change interrupt 6)*/
                constexpr uint8_t PB_6      = (1 << PB6);
                constexpr uint8_t PB_XTAL1  = (1 << PB6);
                constexpr uint8_t PB_TOSC1  = (1 << PB6);
                constexpr uint8_t PB_PCINT6 = (1 << PB6);
                /*XTAL2 (chip clock oscillator pin 2)
                  TOSC2 (timer oscillator pin 2)
                  PCINT7 (pin change interrupt 7)*/
                constexpr uint8_t PB_7      = (1 << PB7);
                constexpr uint8_t PB_XTAL2  = (1 << PB7);
                constexpr uint8_t PB_TOSC2  = (1 << PB7);
                constexpr uint8_t PB_PCINT7 = (1 << PB7);
            }
            /*PORT C*/
            namespace PortC{
                constexpr uint8_t PC_0      = (1 << PC0);
                constexpr uint8_t PC_ADC0   = (1 << PC0);
                constexpr uint8_t PC_PCINT8 = (1 << PC0);
                constexpr uint8_t PC_1      = (1 << PC1);
                constexpr uint8_t PC_ADC1   = (1 << PC1);
                constexpr uint8_t PC_PCINT9 = (1 << PC1);
                constexpr uint8_t PC_2      = (1 << PC2);
                constexpr uint8_t PC_ADC2   = (1 << PC2);
                constexpr uint8_t PC_PCINT10= (1 << PC2);
                constexpr uint8_t PC_3      = (1 << PC3);
                constexpr uint8_t PC_ADC3   = (1 << PC3);
                constexpr uint8_t PC_PCINT11= (1 << PC3);
                constexpr uint8_t PC_4      = (1 << PC4);
                constexpr uint8_t PC_ADC4   = (1 << PC4);
                constexpr uint8_t PC_SDA    = (1 << PC4);
                constexpr uint8_t PC_PCINT12= (1 << PC4);
                constexpr uint8_t PC_5      = (1 << PC5);
                constexpr uint8_t PC_ADC5   = (1 << PC5);
                constexpr uint8_t PC_SCL    = (1 << PC5);
                constexpr uint8_t PC_PCINT13= (1 << PC5);
                constexpr uint8_t PC_6      = (1 << PC6);
            }
        }
    }
}









#endif  // REGS_ATMEGA328P_HPP