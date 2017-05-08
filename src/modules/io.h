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

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

const uint8_t RAMP_SENSE_THRESH = 192;
const uint8_t LOSE_SENSE_THRESH = 192;


/////////////////////////
// PIN DEFINITIONS
/////////////////////////

// Port B
const byte LED_0_PIN = PORTB5;
const byte LED_1_PIN = PORTB4;
const byte LED_2_PIN = PORTB3;

// Port C
const byte BUMP_0_PIN = PORTC2;
const byte BUMP_1_PIN = PORTC1;
const byte BUMP_2_PIN = PORTC0;

// Port D
const byte FLIP_L_PIN = PORTD3;
const byte FLIP_R_PIN = PORTD2;


/////////////////////////
// ENUMERATIONS
/////////////////////////

typedef enum {
	RAMP   = 0,
	LOSE   = 1,
	FLIP_L = 2,
	FLIP_R = 3,
	BUMP_0 = 4,
	BUMP_1 = 5,
	BUMP_2 = 6
} sensor_t

typedef enum {
	BUMP_0 = 0,
	BUMP_1 = 1,
	BUMP_2 = 2
} led_t


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
