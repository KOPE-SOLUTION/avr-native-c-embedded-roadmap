# Toolchain Setup

The source is independent of one IDE and intentionally supports two paths.

## Path A - MPLAB X IDE + MPLAB XC8

This is the recommended first path for the series. The IDE manages the target
device, compiler, build configuration, and Microchip debug/program tools.

### Install

1. Install [MPLAB X IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide).
2. Install [MPLAB XC8](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-compilers/xc8)
   separately. XC8 supports 8-bit AVR targets.
3. In MPLAB X, confirm XC8 appears under **Tools > Options > Embedded >
   Build Tools**.

### Create an episode project

1. Select **File > New Project > Microchip Embedded > Standalone Project**.
2. Select device **ATmega328P**.
3. Select a hardware tool or **Simulator**.
4. Select the installed **XC8** compiler.
5. Name the project after the episode, for example `EP01_GPIO`.
6. Add `EP01_GPIO/src/main.c` to **Source Files**.
7. Build the project.

The examples include `<avr/io.h>`, which XC8 accepts for AVR targets. It maps
register and bit names to the selected MCU. MPLAB projects often use `<xc.h>`;
XC8's `<xc.h>` includes the AVR device definitions, but `<avr/io.h>` keeps the
same source compatible with avr-gcc.

### Program/debug from MPLAB X

The Uno USB connector talks to the bootloader through a USB-to-serial device;
it is not a native MPLAB debug connection. To use **Make and Program Device**
inside MPLAB X, connect a programmer/debugger that MPLAB X lists as compatible
with ATmega328P to the Uno ICSP header.

Do not change clock, reset, or boot fuses as part of these lessons. Programming
through ISP can erase the Uno bootloader depending on the operation and tool
settings.

If you only have the Uno and a USB cable, use Path B to flash through the
existing bootloader.

## Path A2 - MPLAB for VS Code + MPLAB XC8

Microchip also provides
[MPLAB for VS Code](https://www.microchip.com/en-us/tools-resources/develop/mplab-tools-vs-code).
It supports MPLAB projects, XC8, programming, and basic debugging in a lighter
editor. Choose it when VS Code is already familiar or when you prefer its
editing and source-control workflow.

This is still a manufacturer-supported native path. The compiler and register-
level source are the same; only the development environment changes. For a
first video series, MPLAB X remains easier to present as one consistent GUI.

## Path B - avr-gcc + Make + avrdude

Install or place these tools on `PATH`:

- `avr-gcc`
- `avr-objcopy`
- `avr-size`
- GNU Make
- `avrdude`

Build all episodes:

```sh
make all
```

On Windows the Make executable may be named `mingw32-make`:

```powershell
mingw32-make all
```

Build one episode:

```sh
make build-selected EP=EP03_TIMER
```

The Makefile defines:

- MCU: `atmega328p`
- CPU clock: `16000000UL`
- optimization: `-Os`
- warnings: `-Wall -Wextra -Werror`

Override a tool path without changing the file:

```powershell
mingw32-make all AVR_CC="C:\path\to\avr-gcc.exe" AVR_OBJCOPY="C:\path\to\avr-objcopy.exe"
```

## What "native" means here

- No Arduino core startup
- No `setup()` or `loop()`
- No Arduino API or C++ `String`
- No Arduino peripheral libraries
- Direct register configuration based on the ATmega328P datasheet
- Standard compiler headers are allowed because they name registers and vectors

The C runtime still initializes memory and calls `main()`. "Native" does not
mean writing the reset vector and startup assembly from scratch.
