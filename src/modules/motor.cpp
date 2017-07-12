#include "motor.h"

uint8_t Motor_Speed = DEFAULT_MOTOR_SPEED;

void initMotor() {
	DDRB |= (1 << MOTOR_PIN);
}

void setMotor(bool state) {
	cli();
	if (state) {
		TCCR1A = ((1 << COM1B1) | (1 << WGM10));
		OCR1BH = 0;
		OCR1BL = Motor_Speed;
	}
	else {
		TCCR1A = (1 << WGM10);
		OCR1BH = 0;
		OCR1BL = 0;
	}
	sei();
	return;
}

void changeMotorSpeed(uint8_t speed) {
	Motor_Speed = speed;
	if (OCR1BL > 0) {
		setMotor(true);
	}
	return;
}
