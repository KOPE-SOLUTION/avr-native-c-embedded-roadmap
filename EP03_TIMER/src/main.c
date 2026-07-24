#include <avr/io.h>
#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define TIMER1_PRESCALER 64UL
#define TIMER_TICK_HZ    1000UL
#define OCR1A_VALUE      ((F_CPU / TIMER1_PRESCALER / TIMER_TICK_HZ) - 1UL)

static void timer1_init_ctc_1ms(void)
{
    TCCR1A = 0U;
    TCCR1B = 0U;
    TCNT1 = 0U;
    OCR1A = (uint16_t)OCR1A_VALUE;

    /* Clear a pending compare flag before the timer starts. */
    TIFR1 = (1U << OCF1A);

    /* CTC mode, top = OCR1A, prescaler = 64. */
    TCCR1B = (1U << WGM12) | (1U << CS11) | (1U << CS10);
}

int main(void)
{
    uint16_t elapsed_ms = 0U;

    DDRB |= (1U << DDB5);
    PORTB &= ~(1U << PORTB5);
    timer1_init_ctc_1ms();

    while (1) {
        if ((TIFR1 & (1U << OCF1A)) != 0U) {
            TIFR1 = (1U << OCF1A); /* Write one to clear. */
            elapsed_ms++;

            if (elapsed_ms >= 1000U) {
                elapsed_ms = 0U;
                PORTB ^= (1U << PORTB5);
            }
        }
    }
}
