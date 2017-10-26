/*
 * Programa para acrescentar dados em um cartão SD
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
#include <string.h>
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
    
    //Define a saída da porta D6 como nível lógico alto
		PORTB |= _BV(PORTB0) ;
        
    // reboot delay
	_delay_ms(2000);
    
    //Define a saída da porta B0 como nível lógico baixo
    PORTB &= ~_BV(PORTB0);


	// inicializa o cartão
	UINT bw;
	f_mount(0, &FatFs);		// Monta o cartão e fornece uma area de trabalho FatFs ao modulo
	
	// cria o ponteiro fp para referenciar o arquivo a ser aberto
	fp = (FIL *)malloc(sizeof (FIL));
	
	// se o arquivo for aberto, entra na condição
	if (f_open(fp, "file.txt", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {	// abre arquivo existente ou cria novo
	
		if (f_lseek(fp, f_size(fp)) == FR_OK) { //Entra no if se conseguir posicionar o "cursor" de escrita no final do arquivo
		
			char *text2 = "Essa eh uma nova linha marota de texto, acrescentada em um arquivo maroto!\r\n";
			
			f_write(fp, text2, strlen(text2), &bw);	//escreve a string 'text2'no arquivo			
		}	
		
		f_close(fp);// fecha o arquivo		
	}
	
	// desmonta o cartão	
	f_mount(0, &FatFs);
    

    //Define a saída da porta D6 como nível lógico alto
		PORTB |= _BV(PORTB0) ;



	return 0;
}
