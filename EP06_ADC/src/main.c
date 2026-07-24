#include <avr/io.h>
#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <util/delay.h>

#define BAUD_RATE   9600UL
#define UBRR_VALUE  ((F_CPU / (16UL * BAUD_RATE)) - 1UL)

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

static void uart_print_u16(uint16_t value)
{
    char digits[5];
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

static void pwm1a_init_fast_8bit(void)
{
    DDRB |= (1U << DDB1); /* OC1A / D9. */
    TCCR1A = (1U << COM1A1) | (1U << WGM10);
    TCCR1B = (1U << WGM12) | (1U << CS11) | (1U << CS10);
    OCR1A = 0U;
}

static void adc_init(void)
{
    ADMUX = (1U << REFS0); /* AVCC reference, ADC0 channel. */
    ADCSRA = (1U << ADEN) |
              (1U << ADPS2) | (1U << ADPS1) | (1U << ADPS0);
    DIDR0 |= (1U << ADC0D);
}

static uint16_t adc_read(void)
{
    ADCSRA |= (1U << ADSC);
    while ((ADCSRA & (1U << ADSC)) != 0U) {
        /* Wait for this conversion to finish. */
    }
    return ADC;
}

int main(void)
{
    uart_init();
    pwm1a_init_fast_8bit();
    adc_init();

    uart_puts("ADC to PWM ready\n");

    while (1) {
        const uint16_t adc_value = adc_read();
        const uint8_t pwm_value = (uint8_t)(adc_value >> 2U);

        OCR1A = pwm_value;

        uart_puts("ADC: ");
        uart_print_u16(adc_value);
        uart_puts(" | PWM: ");
        uart_print_u16(pwm_value);
        uart_putchar('\n');

        _delay_ms(100.0);
    }
}
