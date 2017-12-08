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
#include "LowPower/LowPower.h"

ISR (INT0_vect)
{
    // Interrupt handler
}

int main(void) {
    DDRD &= ~(1 << DDD2);     // Clear the PD3 pin
    // PD3 (PCINT1 pin) is now an input

    PORTD |= (1 << PORTD2);    // turn On the Pull-up
    // PD3 is now an input with pull-up enabled

    DDRB |= _BV(DDB0); //Set PC3 as output

    EICRA &= ~(1 << ISC10);
    EICRA &= ~(1 << ISC11);    // set INT1 to trigger on low level

    EIMSK |= (1 << INT0);     // Turns on INT1

    // Turns off led on PC3
    PORTB &= ~_BV(PORTB0);

	while(1) {
        // Enable interrupts
        sei();

        //Power Down
        powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

        // Disable interrupts
        cli();

        //Blink led
        int i;
        for (i = 0; i < 5; i++) {
            PORTB |= _BV(PORTB0);
            _delay_ms(200);
            PORTB &= ~_BV(PORTB0);
            _delay_ms(200);
        }
    }

	return 0;
}
