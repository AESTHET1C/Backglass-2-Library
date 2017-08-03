/* Basic I/O Module
 *
 * Allows for reading sensors and setting bumper LED states
 *
 * Analogue reading of the RAMP and LOSE sensors is handled by the ADC module.
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef io
#define io

#include "adc.h"

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

const uint8_t DEFAULT_RAMP_SENSE_THRESH = 72;
const uint8_t DEFAULT_LOSE_SENSE_THRESH = 72;


/////////////////////////
// PIN DEFINITIONS
/////////////////////////

// Port B
#define LED_0_PIN PORTB5
#define LED_1_PIN PORTB4
#define LED_2_PIN PORTB3

// Port C
#define BUMP_0_PIN PORTC2
#define BUMP_1_PIN PORTC1
#define BUMP_2_PIN PORTC0

// Port D
#define FLIP_L_PIN PORTD3
#define FLIP_R_PIN PORTD2


/////////////////////////
// ENUMERATIONS
/////////////////////////

typedef enum {
	BUMP_0 = 0,
	BUMP_1 = 1,
	BUMP_2 = 2,
	RAMP   = 3,
	LOSE   = 4,
	FLIP_L = 5,
	FLIP_R = 6
} sensor_t;

typedef sensor_t led_t;


/////////////////////////
// GLOBAL VARIABLES
/////////////////////////

// ADC
extern volatile uint8_t Ramp_Sense_Value;
extern volatile uint8_t Lose_Sense_Value;


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initIO();
/*
 * Configures pins and activates ADC module
 * Must be called before other module functions
 */

bool readSensor(sensor_t sensor);
/*
 * Returns the current logical state of a sensor
 *
 * INPUT: Sensor to read
 * OUTPUT: State of being engaged
 */

void setLED(led_t bumper, bool state);
/*
 * Turns a bumper LED on or off
 *
 * INPUT: Bumper in question
 *        State of being enabled
 */

#endif
