
#ifndef SERIAL_STREAM_HPP
#define SERIAL_STREAM_HPP

#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <stdlib.h>
#include "HAL/registers.hpp"
#include "HAL/uart.hpp"

/* Kullanım:
SerialStream cout;
cout << "Sicaklik: " << sensorValue << " C" << "\r\n";
*/
namespace cstd{
struct SetPrecision{
    uint8_t value;
};

inline SetPrecision setprecision(uint8_t p){
    return SetPrecision{p};
}

struct EndLine{} inline endl;

class SerialStream{
private:
    uint8_t m_precision = 2;
    inline static mcu::Peripherals::Usart::config_t coutCfg;
public:
    /* Default Usart config for cout
    Baudrate    : 9600
    Data Frame  : 8N1
    Parity      : Disabled
    */
    static void init(){
        coutCfg.usart_baud        = UsartBaudrate::_9600bps;
        coutCfg.usart_data_size   = UsartDataSize::_8bit;
        coutCfg.usart_mode        = UsartMode::Asynchronous;
        coutCfg.usart_parity_mode = UsartParityMode::Disabled;
        coutCfg.usart_rx_en       = true;
        coutCfg.usart_tx_en       = true;
        coutCfg.usart_stop_bits   = UsartStopBits::One;

        mcu::Peripherals::Usart::init(coutCfg);
    }
    void disableUsartRX(){
        mcu::Peripherals::Usart::Receiver.disable();        
    }
    SerialStream& operator<<(char c){
        mcu::Peripherals::Usart::transmitData(c);
        return *this;}
    SerialStream& operator<<(const char* str){
        while (*str) mcu::Peripherals::Usart::transmitData(*str++);
        return *this;
    }
    SerialStream& operator<<(int val){
        char buffer[10];
        itoa(val, buffer, 10);
        return *this << buffer;
    }
    SerialStream& operator<<(uint16_t val){
        char buffer[10];
        itoa(val, buffer, 10);
        return *this << buffer;
    }
    SerialStream& operator<<(uint32_t val){
        char buffer[12]; // 32-bit unsigned long için 10 hane + sign/null yeterlidir
        // itoa yerine ultoa kullanıyoruz
        ultoa(val, buffer, 10); 
        return *this << buffer;
    }
    SerialStream& operator<<(uint8_t val){
        char buffer[20];
        itoa(val, buffer, 10);
        return *this << buffer;
    }
    SerialStream& operator<<(SetPrecision sp){
        m_precision = sp.value;
        return *this;
    }
    SerialStream& operator<<(double val){
        // float değerler AVR'de zaten double olarak işlenir
        char buffer[20]; // "-" işareti, tam sayı kısmı, nokta ve küsurat için yeterli alan
        uint8_t safe_precision = (m_precision > 8) ? 8 : m_precision;
        // dtostrf Kullanımı: (değer, minimum_genişlik, virgülden_sonraki_hane, buffer)
        // Genişlik 1 verilirse sadece sayı kadar yer kaplar (boşluk bırakmaz).
        // 2 değeri, virgülden sonra kaç basamak gösterileceğini belirler.
        dtostrf(val, 1, safe_precision, buffer); 

        return *this << buffer;
    }
    SerialStream& operator<<(SerialStream& (*func)(SerialStream&)){
        return func(*this);
    }
    SerialStream& operator<<(EndLine& /*el*/){
        mcu::Peripherals::Usart::transmitData('\r');
        mcu::Peripherals::Usart::transmitData('\n');
        return *this;
    }
    void printHex(uint8_t val) {
        const char hexChars[] = "0123456789ABCDEF";
        mcu::Peripherals::Usart::transmitData('0');
        mcu::Peripherals::Usart::transmitData('x');
        mcu::Peripherals::Usart::transmitData(hexChars[(val >> 4) & 0x0F]);
        mcu::Peripherals::Usart::transmitData(hexChars[val & 0x0F]);
    }
}cout;

} //namespace cstd

#endif //SERIAL_STREAM_HPP