# คู่มือการ Flash Firmware

## Uno USB Bootloader กับ avrdude

เชื่อมต่อ Uno ด้วย USB แล้วตรวจสอบชื่อ Serial Port

Build และ Flash หนึ่ง EP:

```sh
make flash EP=EP01_GPIO PORT=COM3
```

บน Windows:

```powershell
mingw32-make flash EP=EP01_GPIO PORT=COM3
```

ค่าเริ่มต้นของ Makefile:

```text
MCU          = atmega328p
PROGRAMMER   = arduino
UPLOAD_BAUD  = 115200
```

Uno R3 ของแท้โดยทั่วไปใช้ค่าเหล่านี้ Board Clone หรือบอร์ดที่ใช้ Bootloader
ต่างรุ่นอาจต้องเปลี่ยน Baud Rate:

```sh
make flash EP=EP01_GPIO PORT=COM3 UPLOAD_BAUD=57600
```

ก่อน Upload:

1. ปิด Serial Monitor และโปรแกรมที่กำลังจับ COM Port
2. ถอด Hardware ที่ D0/RX และ D1/TX หากรบกวนการสื่อสาร
3. ยืนยันว่า Board ใช้ ATmega328P และ Clock 16 MHz
4. จ่ายไฟให้ Board ตลอดขั้นตอน

## การใช้ Hardware Programmer ผ่าน ICSP

MPLAB X สามารถ Program ผ่าน Hardware Programmer ที่รองรับและเชื่อมต่อกับ
Uno ICSP Header สัญญาณหกขาคือ MISO, VCC, SCK, MOSI, RESET และ GND

ตรวจสอบก่อน Program:

- Tool รองรับ ATmega328P ใน MPLAB X/Device Pack Version ที่ใช้อยู่
- Target Voltage และ Tool Power Setting ถูกต้อง
- ทิศทาง ICSP ถูกต้อง
- ไม่มีอุปกรณ์ที่ขา SPI ขับ Bus สวนทางกัน
- ปิดการเปลี่ยน Fuse เว้นแต่มีแผนกู้คืนที่ชัดเจน

การ Chip Erase/Program ผ่าน ISP อาจลบ Uno Bootloader สามารถเขียนกลับด้วย
ISP Programmer ได้ แต่ USB Bootloader Upload จะใช้ไม่ได้จนกว่าจะกู้คืน

## การตรวจสอบ Serial Output

EP02, EP06, EP07 และ EP08 ใช้ USART0 ด้วยค่า:

```text
9600 baud, 8 data bits, no parity, 1 stop bit
```

เปิด Serial Terminal ที่ 9600 baud หลัง Upload เสร็จ
