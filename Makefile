MCU          ?= atmega328p
F_CPU        ?= 16000000UL
EP           ?= EP01_GPIO
PROGRAMMER   ?= arduino
PORT         ?= /dev/ttyUSB0
UPLOAD_BAUD  ?= 115200

AVR_CC       ?= avr-gcc
AVR_OBJCOPY  ?= avr-objcopy
AVR_SIZE     ?= avr-size
AVRDUDE      ?= avrdude

EPISODES := \
	EP01_GPIO \
	EP02_UART \
	EP03_TIMER \
	EP04_INTERRUPT \
	EP05_PWM \
	EP06_ADC \
	EP07_SPI \
	EP08_I2C

ELF_FILES := $(addprefix build/,$(addsuffix .elf,$(EPISODES)))
HEX_FILES := $(addprefix build/,$(addsuffix .hex,$(EPISODES)))

CFLAGS  := -mmcu=$(MCU) -DF_CPU=$(F_CPU) -std=c11 -Os
CFLAGS  += -Wall -Wextra -Werror -ffunction-sections -fdata-sections
LDFLAGS := -Wl,--gc-sections

.DEFAULT_GOAL := help

.PHONY: help all build-selected size flash clean

.SECONDARY: $(ELF_FILES)

help:
	@echo AVR Native C Embedded Roadmap
	@echo ""
	@echo   make all
	@echo   make build-selected EP=EP01_GPIO
	@echo   make size EP=EP01_GPIO
	@echo   make flash EP=EP01_GPIO PORT=/dev/ttyUSB0
	@echo   make clean

all: $(HEX_FILES)

build-selected: build/$(EP).hex

size: build/$(EP).elf
	$(AVR_SIZE) -C --mcu=$(MCU) $<

flash: build/$(EP).hex
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(UPLOAD_BAUD) -D -U flash:w:$<:i

build/%.elf: %/src/main.c | build
	$(AVR_CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

build/%.hex: build/%.elf
	$(AVR_OBJCOPY) -O ihex -R .eeprom $< $@

build:
ifeq ($(OS),Windows_NT)
	@if not exist build mkdir build
else
	@mkdir -p build
endif

clean:
ifeq ($(OS),Windows_NT)
	@if exist build rmdir /S /Q build
else
	@rm -rf -- build
endif
