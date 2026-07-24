# EP04 - External Interrupt

Use the INT0 peripheral to respond to a button falling edge without
`attachInterrupt()`.

## Wiring

| Component | Connection |
| --- | --- |
| Push button | D2/INT0 to GND |
| LED | Onboard D13 LED |

PD2 uses its internal pull-up, so pressing the button creates a HIGH-to-LOW
transition.

## Registers and ISR

| Register/feature | Role |
| --- | --- |
| `EICRA` | Select falling-edge trigger with `ISC01:0` |
| `EIFR` | Clear a stale `INTF0` flag |
| `EIMSK` | Enable `INT0` |
| `SREG.I` / `sei()` | Globally enable interrupts |
| `ISR(INT0_vect)` | Interrupt service routine |

The ISR only changes one 8-bit state variable. The main loop applies that state
to the LED, keeping the ISR short.

## Mechanical switch warning

A real push button bounces and may generate several edges from one press. This
minimal example makes that behavior visible; production code should add
hardware or timer-based debounce rather than delaying inside the ISR.

```sh
make build-selected EP=EP04_INTERRUPT
```

Source: [src/main.c](src/main.c)

Next: [EP05 - PWM](../EP05_PWM/README.md)
