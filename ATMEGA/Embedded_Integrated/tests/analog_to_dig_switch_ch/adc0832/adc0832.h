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
#ifndef ADC0832_H
#define ADC0832_H
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/io.h>

#define _ADC0832_CS_DDR DDRC
#define _ADC0832_CS_PORT PORTC
#define _ADC0832_CS PORTC2

#define _ADC0832_CLK_DDR DDRC
#define _ADC0832_CLK_PORT PORTC
#define _ADC0832_CLK PORTC3

#define _ADC0832_DO_DDR DDRC
#define _ADC0832_DO_PIN PINC
#define _ADC0832_DO PINC4

#define _ADC0832_DI_DDR DDRC
#define _ADC0832_DI_PORT PORTC
#define _ADC0832_DI PORTC5

#define _ADC0832_PULSE_WIDTH 20
#define _ADC0832_DELAY_WRITE 20
#define _ADC0832_DELAY_MSB 20
#define _ADC0832_DELAY_LSB 20

#define ADC0832_DELAY_AVR_CONV 100

/*
#define _ADC0832_PULSE_WIDTH 95
#define _ADC0832_DELAY_WRITE 263
#define _ADC0832_DELAY_MSB 1575
#define _ADC0832_DELAY_LSB 630
*/

void    adc0832_init();
uint8_t adc0832_convert(uint8_t* lsb_ret, int ch); //*lsb, channel (0 or 1)
uint8_t adc0832_average_conv(int num_conv, int ch);


#endif
