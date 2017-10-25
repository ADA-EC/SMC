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
 * Henrique Andrews (andrews.marques225@gmail.com)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>
#include "adc0832_alt/adc0832.h"
#include "lcd/lcd.h"

int main() {
    //Inicializa o lcd
	lcd_init();

	sendInst(_LCD_ON | _LCD_CURSOR_ON | _LCD_CURSOR_BLINK);
	sendInst(_LCD_CLR);
	sendInst(_LCD_HOME);

    //Inicializa adc0832
    adc0832_init();

    uint8_t msb, lsb;
    char buffer[20];
    memset(buffer, 0, 20);
    while(1) {
        msb = adc0832_convert(&lsb);
        sprintf(buffer, "0x%02x", msb);
        writeStringXY(buffer, 0, 0);
        sprintf(buffer, "0x%02x", lsb);
        writeStringXY(buffer, 0, 1);
        _delay_ms(200);
    }

    return 0;
}
