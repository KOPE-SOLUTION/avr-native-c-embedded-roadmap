# EP05 - PWM ด้วย Timer1

สร้าง Hardware PWM ที่ Uno D9/OC1A โดยไม่ใช้ `analogWrite()`

## การต่อวงจร

ต่อ D9 ผ่านตัวต้านทาน 220 โอห์มถึง 1 กิโลโอห์มเข้าขา Anode ของ LED และต่อ
Cathode ลง GND ต้องใช้ LED ภายนอกเพราะ LED บนบอร์ด D13 ไม่ได้ต่อกับ D9

## การตั้งค่า Timer1

- Output: OC1A / PB1 / Uno D9
- Mode: 8-bit Fast PWM
- Output Mode: Non-inverting
- Prescaler: 64
- Frequency: `16 MHz / (64 * 256) ~= 976.6 Hz`
- Duty Register: `OCR1A` ช่วงค่า 0-255

| Register | bit สำคัญ |
| --- | --- |
| `DDRB` | `DDB1` กำหนด OC1A เป็น Output |
| `TCCR1A` | `COM1A1`, `WGM10` |
| `TCCR1B` | `WGM12`, `CS11`, `CS10` |
| `OCR1A` | ค่า Duty Cycle |

```text
duty (%) ~= OCR1A / 255 * 100
```

[src/main.c](src/main.c) ทำให้ LED ค่อย ๆ สว่างขึ้นและมืดลง `_delay_ms()`
ควบคุมเพียงความเร็วในการเปลี่ยน Duty ส่วน Timer1 ยังคงสร้างทุก PWM Edge
ด้วย Hardware

```sh
make build-selected EP=EP05_PWM
```

ตอนถัดไป: [EP06 - ADC](../EP06_ADC/README.md)
