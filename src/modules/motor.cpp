#include "motor.h"

uint8_t Motor_Speed = DEFAULT_MOTOR_SPEED;

void initMotor() {
	DDRB |= (1 << MOTOR_PIN);
}

void setMotor(bool state) {
	cli();
	OCR1BH = 0;
	OCR1BL = (state ? Motor_Speed : 0);
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
