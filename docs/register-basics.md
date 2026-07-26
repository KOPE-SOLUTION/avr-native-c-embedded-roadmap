# พื้นฐาน Register-Level

> เอกสารนี้เป็น Reference ที่หลาย EP ใช้ร่วมกัน หากเพิ่งเริ่มต้น ให้ศึกษา
> [EP01 GPIO](../EP01_GPIO/README.md) ก่อน ซึ่งอธิบายที่มาของ Register และโค้ดทีละบรรทัด

## Set, Clear, Toggle และตรวจสอบ bit

```c
/* Set bit */
PORTB |= (1U << PORTB5);

/* Clear bit */
PORTB &= ~(1U << PORTB5);

/* Toggle bit */
PORTB ^= (1U << PORTB5);

/* ตรวจสอบ bit */
if ((PINB & (1U << PINB5)) != 0U){
    /* ขาอยู่ในสถานะ HIGH */
}
```

Read-modify-write Operator ช่วยรักษาค่าของ bit อื่นใน Register เดียวกัน

## GPIO มี Register หลักสามหน้าที่

สำหรับ Port ของ Classic AVR:

- `DDRx`: กำหนดทิศทาง (`1` คือ Output, `0` คือ Input)
- `PORTx`: กำหนดค่า Output หรือเปิด Internal Pull-up ของ Input
- `PINx`: อ่านสถานะไฟฟ้าที่ขาจริง

ตัวอย่าง Input ที่เปิด Internal Pull-up:

```c
DDRD &= ~(1U << DDD2);
PORTD |= (1U << PORTD2);
```

เมื่อต่อปุ่มจาก D2 ลง GND การปล่อยปุ่มจะอ่านได้ HIGH และการกดจะอ่านได้ LOW

## การกำหนดค่าทั้ง Register กับการ OR

```c
TCCR1A = (1U << WGM10);
```

คำสั่งนี้กำหนดค่าทั้ง Register ให้อยู่ในสถานะที่ทราบแน่นอน

```c
TCCR1A |= (1U << COM1A1);
```

คำสั่งนี้รักษา bit อื่นไว้ ระหว่างการ Initialize Peripheral การกำหนดค่า
ทั้ง Register มักอ่านง่ายกว่าเมื่อเราเลือกทุก bit ที่เกี่ยวข้องไว้อย่างชัดเจน

## Flag แบบ Write-one-to-clear

Status Flag บางตัวของ AVR ต้องเขียน `1` เพื่อล้างค่า ไม่ใช่เขียน `0`
ตัวอย่าง:

```c
TIFR1 = (1U << OCF1A);
```

ตรวจรายละเอียด Register ใน Datasheet ก่อนเปลี่ยน Flag เสมอ

## `volatile` และ Interrupt

Compiler Header ประกาศ Device Register เป็น `volatile` ไว้แล้ว ตัวแปรที่ใช้
ร่วมกันระหว่าง `main()` และ ISR ต้องประกาศ `volatile` เช่นกัน:

```c
static volatile uint8_t event_pending;
```

`volatile` ป้องกัน Optimization ที่ไม่เหมาะสม แต่ไม่ได้ทำให้การเข้าถึงข้อมูล
หลาย byte เป็น Atomic โดยอัตโนมัติ AVR 8-bit อาจต้องปิด Interrupt ชั่วคราว
เมื่อ Main Code และ ISR ใช้ค่า 16-bit หรือ 32-bit ร่วมกัน

## ลำดับการอ่าน Datasheet

1. ภาพรวม Peripheral และ Block Diagram
2. Pin Multiplexing
3. Initialization Sequence
4. Register Summary
5. ตาราง bit ของแต่ละ Register
6. Timing Formula และข้อจำกัดทางไฟฟ้า

Comment ใน Repository อธิบายเจตนาของโค้ด แต่ Datasheet ยังคงเป็น
แหล่งข้อมูลหลักที่ต้องใช้อ้างอิง
