#ifndef UART_HPP
#define UART_HPP

#define __AVR_ATmega328P__

#define TX_BUFFER_SIZE 64   //Byte (uint8_t)
#define RX_BUFFER_SIZE 64   //Byte (uint8_t)

#include "registers.hpp"
#include "utils/ring_buffer.hpp"
#include "sysctrl.hpp"
#include <math.h>

using namespace mcu;

enum class UsartBaudrate    : uint32_t{
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
enum class UsartMode        : uint8_t{
    Asynchronous    = 0x00,
    Synchronous     = RegBits::Uart::UCSR0C_UMSEL00,
    MasterSPI       = RegBits::Uart::UCSR0C_UMSEL00 | 
                      RegBits::Uart::UCSR0C_UMSEL01,
};
enum class UsartSyncClockPol: uint8_t{
    TxOnRisingRxOnFalling = 0, //Tx data changed on rising edge of XCK, Rx data sampled on falling edge
    TxOnFallingRxOnRising = 1, //Tx data changed on falling edge of XCK, Rx data sampled on rising edge
};
enum class UsartParityMode  : uint8_t{
    Disabled    = 0x00,
    EvenParity  = RegBits::Uart::UCSR0C_UPM01,
    OddParity   = RegBits::Uart::UCSR0C_UPM01 | 
                  RegBits::Uart::UCSR0C_UPM00,
};
enum class UsartDataSize    : uint8_t{
    //i.e. Character SiZe
    _5bit = 0,
    _6bit = 1,
    _7bit = 2,
    _8bit = 3,
    _9bit = 4,
};
enum class UsartStopBits    : uint8_t{
    One = 0,
    Two = 1,
};
enum class UsartSPIDataOrder : uint8_t{
    MsbFirst = 0,
    LsbFirst = 1
};
enum class UsartInterruptType: uint8_t {
    RxComplete        = RegBits::Uart::UCSR0B_RXCIE0,
    TxComplete        = RegBits::Uart::UCSR0B_TXCIE0,
    DataRegisterEmpty = RegBits::Uart::UCSR0B_UDRIE0,
    AllInterrupts     = RegBits::Uart::UCSR0B_RXCIE0 | 
                        RegBits::Uart::UCSR0B_TXCIE0 | 
                        RegBits::Uart::UCSR0B_UDRIE0,
};
enum class UsartSPIMode     : uint8_t{
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
    inline static Callback rxCompleteCallback = nullptr;
    inline static Callback txCompleteCallback = nullptr;
    inline static Callback dataRegisterEmptyCallback  = nullptr;
    static inline HAL::RingBuffer<uint8_t, TX_BUFFER_SIZE> txBuffer;
    static inline HAL::RingBuffer<uint8_t, RX_BUFFER_SIZE> rxBuffer;


