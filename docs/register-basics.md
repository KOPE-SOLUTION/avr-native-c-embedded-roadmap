# Register-Level Basics

## Set, clear, toggle, and test a bit

```c
/* Set bit */
PORTB |= (1U << PORTB5);

/* Clear bit */
PORTB &= ~(1U << PORTB5);

/* Toggle bit */
PORTB ^= (1U << PORTB5);

/* Test bit */
if ((PINB & (1U << PINB5)) != 0U) {
    /* pin is high */
}
```

The read-modify-write operators preserve unrelated bits in the same register.

## GPIO has three register roles

For classic AVR ports:

- `DDRx`: direction (`1` output, `0` input)
- `PORTx`: output value, or input pull-up enable
- `PINx`: physical input state

An input with its internal pull-up enabled:

```c
DDRD &= ~(1U << DDD2);
PORTD |= (1U << PORTD2);
```

With a button from D2 to GND, released reads high and pressed reads low.

## Assign versus OR

```c
TCCR1A = (1U << WGM10);
```

sets the complete register to a known state.

```c
TCCR1A |= (1U << COM1A1);
```

preserves other bits. During peripheral initialization, full assignment is
often clearer when every relevant bit is intentionally chosen.

## Write-one-to-clear flags

Some AVR status flags are cleared by writing a `1`, not a `0`. For example:

```c
TIFR1 = (1U << OCF1A);
```

Always check the datasheet's register description before changing a flag.

## `volatile` and interrupts

Device registers are already declared volatile by the compiler headers.
Variables shared between `main()` and an ISR also need `volatile`:

```c
static volatile uint8_t event_pending;
```

`volatile` prevents unwanted optimization; it does not make a multi-byte
access atomic. An 8-bit AVR may need interrupts temporarily disabled when
main code and an ISR share a 16- or 32-bit value.

## Read the datasheet in this order

1. Peripheral overview and block diagram
2. Pin multiplexing
3. Initialization sequence
4. Register summary
5. Each register's bit table
6. Timing formula and electrical limits

The comments in this repository explain intent, but the datasheet remains the
source of truth.
