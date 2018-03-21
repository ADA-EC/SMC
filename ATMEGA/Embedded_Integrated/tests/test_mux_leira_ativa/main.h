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
 * Amador M. de Souza Neto (amador.neto@usp.br)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>


#define PORTLED PORTB
#define PINLED  PORTB0
#define PREPLED() DDRB |= _BV(PINLED)
#define AcendeLED() PORTLED |= _BV(PINLED)
#define ApagaLED() PORTLED &= ~_BV(PINLED)

#define SELECT0_DDR DDRD
#define SELECT0_PORT PORTD
#define SELECT0_BIT PORTD6

#define SELECT1_DDR DDRD
#define SELECT1_PORT PORTD
#define SELECT1_BIT PORTD7

#define MUX_LEIRAS_DDR DDRD
#define MUX_LEIRAS_PIN PIND
#define MUX_LEIRAS_BIT PIND5
