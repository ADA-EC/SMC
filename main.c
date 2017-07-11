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
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"

#include "ds18b20/ds18b20.h"

#define UART_BAUD_RATE 2400
#include "uart/uart.h"

int main(void) {
	char printbuff[100];
	double d = 0;

	//init uart
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );

    lcd_init();

	sendInst(_LCD_ON | _LCD_CURSOR_ON | _LCD_CURSOR_BLINK);
	sendInst(_LCD_CLR);
	sendInst(_LCD_HOME);

	//init interrupt
	sei();

	while(1) {
		double temp;
        char printbuff[21];

        temp = ds18b20_gettemp();

		dtostrf(temp, 10, 3, printbuff);

        writeStringXY(printbuff, 0, 0);
		_delay_ms(100);
	}

	return 0;
}
