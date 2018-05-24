/*
 * Programa para acrescentar dados em um cartão SD
 *
 * Copyright (C) 2018 ADA-Projetos em Engenharia de Computacao
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
 * Amador Marcelino de Souza Neto (amador.neto@usp.br)
 * Pedro V. B. Jeronymo (pedrovbj@gmail.com)
 */

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SD/ff.h"
#include "SD/integer.h"

FATFS FatFs;		// Área de trabalho do FAtFs
FIL *fp;		// Estrutura de File

DWORD get_fattime (void)
{
	// Retorna o dia e hora configurado como DWORD
	return	  ((DWORD)(2017 - 1980) << 25)	// Ano 2017
	| ((DWORD)10 << 21)				// Mes 10
	| ((DWORD)26 << 16)				// Dia 26
	| ((DWORD)16 << 11)				// Hora 20 (considerando fuso horario 0)
	| ((DWORD)31 << 5)				// Minuto 0
	| ((DWORD)0 >> 1);				// Segundo 0
}

int main (){
    //Configura como saída
    DDRB |= _BV(DDB0) ;

    //Vref = AVcc
	ADMUX = (1 << REFS0) | _BV(ADLAR);
	//Habilita a conversão AD
	ADCSRA  = (1 << ADEN) ;

    //Define a saída da porta D6 como nível lógico alto
	PORTB |= _BV(PORTB0) ;

    // reboot delay
	_delay_ms(2000);

    //Define a saída da porta B0 como nível lógico baixo
    PORTB &= ~_BV(PORTB0);


    UINT bw;
    f_mount(0, &FatFs);		// Monta o cartão e fornece uma area de trabalho FatFs ao modulo

	_delay_ms(500);

    // cria o ponteiro fp para referenciar o arquivo a ser aberto
    fp = (FIL *)malloc(sizeof (FIL));

    if (f_open(fp, "testemetano.txt", FA_WRITE | FA_CREATE_ALWAYS)!=FR_OK) {	// Cria um arquivo
        while(1) {
            PORTB ^= _BV(PORTB0) ;
            _delay_ms(100);
        }
    }

    int leitura_0_h, leitura_0_l;
    int leitura_1_h, leitura_1_l;
    char text[200];
    int i;
    for(i = 0; i < 3; i++) {
        PORTB |= _BV(PORTB0);
        // Le canal 0
        ADMUX &= ~_BV(MUX0);
        _delay_ms(20);
        //Inicia a conversão AD
		ADCSRA |= (1 << ADSC) ;
		//Espera a conversão terminar
		while (!(ADCSRA & (1 << ADIF))) ;
        leitura_0_h = ADCH;
        leitura_0_l = ADCL >> 6;
        // Le canal 1
        ADMUX |= _BV(MUX0);
        _delay_ms(20);
        //Inicia a conversão AD
		ADCSRA |= (1 << ADSC) ;
		//Espera a conversão terminar
		while (!(ADCSRA & (1 << ADIF))) ;
        leitura_1_h = ADCH;
        leitura_1_l = ADCL >> 6;
        sprintf(text, "CH0: 0x%x%x CH1:0x%x%x\n", leitura_0_h, leitura_0_l, leitura_1_h, leitura_1_l);
        f_write(fp, text, strlen(text), &bw);	//escreve a string 'text'no arquivo
        _delay_ms(1000);
        PORTB &= ~_BV(PORTB0);
        _delay_ms(1000);
    }

    f_close(fp);// fecha o arquivo

	// desmonta o cartão
	f_mount(0, &FatFs);


	while(1) {
		PORTB ^= _BV(PORTB0) ;
		_delay_ms(500);
	}

	return 0;
}
