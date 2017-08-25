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
#include "lcd.h"
#include "rtc/twi.h"
#include "rtc/rtc.h"

int main(void) {
    struct tm* t = NULL;
    char buf[32];
	uint8_t hour, min, sec;
    uint8_t input = 0;


    DDRD &= ~_BV(DDD2); //RTC INT

    lcd_init();

	sendInst(_LCD_ON | _LCD_CURSOR_ON | _LCD_CURSOR_BLINK);
	sendInst(_LCD_CLR);
	sendInst(_LCD_HOME);

	//init interrupt
	sei();

    twi_init_master();
	rtc_init();
	//rtc_set_time_s(19, 8, 0);

	while(1) {
        //t = rtc_get_time();
		//sprintf(buf, "%02d:%02d:%02d", t->hour, t->min, t->sec);
        //writeStringXY(buf, 0, 0);
        rtc_set_alarm_s(0, 0, 1);
        input = (PIND & _BV(PIND2));
        sprintf(buf, "%d", input);
        writeStringXY(buf, 0, 0);
        _delay_ms(100);
	}

	return 0;
}
