# AVR Native C Embedded Roadmap

The native AVR C companion to the
[Arduino Uno Embedded Roadmap](https://github.com/KOPE-SOLUTION/arduino-uno-embedded-roadmap),
using ATmega328P registers and peripherals directly.

This series keeps the original EP01-EP08 learning order while replacing the
Arduino API with register-level C. It exposes how GPIO, USART, timers,
interrupts, PWM, ADC, SPI, and I2C/TWI work inside the microcontroller.

```c
/* Arduino framework */
digitalWrite(13, HIGH);

/* Native AVR C */
PORTB |= (1U << PORTB5);
```

## Scope

| Item | Selection |
| --- | --- |
| Target MCU | ATmega328P |
| Reference board | Arduino Uno R3 (16 MHz) |
| Language | C |
| Framework | None |
| Programming level | Register-level |
| Guided IDE/compiler | MPLAB X IDE + MPLAB XC8 |
| Official lightweight IDE | MPLAB for VS Code + MPLAB XC8 |
| Portable toolchain | avr-gcc + avr-libc + Make + avrdude |

`<avr/io.h>` provides register names for the selected MCU; it is not the
Arduino framework. Every example has its own `main()` and can be studied or
built independently.

> **Learning target:** Microchip currently marks the ATmega328P as
> "Not Recommended for new designs." It remains the correct target here
> because the goal is to compare native C directly with the established
> Arduino Uno series. For a new production design, evaluate a current AVR
> family separately.

## Roadmap

| EP | Topic | Arduino layer | Native registers | Example |
| --- | --- | --- | --- | --- |
| 01 | [GPIO](EP01_GPIO/README.md) | `pinMode`, `digitalRead`, `digitalWrite` | `DDRx`, `PORTx`, `PINx` | Button controls LED |
| 02 | [UART](EP02_UART/README.md) | `Serial` | `UBRR0`, `UCSR0x`, `UDR0` | Text command console |
| 03 | [Timer](EP03_TIMER/README.md) | `millis` | `TCCR1x`, `TCNT1`, `OCR1A`, `TIFR1` | Non-blocking LED blink |
| 04 | [Interrupt](EP04_INTERRUPT/README.md) | `attachInterrupt` | `EICRA`, `EIMSK`, `EIFR`, ISR | Button event |
| 05 | [PWM](EP05_PWM/README.md) | `analogWrite` | `TCCR1x`, `OCR1A` | LED fade on D9 |
| 06 | [ADC](EP06_ADC/README.md) | `analogRead` | `ADMUX`, `ADCSRA`, `ADC` | Potentiometer to PWM |
| 07 | [SPI](EP07_SPI/README.md) | `SPI`, `SD` | `SPCR`, `SPSR`, `SPDR` | Raw SD-card handshake |
| 08 | [I2C/TWI](EP08_I2C/README.md) | `Wire` | `TWBR`, `TWSR`, `TWCR`, `TWDR` | I2C address scanner |

The SPI episode deliberately stops below the FAT filesystem layer. It shows the
native SPI transaction and the first SD command so the boundary between the bus
driver, SD protocol, and filesystem is explicit.

## Choose a build path

### MPLAB X IDE + XC8 (recommended first path)

Create one MPLAB X standalone project per episode and add that episode's
`src/main.c`. Select `ATmega328P` and the XC8 compiler. Full steps and the
difference between an ICSP programmer and the Uno USB bootloader are in
[Toolchain setup](docs/toolchain-setup.md).

Generated MPLAB project metadata is intentionally not committed. The source
stays readable and reusable outside one IDE.

Microchip also supports XC8 through
[MPLAB for VS Code](https://www.microchip.com/en-us/tools-resources/develop/mplab-tools-vs-code).
It is a good lightweight option for learners already comfortable with VS Code;
the register-level source in this repository stays the same.

### avr-gcc + Make + avrdude

Build all episodes:

```sh
make all
```

On Windows, if GNU Make is named `mingw32-make`:

```powershell
mingw32-make all
```

Build or flash one episode:

```sh
make build-selected EP=EP01_GPIO
make flash EP=EP01_GPIO PORT=COM3
```

Change `PORT` for your system. See [Flashing guide](docs/flashing-guide.md)
before programming hardware.

## Repository layout

```text
avr-native-c-embedded-roadmap/
|-- README.md
|-- LICENSE
|-- Makefile
|-- docs/
|   |-- toolchain-setup.md
|   |-- arduino-uno-pin-mapping.md
|   |-- register-basics.md
|   `-- flashing-guide.md
|-- EP01_GPIO/
|   |-- README.md
|   `-- src/main.c
`-- ... EP02_UART through EP08_I2C
```

## Hardware assumptions

- Arduino Uno R3 runs the ATmega328P at 16 MHz.
- Existing Uno clock and bootloader fuses are left unchanged.
- D0/D1 are shared with the board's USB-to-serial interface.
- I2C devices need pull-up resistors on SDA and SCL; many modules include them.
- A bare microSD card is a 3.3 V device. Use a module or level conversion that
  is electrically safe for a 5 V Uno.

Before rewiring, disconnect power. Always join the grounds of devices that
communicate with each other.

## Reference documents

- [ATmega328P product page and datasheet](https://www.microchip.com/en-us/product/atmega328p)
- [MPLAB X IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
- [MPLAB XC8 compiler](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-compilers/xc8)
- [Arduino Uno R3 documentation](https://docs.arduino.cc/hardware/uno-rev3/)

## License

[MIT](LICENSE)
