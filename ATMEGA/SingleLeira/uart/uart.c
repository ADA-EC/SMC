/*
 * Code adapted from https://abratukhin.blogspot.com.br/2015/04/connect-atmega328-with-hc-05-zs-040.html
 * Author: Andrey Bratukhin
 * No licence
 */

#include "uart.h"

void uart_init(uint32_t baudrate, uint8_t double_speed) {
    uint16_t ubrr = 0;
    if (double_speed) {
        UCSR0A = _BV(U2X0);  //Enable 2x speed
        ubrr = (F_CPU / (8UL * baudrate)) - 1;
    } else {
        ubrr = (F_CPU / (16UL * baudrate)) - 1;
    }
    UBRR0H = ubrr >> 8;
    UBRR0L = ubrr;

   UCSR0C &= ~(_BV(UMSEL01) | _BV(UMSEL00)); // enable asynchronous USART
   UCSR0C &= ~(_BV(UPM01) | _BV(UPM00)); // disable parity mode
   UCSR0C &= ~_BV(USBS0); // set 1-bit stop
   UCSR0C &= ~_BV(UCSZ02);
   UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00); // set 8-bit data

   UCSR0B = _BV(RXEN0) | _BV(TXEN0);   // Enable RX and TX
}


uint8_t uart_getchar() {
	loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

uint8_t uart_getchar_int(volatile uint8_t *pin, uint8_t pin_num, uint8_t level) {
    do {
	    uint8_t value = (*pin & _BV(pin_num));
	    if (level && value) {
	           return 0x18; //ascii symbol 'cancel'
	    } else {
	       if (!(value)) {
	           return 0x18; //ascii symbol 'cancel'
	       }
	    }
	} while(bit_is_clear(UCSR0A, RXC0));
    return UDR0;
}

uint8_t uart_read_line_int(uint8_t *value, uint8_t size, volatile uint8_t *pin, uint8_t pin_num, uint8_t level) {
    uint8_t i;
    for (i = 0; i < size; i++) {
        uint8_t c = uart_getchar_int(pin, pin_num, level);
        if (c == 0x18) {
            return 1;
        }
        value[i] = c;
        if (value[i] == '\r') {
            value[i] = '\0';
            break;
        } else if (value[i] == '\n') {
            value[i] = '\0';
            break;
        }
    }
    return 0;
}

void uart_putchar(const uint8_t data) {
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = data;
}

void uart_print(const char *value) {
    while (*value != '\0') {
        uart_putchar(*value++);
    }
}
