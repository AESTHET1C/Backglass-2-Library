#include "adc.h"

volatile uint8_t Ramp_Sense_Value = 255;
volatile uint8_t Lose_Sense_Value = 255;

volatile byte Current_ADC_Channel = RAMP_SENSE_CHANNEL;

void initADC() {
	ADMUX = ((1 << REFS0) | (1 << ADLAR) | Current_ADC_Channel);
	ADCSRA = ((1 << ADEN) | (1 << ADSC) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1));
	return;
}

ISR(ADC_vect) {
	switch Current_ADC_Channel {
		case RAMP_SENSE_CHANNEL:
			Ramp_Sense_Value = ADCH;
			Current_ADC_Channel = LOSE_SENSE_CHANNEL;
			break;
		case LOSE_SENSE_CHANNEL:
			Lose_Sense_Value = ADCH;
		default:
			Current_ADC_Channel = RAMP_SENSE_CHANNEL;
			break;
	}
	ADMUX = ((01 << REFS0) + (1 << ADLAR) + Current_ADC_Channel);
	ADCSRA = ((1 << ADEN) | (1 << ADSC) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1));
	return;
}
