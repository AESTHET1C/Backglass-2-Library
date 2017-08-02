/* Motor Module
 *
 * Handles motor control
 *
 * Actual motor output is handled by the Timer1 module
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef motor
#define motor

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

const uint8_t DEFAULT_MOTOR_SPEED = 72;


/////////////////////////
// PIN DEFINITIONS
/////////////////////////

// Port B
#define MOTOR_PIN PORTB2


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initMotor();
/*
 * Configures motor pin
 * Should be called at startup
 *
 * Timer1 is assumed to be configured
 */

void setMotor(bool state);
/*
 * Turns the motor on or off
 *
 * INPUT: State of motor being enabled
 */

void changeMotorSpeed(uint8_t speed);
/*
 * Changes the speed at which the motor turns
 *
 * Affects motorSpeed
 * INPUT: New speed of motor
 */

#endif
