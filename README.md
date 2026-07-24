# AVR Native C Embedded Roadmap

Repository คู่ขนานแบบ Native AVR C ของ
[Arduino Uno Embedded Roadmap](https://github.com/KOPE-SOLUTION/arduino-uno-embedded-roadmap)
โดยควบคุม Register และ Peripheral ของ ATmega328P โดยตรง

ซีรีส์นี้คงลำดับการเรียน EP01-EP08 เดิม แต่เปลี่ยนจาก Arduino API มาเป็น
Register-Level C เพื่อให้เห็นการทำงานภายในของ GPIO, USART, Timer,
Interrupt, PWM, ADC, SPI และ I2C/TWI

```c
/* Arduino Framework */
digitalWrite(13, HIGH);

/* Native AVR C */
PORTB |= (1U << PORTB5);
```

## ขอบเขตของ Repository

| รายการ | สิ่งที่เลือกใช้ |
| --- | --- |
| MCU เป้าหมาย | ATmega328P |
| บอร์ดอ้างอิง | Arduino Uno R3 ความถี่ 16 MHz |
| ภาษา | C |
| Framework | ไม่มี |
| ระดับการเขียนโปรแกรม | Register-Level |
| IDE/Compiler สำหรับบทเรียนหลัก | MPLAB X IDE + MPLAB XC8 |
| IDE แบบเบาจากผู้ผลิต | MPLAB for VS Code + MPLAB XC8 |
| Toolchain แบบ Command Line | avr-gcc + avr-libc + Make + avrdude |

`<avr/io.h>` ทำหน้าที่ประกาศชื่อ Register ของ MCU ที่เลือก ไม่ใช่ Arduino
Framework ทุกตัวอย่างมี `main()` ของตัวเอง และสามารถศึกษา/Build แยกกันได้

> **หมายเหตุเรื่อง MCU:** Microchip ระบุสถานะ ATmega328P ว่า
> "Not Recommended for new designs" แต่ MCU รุ่นนี้ยังเหมาะกับ Repository นี้
> เพราะเป้าหมายคือเปรียบเทียบ Native C กับ Arduino Uno Series เดิมโดยตรง
> สำหรับผลิตภัณฑ์ใหม่ควรประเมิน AVR Family รุ่นปัจจุบันแยกต่างหาก

## Roadmap การเรียนรู้

| EP | หัวข้อ | ระดับ Arduino | Register สำคัญ | ตัวอย่าง |
| --- | --- | --- | --- | --- |
| 01 | [GPIO](EP01_GPIO/README.md) | `pinMode`, `digitalRead`, `digitalWrite` | `DDRx`, `PORTx`, `PINx` | ปุ่มควบคุม LED |
| 02 | [UART](EP02_UART/README.md) | `Serial` | `UBRR0`, `UCSR0x`, `UDR0` | Console รับคำสั่งข้อความ |
| 03 | [Timer](EP03_TIMER/README.md) | `millis` | `TCCR1x`, `TCNT1`, `OCR1A`, `TIFR1` | LED กระพริบแบบ Non-blocking |
| 04 | [Interrupt](EP04_INTERRUPT/README.md) | `attachInterrupt` | `EICRA`, `EIMSK`, `EIFR`, ISR | Event จากปุ่ม |
| 05 | [PWM](EP05_PWM/README.md) | `analogWrite` | `TCCR1x`, `OCR1A` | Fade LED ที่ D9 |
| 06 | [ADC](EP06_ADC/README.md) | `analogRead` | `ADMUX`, `ADCSRA`, `ADC` | Potentiometer ควบคุม PWM |
| 07 | [SPI](EP07_SPI/README.md) | `SPI`, `SD` | `SPCR`, `SPSR`, `SPDR` | Raw SD Card Handshake |
| 08 | [I2C/TWI](EP08_I2C/README.md) | `Wire` | `TWBR`, `TWSR`, `TWCR`, `TWDR` | I2C Address Scanner |

EP07 จงใจหยุดก่อนชั้น FAT Filesystem เพื่อให้เห็น Native SPI Transaction
และ SD Command แรกอย่างชัดเจน รวมถึงแยกขอบเขตระหว่าง Bus Driver,
SD Protocol และ Filesystem

## เลือกเส้นทางการพัฒนา

### MPLAB X IDE + XC8 (แนะนำสำหรับเริ่มซีรีส์)

สร้าง MPLAB X Standalone Project แยกหนึ่ง Project ต่อหนึ่ง EP แล้วเพิ่มไฟล์
`src/main.c` ของตอนนั้น เลือก Device เป็น `ATmega328P` และ Compiler เป็น
XC8 ดูขั้นตอนและความแตกต่างระหว่าง ICSP Programmer กับ Uno USB Bootloader
ได้ใน [คู่มือติดตั้ง Toolchain](docs/toolchain-setup.md)

Repository นี้จงใจไม่เก็บ Metadata ของ MPLAB Project ที่สร้างอัตโนมัติ
เพื่อให้ซอร์สอ่านง่ายและนำไปใช้กับ Toolchain อื่นได้

Microchip รองรับ XC8 ผ่าน
[MPLAB for VS Code](https://www.microchip.com/en-us/tools-resources/develop/mplab-tools-vs-code)
ด้วย เหมาะสำหรับผู้เรียนที่คุ้นเคยกับ VS Code และต้องการ IDE ที่เบากว่า
โดยซอร์สแบบ Register-Level ใน Repository นี้ไม่ต้องเปลี่ยน

### avr-gcc + Make + avrdude

Build ทุก EP:

```sh
make all
```

บน Windows หาก GNU Make ใช้ชื่อ `mingw32-make`:

```powershell
mingw32-make all
```

Build หรือ Flash เฉพาะ EP:

```sh
make build-selected EP=EP01_GPIO
make flash EP=EP01_GPIO PORT=COM3
```

เปลี่ยน `PORT` ให้ตรงกับระบบของคุณ และอ่าน
[คู่มือการ Flash](docs/flashing-guide.md) ก่อน Program ฮาร์ดแวร์

## โครงสร้าง Repository

```text
avr-native-c-embedded-roadmap/
|-- README.md
|-- LICENSE
|-- Makefile
|-- docs/
|   |-- toolchain-setup.md
|   |-- arduino-uno-pin-mapping.md
|   |-- register-basics.md
|   `-- flashing-guide.md
|-- EP01_GPIO/
|   |-- README.md
|   `-- src/main.c
`-- ... EP02_UART through EP08_I2C
```

## ข้อตกลงเกี่ยวกับฮาร์ดแวร์

- Arduino Uno R3 ใช้ ATmega328P ที่ความถี่ 16 MHz
- ตัวอย่างไม่เปลี่ยน Clock Fuse หรือ Bootloader Fuse เดิมของ Uno
- D0/D1 ใช้ร่วมกับวงจร USB-to-Serial บนบอร์ด
- อุปกรณ์ I2C ต้องมี Pull-up ที่ SDA และ SCL โดย Module หลายรุ่นมีมาให้แล้ว
- MicroSD Card เปล่าใช้ไฟ 3.3 V ต้องใช้ Module หรือ Level Conversion ที่
  ปลอดภัยกับ Logic 5 V ของ Uno

ถอดแหล่งจ่ายไฟก่อนเปลี่ยนการต่อวงจร และเชื่อม GND ของอุปกรณ์ที่สื่อสารกัน
เข้าด้วยกันเสมอ

## เอกสารอ้างอิง

- [หน้าผลิตภัณฑ์และ Datasheet ของ ATmega328P](https://www.microchip.com/en-us/product/atmega328p)
- [MPLAB X IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
- [MPLAB XC8 Compiler](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-compilers/xc8)
- [เอกสาร Arduino Uno R3](https://docs.arduino.cc/hardware/uno-rev3/)

## สิทธิ์ใช้งาน (License)

ใช้ [MIT License](LICENSE)
