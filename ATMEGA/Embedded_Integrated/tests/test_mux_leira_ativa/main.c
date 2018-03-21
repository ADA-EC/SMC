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
#include "main.h"

//(DEBUG)
void blink_led() {
    int i;
    for (i = 0; i < 5; i++) {
        AcendeLED();
        _delay_ms(200);
        ApagaLED();
        _delay_ms(200);
    }
}

int main(void) {
    /* Pin configuration */
    PREPLED(); //(DEBUG LED)
    SELECT0_DDR |= _BV(SELECT0_BIT);
    SELECT1_DDR |= _BV(SELECT1_BIT);
    MUX_LEIRAS_DDR &= ~_BV(MUX_LEIRAS_BIT);



    /* Setup code */

    blink_led();
    _delay_ms(500);


    /* Loop code */
	while(1) {
        uint8_t i;
        for (i = 0; i < 4; i++) {
            // Seleciona a leira
            int lsb = i & 0x01;
            int msb = i & 0x02;
            SELECT0_PORT = (lsb) ? (SELECT0_PORT | _BV(SELECT0_BIT)) : (SELECT0_PORT & ~_BV(SELECT0_BIT));
            SELECT1_PORT = (msb) ? (SELECT1_PORT | _BV(SELECT1_BIT)) : (SELECT1_PORT & ~_BV(SELECT1_BIT));

        if (MUX_LEIRAS_PIN & _BV(MUX_LEIRAS_BIT)) {
            AcendeLED();
        } else {
            ApagaLED();
        }
        _delay_ms(500);
        }
    }

	return 0;
}
