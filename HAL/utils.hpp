#ifndef HAL_UTILS_HPP
#define HAL_UTILS_HPP

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/*Voltage/Current units */
//constexpr unsigned long long operator""V(unsigned long long V){}
//constexpr unsigned long long operator""mV(unsigned long long mV){}
//constexpr unsigned long long operator""A(unsigned long long A){}
//constexpr unsigned long long operator""mA(unsigned long long mA){}

/* Time units*/
constexpr unsigned long long operator""ms(unsigned long long ms) {
    // Formül: (F_CPU / Prescaler / 1000) * ms
    // Prescaler = 256 seçersek:
    return (F_CPU / 256 / 1000) * ms;
}
//constexpr unsigned long long operator""us(unsigned long long us){}
//constexpr unsigned long long operator""sec(unsigned long long sec){}

/* Frequency units*/
//constexpr unsigned long long operator""MHz(unsigned long long MHz){}
constexpr unsigned long long operator""kHz(unsigned long long kHz){
    return 1000*kHz;
}
constexpr unsigned long long operator""Hz(unsigned long long Hz) {
    // Frekans hesabı (CTC Modu için)
    return (F_CPU / 256) / Hz;
}

/* Data rate units*/
constexpr unsigned long long operator""bps(unsigned long long bps){
    return bps;
}
constexpr unsigned long long operator""kbps(unsigned long long kbps){
    return kbps;
}
//constexpr unsigned long long operator""Mbps(unsigned long long Mbps){}

namespace cstd{
    struct true_type {static constexpr bool value = true; };
    struct false_type{static constexpr bool value = false;};

    template<typename T, typename U>struct is_same : false_type {};
    template<typename T> struct is_same<T, T> : true_type {};

    template<typename... Ts> using void_t = void;
};

#endif //HAL_UTILS_HPP