# การเทียบขา Arduino Uno กับ ATmega328P

> ตารางนี้เป็น Reference รวมสำหรับทั้งซีรีส์ สำหรับ D13/PB5 และ D2/PD2
> พร้อมวิธีอ่านชื่อขาแบบละเอียด ให้เริ่มที่ [EP01 GPIO](../EP01_GPIO/README.md)

บอร์ดอ้างอิง: Arduino Uno R3, ATmega328P, 16 MHz

| ขา Uno | สัญญาณ AVR | Port bit | การใช้งานใน Repository |
| --- | --- | --- | --- |
| D0 | RXD | PD0 | UART Receive |
| D1 | TXD | PD1 | UART Transmit |
| D2 | INT0 | PD2 | ปุ่ม / External Interrupt |
| D3 | OC2B / INT1 | PD3 | - |
| D4 | - | PD4 | - |
| D5 | OC0B | PD5 | - |
| D6 | OC0A | PD6 | - |
| D7 | - | PD7 | - |
| D8 | ICP1 | PB0 | - |
| D9 | OC1A | PB1 | PWM Output |
| D10 | SS / OC1B | PB2 | SPI Chip Select |
| D11 | MOSI / OC2A | PB3 | SPI MOSI |
| D12 | MISO | PB4 | SPI MISO |
| D13 | SCK / LED บนบอร์ด | PB5 | LED / SPI Clock |
| A0 | ADC0 | PC0 | Potentiometer |
| A1 | ADC1 | PC1 | - |
| A2 | ADC2 | PC2 | - |
| A3 | ADC3 | PC3 | - |
| A4 | ADC4 / SDA | PC4 | I2C/TWI Data |
| A5 | ADC5 / SCL | PC5 | I2C/TWI Clock |

## กลุ่ม Register

- Port B: `DDRB`, `PORTB`, `PINB`
- Port C: `DDRC`, `PORTC`, `PINC`
- Port D: `DDRD`, `PORTD`, `PIND`

## ข้อควรระวังเกี่ยวกับขาที่ใช้ร่วมกัน

- D0/D1 เชื่อมกับวงจร USB-to-Serial ของ Uno ถอดอุปกรณ์ภายนอกจากขาเหล่านี้
  ระหว่าง Upload หากอุปกรณ์รบกวน Bootloader
- D13 เป็นทั้ง LED บนบอร์ดและ SPI SCK
- A4/A5 เป็น I2C SDA/SCL และต้องมี Pull-up บน Bus
- PWM ที่ D9 ใน EP05/EP06 ใช้ Timer1 การตั้งค่า Timer1 ใหม่จะเปลี่ยน Output นี้

ใช้ [เอกสาร Uno R3 อย่างเป็นทางการ](https://docs.arduino.cc/hardware/uno-rev3/)
และ ATmega328P Datasheet เป็นแหล่งอ้างอิงสุดท้าย
