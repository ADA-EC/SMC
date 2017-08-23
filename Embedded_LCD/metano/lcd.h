/*
 * Library for WM-C1602M LCD Display
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
 * Guilherme Prearo (guiprearo@gmail.com)
 *
 */
#include <avr/io.h>
#include <util/delay.h>

#define _LCD_4_BIT_MODE

#define _LCD_RS_DDR DDRD
#define _LCD_RS_PORT PORTD
#define _LCD_RS PORTD5

#define _LCD_RW_DDR DDRD
#define _LCD_RW_PORT PORTD
#define _LCD_RW PORTD6

#define _LCD_EN_DDR DDRD
#define _LCD_EN_PORT PORTD
#define _LCD_EN PORTD7


#define _LCD_D0 PORTB0
#define _LCD_D0_PORT PORTB
#define _LCD_D0_DDR *(&_LCD_D0_PORT - 1)
#define _LCD_D0_PIN *(&_LCD_D0_PORT - 2)

#define _LCD_D1 PORTB1
#define _LCD_D1_PORT PORTB
#define _LCD_D1_DDR *(&_LCD_D1_PORT - 1)
#define _LCD_D1_PIN *(&_LCD_D1_PORT - 2)

#define _LCD_D2 PORTB2
#define _LCD_D2_PORT PORTB
#define _LCD_D2_DDR *(&_LCD_D2_PORT - 1)
#define _LCD_D2_PIN *(&_LCD_D2_PORT - 2)

#define _LCD_D3 PORTB3
#define _LCD_D3_PORT PORTB
#define _LCD_D3_DDR *(&_LCD_D3_PORT - 1)
#define _LCD_D3_PIN *(&_LCD_D3_PORT - 2)

#define _LCD_D4 PORTB4
#define _LCD_D4_PORT PORTB
#define _LCD_D4_DDR *(&_LCD_D4_PORT - 1)
#define _LCD_D4_PIN *(&_LCD_D4_PORT - 2)

#define _LCD_D5 PORTB5
#define _LCD_D5_PORT PORTB
#define _LCD_D5_DDR *(&_LCD_D5_PORT - 1)
#define _LCD_D5_PIN *(&_LCD_D5_PORT - 2)

#define _LCD_D6 PORTB6
#define _LCD_D6_PORT PORTB
#define _LCD_D6_DDR *(&_LCD_D6_PORT - 1)
#define _LCD_D6_PIN *(&_LCD_D6_PORT - 2)

#define _LCD_D7 PORTB7
#define _LCD_D7_PORT PORTB
#define _LCD_D7_DDR *(&_LCD_D7_PORT - 1)
#define _LCD_D7_PIN *(&_LCD_D7_PORT - 2)



#define _LCD_CLR 0x01
#define _LCD_HOME 0x02
#define _LCD_CURSOR_RIGHT 0x05
#define _LCD_CURSOR_LEFT 0x04
#define _LCD_SHIFT_LEFT 0x06
#define _LCD_SHIFT_RIGHT 0x07
#define _LCD_OFF 0x08
#define _LCD_ON 0x0C
#define _LCD_CURSOR_ON 0x0A
#define _LCD_CURSOR_BLINK 0x09
#define _LCD_CURSOR_OFF 0x0C

void waitBusy() ;
void sendChar( unsigned char i) ;
void sendInst8( unsigned char i) ;
void sendInst4( unsigned char i) ;
void sendInst(unsigned char byte) ;
void sendChar4(unsigned char i) ;
void gotoXY(unsigned char x, unsigned char y) ;
void writeStringXY(char *txt, unsigned char x, unsigned char y) ;
void writeString(char *txt) ;
void writeString8(char *txt) ;
void writeString4(char *txt) ;
void lcd_init() ;
