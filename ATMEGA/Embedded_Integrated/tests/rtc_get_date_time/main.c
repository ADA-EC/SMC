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
#include "bluetooth/bluetooth.h"

// Interrupt 0 handler
ISR (INT0_vect)
{
    PORTC ^= _BV(PORTC3);
}

int main(void) {

    //Interruptions must be enabled to configure RTC
	sei();
    
    bluetooth_init(38400, 1);
    // RTC
    twi_init_master();
    rtc_set_date_time(0,19,7,8,11,17);
    rtc_SQW_enable(false);
    rtc_enable_alarm(true);
    rtc_set_alarm(0b1111, 23, 19, 5);
    
    char linha[15];
    
    uint8_t hora, minuto, segundo, dia, mes, ano;

    /* Loop code */
	while(true) {
        rtc_check_alarm(); //Clear alarm flag if set
        
        rtc_get_date_time(&hora, &minuto, &segundo, &dia, &mes, &ano);
        
        sprintf(linha,"%d:%d:%d  %d/%d/%d\n", hora, minuto, segundo, dia, mes, ano);
        
        bluetooth_print(linha);
        
        _delay_ms(1000);
        
        
    }

	return 0;
}
