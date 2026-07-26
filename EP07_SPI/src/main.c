#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <util/delay.h>

#define BAUD_RATE   9600UL
#define UBRR_VALUE  ((F_CPU / (16UL * BAUD_RATE)) - 1UL)

#define SD_CS_LOW()   (PORTB &= ~(1U << PORTB2))
#define SD_CS_HIGH()  (PORTB |= (1U << PORTB2))

static void uart_init(void){
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8U);
    UBRR0L = (uint8_t)UBRR_VALUE;
    UCSR0A = 0U;
    UCSR0B = (1U << TXEN0);
    UCSR0C = (1U << UCSZ01) | (1U << UCSZ00);
}

static void uart_putchar(char character){
    if (character == '\n'){
        while ((UCSR0A & (1U << UDRE0)) == 0U){
            /* รอให้ Register พร้อม */
        }
        UDR0 = (uint8_t)'\r';
    }

    while ((UCSR0A & (1U << UDRE0)) == 0U){
        /* รอให้ Register พร้อม */
    }
    UDR0 = (uint8_t)character;
}

static void uart_puts(const char *text){
    while (*text != '\0'){
        uart_putchar(*text);
        text++;
    }
}

static void uart_print_hex8(uint8_t value){
    static const char hex[] = "0123456789ABCDEF";
    uart_putchar(hex[value >> 4U]);
    uart_putchar(hex[value & 0x0FU]);
}

static void spi_init_slow(void){
    /* กำหนด SS, MOSI และ SCK เป็น Output; MISO เป็น Input */
    DDRB |= (1U << DDB2) | (1U << DDB3) | (1U << DDB5);
    DDRB &= ~(1U << DDB4);
    SD_CS_HIGH();

    /* เปิด SPI, Controller/Master, Mode 0, MSB First, f_CPU / 128 */
    SPCR = (1U << SPE) | (1U << MSTR) | (1U << SPR1) | (1U << SPR0);
    SPSR &= ~(1U << SPI2X);
}

static uint8_t spi_transfer(uint8_t outgoing){
    SPDR = outgoing;
    while ((SPSR & (1U << SPIF)) == 0U){
        /* รอส่ง/รับข้อมูลครบ 8 bit */
    }
    return SPDR;
}

static void sd_deselect(void){
    SD_CS_HIGH();
    (void)spi_transfer(0xFFU); /* ส่ง Clock อีก 8 รอบหลัง CS เป็น HIGH */
}

static bool sd_wait_ready(void){
    uint16_t attempt;

    for (attempt = 0U; attempt < 1000U; attempt++){
        if (spi_transfer(0xFFU) == 0xFFU){
            return true;
        }
    }
    return false;
}

static uint8_t sd_command(uint8_t command, uint32_t argument, uint8_t crc){
    uint8_t response = 0xFFU;
    uint8_t attempt;

    sd_deselect();
    SD_CS_LOW();

    if (!sd_wait_ready()){
        sd_deselect();
        return 0xFFU;
    }

    (void)spi_transfer((uint8_t)(0x40U | command));
    (void)spi_transfer((uint8_t)(argument >> 24U));
    (void)spi_transfer((uint8_t)(argument >> 16U));
    (void)spi_transfer((uint8_t)(argument >> 8U));
    (void)spi_transfer((uint8_t)argument);
    (void)spi_transfer(crc);

    for (attempt = 0U; attempt < 10U; attempt++){
        response = spi_transfer(0xFFU);
        if ((response & 0x80U) == 0U){
            break;
        }
    }

    return response;
}

int main(void){
    uint8_t response = 0xFFU;
    uint8_t attempt;

    uart_init();
    spi_init_slow();
    uart_puts("SPI SD CMD0 test\n");

    _delay_ms(10.0);

    /* ส่งอย่างน้อย 74 Clock ขณะ CS เป็น HIGH ก่อน SD Command แรก */
    SD_CS_HIGH();
    for (attempt = 0U; attempt < 10U; attempt++){
        (void)spi_transfer(0xFFU);
    }

    for (attempt = 0U; attempt < 10U; attempt++){
        response = sd_command(0U, 0UL, 0x95U); /* CMD0 พร้อม CRC ที่กำหนด */
        sd_deselect();

        if (response == 0x01U){
            break;
        }
        _delay_ms(10.0);
    }

    if (response == 0x01U){
        uart_puts("SD entered idle state (R1=0x01)\n");
    } else{
        uart_puts("CMD0 failed, last R1=0x");
        uart_print_hex8(response);
        uart_putchar('\n');
    }

    while (1){
        /* คงผล Handshake ไว้ให้ตรวจสอบใน Terminal */
    }
}
