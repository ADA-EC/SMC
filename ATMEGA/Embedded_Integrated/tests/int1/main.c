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

ISR (INT1_vect)
{
    PORTC |= _BV(PORTC3);
}

int main(void) {
    
    DDRD &= ~(1<<DDD3);     // Clear the PD3 pin
                            // PD3 (PCINT1 pin) is now an input
    DDRC |= (1<<DDC3);      // Sets PC3 as output

    PORTD |= (1 << PORTD3);    // turn On the Pull-up
    // PD3 is now an input with pull-up enabled

    EICRA &= ~(1 << ISC10);
    EICRA &= ~(1 << ISC11);    // set INT1 to trigger on low level

    EIMSK |= (1 << INT1);     // Turns on INT1

	while(1) {
        cli();
        PORTC &= ~_BV(PORTC3);
        sei();
        _delay_ms(300);        
    }

	return 0;
}
