# EP03 - Timer Basics

Create a 1 ms hardware timebase with Timer1 and blink the onboard LED without
`delay()`, `millis()`, or an interrupt.

## Why polling first?

EP03 isolates the timer peripheral. The CPU polls the compare flag while the
hardware timer counts independently. EP04 then introduces interrupt-driven
events as a separate concept.

## Timer1 setup

- Mode: CTC (Clear Timer on Compare Match)
- Clock: 16 MHz
- Prescaler: 64
- Tick frequency: 250 kHz
- `OCR1A`: 249
- Compare period: 1 ms

```text
OCR1A = F_CPU / prescaler / target_frequency - 1
      = 16,000,000 / 64 / 1,000 - 1
      = 249
```

## Registers

| Register | Role |
| --- | --- |
| `TCCR1A/B` | Waveform mode and clock prescaler |
| `TCNT1` | Current counter value |
| `OCR1A` | Compare value |
| `TIFR1` | Compare-match flag (`OCF1A`) |

`OCF1A` is a write-one-to-clear flag. The source clears it by writing a one.

## Expected behavior

[src/main.c](src/main.c) accumulates 1,000 compare events and toggles D13 once
per second. The main loop never busy-waits for a one-second delay, so other
polling tasks could be added.

```sh
make build-selected EP=EP03_TIMER
```

Next: [EP04 - Interrupt](../EP04_INTERRUPT/README.md)
