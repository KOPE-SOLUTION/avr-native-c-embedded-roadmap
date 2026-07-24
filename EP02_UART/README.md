# EP02 - UART Serial Communication

Configure USART0 directly and build a small command console without Arduino
`Serial` or the C++ `String` class.

## Configuration

```text
Baud:      9600
Data bits: 8
Parity:    none
Stop bits: 1
Mode:      asynchronous, normal speed
```

At 16 MHz, `UBRR0 = 103` gives approximately 9600 baud.

## Registers

| Register | Role |
| --- | --- |
| `UBRR0H/L` | Baud-rate divider |
| `UCSR0A` | TX/RX status flags |
| `UCSR0B` | Enable transmitter and receiver |
| `UCSR0C` | 8N1 frame format |
| `UDR0` | Transmit/receive data byte |

## Hardware

Use the Uno USB connection. USART0 uses D0/RX and D1/TX, which are already
connected to the board's USB-to-serial circuit.

## Test

1. Flash [src/main.c](src/main.c).
2. Open a serial terminal at 9600 baud.
3. Select a line ending that sends LF (`\n`) or CRLF.
4. Send one of these commands:

```text
LED ON
LED OFF
STATUS?
```

The parser uses a fixed 16-byte buffer, rejects overflow, and performs no
dynamic memory allocation.

Build:

```sh
make build-selected EP=EP02_UART
```

## Concepts learned

- Baud-rate calculation
- Polling `UDRE0`, `RXC0`, and `TXC0`-related data flow
- Byte-oriented serial I/O
- Fixed-buffer command parsing
- Why UART is a common firmware debug channel

Next: [EP03 - Timer](../EP03_TIMER/README.md)
