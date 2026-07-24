#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BAUD_RATE   9600UL
#define UBRR_VALUE  ((F_CPU / (16UL * BAUD_RATE)) - 1UL)
#define COMMAND_CAPACITY 16U

static void uart_init(void)
{
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8U);
    UBRR0L = (uint8_t)UBRR_VALUE;

    UCSR0A = 0U; /* ใช้ความเร็วปกติ ไม่เปิด U2X Mode */
    UCSR0B = (1U << RXEN0) | (1U << TXEN0);
    UCSR0C = (1U << UCSZ01) | (1U << UCSZ00); /* 8N1. */
}

static void uart_putchar(char character)
{
    if (character == '\n') {
        while ((UCSR0A & (1U << UDRE0)) == 0U) {
            /* รอให้ Transmit Register พร้อม */
        }
        UDR0 = (uint8_t)'\r';
    }

    while ((UCSR0A & (1U << UDRE0)) == 0U) {
        /* รอให้ Transmit Register พร้อม */
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

static bool uart_rx_ready(void)
{
    return (UCSR0A & (1U << RXC0)) != 0U;
}

static char uart_getchar(void)
{
    return (char)UDR0;
}

static bool text_equal(const char *left, const char *right)
{
    while ((*left != '\0') && (*right != '\0')) {
        if (*left != *right) {
            return false;
        }
        left++;
        right++;
    }

    return *left == *right;
}

static void handle_command(const char *command)
{
    if (text_equal(command, "LED ON")) {
        PORTB |= (1U << PORTB5);
        uart_puts("LED is ON\n");
    } else if (text_equal(command, "LED OFF")) {
        PORTB &= ~(1U << PORTB5);
        uart_puts("LED is OFF\n");
    } else if (text_equal(command, "STATUS?")) {
        if ((PORTB & (1U << PORTB5)) != 0U) {
            uart_puts("LED status: ON\n");
        } else {
            uart_puts("LED status: OFF\n");
        }
    } else if (*command != '\0') {
        uart_puts("Unknown command\n");
    }
}

int main(void)
{
    char command[COMMAND_CAPACITY];
    uint8_t length = 0U;

    DDRB |= (1U << DDB5);
    PORTB &= ~(1U << PORTB5);
    uart_init();

    uart_puts("UART command ready\n");
    uart_puts("Commands: LED ON, LED OFF, STATUS?\n> ");

    while (1) {
        if (uart_rx_ready()) {
            const char received = uart_getchar();

            if (received == '\r') {
                continue;
            }

            if (received == '\n') {
                command[length] = '\0';
                handle_command(command);
                length = 0U;
                uart_puts("> ");
            } else if (length < (COMMAND_CAPACITY - 1U)) {
                command[length] = received;
                length++;
            } else {
                length = 0U;
                uart_puts("\nCommand too long\n> ");
            }
        }
    }
}
