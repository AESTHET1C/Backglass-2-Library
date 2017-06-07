#include "io.h"

void initIO() {
	PORTD |= ((1 << FLIP_L_PIN) | (1 << FLIP_R_PIN));                     // Enable pull-ups
	PORTC |= ((1 << BUMP_0_PIN) | (1 << BUMP_1_PIN) | (1 << BUMP_2_PIN));
	DDRB |= ((1 << LED_0_PIN) | (1 << LED_1_PIN) | (1 << LED_2_PIN));     // Set LEDs to output
	initADC();
	return;
}

bool readSensor(sensor_t sensor) {
	switch (sensor) {
		case RAMP:
			return (Ramp_Sense_Value <= RAMP_SENSE_THRESH);
		case LOSE:
			return (Lose_Sense_Value <= LOSE_SENSE_THRESH);
		case FLIP_L:
			return (!!((PORTD >> FLIP_L_PIN) & 1));
		case FLIP_R:
			return (!!((PORTD >> FLIP_R_PIN) & 1));
		case BUMP_0:
			return (!((PORTC >> BUMP_0_PIN) & 1));
		case BUMP_1:
			return (!((PORTC >> BUMP_1_PIN) & 1));
		case BUMP_2:
			return (!((PORTC >> BUMP_2_PIN) & 1));
		default:
			return false;
	}
	return false;
}

void setLED(led_t bumper, bool state) {
	uint8_t mask;

	switch (bumper) {
		case BUMP_0:
			mask = (1 << LED_0_PIN);
			break;
		case BUMP_1:
			mask = (1 << LED_1_PIN);
			break;
		case BUMP_2:
			mask = (1 << LED_2_PIN);
			break;
		default:
			return;
	}
	PORTB = (state ? (PORTB | mask) : (PORTB & ~mask));
	return;
}
