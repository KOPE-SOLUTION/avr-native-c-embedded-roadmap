# Arduino Uno to ATmega328P Pin Mapping

Reference board: Arduino Uno R3, ATmega328P, 16 MHz.

| Uno pin | AVR signal | Port bit | Used in this repo |
| --- | --- | --- | --- |
| D0 | RXD | PD0 | UART receive |
| D1 | TXD | PD1 | UART transmit |
| D2 | INT0 | PD2 | Button / external interrupt |
| D3 | OC2B / INT1 | PD3 | - |
| D4 | - | PD4 | - |
| D5 | OC0B | PD5 | - |
| D6 | OC0A | PD6 | - |
| D7 | - | PD7 | - |
| D8 | ICP1 | PB0 | - |
| D9 | OC1A | PB1 | PWM output |
| D10 | SS / OC1B | PB2 | SPI chip select |
| D11 | MOSI / OC2A | PB3 | SPI MOSI |
| D12 | MISO | PB4 | SPI MISO |
| D13 | SCK / onboard LED | PB5 | LED / SPI clock |
| A0 | ADC0 | PC0 | Potentiometer |
| A1 | ADC1 | PC1 | - |
| A2 | ADC2 | PC2 | - |
| A3 | ADC3 | PC3 | - |
| A4 | ADC4 / SDA | PC4 | I2C/TWI data |
| A5 | ADC5 / SCL | PC5 | I2C/TWI clock |

## Register groups

- Port B: `DDRB`, `PORTB`, `PINB`
- Port C: `DDRC`, `PORTC`, `PINC`
- Port D: `DDRD`, `PORTD`, `PIND`

## Shared-pin cautions

- D0/D1 are connected to the Uno USB-to-serial circuit. Disconnect external
  devices from these pins while uploading if they disturb the bootloader.
- D13 is both the onboard LED and SPI SCK.
- A4/A5 are I2C SDA/SCL and need pull-ups on the bus.
- D9 PWM in EP05/EP06 uses Timer1. Reconfiguring Timer1 changes that output.

Use the [official Uno R3 documentation](https://docs.arduino.cc/hardware/uno-rev3/)
and ATmega328P datasheet as the final reference.
