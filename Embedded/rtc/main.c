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
#include "bluetooth.h"
#include "librtc/twi.h"
#include "librtc/rtc.h"

ISR (INT0_vect)
{
    PORTD ^= _BV(PORTD3);
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

    DDRD |= _BV(DDD3); //Set PD3 as output


    EICRA |= (1 << ISC00);
    EICRA |= (1 << ISC01);    // set INT0 to trigger on rising edge

    EIMSK |= (1 << INT0);     // Turns on INT0
	
  //  bluetooth_init(38400, 1);

	//init interrupt
	sei();

    //Troca nome transmitido pelo modulo para BLUES
    bluetooth_print("AT+NAME=BLUES\r\n");
    _delay_ms(1000);

    //Troca o pin de pareamento para 0000
    bluetooth_print("AT+PSWD=0000\r\n");
    _delay_ms(1000);

    //Define o baud rate
    bluetooth_print("AT+UART=38400,1,0\r\n");
    _delay_ms(1000);

    twi_init_master();
	rtc_init();

//    sprintf(buf, "%s\r\n", rtc_is_ds1307() ? "ds1307" : "ds3231");
//    bluetooth_print(buf);
    


//    rtc_set_time_s(13,1,23);

    PORTD &= ~_BV(PORTD3);

    //rtc_SQW_enable(true);
    //rtc_set_alarm_once_per_sec();
    
    //cli();


	while(1) {
bluetooth_print("foo\r\n");
    _delay_ms(1000);
    }

	return 0;
}
