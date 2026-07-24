# การติดตั้งและเลือกใช้ Toolchain

ซอร์สใน Repository นี้ไม่ผูกกับ IDE เดียว และรองรับสองเส้นทางหลัก

## เส้นทาง A - MPLAB X IDE + MPLAB XC8

เส้นทางนี้แนะนำสำหรับเริ่มซีรีส์ เพราะ IDE ช่วยจัดการ Device, Compiler,
Build Configuration และเครื่องมือ Debug/Program ของ Microchip

### การติดตั้ง

1. ติดตั้ง [MPLAB X IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
2. ติดตั้ง [MPLAB XC8](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-compilers/xc8)
   แยกต่างหาก โดย XC8 รองรับ MCU ตระกูล AVR 8-bit
3. ใน MPLAB X ตรวจสอบว่า XC8 ปรากฏที่ **Tools > Options > Embedded >
   Build Tools**

### การสร้าง Project ของแต่ละ EP

1. เลือก **File > New Project > Microchip Embedded > Standalone Project**
2. เลือก Device เป็น **ATmega328P**
3. เลือก Hardware Tool หรือ **Simulator**
4. เลือก Compiler **XC8** ที่ติดตั้งไว้
5. ตั้งชื่อ Project ตาม EP เช่น `EP01_GPIO`
6. เพิ่ม `EP01_GPIO/src/main.c` เข้า **Source Files**
7. Build Project

ตัวอย่างใช้ `<avr/io.h>` ซึ่ง XC8 รองรับเมื่อ Target เป็น AVR Header นี้เชื่อม
ชื่อ Register และ bit กับ MCU ที่เลือก MPLAB Project มักใช้ `<xc.h>` เป็น
Top-level Header โดย `<xc.h>` จะ Include Device Definition ของ AVR อีกที
การใช้ `<avr/io.h>` ช่วยให้ซอร์สเดียวกัน Build ด้วย avr-gcc ได้ด้วย

### Program และ Debug จาก MPLAB X

ช่อง USB ของ Uno ติดต่อ Bootloader ผ่าน USB-to-Serial ไม่ใช่ Native MPLAB
Debug Connection หากต้องการใช้ **Make and Program Device** ใน MPLAB X
ต้องต่อ Programmer/Debugger ที่ MPLAB X ระบุว่ารองรับ ATmega328P เข้ากับ
ICSP Header ของ Uno

บทเรียนเหล่านี้ไม่เปลี่ยน Clock, Reset หรือ Boot Fuse การ Program ผ่าน ISP
อาจลบ Uno Bootloader ได้ ขึ้นอยู่กับ Operation และ Tool Setting

หากมีเพียง Uno และสาย USB ให้ใช้เส้นทาง B เพื่อ Flash ผ่าน Bootloader เดิม

## เส้นทาง A2 - MPLAB for VS Code + MPLAB XC8

Microchip มี
[MPLAB for VS Code](https://www.microchip.com/en-us/tools-resources/develop/mplab-tools-vs-code)
ซึ่งรองรับ MPLAB Project, XC8, การ Program และ Basic Debugging ใน Editor
ที่เบากว่า เหมาะเมื่อคุ้นเคยกับ VS Code หรือต้องการ Workflow ด้าน Source
Control ของ VS Code

เส้นทางนี้ยังเป็น Native Toolchain จากผู้ผลิต Compiler และซอร์สแบบ
Register-Level เหมือนเดิม เปลี่ยนเฉพาะ Development Environment สำหรับ
วิดีโอชุดแรก MPLAB X ยังนำเสนอเป็น GUI เดียวกันได้ง่ายกว่า

## เส้นทาง B - avr-gcc + Make + avrdude

ติดตั้งหรือเพิ่ม Tool เหล่านี้ใน `PATH`:

- `avr-gcc`
- `avr-objcopy`
- `avr-size`
- GNU Make
- `avrdude`

Build ทุก EP:

```sh
make all
```

บน Windows โปรแกรม Make อาจใช้ชื่อ `mingw32-make`:

```powershell
mingw32-make all
```

Build เฉพาะ EP:

```sh
make build-selected EP=EP03_TIMER
```

ค่าเริ่มต้นใน Makefile:

- MCU: `atmega328p`
- CPU Clock: `16000000UL`
- Optimization: `-Os`
- Warning: `-Wall -Wextra -Werror`

สามารถระบุ Path ของ Tool โดยไม่แก้ Makefile:

```powershell
mingw32-make all AVR_CC="C:\path\to\avr-gcc.exe" AVR_OBJCOPY="C:\path\to\avr-objcopy.exe"
```

## คำว่า Native ใน Repository นี้หมายถึงอะไร

- ไม่ใช้ Arduino Core Startup
- ไม่มี `setup()` หรือ `loop()`
- ไม่ใช้ Arduino API หรือ C++ `String`
- ไม่ใช้ Arduino Peripheral Library
- ตั้งค่า Hardware จาก ATmega328P Datasheet โดยตรง
- ใช้ Compiler Header มาตรฐานได้ เพราะ Header ทำหน้าที่ประกาศชื่อ Register
  และ Interrupt Vector

C Runtime ยังคงเตรียม Memory และเรียก `main()` คำว่า Native ในที่นี้
ไม่ได้หมายถึงการเขียน Reset Vector และ Startup Assembly ใหม่ทั้งหมด
