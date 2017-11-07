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
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LowPower/LowPower.h"
#include "ds3231/ds3231.h"
#include "bluetooth/bluetooth.h"

bool bluetooth_enabled = false;

//(DEBUG)
void blink_led() {
    int i;
    for (i = 0; i < 5; i++) {
        PORTB |= _BV(PORTB0);
        _delay_ms(200);
        PORTB &= ~_BV(PORTB0);
        _delay_ms(200);
    }
}

// Interrupt 0 handler
ISR (INT0_vect, ISR_BLOCK)
{
    blink_led();
    blink_led();
}

// Interrupt 1 handler
ISR (INT1_vect)
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
    /* Pin configuration */
    DDRD &= ~_BV(DDD2); // PD2 (PCINT0 pin) is now an input
    DDRD &= ~_BV(DDD3); // PD3 (PCINT1 pin) is now an input

    PORTD |= _BV(PORTD2); // PD3 is now an input with pull-up enabled
    PORTD |= _BV(PORTD3); // PD3 is now an input with pull-up enabled

    DDRB |= _BV(DDB0); //Set PB0 as output (DEBUG LED)

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

    // Turns off led on PB0
    PORTB &= ~_BV(PORTB0);

    // Interruptions must be enabled to configure RTC
	sei();
    // RTC configuration
    twi_init_master();
    rtc_set_time(0,19,07);
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

        // Read sensors or Bluetooth Com
        if (bluetooth_enabled) {
            PORTB |= _BV(PORTB0);

            //Init bluetooth
            bluetooth_init(38400, 1);
        }
        while(bluetooth_enabled) {
            if(!(PIND & _BV(PIND3))) {
                PORTB &= ~_BV(PORTB0);
                bluetooth_enabled = false;
                _delay_ms(300);
                EIFR |= _BV(INTF0); //Clear interrupt flags
                EIFR |= _BV(INTF1);
                break;
            }
            bluetooth_print("Beep!\r\n");
            _delay_ms(1000);
        }
    }

	return 0;
}
