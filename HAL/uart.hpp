#ifndef UART_HPP
#define UART_HPP

#define __AVR_ATmega328P__

#define TX_BUFFER_SIZE 64   //Byte (uint8_t)
#define RX_BUFFER_SIZE 64   //Byte (uint8_t)

#include <math.h>

#include "HAL/registers.hpp"
#include "HAL/sysctrl.hpp"
#include "HAL/utils/ring_buffer.hpp"
#include "HAL/gpio.hpp"

using namespace mcu;
using namespace HAL;

enum class UsartBaudrate      : uint32_t{
    _2400bps   = 2400,
    _4800bps   = 4800,
    _9600bps   = 9600,
    _14400bps  = 14400,
    _19200bps  = 19200,
    _28800bps  = 28800,
    _38400bps  = 38400,
    _57600bps  = 57600,
    _76800bps  = 76800,
    _115200bps = 115200,
    _230400bps = 230400,
    _250kbps   = 250000,
    _500kbps   = 500000,
    _1Mbps     = 1000000,
    _2Mbps     = 2000000,
};
enum class UsartMode          : uint8_t{
    Asynchronous    = 0x00,
    Synchronous     = RegBits::Uart::UCSR0C_UMSEL00,
    MasterSPI       = RegBits::Uart::UCSR0C_UMSEL00 | 
                      RegBits::Uart::UCSR0C_UMSEL01,
};
enum class UsartSyncClockPol  : uint8_t{
    TxOnRisingRxOnFalling = 0, //Tx data changed on rising edge of XCK, Rx data sampled on falling edge
    TxOnFallingRxOnRising = 1, //Tx data changed on falling edge of XCK, Rx data sampled on rising edge
};
enum class UsartParityMode    : uint8_t{
    Disabled    = 0x00,
    EvenParity  = RegBits::Uart::UCSR0C_UPM01,
    OddParity   = RegBits::Uart::UCSR0C_UPM01 | 
                  RegBits::Uart::UCSR0C_UPM00,
};
enum class UsartDataSize      : uint8_t{
    //i.e. Character SiZe
    _5bit = 0,
    _6bit = 1,
    _7bit = 2,
    _8bit = 3,
    _9bit = 4,
};
enum class UsartStopBits      : uint8_t{
    One = 0,
    Two = 1,
};
enum class UsartSPIDataOrder  : uint8_t{
    MsbFirst = 0,
    LsbFirst = 1
};
enum class UsartInterruptType : uint8_t{
    RxComplete        = RegBits::Uart::UCSR0B_RXCIE0,
    TxComplete        = RegBits::Uart::UCSR0B_TXCIE0,
    DataRegisterEmpty = RegBits::Uart::UCSR0B_UDRIE0,
    AllInterrupts     = RegBits::Uart::UCSR0B_RXCIE0 | 
                        RegBits::Uart::UCSR0B_TXCIE0 | 
                        RegBits::Uart::UCSR0B_UDRIE0,
};
enum class UsartSPIMode       : uint8_t{
    Mode0 = 0x00,
    Mode1 = RegBits::Uart::UCSR0C_UCPHA0,
    Mode2 = RegBits::Uart::UCSR0C_UCPOL0,
    Mode3 = RegBits::Uart::UCSR0C_UCPHA0 | RegBits::Uart::UCSR0C_UCPOL0,
};

