#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

int main() {
    DDRB |= _BV(DDB0);

    while(1) {
        PORTB ^= _BV(PORTB0);
        _delay_ms(500);
    }

    return 0;
}
