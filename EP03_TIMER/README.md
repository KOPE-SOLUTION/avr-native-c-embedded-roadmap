# EP03 - พื้นฐาน Timer

สร้าง Timebase 1 ms ด้วย Timer1 และทำให้ LED บนบอร์ดกระพริบโดยไม่ใช้
`delay()`, `millis()` หรือ Interrupt

## ทำไมเริ่มจาก Polling

EP03 แยกการเรียน Timer Peripheral ออกจาก Interrupt โดย CPU จะ Poll
Compare Flag ขณะที่ Hardware Timer นับเวลาอย่างอิสระ จากนั้น EP04 จึงเพิ่ม
แนวคิด Event แบบ Interrupt

## การตั้งค่า Timer1

- Mode: CTC (Clear Timer on Compare Match)
- CPU Clock: 16 MHz
- Prescaler: 64
- Timer Tick Frequency: 250 kHz
- ค่า `OCR1A`: 249
- Compare Period: 1 ms

```text
OCR1A = F_CPU / prescaler / target_frequency - 1
      = 16,000,000 / 64 / 1,000 - 1
      = 249
```

## Register สำคัญ

| Register | หน้าที่ |
| --- | --- |
| `TCCR1A/B` | กำหนด Waveform Mode และ Clock Prescaler |
| `TCNT1` | ค่า Counter ปัจจุบัน |
| `OCR1A` | ค่า Compare |
| `TIFR1` | Compare-match Flag (`OCF1A`) |

`OCF1A` เป็น Flag แบบ Write-one-to-clear ซอร์สจึงล้าง Flag ด้วยการเขียน 1

## ผลลัพธ์ที่คาดหวัง

[src/main.c](src/main.c) สะสม Compare Event จำนวน 1,000 ครั้งแล้ว Toggle D13
หนึ่งครั้งต่อวินาที Main Loop ไม่ Busy-wait เป็นเวลาหนึ่งวินาที จึงสามารถเพิ่ม
งาน Polling อื่นใน Loop ได้

```sh
make build-selected EP=EP03_TIMER
```

ตอนถัดไป: [EP04 - Interrupt](../EP04_INTERRUPT/README.md)
