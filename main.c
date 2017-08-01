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
#include "bluetooth.h"

#include "ds18b20/ds18b20.h"

int main(void) {
	char printbuff[100];
	double d = 0;

    lcd_init();

	sendInst(_LCD_ON | _LCD_CURSOR_ON | _LCD_CURSOR_BLINK);
	sendInst(_LCD_CLR);
	sendInst(_LCD_HOME);

    uart_init(38400, 1);

	//init interrupt
	sei();

    uart_print("AT+NAME=BLUES\r\n");
    _delay_ms(1000);

    uart_print("AT+PSWD=0000\r\n");
    _delay_ms(1000);

    uart_print("AT+UART=38400,1,0\r\n");
    _delay_ms(1000);

	while(1) {
		double temp;
        char printbuff[21];

        temp = ds18b20_gettemp();

		dtostrf(temp, 6, 3, printbuff);
        writeStringXY(printbuff, 0, 0);

        printbuff[6] = '\r';
        printbuff[7] = '\n';
        printbuff[8] = '\0';

        uart_print(printbuff);
	}

	return 0;
}
