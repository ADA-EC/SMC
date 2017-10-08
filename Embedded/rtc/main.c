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
#include "librtc/twi.h"
#include "librtc/rtc.h"

ISR (INT0_vect)
{
    //Interruption handler
    PORTC ^= _BV(PORTC3);
}

int main(void) {
    struct tm* t = NULL;
    char buf[32];
	uint8_t hour, min, sec;
    uint8_t input = 0;

    DDRD &= ~(1 << DDD2);     // Clear the PD2 pin
    // PD2 (PCINT0 pin) is now an input

    PORTD |= (1 << PORTD2);    // turn On the Pull-up
    // PD2 is now an input with pull-up enabled

    DDRC |= _BV(DDC3); //Set PC3 as output

    // set INT0 to trigger on LOW level
    EICRA &= ~_BV(ISC00);
    EICRA &= ~_BV(ISC01);

    EIMSK |= (1 << INT0);     // Turns on INT0


    PORTC &= ~_BV(PORTC3);
	//init interrupt
	sei();

    twi_init_master();
	rtc_init();
    rtc_set_time_s(0,0,0);
    PORTC |= _BV(PORTC3);

	while(1) {
    }

	return 0;
}
