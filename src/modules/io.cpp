#include "io.h"

// Reflectance sensor threshold values
uint8_t Ramp_Sense_Thresh = DEFAULT_RAMP_SENSE_THRESH;
uint8_t Lose_Sense_Thresh = DEFAULT_LOSE_SENSE_THRESH;

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
			return (Ramp_Sense_Value <= Ramp_Sense_Thresh);
		case LOSE:
			return (Lose_Sense_Value <= Lose_Sense_Thresh);
		case FLIP_L:
			return (!!((PIND >> FLIP_L_PIN) & 1));
		case FLIP_R:
			return (!!((PIND >> FLIP_R_PIN) & 1));
		case BUMP_0:
			return (!((PINC >> BUMP_0_PIN) & 1));
		case BUMP_1:
			return (!((PINC >> BUMP_1_PIN) & 1));
		case BUMP_2:
			return (!((PINC >> BUMP_2_PIN) & 1));
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
