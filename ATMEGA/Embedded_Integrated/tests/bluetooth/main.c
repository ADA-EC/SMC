/*
 * Sistema de Monitoramento de Compostagem
 *
 * Tem como proposito monitorar as leiras do grupo GEISA (USP-SC)
 *
 * Copyright (C) 2017 ADA-Projetos em Engenharia de Computacao
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Pedro V. B. Jeronymo (pedrovbj@gmail.com)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bluetooth.h"
#include <avr/io.h>

#define PORTLED PORTB
#define PINLED  PORTB0
#define PREPLED() DDRB |= _BV(PINLED)
#define ApagaLED() PORTLED &= ~_BV(PINLED)
#define AcendeLED() PORTLED |= _BV(PINLED)

int main(void) {
    PREPLED();
    bluetooth_init(38400, 0);

	//init interrupt
	sei();

    bluetooth_print("AT+NAME=GEISA\r\n");
    _delay_ms(1000);

    bluetooth_print("AT+PSWD=1234\r\n");
    _delay_ms(1000);

    bluetooth_print("AT+bluetooth=38400,1,0\r\n");
    _delay_ms(1000);

	while(1) {
        bluetooth_print("Beep!\r\n");
        AcendeLED();
        _delay_ms(500);
        ApagaLED();
        _delay_ms(500);
	}

	return 0;
}
