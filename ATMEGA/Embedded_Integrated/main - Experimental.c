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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "LowPower/LowPower.h"
#include "ds3231/ds3231.h"
#include "bluetooth/bluetooth.h"
#include "SD/ff.h"
#include "SD/integer.h"

#define PORTLED PORTB
#define PINLED  PORTB0
#define PREPLED() DDRB |= _BV(PINLED)
#define AcendeLED() PORTLED |= _BV(PINLED)
#define ApagaLED() PORTLED &= ~_BV(PINLED)

#define BUFFERLEITURA 1025
#define TAMANHOLINHA 100


FATFS FatFs;		// Área de trabalho do FAtFs 
FIL *fp;		// Estrutura de File

void InicializarSistema();
int LerArquivoTodoEPassarPorBluetooth(int Leira);

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

// Interrupt 0 handler
ISR (INT0_vect, ISR_BLOCK)
{
    blink_led();
    blink_led();
}

// Interrupt 1 handler
ISR (INT1_vect, ISR_BLOCK)
{
    blink_led();
}

int main(void) {
    
   
    InicializarSistema();

    /* Loop code */
	while(1) {
        // Enable interrupts
        sei();

        // Clear alarm flag
        rtc_check_alarm();

        // Powers Down ATMEGA
        powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

        // Disable interrupts
        cli();

        // Read sensors
        double temperatura = 25.2;
        uint8_t umidade = 50;
        uint8_t metano = 30;
                
        // Write data
        // inicializa o cartão
        UINT bw;
        f_mount(0, &FatFs);		// Monta o cartão e fornece uma area de trabalho FatFs ao modulo
	
        // cria o ponteiro fp para referenciar o arquivo a ser aberto
        fp = (FIL *)malloc(sizeof (FIL));
        
        // se o arquivo for aberto, entra na condição
        if (f_open(fp, "leira1.txt", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {	// abre arquivo existente ou cria novo
        
            if (f_lseek(fp, f_size(fp)) == FR_OK) { //Entra no if se conseguir posicionar o "cursor" de escrita no final do arquivo
            
                char text[TAMANHOLINHA];
                sprintf(text, "%f,%u,%u\r\n", temperatura, umidade, metano);
                
                f_write(fp, text, strlen(text), &bw);	//escreve a string 'text'no arquivo			
            }	
            
            f_close(fp);// fecha o arquivo		
        }
        
        // desmonta o cartão	
        f_mount(0, &FatFs);
    
        
        
        
    }

	return 0;
}

void InicializarSistema(){
	
/* Pin configuration */
    DDRD &= ~_BV(DDD2); // PD2 (PCINT0 pin) is now an input
    DDRD &= ~_BV(DDD3); // PD3 (PCINT1 pin) is now an input

    PORTD |= _BV(PORTD2); // PD3 is now an input with pull-up enabled
    PORTD |= _BV(PORTD3); // PD3 is now an input with pull-up enabled

    PREPLED(); //Set PC3 as output (DEBUG LED)

    /* Interruption setup*/
    // Set INT0 to trigger on low level
    EICRA &= ~_BV(ISC00);
    EICRA |= _BV(ISC01);

    // Set INT1 to trigger on low level
    EICRA &= ~_BV(ISC10);
    EICRA &= ~_BV(ISC11);

    EIMSK |= _BV(INT0); // Turns on INT0
    EIMSK |= _BV(INT1); // Turns on INT1

    /* Setup code */

    // Turns off led on PC3
    PORTC &= ~_BV(PORTC3);

    // Interruptions must be enabled to configure RTC
	sei();
    // RTC configuration
    twi_init_master();
    rtc_set_time(0,19,07);
    rtc_SQW_enable(false);
    rtc_enable_alarm(true);
    rtc_set_alarm(0b1110, 23, 19, 5);
	
}


//Le todo o arquivo relativo à leira referenciada e o transmite por bluetooth
//Retorna  1 se der certo
//Retorna  0 se não conseguir montar o cartão SD
//Retorna -1 se o número de leira passado por argumento é inválido
//Retorna -2 se não conseguir abrir o arquivo indicado
int LerArquivoTodoEPassarPorBluetooth(int Leira){
	
	//Desabilita interrupções
	cli();
	
	FRESULT abriu;
	char bufferLeitura[BUFFERLEITURA];
	
	abriu = f_mount(0, &FatFs);		// Fornece uma area de trabalho FatFs ao modulo
	
	if(abriu != FR_OK){
		sei();	//Habilita interrupções
		return 0;	// Se não conseguir montar o cartão SD retorna 0
	}
	
	// cria o ponteiro fp para referenciar o arquivo a ser aberto
	fp = (FIL *)malloc(sizeof (FIL));
	
	//Tenta abrir o arquivo da leira desejada
	if(Leira==0)
		abriu = f_open(fp, "leira0.txt", FA_READ);
	else if(Leira==1)
		abriu = f_open(fp, "leira1.txt", FA_READ);
	else if(Leira==2)
		abriu = f_open(fp, "leira2.txt", FA_READ);
	else if(Leira==3)
		abriu = f_open(fp, "leira3.txt", FA_READ);
	else{
		sei();	//Habilita interrupções
		return -1; //Se tentar acessar um número inválido de leira retorna -1
	}
	
	if(abriu != FR_OK){
		sei();	//Habilita interrupções
		return -2; //Se falhar em abrir o arquivo desejado retorna -2
	}
	
    //inicializa o bluetooth
	bluetooth_init(38400, 1);
	
	UINT br = 5;  //variável que carregará o número de bytes lidos em cada f_read, será usado como condição de parada do while
	
	//Enquanto houver algo para ler no arquivo aberto, a leitura continuará sendo feita
	while(br != 0){
		
		//f_read(arquivo, buffer de leitura, numero de bytes a serem lidos, ponteiro para numero de bytes lidos)
		f_read(fp, bufferLeitura, BUFFERLEITURA - 1, &br); 
		
		//atribui '\0' na ultima posição de bufferLeitura para que bluetooth_print reconheça o fim da string
		bufferLeitura[strlen(bufferLeitura) - 1] = '\0';
		
		//transmite por bluetooth a string bufferLeitura
		bluetooth_print(bufferLeitura);
		
	}
	
	f_close(fp);// Fecha o arquivo
	
	// desmonta o cartão	
	f_mount(0, &FatFs);	
	
	sei();	//Habilita interrupções
	
	return 1; //Retorna 1 indicando o sucesso da operação
}