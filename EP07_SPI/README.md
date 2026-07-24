# EP07 - SPI Communication and SD Handshake

Configure the ATmega328P SPI peripheral as a controller and send the first raw
SD-card command without Arduino `SPI` or `SD` libraries.

## Why not write a file yet?

The Arduino `SD` example combines several layers:

```text
SPI electrical bus -> SD command protocol -> block device -> FAT filesystem -> file API
```

This episode isolates the first two layers. It sends `CMD0` and expects the SD
card to enter idle state (`R1 = 0x01`). Writing `test.txt` natively requires an
SD block driver plus a FAT implementation and belongs in a later storage
module, not inside an SPI fundamentals example.

## Wiring

| SD module | Uno / ATmega328P |
| --- | --- |
| CS | D10 / PB2 |
| MOSI | D11 / PB3 |
| MISO | D12 / PB4 |
| SCK | D13 / PB5 |
| GND | GND |

**Voltage warning:** a bare microSD card uses 3.3 V signaling. Do not connect
it directly to 5 V outputs. Use an SD module/level conversion that is safe for
a 5 V Uno, and follow the module's supply specification.

## SPI initialization

- Controller/master mode
- Mode 0 (`CPOL=0`, `CPHA=0`)
- MSB first
- `f_CPU / 128 = 125 kHz` during card initialization

| Register | Role |
| --- | --- |
| `SPCR` | Enable SPI, controller mode, clock divider |
| `SPSR` | Transfer-complete and double-speed status |
| `SPDR` | Transmit/receive data byte |

## Test

1. Insert the card with power off and check wiring.
2. Flash [src/main.c](src/main.c).
3. Open a terminal at 9600 baud.

Success:

```text
SPI SD CMD0 test
SD entered idle state (R1=0x01)
```

Failure prints the last R1 byte. Check power, level shifting, CS wiring, and
card insertion before changing software.

```sh
make build-selected EP=EP07_SPI
```

Next: [EP08 - I2C/TWI](../EP08_I2C/README.md)
