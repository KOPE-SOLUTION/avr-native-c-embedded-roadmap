# EP04 - External Interrupt

ใช้ INT0 Peripheral เพื่อตอบสนองต่อ Falling Edge จากปุ่ม โดยไม่ใช้
`attachInterrupt()`

## การต่อวงจร

| อุปกรณ์ | การเชื่อมต่อ |
| --- | --- |
| Push button | D2/INT0 ลง GND |
| LED | ใช้ LED บนบอร์ดที่ D13 |

PD2 เปิด Internal Pull-up ดังนั้นการกดปุ่มจะทำให้เกิดการเปลี่ยนสถานะจาก
HIGH ไป LOW

## Register และ ISR

| Register/Feature | หน้าที่ |
| --- | --- |
| `EICRA` | เลือก Falling-edge Trigger ด้วย `ISC01:0` |
| `EIFR` | ล้าง `INTF0` Flag ที่อาจค้างอยู่ |
| `EIMSK` | เปิดใช้งาน `INT0` |
| `SREG.I` / `sei()` | เปิด Global Interrupt |
| `ISR(INT0_vect)` | Interrupt Service Routine |

ISR เปลี่ยนเพียงตัวแปรสถานะขนาด 8 bit ส่วน Main Loop นำสถานะไปควบคุม LED
ทำให้ ISR สั้นและทำงานเสร็จเร็ว

## คำเตือนเรื่อง Mechanical Switch

Push button จริงมีอาการ Contact Bounce และอาจสร้างหลาย Edge จากการกดหนึ่งครั้ง
ตัวอย่างพื้นฐานนี้จงใจให้เห็นพฤติกรรมดังกล่าว ในระบบจริงควรเพิ่ม Hardware
Debounce หรือ Timer-based Debounce และไม่ควรหน่วงเวลาภายใน ISR

```sh
make build-selected EP=EP04_INTERRUPT
```

ซอร์ส: [src/main.c](src/main.c)

ตอนถัดไป: [EP05 - PWM](../EP05_PWM/README.md)
