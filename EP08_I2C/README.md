# EP08 - I2C/TWI Address Scanner

Use the ATmega328P Two-Wire Interface (TWI) registers to scan a 7-bit I2C bus
without Arduino `Wire`.

## Wiring

| I2C device | Uno / ATmega328P |
| --- | --- |
| SDA | A4 / PC4 / SDA |
| SCL | A5 / PC5 / SCL |
| GND | GND |
| VCC | Device-specific supply |

SDA and SCL are open-drain bus lines and require pull-up resistors. Many OLED
and sensor modules already include pull-ups. Make sure the pull-up voltage is
safe for every device on the bus.

## TWI configuration

- Controller/master transmitter for address probing
- Bus rate: 100 kHz at `F_CPU = 16 MHz`
- Prescaler: 1
- `TWBR = 72`

```text
SCL = F_CPU / (16 + 2 * TWBR * prescaler)
```

| Register | Role |
| --- | --- |
| `TWBR` | Bit-rate divider |
| `TWSR` | Prescaler and bus status code |
| `TWCR` | START, STOP, enable, and completion control |
| `TWDR` | Address/data byte |

## Test

1. Power off and connect an I2C module.
2. Flash [src/main.c](src/main.c).
3. Open a terminal at 9600 baud.

Example for a display at address `0x3C`:

```text
I2C scan
Found 0x3C
Devices: 1
```

The scan repeats every five seconds. It probes addresses `0x08-0x77`, leaving
reserved address ranges alone.

```sh
make build-selected EP=EP08_I2C
```

This finishes the EP01-EP08 native companion. A natural next module is writing
small reusable peripheral drivers and testing them across multiple AVR MCUs.
