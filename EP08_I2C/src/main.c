#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <util/delay.h>

#define BAUD_RATE       9600UL
#define UBRR_VALUE      ((F_CPU / (16UL * BAUD_RATE)) - 1UL)
#define TWI_FREQUENCY   100000UL
#define TWBR_VALUE      ((F_CPU / TWI_FREQUENCY - 16UL) / 2UL)

#define TWI_STATUS_MASK 0xF8U
#define TWI_START_SENT  0x08U
#define TWI_RESTART_SENT 0x10U
#define TWI_SLA_W_ACK   0x18U

static void uart_init(void)
{
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8U);
    UBRR0L = (uint8_t)UBRR_VALUE;
    UCSR0A = 0U;
    UCSR0B = (1U << TXEN0);
    UCSR0C = (1U << UCSZ01) | (1U << UCSZ00);
}

static void uart_putchar(char character)
{
    if (character == '\n') {
        while ((UCSR0A & (1U << UDRE0)) == 0U) {
            /* Wait. */
        }
        UDR0 = (uint8_t)'\r';
    }

    while ((UCSR0A & (1U << UDRE0)) == 0U) {
        /* Wait. */
    }
    UDR0 = (uint8_t)character;
}

static void uart_puts(const char *text)
{
    while (*text != '\0') {
        uart_putchar(*text);
        text++;
    }
}

static void uart_print_hex8(uint8_t value)
{
    static const char hex[] = "0123456789ABCDEF";
    uart_putchar(hex[value >> 4U]);
    uart_putchar(hex[value & 0x0FU]);
}

static void uart_print_u8(uint8_t value)
{
    char digits[3];
    uint8_t count = 0U;

    do {
        digits[count] = (char)('0' + (value % 10U));
        count++;
        value /= 10U;
    } while (value != 0U);

    while (count != 0U) {
        count--;
        uart_putchar(digits[count]);
    }
}

static void twi_init(void)
{
    TWSR &= ~((1U << TWPS1) | (1U << TWPS0)); /* Prescaler 1. */
    TWBR = (uint8_t)TWBR_VALUE;
    TWCR = (1U << TWEN);
}

static bool twi_wait(void)
{
    uint16_t timeout = 0xFFFFU;

    while ((TWCR & (1U << TWINT)) == 0U) {
        timeout--;
        if (timeout == 0U) {
            return false;
        }
    }
    return true;
}

static void twi_stop(void)
{
    uint16_t timeout = 0xFFFFU;

    TWCR = (1U << TWINT) | (1U << TWEN) | (1U << TWSTO);

    /* Hardware clears TWSTO after the STOP condition reaches the bus. */
    while ((TWCR & (1U << TWSTO)) != 0U) {
        timeout--;
        if (timeout == 0U) {
            break;
        }
    }
}

static bool twi_probe(uint8_t address)
{
    uint8_t status;

    TWCR = (1U << TWINT) | (1U << TWEN) | (1U << TWSTA);
    if (!twi_wait()) {
        twi_stop();
        return false;
    }

    status = TWSR & TWI_STATUS_MASK;
    if ((status != TWI_START_SENT) && (status != TWI_RESTART_SENT)) {
        twi_stop();
        return false;
    }

    TWDR = (uint8_t)(address << 1U); /* SLA+W. */
    TWCR = (1U << TWINT) | (1U << TWEN);
    if (!twi_wait()) {
        twi_stop();
        return false;
    }

    status = TWSR & TWI_STATUS_MASK;
    twi_stop();
    return status == TWI_SLA_W_ACK;
}

int main(void)
{
    uint8_t address;
    uint8_t found;
    uint8_t second;

    uart_init();
    twi_init();
    uart_puts("Native AVR TWI scanner ready\n");

    while (1) {
        found = 0U;
        uart_puts("\nI2C scan\n");

        for (address = 0x08U; address <= 0x77U; address++) {
            if (twi_probe(address)) {
                uart_puts("Found 0x");
                uart_print_hex8(address);
                uart_putchar('\n');
                found++;
            }
        }

        uart_puts("Devices: ");
        uart_print_u8(found);
        uart_putchar('\n');

        for (second = 0U; second < 5U; second++) {
            _delay_ms(1000.0);
        }
    }
}
