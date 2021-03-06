/*
 * Code adapted from https://abratukhin.blogspot.com.br/2015/04/connect-atmega328-with-hc-05-zs-040.html
 * Author: Andrey Bratukhin
 * No licence
 */

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include <util/delay.h>

void uart_init(uint32_t baudrate, uint8_t double_speed);
uint8_t uart_getchar();
void uart_read_line(uint8_t *value, uint8_t size);
uint8_t uart_read_line_int(uint8_t *value, uint8_t size, volatile uint8_t *pin, uint8_t pin_num, uint8_t level);
void uart_putchar(const uint8_t data);
void uart_print(const char *value);
