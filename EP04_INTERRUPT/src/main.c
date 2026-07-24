#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

static volatile uint8_t led_state;

ISR(INT0_vect)
{
    led_state ^= 1U;
}

static void int0_init_falling_edge(void)
{
    /* Falling edge: ISC01 = 1, ISC00 = 0. */
    EICRA = (EICRA & ~((1U << ISC01) | (1U << ISC00))) |
            (1U << ISC01);

    EIFR = (1U << INTF0);  /* Clear a stale flag. */
    EIMSK |= (1U << INT0); /* Enable external interrupt 0. */
}

int main(void)
{
    DDRB |= (1U << DDB5);
    PORTB &= ~(1U << PORTB5);

    DDRD &= ~(1U << DDD2);
    PORTD |= (1U << PORTD2); /* Internal pull-up on D2. */

    int0_init_falling_edge();
    sei();

    while (1) {
        if (led_state != 0U) {
            PORTB |= (1U << PORTB5);
        } else {
            PORTB &= ~(1U << PORTB5);
        }
    }
}
