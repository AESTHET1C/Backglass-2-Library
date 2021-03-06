/* ADC Module
 *
 * Alternatingly reads ADC channels of ramp and lose sensor and updates status variables
 *
 * Updating of each channel happens at approximately 125 kHz. The variables "Ramp_Sense_Value" and
 * "Lose_Sense_Value" give the most up-to-date readings with 8-bit precision.
 *
 * Do not use Arduino's analogRead() function with this; code may break.
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef adc
#define adc

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

typedef uint8_t byte;

/////////////////////////
// PIN DEFINITIONS
/////////////////////////

#define RAMP_SENSE_CHANNEL 6
#define LOSE_SENSE_CHANNEL 7


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initADC();
/*
 * Sets up the ADC interrupt and conversion
 * Should be called at startup
 */


#endif
