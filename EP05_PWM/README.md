# EP05 - PWM with Timer1

Generate hardware PWM on Uno D9/OC1A without `analogWrite()`.

## Wiring

Connect D9 through a 220-1k ohm resistor to an LED anode. Connect the LED cathode
to GND. The onboard D13 LED is not connected to D9, so use an external LED.

## Timer1 configuration

- Output: OC1A / PB1 / Uno D9
- Mode: 8-bit Fast PWM
- Output behavior: non-inverting
- Prescaler: 64
- Frequency: `16 MHz / (64 * 256) ~= 976.6 Hz`
- Duty register: `OCR1A`, range 0-255

| Register | Important bits |
| --- | --- |
| `DDRB` | `DDB1` makes OC1A an output |
| `TCCR1A` | `COM1A1`, `WGM10` |
| `TCCR1B` | `WGM12`, `CS11`, `CS10` |
| `OCR1A` | Duty-cycle value |

```text
duty (%) ~= OCR1A / 255 * 100
```

[src/main.c](src/main.c) fades the LED up and down. `_delay_ms()` only controls
how quickly the demonstration changes duty; Timer1 continues generating every
PWM edge in hardware.

```sh
make build-selected EP=EP05_PWM
```

Next: [EP06 - ADC](../EP06_ADC/README.md)
