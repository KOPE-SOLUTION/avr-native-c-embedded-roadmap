#include <avr/io.h>

#define LED_BIT     PORTB5 /* Uno D13 / PB5 */
#define BUTTON_BIT  PIND2  /* Uno D2 / PD2 */

int main(void)
{
    /* PB5 output: onboard LED. */
    DDRB |= (1U << DDB5);

    /* PD2 input with internal pull-up. */
    DDRD &= ~(1U << DDD2);
    PORTD |= (1U << PORTD2);

    while (1) {
        if ((PIND & (1U << BUTTON_BIT)) == 0U) {
            PORTB |= (1U << LED_BIT);
        } else {
            PORTB &= ~(1U << LED_BIT);
        }
    }
}
