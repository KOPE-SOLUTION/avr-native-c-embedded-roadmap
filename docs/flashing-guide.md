# Flashing Guide

## Uno USB bootloader with avrdude

Connect the Uno by USB and find its serial port.

Build and flash one episode:

```sh
make flash EP=EP01_GPIO PORT=COM3
```

Windows:

```powershell
mingw32-make flash EP=EP01_GPIO PORT=COM3
```

The Makefile defaults are:

```text
MCU          = atmega328p
PROGRAMMER   = arduino
UPLOAD_BAUD  = 115200
```

An official Uno R3 normally uses these settings. A clone or a board with a
different bootloader may need another baud rate:

```sh
make flash EP=EP01_GPIO PORT=COM3 UPLOAD_BAUD=57600
```

Before uploading:

1. Close Serial Monitor and any program holding the COM port.
2. Disconnect hardware on D0/RX and D1/TX if it interferes.
3. Confirm the selected board is an ATmega328P Uno running at 16 MHz.
4. Keep the board powered during the complete operation.

## Hardware programmer through ICSP

MPLAB X can program through a compatible hardware programmer connected to the
Uno ICSP header. The six signals are MISO, VCC, SCK, MOSI, RESET, and GND.

Check all of the following before programming:

- The tool explicitly supports ATmega328P in your MPLAB X/device-pack version.
- Target voltage and tool power settings are correct.
- ICSP orientation is correct.
- Nothing attached to SPI pins is driving the bus.
- Fuse changes are disabled unless you have a deliberate recovery plan.

An ISP chip erase/program operation can remove the Uno bootloader. It can be
restored later with an ISP programmer, but USB bootloader uploads will not work
until it is restored.

## Serial output

Episodes 02, 06, 07, and 08 use USART0 at:

```text
9600 baud, 8 data bits, no parity, 1 stop bit
```

Open any serial terminal at 9600 baud after the upload completes.
