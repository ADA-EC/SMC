#
# Sistema de Monitoramento de Compostagem
#
# Tem como proposito monitorar as leiras do grupo GEISA (USP-SC)
#
# Copyright (C) 2017 ADA-Projetos em Engenharia de Computacao
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Amador Marcelino de Souza Neto(amador.neto@usp.br)
#
NAME=append
CPU_F=8000000

all: exe
	avr-objcopy -O ihex -R .eeprom $(NAME).out $(NAME).hex
exe: obj
	avr-gcc -mmcu=atmega328p $(NAME).o ff.o sdmm.o -o $(NAME).out
obj: $(NAME).c SD/ff.cpp SD/sdmm.cpp
	avr-gcc -Os -DF_CPU=$(CPU_F) -mmcu=atmega328p -c -o $(NAME).o $(NAME).c
	avr-gcc -Os -DF_CPU=$(CPU_F) -mmcu=atmega328p -c -o sdmm.o SD/sdmm.cpp
	avr-gcc -Os -DF_CPU=$(CPU_F) -mmcu=atmega328p -c -o ff.o SD/ff.cpp

upload: all
	avrdude -c avrisp -p m328p -P /dev/ttyACM0 -b 19200 -U flash:w:$(NAME).hex



clear:
	rm $(NAME)
	rm $(NAME).o
	rm $(NAME).hex
