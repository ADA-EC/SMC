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
# Pedro V. B. Jeronymo (pedrovbj@gmail.com)
# Henrique Andrews (andrews.marques225@gmail.com)
#
NAME=main
CFLAGS=-Os -DF_CPU=$(CPU_F) -std=c11
MICRO=328p
CPU_F=8000000
OBJ = main.o adc0832/adc0832.o bluetooth/bluetooth.o

all: exe
	avr-objcopy -O ihex -R .eeprom $(NAME).out $(NAME).hex

exe: $(OBJ)
	avr-gcc -mmcu=atmega$(MICRO) $(OBJ) -o $(NAME).out

%.o: %.c
	avr-gcc -c -o $@ $< $(CFLAGS) -mmcu=atmega$(MICRO)

upload: all
	avrdude -c avrisp -p m$(MICRO) -P /dev/ttyACM0 -b 19200 -U flash:w:$(NAME).hex

clean:
	rm $(NAME).out
	rm *.o
	rm $(NAME).hex
