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
#include <string.h>
#include "bluetooth.h"

int main() {
	bluetooth_init(9600,0);

    while(1) {
        bluetooth_print("Hello World!\n");
        _delay_ms(1000);
    }

    return 0;
}
