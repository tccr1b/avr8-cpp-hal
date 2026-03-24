# ==========================================================
# 1. TEMEL AYARLAR
# ==========================================================

DEVICE      = atmega328p
F_CPU       = 16000000UL

PROGRAMMER  = arduino
PORT        = /dev/ttyACM0
BAUDRATE    = 115200

TARGET      = main

# ==========================================================
# 2. ARAÇ ZİNCİRİ TANIMLARI
# ==========================================================
TOOLCHAIN_PATH = /opt/atmel/avr8/avr8-gnu-toolchain-linux_x86_64/bin
CC          = $(TOOLCHAIN_PATH)/avr-g++         # C++ Derleyici
OBJCOPY     = $(TOOLCHAIN_PATH)/avr-objcopy
OBJDUMP		= $(TOOLCHAIN_PATH)/avr-objdump
SIZE		= $(TOOLCHAIN_PATH)/avr-size
AVRDUDE     = avrdude	# Yeni avr8-toolchain'in içinde avrdude yoktu, eskisiyle devam.

# ==========================================================
# 3. KAYNAKLAR ve DERLEYİCİ BAYRAKLARI
# ==========================================================
SRCS        = $(TARGET).cpp             # main.cpp
OBJS        = $(SRCS:.cpp=.o)           # main.o
ELF         = $(TARGET).elf
HEX         = $(TARGET).hex
LSS			= $(TARGET).lss

# CFLAGS: -std=c++17 eklenmiştir
#CFLAGS = -Wall -Wextra -g -Os -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -I. -std=c++17
CXXFLAGS = -Wall -Wextra -g -Os -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -I. -std=c++17

# Avrdude Bayrakları
AVRDUDE_FLAGS = -c $(PROGRAMMER) -p $(DEVICE) -P $(PORT) -b $(BAUDRATE)

# ==========================================================
# 4. DERLEME KURALLARI
# ==========================================================

all: $(HEX)

# Kural A: ELF dosyasını HEX dosyasına dönüştür
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Kural B: Nesne dosyalarını bağlayarak ELF dosyasını oluşturur.
$(ELF): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^

# Kural C: C++ Kaynaklarını Nesneye Dönüştürme
%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

# ==========================================================
# 5a. RAPORLAMA ve BİLGİ
# ==========================================================
size:
	@echo "--- HAFIZA KULLANIMI ---"
	@$(SIZE) --format-avr --mmcu=$(DEVICE) $(ELF)

# ==========================================================
# 5b. YÜKLEME VE TEMİZLİK
# ==========================================================
upload: all
	@echo "--- YÜKLEME BAŞLATILIYOR ---"
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$(HEX):i

clean:
	@echo "--- TEMİZLENİYOR ---"
	rm -f $(OBJS) $(ELF) $(HEX)

.PHONY: all clean upload