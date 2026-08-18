#ifndef SYS_CTRL_HPP
#define SYS_CTRL_HPP

#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#define BITMASK_SLEEPMODE   0xF1

#include <avr/boot.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "HAL/registers.hpp"

using namespace mcu;

enum class SleepMode : uint8_t{
    Idle              = 0x00,
    AdcNoiseReduction = RegBits::Core::SMCR_SM0,
    PowerDown         = RegBits::Core::SMCR_SM1,
    PowerSave         = RegBits::Core::SMCR_SM1 | RegBits::Core::SMCR_SM0,
    StandBy           = RegBits::Core::SMCR_SM2 | RegBits::Core::SMCR_SM1,
    ExtendedStandBy   = RegBits::Core::SMCR_SM2 | RegBits::Core::SMCR_SM1 | RegBits::Core::SMCR_SM0,
};
enum class SysClock  : uint8_t{
    NoPrescaling= 0x00,
    DividedBy2  = RegBits::Core::CLKPR_CLKPS0,
    DividedBy4  = RegBits::Core::CLKPR_CLKPS1,
    DividedBy8  = RegBits::Core::CLKPR_CLKPS0 | RegBits::Core::CLKPR_CLKPS1,
    DividedBy16 = RegBits::Core::CLKPR_CLKPS2,
    DividedBy32 = RegBits::Core::CLKPR_CLKPS2 | RegBits::Core::CLKPR_CLKPS0,
    DividedBy64 = RegBits::Core::CLKPR_CLKPS2 | RegBits::Core::CLKPR_CLKPS1,
    DividedBy128= RegBits::Core::CLKPR_CLKPS2 | RegBits::Core::CLKPR_CLKPS1 | RegBits::Core::CLKPR_CLKPS0,
    DividedBy256= RegBits::Core::CLKPR_CLKPS3,
};
enum class Peripheral: uint8_t{
    Twi     = RegBits::Core::PRR_PRTWI,
    Timer2  = RegBits::Core::PRR_PRTIM2,
    Timer0  = RegBits::Core::PRR_PRTIM0,
    Timer1  = RegBits::Core::PRR_PRTIM1,
    Spi     = RegBits::Core::PRR_PRSPI,
    Usart0  = RegBits::Core::PRR_PRUSART0,
    Adc     = RegBits::Core::PRR_PRADC,
    All     = RegBits::Core::PRR_PRADC | RegBits::Core::PRR_PRSPI | RegBits::Core::PRR_PRTIM0| 
              RegBits::Core::PRR_PRTIM1| RegBits::Core::PRR_PRTIM2| RegBits::Core::PRR_PRTWI |
              RegBits::Core::PRR_PRUSART0,
};

enum class ResetReason : uint8_t {
    PowerOn     = (1 << 0),
    External    = (1 << 1),
    BrownOut    = (1 << 2),
    Watchdog    = (1 << 3),
    Unknown     = 0
};

const char ck00[] __atr_section_progmem__ = {"External Clock"};
const char ck02[] __atr_section_progmem__ = {"Calibrated Internal 8MHz RC Oscillator"};
const char ck03[] __atr_section_progmem__ = {"Internal 128kHz RC Oscillator"};
const char ck05[] __atr_section_progmem__ = {"Low Frequency Crystal Oscillator"};
const char ck07[] __atr_section_progmem__ = {"Full Swing Crystal Oscillator"};
const char ck0F[] __atr_section_progmem__ = {"Low Power Crystal Oscillator"};
const char ckDef[]__atr_section_progmem__ = {"Unknown (or undefined) Clock Source"};

