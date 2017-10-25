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
#include "adc0832.h"

//Inverse of clock frequency (in us)
#define _adc0832_pulse_width 20

void _adc0832_pulse() {
    PORTD |= _BV(PORTD5);
    _delay_us(_adc0832_pulse_width);
    PORTD &= ~_BV(PORTD5);
}

void _adc0832_send_bit(uint8_t bit) {
    if (bit) {
        PORTD |= _BV(PORTD7);
    } else {
        PORTD &= ~_BV(PORTD7);
    }
    _delay_us(_adc0832_pulse_width);
    _adc0832_pulse();
}

uint8_t _adc0832_recv_bit() {
    uint8_t bit;
    _adc0832_pulse();
    _delay_us(_adc0832_pulse_width);
    bit = (PINB & _BV(PIND6));
}

uint8_t adc0832_convert() {
    uint8_t bit;
    uint8_t msb = 0;
    uint8_t lsb = 0;
    int i;

    //_adc0832_pulse_width = (uint8_t) (1e3/freq);

    //Init Conversion
    PORTD &= ~_BV(PORTD5); //Clock init at low level
    PORTB &= ~_BV(PORTB7); //Select chip
    _adc0832_send_bit(1);  //Start bit
    _adc0832_send_bit(0);  //SGL/~DIF bit (Select Single-ended)
    _adc0832_send_bit(0);  //ODD/SIGN bit (Select CH0)

    //Read MSB
    for(i = 0; i < 8; i++) {
        bit = _adc0832_recv_bit();
        msb <<= 1;
        msb |= bit;
    }

    //Read LSB
    for(i = 0; i < 8; i++) {
        bit = _adc0832_recv_bit();
        lsb <<= 1;
        lsb |= bit;
    }

    PORTB |= _BV(PORTB7); //Deselect chip

    //Check if msb and lsb match
    return msb;
    //return (msb == lsb) ? lsb : 0;
}

/*
int main() {
    DDRB |= _BV(DDB7);  //Sets PB7 as output (~CS)
    DDRD |= _BV(DDD5);  //Sets PD5 as output (CLK)
    DDRD &= ~_BV(DDD6); //Sets PD6 as input  (DO)
    DDRD |= _BV(DDD7);  //Sets PD7 as output (DI)

    PORTB |= _BV(PORTB7); //ADC idle



    return 0;
}
*/
