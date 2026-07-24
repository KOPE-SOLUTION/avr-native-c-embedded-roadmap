# การติดตั้งและเลือกใช้ Toolchain

Repository นี้รองรับทั้ง MPLAB XC8 และ avr-gcc โค้ดทุก EP เป็น
Register-Level C สำหรับ ATmega328P จึงควรเลือก Toolchain จาก Workflow,
Hardware และวิธี Debug ที่ต้องการ ไม่ควรตัดสินจากคำว่า Native เพียงอย่างเดียว

## คำตอบสั้นๆ: ควรเลือกอะไร

| ความต้องการ | ตัวเลือกที่เหมาะ |
| --- | --- |
| เพิ่งเริ่มและต้องการ GUI จากผู้ผลิต | MPLAB X + XC8 |
| ต้องการ MPLAB Ecosystem ใน Editor ที่คุ้นเคย | MPLAB for VS Code + XC8 |
| ต้องการทำตามซีรีส์นี้ทุกคำสั่ง | WSL 2 + Ubuntu + avr-gcc |
| ต้องการ Build แบบทำซ้ำได้ด้วย Makefile | WSL/Linux + avr-gcc |
| ต้องการ Program/Debug ผ่าน Microchip Hardware Tool | MPLAB + XC8 + Programmer/Debugger |
| ต้องการ Upload Uno ด้วยสาย USB เดิม | avrdude ผ่าน WSL หรือ Windows |

ถ้าถามเฉพาะว่า “ตัวไหนเริ่มใช้ง่ายกว่า” MPLAB X มีข้อได้เปรียบเพราะมี
Project Wizard, Device Selection, Compiler Setting และหน้าต่างสำหรับดู
Register/bit แต่ถ้าถามว่า “ซีรีส์นี้ใช้อะไร” คำตอบคือ WSL + avr-gcc
ด้วยเหตุผลด้าน Workflow ของผู้จัดทำ

## ทางเลือกที่ 1 - MPLAB X IDE + MPLAB XC8

นี่คือเส้นทาง GUI จาก Microchip และเหมาะสำหรับผู้ที่ต้องการทำงานอยู่ใน
Ecosystem ของผู้ผลิตตั้งแต่สร้าง Project ไปจนถึง Program และ Debug

### จุดเด่น

- เลือก Device, Compiler และ Build Configuration ผ่าน GUI
- เข้าถึงชื่อ Register, bit และ Datasheet จาก Project ได้สะดวก
- รองรับ Simulator และ Hardware Programmer/Debugger ของ Microchip
- XC8 รองรับทั้ง PIC 8-bit และ AVR 8-bit
- ไม่ต้องเขียน Makefile เพื่อเริ่ม Project แรก

### การติดตั้ง

