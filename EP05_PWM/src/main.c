#include <avr/io.h>
#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <util/delay.h>

static void pwm1a_init_fast_8bit(void){
    DDRB |= (1U << DDB1); /* กำหนด OC1A / Uno D9 เป็น Output */

    /* 8-bit Fast PWM แบบ Non-inverting ที่ OC1A */
    TCCR1A = (1U << COM1A1) | (1U << WGM10);
    TCCR1B = (1U << WGM12) | (1U << CS11) | (1U << CS10);
    OCR1A = 0U;
}

int main(void){
    int16_t duty = 0;
    int8_t step = 5;

    pwm1a_init_fast_8bit();

    while (1){
        OCR1A = (uint8_t)duty;
        _delay_ms(30.0);

        duty += step;
        if (duty >= 255){
            duty = 255;
            step = -5;
        } else if (duty <= 0){
            duty = 0;
            step = 5;
        }
    }
}
