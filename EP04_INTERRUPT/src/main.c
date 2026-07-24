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
    /* Falling Edge: ISC01 = 1 และ ISC00 = 0 */
    EICRA = (EICRA & ~((1U << ISC01) | (1U << ISC00))) |
            (1U << ISC01);

    EIFR = (1U << INTF0);  /* ล้าง Flag ที่อาจค้างอยู่ */
    EIMSK |= (1U << INT0); /* เปิด External Interrupt 0 */
}

int main(void)
{
    DDRB |= (1U << DDB5);
    PORTB &= ~(1U << PORTB5);

    DDRD &= ~(1U << DDD2);
    PORTD |= (1U << PORTD2); /* เปิด Internal Pull-up ที่ D2 */

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