1. ติดตั้ง [MPLAB X IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
2. ติดตั้ง [MPLAB XC8](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers/xc8)
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

ตัวอย่างใช้ `<avr/io.h>` ซึ่ง XC8 รองรับเมื่อ Target เป็น AVR Header นี้
ประกาศชื่อ Register และ bit ให้ตรงกับ MCU ที่เลือก การใช้ Header เดียวกัน
ช่วยให้ซอร์สเดียวกัน Build ด้วย avr-gcc ได้ด้วย

Repository ไม่เก็บ Metadata ของ MPLAB Project ที่ IDE สร้างอัตโนมัติ
ผู้ใช้ MPLAB จึงสร้าง Project ในเครื่องแล้วอ้างอิง `src/main.c` ของแต่ละ EP

### ข้อจำกัดเมื่อบอร์ดอ้างอิงคือ Arduino Uno

ช่อง USB ของ Uno ใช้ USB-to-Serial ติดต่อ Bootloader ไม่ใช่ Native MPLAB
Debug Connection หากต้องการใช้คำสั่ง Program/Debug ของ MPLAB แบบเต็มรูปแบบ
ต้องต่อ Programmer/Debugger ที่รองรับ ATmega328P เข้ากับ ICSP Header

หากมีเพียง Uno และสาย USB ยังสามารถ Build ด้วย XC8 ได้ แต่ขั้นตอน Upload
ผ่าน Bootloader ต้องจัดการแยก เช่น ใช้ `avrdude` การ Chip Erase หรือ Program
ผ่าน ISP อาจลบ Uno Bootloader ได้ จึงต้องตรวจ Operation และ Fuse ให้ชัดเจน

## ทางเลือกที่ 1.1 - MPLAB for VS Code + MPLAB XC8

Microchip มี
[MPLAB for VS Code](https://www.microchip.com/en-us/tools-resources/develop/mplab-tools-vs-code)
เพื่อใช้ MPLAB Project, XC8, Device Pack และ Hardware Tool ภายใน VS Code
เหมาะเมื่อชอบ Editor และ Source Control Workflow ของ VS Code

MPLAB X ยังคงเข้าใจง่ายสำหรับการสอน Project Wizard แบบ GUI เต็มรูปแบบ
ส่วน MPLAB for VS Code เหมาะกับผู้ที่คุ้นเคยกับ Extension และ VS Code แล้ว
ควรตรวจเอกสาร Microchip ตาม Version ที่ติดตั้ง เพราะความสามารถและหน้าจอ
ของ Extension สามารถเปลี่ยนได้

## ทางเลือกที่ 2 - WSL 2 + Ubuntu + avr-gcc

นี่คือสภาพแวดล้อมที่ใช้สาธิตจริงในซีรีส์ ไม่ได้ถูกเลือกเพราะ MPLAB ไม่ดี
หรือเพราะ WSL ทำให้ Firmware เป็น Native กว่า แต่เลือกจากประสบการณ์ของ
ผู้จัดทำและรูปแบบบทเรียนที่ต้องการเปิดเผยขั้นตอน Build

### ที่มาจากการทดลอง Native C บน Arduino Uno

ก่อนสร้าง Repository นี้ ผู้จัดทำใช้ Arduino Uno และ ATmega328P ทดลอง
แยกขั้นตอน Build ออกจาก Arduino IDE:

- เขียน `main.c` ควบคุม D13/PB5 ผ่าน `DDRB` และ `PORTB`
- Compile ด้วย `avr-gcc` ใน WSL เป็น ELF
- แปลง ELF เป็น HEX ด้วย `avr-objcopy`
- Flash ผ่าน Uno Bootloader ด้วย `avrdude`

การทดลองนี้ทำให้เห็น Build Pipeline ตั้งแต่ Source Code ถึง Firmware ใน MCU
โดยไม่เรียก Arduino API เมื่อนำมารวมกับ Arduino Uno Embedded Roadmap เดิม
จึงเกิดซีรีส์คู่ขนาน EP01–EP08 ที่ใช้บอร์ดและหัวข้อเดิม แต่เปลี่ยนเป็น
Register-Level C

Workflow ปัจจุบันพยายามใช้ `avrdude` ใน WSL ผ่าน `usbipd-win` เพื่อให้ Build
และ Flash อยู่ใน Terminal เดียว ส่วน Workflow แบบผสมที่ Build ใน WSL แล้ว
Flash บน Windows ยังคงเป็น Fallback ที่ถูกต้องเมื่อ USB Passthrough มีปัญหา

### เหตุผลที่ซีรีส์เลือก WSL

- เป็น Workflow ที่เคยใช้กับ Arduino Uno สำเร็จและเข้าใจวิธีแก้ปัญหาอยู่แล้ว
- เครื่องที่ใช้ผลิตซีรีส์มี WSL, Ubuntu และ VS Code อยู่แล้ว จึงไม่ต้องการ
  ติดตั้ง MPLAB X, XC8 และส่วนประกอบของ IDE เพิ่มสำหรับ Repository นี้
  ทำให้ใช้พื้นที่ติดตั้งเพิ่มเติมน้อยลงและไม่สร้าง Environment ซ้ำซ้อน
- ผู้จัดทำเคยแก้ปัญหา `PATH`, Package และ Version ของ AVR Toolchain บน
  Windows โดยตรงไม่สำเร็จ แต่ Workflow บน WSL ทำงานได้สม่ำเสมอในเครื่อง
  ที่ใช้ผลิตซีรีส์
- ผู้เรียนติดตั้ง Dependency ผ่าน Ubuntu Package Manager ด้วยคำสั่งเดียว
- Makefile ทำให้เห็น Compiler Flag, Linker และการสร้าง HEX อย่างชัดเจน
- คำสั่งเดียวกันใช้ซ้ำในทุก EP และนำไปใช้ใน Automation/CI ได้ง่าย
- Build Configuration เป็น Text File และ Review ผ่าน Git ได้
- ใช้ Editor และ Terminal ชุดเดิมข้าม Project และ MCU Family ได้ โดยไม่
  ต้องเปลี่ยน Workflow ตาม IDE
- ใช้ VS Code เชื่อมเข้า WSL ได้โดยยังทำงานอยู่บน Windows
- ไฟล์ `main.c` ไม่ผูกกับ IDE และย้ายไป XC8/MPLAB ได้ภายหลัง

เหตุผลเรื่องพื้นที่ใช้ได้เฉพาะเมื่อมี WSL/Ubuntu อยู่แล้ว หากต้องติดตั้ง WSL
ใหม่เพื่อ Repository นี้เพียงอย่างเดียว พื้นที่รวมและขั้นตอนเริ่มต้นอาจไม่
น้อยกว่า MPLAB ผู้เรียนที่มี MPLAB พร้อมใช้งานจึงไม่จำเป็นต้องเปลี่ยนมาใช้
WSL ตามผู้จัดทำ

### ข้อแลกเปลี่ยนของ WSL

- ต้องรู้คำสั่ง Terminal ขั้นพื้นฐาน
- ต้องติดตั้งและ Attach USB ผ่าน `usbipd-win`
- ขั้นตอนแรกยาวกว่า IDE ที่รวม Tool ไว้แล้ว
- ไม่มี One-click Debug หากไม่มี Hardware/Configuration เพิ่มเติม
- Package Version ของ Ubuntu อาจไม่ตรงกับ AVR GNU Toolchain ล่าสุดจาก
  Microchip จึงควรบันทึก Version ที่ใช้เมื่อทำงานจริงจัง

### ติดตั้งแบบย่อ

เปิด Ubuntu จาก Windows PowerShell:

```powershell
wsl -d Ubuntu-24.04
```

จากนั้นรันใน Ubuntu:

```sh
sudo apt update
sudo apt install -y gcc-avr avr-libc binutils-avr avrdude make usbutils picocom
```

ตรวจ Toolchain:

```sh
avr-gcc --version
avr-objcopy --version
avr-size --version
avrdude -?
make --version
```

Build ทุก EP จาก Root ของ Repository:

```sh
make clean
make all
```

Build เฉพาะ EP:

```sh
make build-selected EP=EP03_TIMER
make size EP=EP03_TIMER
```

ค่าเริ่มต้นใน Makefile:

- MCU: `atmega328p`
- CPU Clock: `16000000UL`
- Optimization: `-Os`
- Warning: `-Wall -Wextra -Werror`

ขั้นตอนส่ง USB Device จาก Windows เข้า WSL และ Flash ผ่าน Uno Bootloader
อยู่ใน [คู่มือเริ่มต้นด้วย WSL](wsl-setup.md)

## avr-gcc เป็น Toolchain ที่พบได้ทั่วไปหรือไม่

`avr-gcc` เป็นส่วนหนึ่งของ AVR GNU Toolchain ซึ่ง Microchip มีชุด Download
สำหรับ Windows, Linux และ macOS โดยตรง และ Arduino AVR Platform ก็เรียก
`avr-gcc` เพื่อ Compile กับ `avrdude` เพื่อ Upload อยู่เบื้องหลัง

ดังนั้นการเลือก avr-gcc ไม่ใช่การสร้าง Workflow ที่แปลกแยกจาก Arduino
แต่เป็นการนำ Toolchain ชั้นล่างที่ Arduino ใช้อยู่แล้วมาเรียกเอง โดยตัด
Arduino Core, `setup()`, `loop()` และ API ระดับสูงออก

เอกสารอ้างอิง:

- [Microchip AVR GNU Toolchain](https://www.microchip.com/en-us/development-tool/AVR-GCC)
- [Arduino AVR build process](https://docs.arduino.cc/arduino-cli/sketch-build-process/)
- [Arduino Platform Specification](https://docs.arduino.cc/arduino-cli/platform-specification/)
- [VS Code กับ WSL](https://learn.microsoft.com/en-us/windows/wsl/tutorials/wsl-vscode)

## หากต้องการ Build บน Windows โดยตรง

Makefile ไม่ได้ห้ามใช้ Native Windows Toolchain หากติดตั้ง `avr-gcc`,
`avr-objcopy`, `avr-size`, `avrdude` และ GNU Make ครบก็สามารถใช้ได้
แต่คำสั่งหลักของซีรีส์ใช้ WSL เพื่อให้ผู้จัดทำสาธิตและแก้ปัญหาเพียง
Environment เดียว

หาก Tool ใช้ชื่อหรือ Path ต่างจากค่าเริ่มต้น สามารถ Override ตัวแปรได้โดย
ไม่แก้ Makefile เช่น:

```powershell
make all AVR_CC="C:\path\to\avr-gcc.exe" AVR_OBJCOPY="C:\path\to\avr-objcopy.exe"
```

## คำว่า Native ใน Repository นี้หมายถึงอะไร

- ไม่ใช้ Arduino Core Startup
- ไม่มี `setup()` หรือ `loop()`
- ไม่ใช้ Arduino API หรือ C++ `String`
- ไม่ใช้ Arduino Peripheral Library
- ตั้งค่า Hardware จาก ATmega328P Datasheet โดยตรง
- ใช้ Compiler Header เพื่อประกาศชื่อ Register และ Interrupt Vector

C Runtime ยังคงเตรียม Memory และเรียก `main()` คำว่า Native ในที่นี้
ไม่ได้หมายถึงการเขียน Reset Vector และ Startup Assembly ใหม่ทั้งหมด

## ข้อสรุปสำหรับผู้เรียน

หากต้องการ GUI ที่เริ่มง่ายและทำงานกับ Tool ของ Microchip ให้เลือก MPLAB
หากต้องการทำตามซีรีส์นี้แบบคำสั่งต่อคำสั่ง ให้เลือก WSL + avr-gcc
ทั้งสองทางนำไปสู่เป้าหมายเดียวกันคือเข้าใจและควบคุม Register ของ
ATmega328P ด้วยภาษา C โดยตรง
