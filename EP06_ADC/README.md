# EP06 - ADC Measurement

Read a potentiometer on ADC0, map the 10-bit result to Timer1's 8-bit PWM, and
report both values through USART0--without `analogRead()`, `map()`, or Arduino
`Serial`.

## Wiring

| Potentiometer pin | Uno |
| --- | --- |
| One outer pin | 5V |
| Other outer pin | GND |
| Wiper | A0 / ADC0 |

Connect an external LED and resistor to D9 as described in EP05. Keep all
analog input voltages between GND and the selected reference voltage.

## ADC configuration

- Reference: AVCC
- Channel: ADC0 / A0
- Resolution: 10 bit (`0-1023`)
- ADC prescaler: 128
- ADC clock: `16 MHz / 128 = 125 kHz`
- Conversion mode: single conversion, polling

| Register | Role |
| --- | --- |
| `ADMUX` | Reference and channel selection |
| `ADCSRA` | Enable, start, status, and prescaler |
| `ADC` | Combined 10-bit result |
| `DIDR0` | Disable ADC0 digital input buffer |

The mapping is a bit shift:

```c
pwm_value = (uint8_t)(adc_value >> 2U); /* 10 bit to 8 bit */
```

## Test

Open a terminal at 9600 baud and rotate the potentiometer:

```text
ADC: 0 | PWM: 0
ADC: 512 | PWM: 128
ADC: 1023 | PWM: 255
```

```sh
make build-selected EP=EP06_ADC
```

Source: [src/main.c](src/main.c)

Next: [EP07 - SPI](../EP07_SPI/README.md)
