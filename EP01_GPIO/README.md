# EP01 - GPIO Input and Output

Control the Arduino Uno onboard LED from a push button without `pinMode()`,
`digitalRead()`, or `digitalWrite()`.

## Arduino API to register-level C

| Intent | Arduino | ATmega328P |
| --- | --- | --- |
| D13 output | `pinMode(13, OUTPUT)` | Set `DDB5` in `DDRB` |
| D2 input pull-up | `pinMode(2, INPUT_PULLUP)` | Clear `DDD2`; set `PORTD2` |
| Read D2 | `digitalRead(2)` | Read `PIND2` from `PIND` |
| Drive D13 | `digitalWrite(13, value)` | Write `PORTB5` in `PORTB` |

## Hardware and wiring

| Component | Connection |
| --- | --- |
| Arduino Uno | USB/power |
| Push button | D2 to GND |
| LED | Use onboard D13 LED |

The internal pull-up makes the input active low:

```text
released = HIGH
pressed  = LOW
```

If using an external LED, connect D13 through a 220-1k ohm resistor to the LED
anode and connect its cathode to GND.

## Source

[src/main.c](src/main.c) performs read-modify-write operations so unrelated
pins in the same port keep their state.

Build:

```sh
make build-selected EP=EP01_GPIO
```

## Expected behavior

- Press button: LED on
- Release button: LED off

## Registers learned

- `DDRB`, `PORTB`
- `DDRD`, `PORTD`, `PIND`
- Internal pull-up and active-low logic

Next: [EP02 - UART](../EP02_UART/README.md)
