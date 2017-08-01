#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include <util/delay.h>

void uart_init(uint32_t baudrate, uint8_t double_speed);
uint8_t uart_getchar();
void uart_read_line(uint8_t *value, uint8_t size);
void uart_putchar(const uint8_t data);
void uart_print(const char *value);
