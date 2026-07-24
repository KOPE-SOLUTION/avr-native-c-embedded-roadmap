# EP08 - I2C/TWI Address Scanner

ใช้ Two-Wire Interface (TWI) Register ของ ATmega328P เพื่อ Scan I2C Bus
แบบ 7-bit โดยไม่ใช้ Arduino `Wire`

## การต่อวงจร

| อุปกรณ์ I2C | Uno / ATmega328P |
| --- | --- |
| SDA | A4 / PC4 / SDA |
| SCL | A5 / PC5 / SCL |
| GND | GND |
| VCC | ใช้แรงดันตามข้อกำหนดของอุปกรณ์ |

SDA และ SCL เป็น Open-drain Bus Line และต้องมี Pull-up Resistor โดย OLED
และ Sensor Module หลายรุ่นมี Pull-up มาให้แล้ว ตรวจให้แน่ใจว่า Pull-up
Voltage ปลอดภัยกับอุปกรณ์ทุกตัวบน Bus

## การตั้งค่า TWI

- Controller/Master Transmitter สำหรับ Probe Address
- Bus Rate: 100 kHz เมื่อ `F_CPU = 16 MHz`
- Prescaler: 1
- `TWBR = 72`

```text
SCL = F_CPU / (16 + 2 * TWBR * prescaler)
```

| Register | หน้าที่ |
| --- | --- |
| `TWBR` | Bit-rate Divider |
| `TWSR` | Prescaler และ Bus Status Code |
| `TWCR` | ควบคุม START, STOP, Enable และ Completion |
| `TWDR` | Address/Data byte |

## วิธีทดสอบ

1. ปิดไฟแล้วเชื่อมต่อ I2C Module
2. Flash [src/main.c](src/main.c)
3. เปิด Serial Terminal ที่ 9600 baud

ตัวอย่าง Display ที่ Address `0x3C`:

```text
I2C scan
Found 0x3C
Devices: 1
```

Scanner ทำงานซ้ำทุกห้าวินาทีและ Probe Address `0x08-0x77` โดยไม่แตะช่วง
Reserved Address

```sh
make build-selected EP=EP08_I2C
```

EP นี้จบ Native Companion ชุด EP01-EP08 ขั้นต่อไปที่เหมาะสมคือการสร้าง
Peripheral Driver ขนาดเล็กที่นำกลับมาใช้ซ้ำและทดสอบกับ AVR หลายรุ่น
