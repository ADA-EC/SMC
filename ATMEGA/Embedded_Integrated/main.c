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

#define BUFFERLEITURA 200
#define TAMANHOLINHA 100

bool bluetooth_enabled = false;

int GravarMedicao(int Leira, double temperatura, uint8_t umidade, uint8_t metano);
int LerArquivoTodoEPassarPorBluetooth(int Leira);

FATFS FatFs;		// Área de trabalho do FAtFs
FIL *fp;		// Estrutura de File

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
	uint8_t hora, minuto, segundo, dia, mes, ano;
    
    sei();
    
    rtc_get_date_time(&hora, &minuto, &segundo, &dia, &mes, &ano);
    
    cli();
    
	// Retorna o dia e hora configurado como DWORD
	return	  ((DWORD)(ano - 1980) << 25)	// Ano 2017
	| ((DWORD)mes << 21)				// Mes 10
	| ((DWORD)dia << 16)				// Dia 26
	| ((DWORD)hora << 11)				// Hora 20 (considerando fuso horario 0)
	| ((DWORD)minuto << 5)				// Minuto 0
	| ((DWORD)segundo >> 1);				// Segundo 0
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
    cli();
    // SREG &= ~0x80;
    // EIFR |= _BV(INTF1);
    _delay_ms(300);
    if (PIND & _BV(PIND3)) {
        blink_led();
        bluetooth_enabled = true;
    } else {
        bluetooth_enabled = false;
    }
}

int main(void) {

    int debug;
    char debugchar[15];
    
    /* Pin configuration */
    DDRD &= ~_BV(DDD2); // PD2 (PCINT0 pin) is now an input
    DDRD &= ~_BV(DDD3); // PD3 (PCINT1 pin) is now an input

    PORTD |= _BV(PORTD2); // PD3 is now an input with pull-up enabled
    PORTD |= _BV(PORTD3); // PD3 is now an input with pull-up enabled

    PREPLED(); //Set PC3 as output (DEBUG LED)

    /* Interruption setup*/
    // Set INT0 to trigger on falling edge
    EICRA &= ~_BV(ISC00);
    EICRA |= _BV(ISC01);

    // Set INT1 to trigger on rising edge
    EICRA |= _BV(ISC10);
    EICRA |= _BV(ISC11);

    EIMSK |= _BV(INT0); // Turns on INT0
    EIMSK |= _BV(INT1); // Turns on INT1

    /* Setup code */

    // Turns off led on PC3
    PORTC &= ~_BV(PORTC3);

    // Interruptions must be enabled to configure RTC
	sei();
    // RTC configuration
    twi_init_master();
    rtc_set_date_time(0,19,7,8,11,17);
    rtc_SQW_enable(false);
    rtc_enable_alarm(true);
    rtc_set_alarm(0b1110, 23, 19, 5);

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

        // Bluetooth Communication
        if (bluetooth_enabled) {
            PORTB |= _BV(PORTB0);

            //Init bluetooth
            bluetooth_init(38400, 1);
        }
        while(bluetooth_enabled) {
            if(!(PIND & _BV(PIND3))) {
                PORTB &= ~_BV(PORTB0);
                _delay_ms(300);
                EIFR |= _BV(INTF0); //Clear interrupt flags
                EIFR |= _BV(INTF1);
                break;
            }
            debug = LerArquivoTodoEPassarPorBluetooth(0);
            sprintf(debugchar, "beep %d\r\n", debug);
            bluetooth_print(debugchar);
            _delay_ms(1000);
        }
        if(bluetooth_enabled) {
            bluetooth_enabled = false;
            continue;
        }

        // Read sensors
        double temperatura = 25.2;
        uint8_t umidade = 50;
        uint8_t metano = 30;

        GravarMedicao(0, temperatura, umidade, metano);
        
        
        

    }

	return 0;
}


