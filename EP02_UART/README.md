# EP02 - การสื่อสาร UART Serial

ตั้งค่า USART0 โดยตรงและสร้าง Command Console ขนาดเล็ก โดยไม่ใช้ Arduino
`Serial` หรือ C++ `String`

## การตั้งค่า

```text
Baud:      9600
Data bits: 8
Parity:    none
Stop bits: 1
Mode:      asynchronous, normal speed
```

เมื่อ CPU Clock เท่ากับ 16 MHz ค่า `UBRR0 = 103` จะให้อัตรา Baud ประมาณ
9600 baud

## Register สำคัญ

| Register | หน้าที่ |
| --- | --- |
| `UBRR0H/L` | กำหนด Baud-rate Divider |
| `UCSR0A` | Status Flag ของ TX/RX |
| `UCSR0B` | เปิดใช้งาน Transmitter และ Receiver |
| `UCSR0C` | กำหนด Frame Format แบบ 8N1 |
| `UDR0` | Register สำหรับส่งและรับข้อมูลหนึ่ง byte |

## ฮาร์ดแวร์

ใช้การเชื่อมต่อ USB ของ Uno โดย USART0 ใช้ D0/RX และ D1/TX ซึ่งเชื่อมกับ
วงจร USB-to-Serial บนบอร์ดอยู่แล้ว

## วิธีทดสอบ

1. Flash [src/main.c](src/main.c)
2. เปิด Serial Terminal ที่ 9600 baud
3. เลือก Line Ending ให้ส่ง LF (`\n`) หรือ CRLF
4. ส่งคำสั่งต่อไปนี้:

```text
LED ON
LED OFF
STATUS?
```

Parser ใช้ Buffer ขนาดคงที่ 16 byte ปฏิเสธข้อมูลที่ยาวเกิน และไม่ใช้
Dynamic Memory Allocation

คำสั่ง Build:

```sh
make build-selected EP=EP02_UART
```

## สิ่งที่เรียนรู้

- การคำนวณ Baud Rate
- การ Poll Flag `UDRE0` และ `RXC0`
- Serial I/O แบบหนึ่ง byte ต่อครั้ง
- การ Parse คำสั่งด้วย Buffer ขนาดคงที่
- เหตุผลที่ UART เป็น Debug Channel สำคัญของ Firmware

ตอนถัดไป: [EP03 - Timer](../EP03_TIMER/README.md)
