#include <avr/io.h>
#include "methane.h"

int analogRead() {
	ADMUX = _BV(REFS0) | MQ2_ADC_CHANNEL ;
	ADCSRA = (1 << ADEN) ;

	ADCSRA |= _BV(ADSC) ;
	while (!(ADCSRA & _BV(ADIF))) ;
	return ADC ;
}