//Grava no arquivo relativo à leira referenciada as medições feitas e a data e hora atuais
//OBS: não será responsabilidade dessa função habilitar ou desabilitar interrupções
//Retorna  1 se der certo
//Retorna  0 se não conseguir montar o cartão SD
//Retorna -1 se o número de leira passado por argumento é inválido
//Retorna -2 se não conseguir abrir o arquivo indicado
//Retorna -3 se falhar em posicionar o cursor no fim do arquivo
int GravarMedicao(int Leira, double temperatura, uint8_t umidade, uint8_t metano){
   	
	FRESULT abriu;
	char bufferLeitura[BUFFERLEITURA];
	UINT bw;
    
    //Se o número de leira passado como argumento for inválido, retorna -1
    if(Leira<0 || Leira >3) return -1;
    
    uint8_t hora, minuto, segundo, dia, mes, ano;
    
    sei();
    
    rtc_get_date_time(&hora, &minuto, &segundo, &dia, &mes, &ano);
    
    cli();
    
    // Tenta montar o cartão, se não conseguir retorna 0
	if(f_mount(0, &FatFs) != FR_OK) return 0;	
    
    // cria o ponteiro fp para referenciar o arquivo a ser aberto
    fp = (FIL *)malloc(sizeof (FIL));
    
    
    //Cria uma string em vai ser armazenada o nome do arquivo a ser aberto
    char nomeArquivo[15];
    sprintf(nomeArquivo, "leira%d.csv", Leira);
    
    
    // se o arquivo for aberto, entra na condição
    if (f_open(fp, nomeArquivo, FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {	// abre arquivo existente ou cria novo
                
        //Se o arquivo aberto está vazio, cria o cabeçalho
        if(f_size(fp)==0){
            char cabecalho[50] = "Data, Hora, Temperatura, Umidade, Metano\r\n";	//Define a string de cabeçalho
            f_write(fp, cabecalho, strlen(cabecalho), &bw);	//escreve a string 'cabecalho'no arquivo	
        }
        
        //Entra no if se conseguir posicionar o "cursor" de escrita no final do arquivo
        if (f_lseek(fp, f_size(fp)) == FR_OK) { 
            
            //Atribui a temperaturaInteira o valor antes da virgula de temperatura
            //e atribui a temperaturaDecimal 3 casas depois da virgula
            int temperaturaInteira, temperaturaDecimal;
            temperaturaInteira = temperatura;
            temperaturaDecimal = temperatura*1000;
            temperaturaDecimal = temperaturaDecimal%1000;
            
            //Coloca na string 'linha' a linha que será gravada no arquivo
            char linha[TAMANHOLINHA];
            sprintf(linha, "%02d/%02d/%02d,%02d:%02d,%d.%d,%u,%u\r\n", dia, mes, ano, hora, minuto, temperaturaInteira, temperaturaDecimal, umidade, metano);
                        
            //escreve a string 'linha' no arquivo	
            f_write(fp, linha, strlen(linha), &bw);	
            	
        } else {  //Retorna -3 se falhar em posicionar o cursor no fim do arquivo
            f_close(fp);// fecha o arquivo	
            f_mount(0, &FatFs);// desmonta o cartão
            return -3;
        }	
        
        f_close(fp);  // fecha o arquivo	
        
    } else {  //Se falhar em abrir o arquivo desejado retorna -2
        f_mount(0, &FatFs);// desmonta o cartão
		return -2; 
	}
	
	// desmonta o cartão	
    f_mount(0, &FatFs);
    
    //Se deu tudo certo retorna 1
    return 1;
}


//Le todo o arquivo relativo à leira referenciada e o transmite por bluetooth
//OBS: não será responsabilidade dessa função habilitar ou desabilitar interrupções
//Retorna  1 se der certo
//Retorna  0 se não conseguir montar o cartão SD
//Retorna -1 se o número de leira passado por argumento é inválido
//Retorna -2 se não conseguir abrir o arquivo indicado
int LerArquivoTodoEPassarPorBluetooth(int Leira){
    FRESULT abriu;
	char* bufferLeitura;
	UINT bw;
    
    //O vetor bufferLeitura é alocado com uma posição a mais para armazenar o '\0' necessário para transmissão bluetooth
    bufferLeitura = (char*)malloc((BUFFERLEITURA+1)*sizeof(char));
    
    //Se o número de leira passado como argumento for inválido, retorna -1
    if(Leira<0 || Leira >3)
        return -1;
    
    
    
    // Tenta montar o cartão, se não conseguir retorna 0
	if(f_mount(0, &FatFs) != FR_OK)
		return 0;	
    
    // cria o ponteiro fp para referenciar o arquivo a ser aberto
    fp = (FIL *)malloc(sizeof (FIL));
    
    
    //Cria uma string em vai ser armazenada o nome do arquivo a ser aberto
    char nomeArquivo[15];
    sprintf(nomeArquivo, "LEIRA%d.CSV", Leira);
    
    
    // se o arquivo for aberto, entra na condição
    if (f_open(fp, nomeArquivo, FA_READ) == FR_OK) {	// tenta abrir o arquivo da leira desejada para leitura
        
	
        UINT br;  //variável que carregará o número de bytes lidos em cada f_read, será usado como condição de parada do while
        
        //faz uma primeira leitura no arquivo aberto
        //f_read(arquivo, buffer de leitura, numero de bytes a serem lidos, ponteiro para numero de bytes lidos)
        f_read(fp, bufferLeitura, BUFFERLEITURA, &br); 
            
        //Enquanto houver algo para ler no arquivo aberto, a leitura continuará sendo feita
        while(br != 0){
            
            //atribui '\0' na ultima posição de bufferLeitura para que bluetooth_print reconheça o fim da string
            bufferLeitura[br] = '\0';
            
            //transmite por bluetooth a string bufferLeitura
            bluetooth_print(bufferLeitura);
            
            //faz uma nova leitura
            f_read(fp, bufferLeitura, BUFFERLEITURA, &br); 
            
        }
        
    
    
        
    } else {  //Se falhar em abrir o arquivo desejado retorna -2
        f_mount(0, &FatFs); // desmonta o cartão
		return -2; 
	}
	
	// desmonta o cartão	
        f_mount(0, &FatFs);
    
    //Se deu tudo certo retorna 1
    return 1;
    
}
