#include "adc.h"

volatile uint8_t rampSenseValue = 255;
volatile uint8_t loseSenseValue = 255;

volatile byte currentADCChannel = RAMP_SENSE_CHANNEL;

void initADC() {
	ADMUX = ((1 << REFS0) | (1 << ADLAR) | currentADCChannel);
	ADCSRA = ((1 << ADEN) | (1 << ADSC) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1));
	return;
}

ISR(ADC_vect) {
	switch currentADCChannel {
		case RAMP_SENSE_CHANNEL:
			rampSenseValue = ADCH;
			currentADCChannel = LOSE_SENSE_CHANNEL;
			break;
		case LOSE_SENSE_CHANNEL:
			loseSenseValue = ADCH;
		default:
			currentADCChannel = RAMP_SENSE_CHANNEL;
			break;
	}
	ADMUX = ((01 << REFS0) + (1 << ADLAR) + currentADCChannel);
	ADCSRA = ((1 << ADEN) | (1 << ADSC) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1));
	return;
}
