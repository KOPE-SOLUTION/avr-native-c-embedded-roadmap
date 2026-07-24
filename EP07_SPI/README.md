# EP07 - การสื่อสาร SPI และ SD Card Handshake

ตั้งค่า SPI Peripheral ของ ATmega328P เป็น Controller/Master และส่ง SD Card
Command แรกโดยไม่ใช้ Arduino `SPI` หรือ `SD` Library

## ทำไมยังไม่เขียนไฟล์ในตอนนี้

ตัวอย่าง Arduino `SD` รวมการทำงานหลายชั้น:

```text
SPI electrical bus -> SD command protocol -> block device -> FAT filesystem -> file API
```

EP นี้แยกศึกษาเฉพาะสองชั้นแรก โดยส่ง `CMD0` และตรวจว่า SD Card เข้าสู่
Idle State (`R1 = 0x01`) การเขียน `test.txt` แบบ Native ต้องมี SD Block
Driver และ FAT Implementation ซึ่งควรแยกเป็น Storage Module ภายหลัง ไม่ควร
ซ่อนรายละเอียดเหล่านี้ไว้ในบทเรียนพื้นฐาน SPI

## การต่อวงจร

| SD Module | Uno / ATmega328P |
| --- | --- |
| CS | D10 / PB2 |
| MOSI | D11 / PB3 |
| MISO | D12 / PB4 |
| SCK | D13 / PB5 |
| GND | GND |

**คำเตือนเรื่องแรงดัน:** MicroSD Card เปล่าใช้สัญญาณ 3.3 V ห้ามต่อกับ
Output 5 V โดยตรง ใช้ SD Module/Level Conversion ที่ปลอดภัยกับ Uno 5 V
และตรวจข้อกำหนดแหล่งจ่ายของ Module

## การ Initialize SPI

- Controller/Master Mode
- Mode 0 (`CPOL=0`, `CPHA=0`)
- MSB First
- `f_CPU / 128 = 125 kHz` ระหว่าง Initialize Card

| Register | หน้าที่ |
| --- | --- |
| `SPCR` | Enable SPI, Controller Mode และ Clock Divider |
| `SPSR` | Transfer-complete และ Double-speed Status |
| `SPDR` | ข้อมูลหนึ่ง byte ที่ส่ง/รับ |

## วิธีทดสอบ

1. ปิดไฟ ใส่ Card และตรวจการต่อวงจร
2. Flash [src/main.c](src/main.c)
3. เปิด Serial Terminal ที่ 9600 baud

เมื่อสำเร็จ:

```text
SPI SD CMD0 test
SD entered idle state (R1=0x01)
```

เมื่อไม่สำเร็จ โปรแกรมจะแสดง R1 byte สุดท้าย ตรวจแหล่งจ่าย, Level Shifting,
ขา CS และการใส่ Card ก่อนแก้ Software

```sh
make build-selected EP=EP07_SPI
```

ตอนถัดไป: [EP08 - I2C/TWI](../EP08_I2C/README.md)
