# คู่มือการ Flash Firmware

ซีรีส์นี้สาธิตการ Flash Arduino Uno จาก Ubuntu บน WSL 2 ด้วย `avrdude`
ผ่าน USB Bootloader เดิมของบอร์ด นี่คือ Workflow ที่ผู้จัดทำเลือกใช้ ไม่ใช่
ข้อกำหนดของ Native AVR C หากยังไม่ได้ติดตั้ง Toolchain
หรือยังไม่เคยเชื่อม USB เข้า WSL ให้อ่าน
[คู่มือเริ่มต้นด้วย WSL](wsl-setup.md) ก่อน

## Uno USB Bootloader ผ่าน WSL

### 1. Attach บอร์ดให้ WSL

หลังเสียบบอร์ด ให้หา BUSID ใน Windows PowerShell:

```powershell
usbipd list
```

หากอุปกรณ์ถูก Share แล้ว ให้ Attach ด้วย:

```powershell
usbipd attach --wsl --busid <BUSID>
```

การ `bind` ครั้งแรกต้องทำใน Administrator PowerShell ตามขั้นตอนใน
[คู่มือ WSL](wsl-setup.md#6-เชื่อม-arduino-uno-จาก-windows-เข้า-wsl)

### 2. ตรวจ Serial Port ใน Ubuntu

```sh
lsusb
ls -l /dev/ttyUSB* /dev/ttyACM* 2>/dev/null
```

Port ที่พบบ่อยคือ:

- Uno Clone ที่ใช้ CH340/CH341: `/dev/ttyUSB0`
- Uno R3 ที่ใช้ ATmega16U2: `/dev/ttyACM0`

ใช้ชื่อที่ระบบแสดงจริง เพราะเลขท้ายอาจเปลี่ยนเมื่อมี Serial Device หลายตัว

### 3. Build และ Flash

ตัวอย่างสำหรับ CH340:

```sh
make flash EP=EP01_GPIO PORT=/dev/ttyUSB0
```

ตัวอย่างสำหรับ Uno R3 ที่ปรากฏเป็น ACM Device:

```sh
make flash EP=EP01_GPIO PORT=/dev/ttyACM0
```

Target `flash` จะ Build ไฟล์ `.hex` ของ EP ที่เลือกก่อนเรียก `avrdude`
โดยอัตโนมัติ

ค่าเริ่มต้นของ Makefile:

```text
MCU          = atmega328p
PROGRAMMER   = arduino
UPLOAD_BAUD  = 115200
```

Uno R3 Bootloader โดยทั่วไปใช้ค่าเหล่านี้ บอร์ด Clone หรือบอร์ดที่ใช้
Bootloader รุ่นเก่าอาจต้องเปลี่ยน Baud Rate:

```sh
make flash EP=EP01_GPIO PORT=/dev/ttyUSB0 UPLOAD_BAUD=57600
```

ก่อน Upload:

1. ปิด `picocom`, Arduino Serial Monitor และโปรแกรมที่กำลังจับ Port
2. ถอด Hardware ที่ D0/RX และ D1/TX หากรบกวนการสื่อสาร
3. ยืนยันว่า Board ใช้ ATmega328P และ Clock 16 MHz
4. จ่ายไฟให้ Board ตลอดขั้นตอน
5. ตรวจว่า Board ยัง Attach อยู่กับ WSL หลังถอดสายหรือ Restart

ไม่ควรใช้ `sudo make flash` เพื่อหลบปัญหา Permission ให้เพิ่มผู้ใช้เข้า
กลุ่ม `dialout` และเปิด WSL Session ใหม่ตามคู่มือแทน

## Workflow แบบผสม: Build ใน WSL และ Flash บน Windows

นี่คือรูปแบบเดียวกับการทดลอง Arduino Uno ที่เป็นจุดเริ่มต้นของ Repository
และใช้เป็น Fallback ได้เมื่อ `usbipd` หรือ USB Passthrough มีปัญหา

Build ใน Ubuntu:

```sh
make build-selected EP=EP01_GPIO
cp build/EP01_GPIO.hex /mnt/c/Users/<WindowsUser>/Desktop/
```

จากนั้นเปิด Windows PowerShell และใช้ `avrdude` ที่ติดตั้งไว้หรือ Binary
ที่มากับ Arduino Toolchain:

```powershell
avrdude -p atmega328p -c arduino -P COM3 -b 115200 -D `
  -U flash:w:C:\Users\<WindowsUser>\Desktop\EP01_GPIO.hex:i
```

หากเรียก Binary ที่มากับ Arduino Package โดยใช้ Full Path อาจต้องระบุ
`-C` ให้ชี้ไปยัง `avrdude.conf` ของ Version เดียวกัน ตรวจตำแหน่งจริงจาก
Arduino Data Directory บนเครื่อง ไม่ควร Copy Path หรือ Version จากเครื่อง
ของผู้จัดทำโดยตรง

วิธีนี้ยัง Compile ด้วย WSL เหมือน Workflow หลัก แต่ให้ Windows เป็นผู้
ถือ COM Port จึงไม่ต้อง Attach USB Device เข้า WSL

## Flash จาก Windows โดยตรง (ทางเลือก)

หากติดตั้ง `avr-gcc`, GNU Make และ `avrdude` บน Windows ครบแล้ว สามารถใช้
COM Port ได้ เช่น:

```powershell
make flash EP=EP01_GPIO PORT=COM3
```

อย่างไรก็ตาม เอกสารหลักของ Repository ใช้ WSL เพื่อหลีกเลี่ยงความต่างของ
Package, `PATH` และชื่อคำสั่ง Make ระหว่างชุด Toolchain บน Windows

USB Device หนึ่งตัวใช้พร้อมกันระหว่าง Windows และ WSL ไม่ได้ หาก Uno ยัง
Attach อยู่กับ WSL ให้คืนอุปกรณ์ก่อน:

```powershell
usbipd detach --busid <BUSID>
```

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

เปิด Serial Terminal ใน Ubuntu:

```sh
picocom --echo --omap crcrlf -b 9600 /dev/ttyUSB0
```

`--omap crcrlf` แปลง CR จากปุ่ม Enter เป็น CR+LF ก่อนส่งไปยังบอร์ด
ส่วน `--imap` แปลงข้อมูลในทิศทางจากบอร์ดมายัง Terminal จึงไม่ใช้สำหรับ
กำหนด Line Ending ของคำสั่งที่พิมพ์

เปลี่ยน Port เป็น `/dev/ttyACM0` หากระบบแสดงชื่อนั้น ออกจาก `picocom`
ด้วย `Ctrl+A` แล้ว `Ctrl+X` และปิดก่อน Flash Firmware ครั้งต่อไป

## เมื่อ Flash ไม่ผ่าน

- `Permission denied`: ตรวจกลุ่ม `dialout` แล้วเปิด WSL Session ใหม่
- ไม่พบ Port: Attach ผ่าน `usbipd` ใหม่ แล้วตรวจ `lsusb` กับ `dmesg`
- Port ถูกใช้งาน: ปิด Serial Terminal หรือ Arduino IDE
- `stk500_recv()` Timeout: ตรวจ Port, Baud Rate, สาย USB และวงจร D0/D1
- หลัง Reboot/ถอดสาย: `usbipd bind` ยังอยู่ แต่ต้อง `attach` ใหม่

ดูขั้นตอนวิเคราะห์โดยละเอียดในหัวข้อ
[แก้ปัญหาที่พบบ่อย](wsl-setup.md#11-แก้ปัญหาที่พบบ่อย)
