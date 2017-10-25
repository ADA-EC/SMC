/*
A python library to use the ADC0832 analog to digital converter with a Raspberry Pi
# Copyright (C) 2016  Sahithyen Kanaganayagam
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * Addapted to C code for the atmega328p by
 * ADA - Projetos em Engenharia de Computacao
 *
 * Author: Pedro V. B. Jeronymo (pedrovbj@gmail.com)
*/

#include "adc0832.h"

void adc0832_init() {
    _ADC0832_CS_DDR  |= _BV(_ADC0832_CS);  //Sets PB7 as output (~CS)
    _ADC0832_CLK_DDR |= _BV(_ADC0832_CLK); //Sets PD5 as output (CLK)
    _ADC0832_DO_DDR  &= ~_BV(_ADC0832_DO); //Sets PD6 as input  (DO)
    _ADC0832_DI_DDR  |= _BV(_ADC0832_DI);  //Sets PD7 as output (DI)

    // Deselect chip
    _ADC0832_CS_PORT |= _BV(_ADC0832_CS);
}

uint8_t adc0832_convert(uint8_t* lsb_ret) {
    int i;
    uint8_t bit;
    uint8_t msb = 0;
    uint8_t lsb = 0;

    /* Select chip */
    _ADC0832_CS_PORT &= ~_BV(_ADC0832_CS);

    /* Request data */
    // Start bit
    _ADC0832_DI_PORT |= _BV(_ADC0832_DI);
    _delay_us(_ADC0832_DELAY_WRITE);
    _ADC0832_CLK_PORT |= _BV(_ADC0832_CLK);
    _delay_us(_ADC0832_PULSE_WIDTH);
    _ADC0832_CLK_PORT &= ~_BV(_ADC0832_CLK);

    // SGL/~DIF bit
    _ADC0832_DI_PORT |= _BV(_ADC0832_DI);
    //_ADC0832_DI_PORT &= ~_BV(_ADC0832_DI);
    _delay_us(_ADC0832_DELAY_WRITE);
    _ADC0832_CLK_PORT |= _BV(_ADC0832_CLK);
    _delay_us(_ADC0832_PULSE_WIDTH);
    _ADC0832_CLK_PORT &= ~_BV(_ADC0832_CLK);

    // ODD/SIGN bit
    _ADC0832_DI_PORT &= ~_BV(_ADC0832_DI);
    _delay_us(_ADC0832_DELAY_WRITE);
    _ADC0832_CLK_PORT |= _BV(_ADC0832_CLK);
    _delay_us(_ADC0832_PULSE_WIDTH);
    _ADC0832_CLK_PORT &= ~_BV(_ADC0832_CLK);

    /* Read data */
    // Read MSB data
    for(i = 0; i < 8; i++) {
        _ADC0832_CLK_PORT |= _BV(_ADC0832_CLK);
        _delay_us(_ADC0832_PULSE_WIDTH);
        _ADC0832_CLK_PORT &= ~_BV(_ADC0832_CLK);
        _delay_us(_ADC0832_DELAY_MSB);
        msb <<= 1;
        if(bit = (_ADC0832_DO_PIN & _BV(_ADC0832_DO))) {
            msb |= 0x01;
        }
    }

    // Read LSB data
    for(i = 0; i < 8; i++) {
        lsb >>= 1;
        if(bit = (_ADC0832_DO_PIN & _BV(_ADC0832_DO))) {
            lsb |= 0x80;
        }
        _ADC0832_CLK_PORT |= _BV(_ADC0832_CLK);
        _delay_us(_ADC0832_PULSE_WIDTH);
        _ADC0832_CLK_PORT &= ~_BV(_ADC0832_CLK);
        _delay_us(_ADC0832_DELAY_LSB);
    }

    /* Deselect chip */
    _ADC0832_CS_PORT |= _BV(_ADC0832_CS);

    /* Check if lsb and msb match */
    //return (lsb == msb) ? lsb : 0;
    *lsb_ret = lsb;
    return msb;
}