namespace mcu{
namespace Peripherals{

class Usart{
private:
    static inline UsartBaudrate currentBaudrate = UsartBaudrate::_9600bps;
    using Callback = void(*)();
    inline static Callback cbRxCompleteCallback = nullptr;
    inline static Callback cbTxCompleteCallback = nullptr;
    inline static Callback cbDataRegisterEmptyCallback  = nullptr;
    static inline HAL::RingBuffer<uint8_t, TX_BUFFER_SIZE> txBuffer;
    static inline HAL::RingBuffer<uint8_t, RX_BUFFER_SIZE> rxBuffer;
    template<typename regAddr, uint8_t bitPosBitmask> struct Feature{
        void enable()   {regAddr().setBitmask(bitPosBitmask);}
        void disable()  {regAddr().clearBitmask(bitPosBitmask);}
        [[nodiscard]] bool isEnabled(){return regAddr().readBit(bitPosBitmask);}
    };
    static bool transmissionsCompleted(){
        return (!Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_RXC0) && 
                Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_TXC0));
    }
    static void setUsartMode(UsartMode usartMode){
        constexpr uint8_t bitmask_usart_mode = ~(mcu::RegBits::Uart::UCSR0C_UMSEL00 | mcu::RegBits::Uart::UCSR0C_UMSEL01);
        Regs::Uart::UartControlAndStatusRegC.writeMasked(static_cast<uint8_t>(usartMode), bitmask_usart_mode);
        /* Disable X2 speed when using one of synchronous operation modes.*/
        if(usartMode != UsartMode::Asynchronous) DoubleSpeed.disable();
        /* Clock Polarity (UCPOL0) bit is used for synchronous mode only. Write this bit to zero when asynchronous mode is used.*/
        if(usartMode == UsartMode::Asynchronous) setClockPolarity(UsartSyncClockPol::TxOnRisingRxOnFalling);
        /* The power reduction USART bit, PRUSART0 must be disabled by writing a logical zero to it.*/
        Regs::Core::PowerReductionReg.clearBitmask(RegBits::Core::PRR_PRUSART0);
    }
    static UsartMode getUsartMode(){
        constexpr uint8_t bitmask_usart_mode = (mcu::RegBits::Uart::UCSR0C_UMSEL00 | mcu::RegBits::Uart::UCSR0C_UMSEL01);
        uint8_t newMode = (uint8_t)(Regs::Uart::UartControlAndStatusRegC) & bitmask_usart_mode;

        return (static_cast<UsartMode>(newMode));
    }
    struct{ //Double speed X2
        void enable(){
            // Enable double speed only if baudrate is not yet set.
            if (static_cast<uint8_t>(Regs::Uart::UartBaudrateRegLByte) == 0x00 && 
                static_cast<uint8_t>(Regs::Uart::UartBaudrateRegHByte) == 0x00){
                Regs::Uart::UartControlAndStatusRegA.setBitmask(RegBits::Uart::UCSR0A_U2X0);
            }
        }
        void disable(){Regs::Uart::UartControlAndStatusRegA.clearBitmask(RegBits::Uart::UCSR0A_U2X0);}
        void isEnabled(){Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_U2X0);}
    }static DoubleSpeed;
    static void setParityMode(UsartParityMode parityMode){
        constexpr uint8_t bitmask_parity_mode = ~(RegBits::Uart::UCSR0C_UPM00 | RegBits::Uart::UCSR0C_UPM01);
        Regs::Uart::UartControlAndStatusRegC.writeMasked(static_cast<uint8_t>(parityMode), bitmask_parity_mode);
    }
    static void setStopBit(UsartStopBits stopBit){
        if(stopBit == UsartStopBits::Two){
            Regs::Uart::UartControlAndStatusRegC.setBitmask(RegBits::Uart::UCSR0C_USBS0);
        }else{
            Regs::Uart::UartControlAndStatusRegC.clearBitmask(RegBits::Uart::UCSR0C_USBS0);
        }
    }
    static void setDataSize(UsartDataSize dataSize){
        switch (dataSize){
        case UsartDataSize::_5bit:
            Regs::Uart::UartControlAndStatusRegC.clearBitmask(RegBits::Uart::UCSR0C_UCSZ00 |
                                                              RegBits::Uart::UCSR0C_UCSZ01);
            Regs::Uart::UartControlAndStatusRegB.clearBitmask(RegBits::Uart::UCSR0B_UCSZ02);
            break;
        case UsartDataSize::_6bit:
            Regs::Uart::UartControlAndStatusRegC.clearBitmask(RegBits::Uart::UCSR0C_UCSZ01);
            Regs::Uart::UartControlAndStatusRegC.setBitmask(RegBits::Uart::UCSR0C_UCSZ00);
            Regs::Uart::UartControlAndStatusRegB.clearBitmask(RegBits::Uart::UCSR0B_UCSZ02);
            break;
        case UsartDataSize::_7bit:
            Regs::Uart::UartControlAndStatusRegC.setBitmask(RegBits::Uart::UCSR0C_UCSZ01);
            Regs::Uart::UartControlAndStatusRegC.clearBitmask(RegBits::Uart::UCSR0C_UCSZ00);
            Regs::Uart::UartControlAndStatusRegB.clearBitmask(RegBits::Uart::UCSR0B_UCSZ02);
            break;
        case UsartDataSize::_8bit:
            Regs::Uart::UartControlAndStatusRegC.setBitmask(RegBits::Uart::UCSR0C_UCSZ00 | 
                                                            RegBits::Uart::UCSR0C_UCSZ01);
            Regs::Uart::UartControlAndStatusRegB.clearBitmask(RegBits::Uart::UCSR0B_UCSZ02);
            break;
        case UsartDataSize::_9bit:
            Regs::Uart::UartControlAndStatusRegC.setBitmask(RegBits::Uart::UCSR0C_UCSZ00 | 
                                                            RegBits::Uart::UCSR0C_UCSZ01);
            Regs::Uart::UartControlAndStatusRegB.setBitmask(RegBits::Uart::UCSR0B_UCSZ02);
            break;
        }
    }
    static void setClockPolarity(UsartSyncClockPol xckPolarity){   
        /* */             
        switch (xckPolarity){
        case UsartSyncClockPol::TxOnFallingRxOnRising:
            Regs::Uart::UartControlAndStatusRegC.setBitmask(RegBits::Uart::UCSR0C_UCPOL0);
            break;
        case UsartSyncClockPol::TxOnRisingRxOnFalling:
            Regs::Uart::UartControlAndStatusRegC.clearBitmask(RegBits::Uart::UCSR0C_UCPOL0);
            break;
        }
    }
    static void setBaudrate (UsartBaudrate baud){
        /* Clear baudrate settings*/
        Regs::Uart::UartBaudrateRegHByte = 0x00;
        Regs::Uart::UartBaudrateRegLByte = 0x00;

        currentBaudrate = baud;
        uint8_t modeDivisorConstant;
        uint32_t fCpu = mcu::System::Clock::getCpuFrequency();
        uint32_t baudValue = static_cast<uint32_t>(baud);
        
        switch (getUsartMode()){
        case UsartMode::Asynchronous:
            /* U2X option can be used.*/
            /* Check UBBR value whether it is appropriate for desired baudrate*/
            {
            uint8_t mdc = 16; //x2 off
            uint16_t tmpUbbr = (uint16_t)(fCpu/(mdc * baudValue)) - 1;  //x2 off
            uint32_t tmpBaud = fCpu/(mdc * (tmpUbbr + 1));     //x2 off            
            int16_t errorWithoutX2 = ((tmpBaud * 100) / baudValue) - 100;

            if(abs(errorWithoutX2)>3){
                /* Double Speed: U2X0:1*/
                DoubleSpeed.enable();
                modeDivisorConstant = 8;
            }else{
                /* Normal Speed: U2X0:0*/
                DoubleSpeed.disable();
                modeDivisorConstant = 16;
            }
            }
            break;
        case UsartMode::Synchronous:
        case UsartMode::MasterSPI:
            DoubleSpeed.disable();
            modeDivisorConstant = 2;
            break;
        }
        uint16_t ubrrValue = (fCpu / (modeDivisorConstant * baudValue)) - 1;
        // UBRR0H (Üst 4 bit) ve UBRR0L (Alt 8 bit)
        Regs::Uart::UartBaudrateRegHByte.setValue(static_cast<uint8_t>(ubrrValue >> 8));
        Regs::Uart::UartBaudrateRegLByte.setValue(static_cast<uint8_t>(ubrrValue));
    }
    static void setDataOrder(UsartSPIDataOrder dataOrder){
        /* This is for MSPImode onyl.*/
        if(UsartMode::MasterSPI != getUsartMode()) return;
        if(static_cast<bool>(dataOrder)){
            Regs::Uart::UartControlAndStatusRegC.setBitmask(RegBits::Uart::UCSR0C_UDORD0);
        }else{
            Regs::Uart::UartControlAndStatusRegC.clearBitmask(RegBits::Uart::UCSR0C_UDORD0);
        }
    }
    static bool setMSPIMode(UsartSPIMode spiMode){
        /* For MSPIM only.*/
        if(UsartMode::MasterSPI != getUsartMode()) return false;
        /* Bitmask for mspim usart*/
        constexpr uint8_t bitmask_mspim_mode = ~(RegBits::Uart::UCSR0C_UCPHA0 | RegBits::Uart::UCSR0C_UCPOL0);
        /* Apply mode*/
        mcu::Regs::Uart::UartControlAndStatusRegC.writeMasked(static_cast<uint8_t>(spiMode), bitmask_mspim_mode);
    }

    public:
    struct Config{
        UsartMode       usart_mode;
        UsartBaudrate   usart_baud;
        UsartDataSize   usart_data_size;
        UsartStopBits   usart_stop_bits;
        UsartParityMode usart_parity_mode;
        bool            usart_tx_en;
        bool            usart_rx_en;
    };
    static void handleTxInterrupt(){
        uint8_t data;
        if(txBuffer.pop(data)){
            Regs::Uart::UartDataReg.setValue(data);
        }else{
            Interrupts.disable(UsartInterruptType::DataRegisterEmpty);
        }
    }
    static void handleRxInterrupt(){
        if(!rxBuffer.isFull()){
            rxBuffer.push(Regs::Uart::UartDataReg);
        }else{
            volatile uint8_t dummy = Regs::Uart::UartDataReg;
            (void)dummy;
        }
    }
    static Usart::Feature<decltype(Regs::Uart::UartControlAndStatusRegA), RegBits::Uart::UCSR0A_MPCM0> MultiprocessorMode;
    static Usart::Feature<decltype(Regs::Uart::UartControlAndStatusRegB), RegBits::Uart::UCSR0B_RXEN0> Receiver;
    static Usart::Feature<decltype(Regs::Uart::UartControlAndStatusRegB), RegBits::Uart::UCSR0B_TXEN0> Transmitter;

    /* Checking*/
    [[nodiscard]] static bool isAvailable(){return !rxBuffer.isEmpty();}
    [[nodiscard]] static bool isReceivingCompleted(){
        return Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_RXC0);
    }
    [[nodiscard]] static bool isTransmittingCompleted(){
        return Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_TXC0);
    }
    [[nodiscard]] static bool isDataRegisterEmpty(){
        return Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_UDRE0);
    }
    [[nodiscard]] static bool checkDataOverrun(){
        return Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_DOR0);
    }
    [[nodiscard]] static bool checkFrameError(){
        return Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_FE0);
    }
    [[nodiscard]] static bool checkParityError(){
        return Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_UPE0);
    }

    /* Controlling*/
    struct{ //Interrrupts
        void enable(UsartInterruptType intType){
            mcu::Regs::Uart::UartControlAndStatusRegB.setBitmask(static_cast<uint8_t>(intType));
            /* Enable global interrupt (?)*/
            sei();
        }
        void disable(UsartInterruptType intType){
            mcu::Regs::Uart::UartControlAndStatusRegB.clearBitmask(static_cast<uint8_t>(intType));
        }
        void attach(UsartInterruptType intType, Callback callbackFunc){
            switch (intType){
                case UsartInterruptType::RxComplete       :cbRxCompleteCallback        = callbackFunc; break;
                case UsartInterruptType::TxComplete       :cbTxCompleteCallback        = callbackFunc; break;
                case UsartInterruptType::DataRegisterEmpty:cbDataRegisterEmptyCallback = callbackFunc; break;
                case UsartInterruptType::AllInterrupts: break;
            }
            /* Enable related interrupt*/
            this->enable(intType);
        }
        void detach(UsartInterruptType intType){
            switch (intType){
                case UsartInterruptType::RxComplete       :cbRxCompleteCallback       = nullptr; break;
                case UsartInterruptType::TxComplete       :cbTxCompleteCallback       = nullptr; break;
                case UsartInterruptType::DataRegisterEmpty:cbDataRegisterEmptyCallback= nullptr; break;
                case UsartInterruptType::AllInterrupts    :cbRxCompleteCallback       = nullptr;
                                                           cbTxCompleteCallback       = nullptr;
                                                           cbDataRegisterEmptyCallback= nullptr; break;
            }
            /* Disable related interrupt*/
            this->disable(intType);
        }
        inline void handle(UsartInterruptType intType) __atr_always_inline__{
            switch (intType){
                case UsartInterruptType::RxComplete       : if(cbRxCompleteCallback)        cbRxCompleteCallback()       ;break;
                case UsartInterruptType::TxComplete       : if(cbTxCompleteCallback)        cbTxCompleteCallback()       ;break;
                case UsartInterruptType::DataRegisterEmpty: if(cbDataRegisterEmptyCallback) cbDataRegisterEmptyCallback();break;
            }
        }
    }static Interrupts;
    static void init(UsartMode       usart_mode  = UsartMode::Asynchronous, 
                     UsartBaudrate   baudrate    = UsartBaudrate::_9600bps, 
                     UsartDataSize   data_size   = UsartDataSize::_8bit, 
                     UsartStopBits   stop_bit    = UsartStopBits::One, 
                     UsartParityMode parity_mode = UsartParityMode::Disabled){
        /* Activate usart0 in power reduction reg.*/
        mcu::System::Power::activatePeripheral(Peripheral::Usart0);

        /* Usart çalışma ayarı*/
        setUsartMode(usart_mode);
        
        /* Mode-specific settings*/
        /* Write this bit to zero when asynchronous mode is used*/
        if(usart_mode != UsartMode::Asynchronous){setClockPolarity(UsartSyncClockPol::TxOnFallingRxOnRising);}
        
        /* Baudrate settings */
        setBaudrate(baudrate);

        /* Frame ayarları*/
        setDataSize(data_size);
        setParityMode(parity_mode);
        setStopBit(stop_bit);

        /**/
        Interrupts.attach(UsartInterruptType::RxComplete, handleRxInterrupt);
        Interrupts.attach(UsartInterruptType::DataRegisterEmpty, handleTxInterrupt);

        /* RX'i ve TX'i etkinleştir*/
        Receiver.enable();
        Transmitter.disable();
    }
    static void init(Config* cfg){
        mcu::System::Power::activatePeripheral(Peripheral::Usart0);
        setUsartMode (cfg->usart_mode);
        setBaudrate  (cfg->usart_baud);
        setDataSize  (cfg->usart_data_size);
        setParityMode(cfg->usart_parity_mode);
        setStopBit   (cfg->usart_stop_bits);

        /* Configure interrupts for non-blocking usart usage*/
        Interrupts.attach(UsartInterruptType::RxComplete, handleRxInterrupt);
        Interrupts.attach(UsartInterruptType::DataRegisterEmpty, handleTxInterrupt);
        
        if(cfg->usart_rx_en) Receiver.enable();
        if(cfg->usart_tx_en) Transmitter.enable();
    }
    static void reset(){
        Interrupts.disable(UsartInterruptType::AllInterrupts);
        Receiver.disable();
        Transmitter.disable();
        DoubleSpeed.disable();
        Regs::Uart::UartBaudrateRegHByte = 0x00;
        Regs::Uart::UartBaudrateRegLByte = 0x00;
        rxBuffer.reset();
        txBuffer.reset();
    }
    /* Callback func if fCpu is changed.*/
    static void updateBaudrate(){setBaudrate(currentBaudrate);}
    /* ????*/
    static void updateBaudrate(UsartBaudrate newBaud){setBaudrate(newBaud);}
    static void transmitData(uint8_t data){
        /* boş döngü, watchdog timer kullanilabilir*/
        while(txBuffer.isFull()){}
        txBuffer.push(data);
        Interrupts.enable(UsartInterruptType::DataRegisterEmpty);
    }
    static uint8_t readData(){
        uint8_t data = 0;
        if(rxBuffer.pop(data)){
            return data;
        }else{
            return 0;
        }
    }

};
};
};

ISR(USART_UDRE_vect){mcu::Peripherals::Usart::Interrupts.handle(UsartInterruptType::DataRegisterEmpty);}
ISR(USART_RX_vect)  {mcu::Peripherals::Usart::Interrupts.handle(UsartInterruptType::RxComplete);}
ISR(USART_TX_vect)  {mcu::Peripherals::Usart::Interrupts.handle(UsartInterruptType::TxComplete);}

#endif //UART_HPP