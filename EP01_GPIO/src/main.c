#include <avr/io.h>

/*
 * ชื่อด้านซ้ายเป็นชื่อที่กำหนดขึ้นสำหรับตัวอย่างนี้
 * ชื่อด้านขวามาจาก Device Header ของ ATmega328P
 */
#define LED_DDR_BIT          DDB5   /* Uno D13: bit กำหนดทิศทางของ PB5 */
#define LED_OUTPUT_BIT       PORTB5 /* Uno D13: bit ขาออกของ PB5 */
#define BUTTON_DDR_BIT       DDD2   /* Uno D2: bit กำหนดทิศทางของ PD2 */
#define BUTTON_PULLUP_BIT    PORTD2 /* Uno D2: bit เปิด Pull-up ของ PD2 */
#define BUTTON_INPUT_BIT     PIND2  /* Uno D2: bit อ่านขาเข้าของ PD2 */

int main(void){
    /* 1. กำหนด PB5 (Uno D13) เป็น Output สำหรับ LED บนบอร์ด */
    DDRB |= (1U << LED_DDR_BIT);

    /* 2. กำหนด PD2 (Uno D2) เป็น Input และเปิด Internal Pull-up */
    DDRD &= ~(1U << BUTTON_DDR_BIT);
    PORTD |= (1U << BUTTON_PULLUP_BIT);

    while (1){
        /* Pull-up ทำให้การกดปุ่มอ่านได้ LOW หรือ Active-low */
        if ((PIND & (1U << BUTTON_INPUT_BIT)) == 0U){
            PORTB |= (1U << LED_OUTPUT_BIT);   /* กำหนด PB5 เป็น 1: เปิด LED */
        } else{
            PORTB &= ~(1U << LED_OUTPUT_BIT); /* ล้าง PB5 เป็น 0: ปิด LED */
        }
    }
}
