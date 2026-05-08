
#ifndef SERIAL_STREAM_HPP
#define SERIAL_STREAM_HPP

#include <stdlib.h>
#include "registers.hpp"
#include "uart.hpp"

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

class SerialStream{
private:
    uint8_t m_precision = 2;
public:
    // Karakter basma
    SerialStream& operator<<(char c){
        mcu::Peripherals::Usart::transmitData(c); // Sizin yazdığınız UART sürücüsü
        return *this;
    }
    // Metin basma
    SerialStream& operator<<(const char* str){
        while (*str) mcu::Peripherals::Usart::transmitData(*str++);
        return *this;
    }
    // Sayı basma (Integer)
    SerialStream& operator<<(int val) {
        char buffer[10];
        itoa(val, buffer, 10);
        return *this << buffer;
    }
    SerialStream& operator<<(uint16_t val){
        char buffer[10];
        itoa(val, buffer, 10);
        return *this << buffer;
    }
    // Sayı basma (Integer)
    SerialStream& operator<<(uint32_t val) {
        char buffer[12]; // 32-bit unsigned long için 10 hane + sign/null yeterlidir
        // itoa yerine ultoa kullanıyoruz
        ultoa(val, buffer, 10); 
        return *this << buffer;
    }
    // Sayı basma (Integer)
    SerialStream& operator<<(uint8_t val) {
        char buffer[20];
        itoa(val, buffer, 10);
        return *this << buffer;
    }
    void printHex(uint8_t val) {
        const char hexChars[] = "0123456789ABCDEF";
        mcu::Peripherals::Usart::transmitData('0');
        mcu::Peripherals::Usart::transmitData('x');
        mcu::Peripherals::Usart::transmitData(hexChars[(val >> 4) & 0x0F]);
        mcu::Peripherals::Usart::transmitData(hexChars[val & 0x0F]);
    }

    SerialStream& operator<<(SetPrecision sp){
        m_precision = sp.value;
        return *this;
    }
    SerialStream& operator<<(double val) {
        // float değerler AVR'de zaten double olarak işlenir
        char buffer[20]; // "-" işareti, tam sayı kısmı, nokta ve küsurat için yeterli alan
        uint8_t safe_precision = (m_precision > 8) ? 8 : m_precision;
        // dtostrf Kullanımı: (değer, minimum_genişlik, virgülden_sonraki_hane, buffer)
        // Genişlik 1 verilirse sadece sayı kadar yer kaplar (boşluk bırakmaz).
        // 2 değeri, virgülden sonra kaç basamak gösterileceğini belirler.
        dtostrf(val, 1, safe_precision, buffer); 

        return *this << buffer;
    }
    // Fonksiyon işaretçisi alan operatör
    SerialStream& operator<<(SerialStream& (*func)(SerialStream&)){
        return func(*this);
    }
}cout;

inline SerialStream& endl(SerialStream& stream){
    stream << "\r\n";
    return stream;
}


} //namespace cstd

#endif //SERIAL_STREAM_HPP