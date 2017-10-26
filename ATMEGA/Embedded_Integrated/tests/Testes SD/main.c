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
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "bluetooth/bluetooth.h"
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


int main(void) {
   
    sei();
    bluetooth_init(38400, 1);
    
    char text[1000]; 	//buffer de leitura
    
    // reboot delay
	_delay_ms(200);

	// inicializa o cartão
	UINT bw;
	f_mount(0, &FatFs);		// Fornece uma area de trabalho FatFs ao modulo
	
	// cria o ponteiro fp para referenciar o arquivo a ser aberto
	fp = (FIL *)malloc(sizeof (FIL));
	
	// se o arquivo for aberto, entra na condição
	if (f_open(fp, "file.txt", FA_READ ) == FR_OK) {	// Abre um arquivo
	
		
	
		UINT br;
		
		//f_read(arquivo, buffer de leitura, numero de bytes a serem lidos, ponteiro para numero de bytes lidos)
		f_read(fp, text, 999, &br); 
		
		f_close(fp);// Fecha o arquivo
		
	}

	// desmonta o cartão	
	f_mount(0, &FatFs);
    
    text[strlen(text)-1] = '\0';
    
    while(1){
        bluetooth_print(text);
        _delay_ms(1000);

    }

       
	return 0;
}