    static bool transmissionsCompleted(){
        return (!Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_RXC0) && 
                Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_TXC0));
//        return (!Regs::Uart::UcsrA.readBit(RegBits::Uart::UCSRA_RXC0) && 
//                Regs::Uart::UcsrA.readBit(RegBits::Uart::UCSRA_TXC0));
    }
    static void setUsartMode(UsartMode usartMode){
        constexpr uint8_t bitmask_usart_mode = ~(mcu::RegBits::Uart::UCSR0C_UMSEL00 | mcu::RegBits::Uart::UCSR0C_UMSEL01);
        Regs::Uart::UartControlAndStatusRegC.writeMasked(static_cast<uint8_t>(usartMode), bitmask_usart_mode);
        /* Disable X2 speed when using one of synchronous operation modes.*/
        if(usartMode != UsartMode::Asynchronous) disableDoubleSpeed();
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
    inline static void enableDoubleSpeed(){
        // Enable double speed only if baud rate is not yet set.
        if (static_cast<uint8_t>(Regs::Uart::UartBaudrateRegLByte) == 0x00 && 
            static_cast<uint8_t>(Regs::Uart::UartBaudrateRegHByte) == 0x00){
            Regs::Uart::UartControlAndStatusRegA.setBitmask(RegBits::Uart::UCSR0A_U2X0);
        }
    }
    inline static void disableDoubleSpeed(){
        Regs::Uart::UartControlAndStatusRegA.clearBitmask(RegBits::Uart::UCSR0A_U2X0);
    }
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
                enableDoubleSpeed();
                modeDivisorConstant = 8;
            }else{
                /* Normal Speed: U2X0:0*/
                disableDoubleSpeed();
                modeDivisorConstant = 16;
            }
            }
            break;
        case UsartMode::Synchronous:
        case UsartMode::MasterSPI:
            disableDoubleSpeed();
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
    static void handleTxInterrupt(){
        uint8_t data;
        if(txBuffer.pop(data)){
            Regs::Uart::UartDataReg.setValue(data);
        }else{
            disableInterrupt(UsartInterruptType::DataRegisterEmpty);
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
    
    /* Checking*/
    [[nodiscard]] static bool isAvailable(){ return !rxBuffer.isEmpty();}
    [[nodiscard]] static bool isReceivingCompleted(){
        return Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_RXC0);
    }
    [[nodiscard]] static bool isTransmittingCompleted(){
        return Regs::Uart::UartControlAndStatusRegA.readBit(RegBits::Uart::UCSR0A_TXC0);
    }
    [[nodiscard]] static bool isReceiverEnabled(){
        return Regs::Uart::UartControlAndStatusRegB.readBit(RegBits::Uart::UCSR0B_RXEN0);
    }
    [[nodiscard]] static bool isTransmitterEnabled(){
        return Regs::Uart::UartControlAndStatusRegB.readBit(RegBits::Uart::UCSR0B_TXEN0);
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
    static void enableInterrupt(UsartInterruptType intType){
        mcu::Regs::Uart::UartControlAndStatusRegB.setBitmask(static_cast<uint8_t>(intType));
        /* Enable global interrupt (?)*/
        sei();
    }
    static void disableInterrupt(UsartInterruptType intType){
        mcu::Regs::Uart::UartControlAndStatusRegB.clearBitmask(static_cast<uint8_t>(intType));
    }
    static void attachInterrupt(UsartInterruptType intType, Callback callbackFunc){
        switch (intType){
        case UsartInterruptType::RxComplete: rxCompleteCallback = callbackFunc; break;
        case UsartInterruptType::TxComplete: txCompleteCallback = callbackFunc; break;
        case UsartInterruptType::DataRegisterEmpty: dataRegisterEmptyCallback = callbackFunc; break;
        case UsartInterruptType::AllInterrupts: break;}
        /* Enable related interrupt*/
        enableInterrupt(intType);
    }
    static void detachInterrupt(UsartInterruptType intType){
        switch (intType){
        case UsartInterruptType::RxComplete: rxCompleteCallback = nullptr; break;
        case UsartInterruptType::TxComplete: txCompleteCallback = nullptr; break;
        case UsartInterruptType::DataRegisterEmpty: dataRegisterEmptyCallback = nullptr; break;
        case UsartInterruptType::AllInterrupts: ;break;}
        /* Disable related interrupt*/
        disableInterrupt(intType);

    }
    static void interruptHandler(UsartInterruptType intType){
        switch (intType){
        case UsartInterruptType::RxComplete: if(rxCompleteCallback) rxCompleteCallback(); break;
        case UsartInterruptType::TxComplete: if(txCompleteCallback) txCompleteCallback(); break;
        case UsartInterruptType::DataRegisterEmpty: if(dataRegisterEmptyCallback) dataRegisterEmptyCallback(); break;}
    }
    static void enableMultiprocessorMode(){
        Regs::Uart::UartControlAndStatusRegA.setBitmask(RegBits::Uart::UCSR0A_MPCM0);
    }
    static void disableMultiprocessorMode(){
        Regs::Uart::UartControlAndStatusRegA.clearBitmask(RegBits::Uart::UCSR0A_MPCM0);
    }
    static void enableReceiver(){
        /* Enable usart receiver
        The receiver will override normal port operation for the RxDn pin when enabled*/
        Regs::Uart::UartControlAndStatusRegB.setBitmask(RegBits::Uart::UCSR0B_RXEN0);
    }
    static void disableReceiver(){
        Regs::Uart::UartControlAndStatusRegB.clearBitmask(RegBits::Uart::UCSR0B_RXEN0);
        /* Config. rx pin Hi-Z. (not necessary)*/
        mcu::Gpio::PinRXD::setPinMode(PinMode::HighImpedance);
    }
    static void enableTransmitter(){
        /* Enable usart transmitter
        The transmitter will override normal port operation for the TxDn pin when enabled*/
        Regs::Uart::UartControlAndStatusRegB.setBitmask(RegBits::Uart::UCSR0B_TXEN0);
    }
    static void disableTransmitter(){
        /* When disabled, the transmitter will no longer override the TxDn port*/
        Regs::Uart::UartControlAndStatusRegB.clearBitmask(RegBits::Uart::UCSR0B_TXEN0);
    }

    static void init(UsartMode       usartMode   = UsartMode::Asynchronous, 
                     UsartBaudrate   baud        = UsartBaudrate::_9600bps, 
                     UsartDataSize   dataSize    = UsartDataSize::_8bit, 
                     UsartStopBits   stopBit     = UsartStopBits::One, 
                     UsartParityMode parityMode  = UsartParityMode::Disabled){
        /* Activate usart0 in power reduction reg.*/
        System::Power::activatePeripheral(Peripheral::Usart0);

        /* Usart çalışma ayarı*/
        setUsartMode(usartMode);
        
        /* Mode-specific settings*/
        /* Write this bit to zero when asynchronous mode is used*/
        if(usartMode != UsartMode::Asynchronous){setClockPolarity(UsartSyncClockPol::TxOnFallingRxOnRising);}
        
        /* Baudrate settings */
        setBaudrate(baud);

        /* Frame ayarları*/
        setDataSize(dataSize);
        setParityMode(parityMode);
        setStopBit(stopBit);

        /**/
        attachInterrupt(UsartInterruptType::RxComplete, handleRxInterrupt);
        attachInterrupt(UsartInterruptType::DataRegisterEmpty, handleTxInterrupt);

        /* RX'i ve TX'i etkinleştir*/
        enableReceiver();
        enableTransmitter();
    }
    static void reset(){
        disableInterrupt(UsartInterruptType::AllInterrupts);
        disableReceiver();
        disableTransmitter();
        disableDoubleSpeed();
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
        enableInterrupt(UsartInterruptType::DataRegisterEmpty);
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

ISR(USART_UDRE_vect){Peripherals::Usart::interruptHandler(UsartInterruptType::DataRegisterEmpty);}
ISR(USART_RX_vect)  {Peripherals::Usart::interruptHandler(UsartInterruptType::RxComplete);}
ISR(USART_TX_vect)  {}

#endif //UART_HPP