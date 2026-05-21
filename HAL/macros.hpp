#pragma once

/* Kodu doğrudan çağrıldığı yere kopyalar*/
#define __attrib_always_inline__    __attribute__((always_inline))
#define __attrib_no_inline__        __attribute__((noinline))
/* Bir fonksiyona eklendiğinde, o fonksiyonun kendi içinden çağırdığı diğer tüm fonksiyonları o anki kapsama inline 
olarak gömmeye çalışır.*/
#define __attrib_flatten__          __attribute__((flatten))
/* Derleyiciye "Bu fonksiyonun giriş (prologue) ve çıkış (epilogue) kodlarını (PUSH/POP işlemleri) üretme" der.*/
#define __attrib_naked__            __attribute__((naked))
#define __attrib_weak__             __attribute__((weak))
#define __attrib_section_progmem__  __attribute__((section(".progmem")))
#define __attrib_section_ram__      __attribute__((section(".ram")))
/* Bu veriler cihazın .hex dosyası ile birlikte bir .eep dosyası olarak çıkarılır.Programlayıcı (avrdude) ile cihaza 
yüklenirken EEPROM bölgesine yazılır.*/
#define __attrib_section_eeprom__   __attribute__((section(".eeprom")))
/* .noinit bölümüne atılan değişkenlere C startup kodu dokunmaz. Sadece cihazın elektriği tamamen 
kesilirse içindeki veri kaybolur.*/
#define __attrib_section_no_init__  __attribute__((section(".noinit")))
