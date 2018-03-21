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
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ds3231/ds3231.h"

// Interrupt 0 handler
ISR (INT0_vect)
{
    PORTB ^= _BV(PORTB0);
}

int main(void) {
    /* Pin configuration */
    DDRD &= ~_BV(DDD2); // PD2 (PCINT0 pin) is now an input

    PORTD |= _BV(PORTD2); // PD2 is now an input with pull-up enabled

    DDRB |= _BV(DDB0); //Set PB0 as output (DEBUG LED)

    /* Interruption setup*/
    // Set INT0 to trigger on falling edge
    EICRA &= ~_BV(ISC00);
    EICRA |= _BV(ISC01);

    EIMSK |= _BV(INT0); // Turns on INT0

    /* Setup code */

    //Turns off DEBUG LED
    PORTB &= ~_BV(PORTB0);

    //Interruptions must be enabled to configure RTC
	sei();
    // RTC
    twi_init_master();
    rtc_set_time(0,19,07);
    rtc_SQW_enable(false);
    rtc_enable_alarm(true);
    rtc_set_alarm(0b1111, 23, 19, 5);

    /* Loop code */
	while(true) {
        rtc_check_alarm(); //Clear alarm flag if set
    }

	return 0;
}
