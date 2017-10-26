/*
 * Programa para ler dados de um lcd
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


int main (){

	// reboot delay
	_delay_ms(200);

	// inicializa o cartão
	UINT bw;
	debug = f_mount(0, &FatFs);		// Fornece uma area de trabalho FatFs ao modulo
	
	// cria o ponteiro fp para referenciar o arquivo a ser aberto
	fp = (FIL *)malloc(sizeof (FIL));
	
	// se o arquivo for aberto, entra na condição
	if (f_open(fp, "file.txt", FA_READ ) == FR_OK) {	// Abre um arquivo
	
		char text[255]; 	//buffer de leitura
	
		UINT br;
		
		//f_read(arquivo, buffer de leitura, numero de bytes a serem lidos, ponteiro para numero de bytes lidos)
		f_read(fp, text, 255, &br); 
		
		f_close(fp);// Fecha o arquivo
		
	}

	// desmonta o cartão	
	f_mount(0, &FatFs);



	return 0;
}