namespace mcu{
namespace System{

class Sleep{
    public:
        static void setMode(SleepMode sleepMode){
            constexpr static uint8_t bitmask_smcr_sleep_bits = (RegBits::Core::SMCR_SM2|
                                                                RegBits::Core::SMCR_SM1|
                                                                RegBits::Core::SMCR_SM0);
            mcu::Regs::Core::SleepModeControlReg.writeMasked(static_cast<uint8_t>(sleepMode), bitmask_smcr_sleep_bits);
        }
        static SleepMode getMode(){
            return static_cast<SleepMode>(~BITMASK_SLEEPMODE & Regs::Core::SleepModeControlReg.getValue());
        }
        static inline void enable(){
            Regs::Core::SleepModeControlReg.setBitmask(RegBits::Core::SMCR_SE);
        }
        static inline void disable(){
            Regs::Core::SleepModeControlReg.clearBitmask(RegBits::Core::SMCR_SE);
        }
        static bool isEnabled(){
            return Regs::Core::SleepModeControlReg.readBit(RegBits::Core::SMCR_SE);
        }
};

class Fuses {
    public:
        // Sınıftan nesne üretilmesini engelle (sadece statik kullanılacak)
        Fuses() = delete;
        static inline uint8_t getLowFuseBits() {
            return boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
        }
        static inline uint8_t getHighFuseBits() {
            return boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
        }
        static inline uint8_t getExtendedFuseBits() {
            return boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
        }
        static inline uint8_t getLockBits() {
            return boot_lock_fuse_bits_get(GET_LOCK_BITS);
        }
        static bool isBodEnabled(){
            return (getExtendedFuseBits() ^ (uint8_t)0xFF); 
        }
        static inline uint32_t readSignature(){
            return 0;
        }
    };

class Power{
public:
    static void shutdownPeripheral(Peripheral p){
        mcu::Regs::Core::PowerReductionReg.setBitmask(static_cast<uint8_t>(p));}
    static void activatePeripheral(Peripheral p){
        mcu::Regs::Core::PowerReductionReg.clearBitmask(static_cast<uint8_t>(p));}
};

class Clock{
public:
    static void         setPrescaler(SysClock prescalerValue){
        /* Save status reg and disable global interrupts*/
        uint8_t sreg = Regs::Core::StatusReg;
        cli();
        /* Set Clock Prescaler Change Enable bit (enable bit only)*/
        Regs::Core::ClockPrescaleReg = RegBits::Core::CLKPR_CLKPCE;
        /* Write new prescaler value within 4 clock cycles*/
        Regs::Core::ClockPrescaleReg.setValue(static_cast<uint8_t>(prescalerValue));
        /* Restore status reg */
        Regs::Core::StatusReg.setValue(sreg);
    }
    static SysClock     getPrescaler(){
        constexpr uint8_t bitmask_clk_prescaler = RegBits::Core::CLKPR_CLKPS3| 
                                                  RegBits::Core::CLKPR_CLKPS2|
                                                  RegBits::Core::CLKPR_CLKPS1|
                                                  RegBits::Core::CLKPR_CLKPS0;
        return static_cast<SysClock>(mcu::Regs::Core::ClockPrescaleReg.getValue(bitmask_clk_prescaler));
    }
    static uint32_t     getCpuFrequency(){
        return (F_CPU >> static_cast<uint8_t>(getPrescaler()));
    }
    static uint8_t      getMasterClockSource(){
        return (0x0F & Fuses::getLowFuseBits());
    }
    static const char*  getMasterClockSourceStr(){
        uint8_t cksel   = getMasterClockSource();

        switch (cksel){
            case 0x00: return ck00; break; // CKSEL3..0 = 0000 0x00
            case 0x02: return ck02; break; // CKSEL3..0 = 0010 0x02
            case 0x03: return ck03; break; // CKSEL3..0 = 0011 0x03
            case 0x04:                     // CKSEL3..0 = 0100 0x04
            case 0x05: return ck05; break; // CKSEL3..0 = 0101 0x05
            case 0x06:                     // CKSEL3..0 = 0110 0x06
            case 0x07: return ck07; break; // CKSEL3..0 = 0111 0x07
            case 0x08:                     // CKSEL3..0 = 1000 - 1111 
            case 0x0F: return ck0F; break;
            default  : return ckDef;break;
        }
    }
    static bool         isDividedBy8(){
        uint8_t lowFuse = Fuses::getLowFuseBits();
        return !(lowFuse & (1 << 7));
    }
    static void         calibrateInternalOscillator(){}
    static void         setOscCalValue(uint8_t calVal){mcu::Regs::Core::OscillatorCalibrationReg = calVal;}
    static uint8_t      getOscCalValue(){return mcu::Regs::Core::OscillatorCalibrationReg;}
};

class SysTick{
private:
    static volatile uint32_t _millis_counter;
    static volatile inline uint32_t sys_freq = 0;
public:
    void init(uint16_t systick_interval_ms){

    }

};

} //namespace System
} //namespace mcu

namespace mcu{
namespace System{
    static inline void delayUs(uint16_t){/* Some delay functions*/};
    static inline void delayMs(uint16_t){/* Some delay functions*/};
    static inline uint32_t getMillis()  {/* Code*/};
    static inline void globalInterruptEnable(){sei();}
    static inline void globalInterruptDisable(){cli();}
    [[flatten, always_inline]] inline static void sleep(){
        mcu::System::Sleep::enable();
        __asm__ __volatile__ ("sleep");
    }
} // namespace System
} // namespace mcu


#endif //SYS_CTRL_HPP