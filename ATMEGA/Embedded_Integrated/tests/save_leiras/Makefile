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
#
NAME=main
CFLAGS=-Os -DF_CPU=$(CPU_F)
MICRO=328p
CPU_F=8000000
OBJ = main.o ds3231/ds3231.o ds3231/twi.o ds3231/twi-lowlevel.o \
LowPower/LowPower.o bluetooth/bluetooth.o adc0832/adc0832.o
OBJCPP = SD/ff.o SD/sdmm.o

all: exe
	avr-objcopy -O ihex -R .eeprom $(NAME).out $(NAME).hex

exe: $(OBJ) $(OBJCPP)
	avr-gcc -mmcu=atmega$(MICRO) $(OBJ) $(OBJCPP) -o $(NAME).out

%.o: %.c
	avr-gcc -c -o $@ $< $(CFLAGS) -std=c11 -mmcu=atmega$(MICRO)

%.o: %.cpp
	avr-gcc -c -o $@ $< $(CFLAGS) -mmcu=atmega$(MICRO)

upload: all
	avrdude -c avrisp -p m$(MICRO) -P /dev/ttyUSB0 -b 19200 -U flash:w:$(NAME).hex

clean:
	rm $(NAME).out
	rm *.o */*.o
	rm $(NAME).hex
