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
#include "adc0832/adc0832.h"
#include "bluetooth/bluetooth.h"

int main() {
    DDRB |= _BV(DDB7);  //Sets PB7 as output (~CS)
    DDRD |= _BV(DDD5);  //Sets PD5 as output (CLK)
    DDRD &= ~_BV(DDD6); //Sets PD6 as input  (DO)
    DDRD |= _BV(DDD7);  //Sets PD7 as output (DI)
    DDRC |= _BV(DDC3); //Set PC3 as output (DEBUG LED)

    //Config
    PORTB |= _BV(PORTB7); //ADC idle
    bluetooth_init(38400, 1);

    uint8_t conv;
    char buffer[20];
    while(1) {
        conv = adc0832_convert();
        sprintf(buffer, "0x%x\r\n", conv);
        bluetooth_print(buffer);
        _delay_ms(1000);
    }

    return 0;
}
