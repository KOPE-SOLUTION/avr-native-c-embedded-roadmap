# EP06 - การวัดค่าด้วย ADC

อ่าน Potentiometer ที่ ADC0 แปลงผลลัพธ์ 10-bit เป็น PWM 8-bit ของ Timer1
และรายงานค่าผ่าน USART0 โดยไม่ใช้ `analogRead()`, `map()` หรือ Arduino
`Serial`

## การต่อวงจร

| ขา Potentiometer | Uno |
| --- | --- |
| ขานอกด้านหนึ่ง | 5V |
| ขานอกอีกด้าน | GND |
| ขากลาง/Wiper | A0 / ADC0 |

ต่อ LED ภายนอกพร้อมตัวต้านทานที่ D9 ตาม EP05 และรักษาแรงดัน Analog Input
ให้อยู่ระหว่าง GND กับ Reference Voltage ที่เลือก

## การตั้งค่า ADC

- Reference: AVCC
- Channel: ADC0 / A0
- Resolution: 10 bit (`0-1023`)
- ADC Prescaler: 128
- ADC Clock: `16 MHz / 128 = 125 kHz`
- Conversion Mode: Single Conversion แบบ Polling

| Register | หน้าที่ |
| --- | --- |
| `ADMUX` | เลือก Reference และ Channel |
| `ADCSRA` | Enable, Start, Status และ Prescaler |
| `ADC` | ผลลัพธ์ 10-bit ที่รวมแล้ว |
| `DIDR0` | ปิด Digital Input Buffer ของ ADC0 |

การแปลงค่าใช้การ Shift bit:

```c
pwm_value = (uint8_t)(adc_value >> 2U); /* 10 bit เป็น 8 bit */
```

## วิธีทดสอบ

เปิด Serial Terminal ที่ 9600 baud แล้วหมุน Potentiometer:

```text
ADC: 0 | PWM: 0
ADC: 512 | PWM: 128
ADC: 1023 | PWM: 255
```

```sh
make build-selected EP=EP06_ADC
```

ซอร์ส: [src/main.c](src/main.c)

ตอนถัดไป: [EP07 - SPI](../EP07_SPI/README.md)
