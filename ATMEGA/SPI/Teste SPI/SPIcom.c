/*
 * Programa para realizar comunicação SPI
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
#include <stdio.h>
#include <util/delay.h>
#include "bluetooth/bluetooth.h"

//Neste programa exemplo será usado o pino B3 como MOSI(Master Out Slave In), B4 como MISO(Master In Slave Out),
//B5 como SCK(SPI Clock), C0 como USS(Umidade Slave Select) e C1 como MSS(Metano Slave Select)

// Define MOSI
#define MOSI_PIN	PB3						//Define PB3 como pino MOSI
#define MOSI_INIT()	DDRB  |= (1<<MOSI_PIN)	//Inicializa o pino MOSI como output

// Define MISO
#define MISO_PIN	PB4						//Define PB4 como pino MISO(por ser input, não necessita de init)

// Define SCK
#define SCK_PIN		PB5						//Define PB5 como pino SCK
#define SCK_INIT()	DDRB  |= (1<<SCK_PIN)	//Inicializa o pino SCK como output

// Define SS de Umidade
#define USS_PIN		PC0						//Define PC0 como pino USS
#define USS_INIT()	DDRC  |= (1<<USS_PIN)	//Inicializa o pino USS como output
#define	USS_H()		PORTC |= (1<<USS_PIN)	//Seta USS como "HIGH"
#define USS_L()		PORTC &= ~(1<<USS_PIN)	//Seta USS como "LOW"

// Define SS do Metano
#define MSS_PIN		PC1						//Define PC1 como pino MSS
#define MSS_INIT()	DDRC  |= (1<<MSS_PIN)	//Inicializa o pino MSS como output
#define	MSS_H()		PORTC |= (1<<MSS_PIN)	//Seta MSS como "HIGH"
#define MSS_L()		PORTC &= ~(1<<MSS_PIN)	//Seta MSS como "LOW"


//Recebe um byte(colocado em um char) transmitido(sim, a leitura é byte a byte)
char ReceberChar(){

   //Espera até recepção terminar
   while(!(SPSR & (1<<SPIF)));

   //Retorna o byte transmitido no registrador SPDR(SPI Data Register)
   return SPDR;

}

void TransmitirChar(char Dado)
{
   //Começa a transmissão ao colocar um byte no registrador SPDR(SPI Data Register)
   SPDR = Dado;

   //Espera até transmissão terminar
   while(!(SPSR & (1<<SPIF)));

}

int main (){

    DDRB |= _BV(DDB0);
	//Inicializa MOSI e SCK
	MOSI_INIT();
	SCK_INIT();

	//Desativa os slaves(Umidade e Metano) ao colocar "HIGH" em seus SS
	USS_H();
	MSS_H();

	//Seta SPE por ser SPI Enable
	//Seta MSTR para configurar o AtMega como Master
	//Seta SPR1 e não seta SPR0 para que a frequência de clock de comunicação SPI seja 1/64 do clock do AtMega
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);

	//Ativa o slave umidade ao colocar "LOW" em USS
	USS_L();

	char dado;

    char buffer[50];

    bluetooth_init(38400, 1);
    //init interrupt
	sei();

    // PORTD |= _BV(PORTD7);
    // _delay_ms(1000);
    // PORTD &= ~_BV(PORTD7);

    while(1){
        //Recebe um char transmitido pelo slave umidade
        dado = ReceberChar();
        //dado = 1;
        PORTB |= _BV(PORTB0);
        _delay_ms(500);
        PORTB &= ~_BV(PORTB0);
        sprintf(buffer, "%x", dado);
        bluetooth_print(buffer);
        _delay_ms(500);
    }


	return 0;
}
