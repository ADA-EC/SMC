/*
 * Programa para ler sinal analógico e exibir em LCD
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
 * Amador Marcelino de Souza Neto (amador.neto@usp.br)
 *
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "lcd.h"

//Ligar o sinal analógico em PC0


int main() {
	
	//Vref = AVcc; Bits ajustados a esquerda(haverão 8 bits em ADCH e 2 em ADCL); Canal 0
	ADMUX = (1 << REFS0) | (1 << ADLAR) ;
	//Habilita a conversão AD
	ADCSRA  = (1 << ADEN) ;
	
	//inicializa o lcd
	lcd_init();
	
	sendInst(_LCD_ON | _LCD_CURSOR_ON | _LCD_CURSOR_BLINK);
	sendInst(_LCD_CLR);
	sendInst(_LCD_HOME);
	
	while (1) {
		double leitura;
        char printbuff[21];

		//Inicia a conversão AD
		ADCSRA |= (1 << ADSC) ;
		//Espera a conversão terminar
		while (!(ADCSRA & (1 << ADIF))) ;
		
		//Atribue o valor da conversão à variável leitura
		leitura = ADCH ;
		
		//Converte a variável leitura (tipo double) para string 'printbuff'
		dtostrf(leitura, 10, 3, printbuff);
		
		//Manda para o LCD a string 'printbuff'
		writeStringXY(printbuff, 0, 0);
		
		//Espera um pouco antes da próxima conversão
		_delay_ms(100) ;
	}
	return 0 ;
}
