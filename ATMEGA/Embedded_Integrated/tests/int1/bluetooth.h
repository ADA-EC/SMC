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

void bluetooth_init(uint32_t baudrate, uint8_t double_speed);
uint8_t bluetooth_getchar();
void bluetooth_read_line(uint8_t *value, uint8_t size);
void bluetooth_putchar(const uint8_t data);
void bluetooth_print(const char *value);
