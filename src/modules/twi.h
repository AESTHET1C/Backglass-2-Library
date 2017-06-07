/* Two-Wire Interface Module
 *
 * Used to transmit control packets to the audio/visual coprocessor
 *
 * Packets are expected to include exactly 4, 8, or 12 bytes of data. The scheme is as follows:
 *
 * Note 0, Note 1, Note 2, Note 3,
 * Voice 0, Voice 1, Voice 2, Voice 3,
 * Digit 0, Digit 1, Digit 2, Digit 3
 *
 * These data bytes should be added to TWI_Queue_Data[] elsewhere, in addition to correctly
 * setting TWI_Queue_Length. Improper data transmissions are not determined in this module.
 *
 * Electrical errors during transmission are handled by this module by either re-sending the
 * previous byte of data, or repeating the entire transmission.
 *
 * Upon completion of a transmission, the interface's clock is disabled to save power.
 *
 * Do not use Arduino's "Wire" library with this module; code may break.
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef twi
#define twi

#include <binary.h>
#include "audio.h"
#include "display.h"

typedef uint8_t byte;

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

#define TWI_BIT_RATE 400000
#define TWI_EXT_ADDR 1       // Address of audio/visual coprocessor


/////////////////////////
// PIN DEFINITIONS
/////////////////////////

// Port C
#define TWI_SDA_PIN PORTC4
#define TWI_SCL_PIN PORTC5


/////////////////////////
// TWI STATUS CONSTANTS
/////////////////////////

const uint8_t TWI_START_TRANSMITTED = 0x08;
const uint8_t TWI_REPEAT_START_TRANSMITTED = 0x10;
const uint8_t TWI_ADDR_TRANSMITTED = 0x18;
const uint8_t TWI_DATA_TRANSMITTED = 0x28;


/////////////////////////
// MISC CONSTANTS
/////////////////////////

// Queue pointer offsets
#define TWI_QUEUE_VOICE_OFFSET NUMBER_OF_CHANNELS
#define TWI_QUEUE_DISPLAY_OFFSET (NUMBER_OF_CHANNELS + NUMBER_OF_CHANNELS)

// Other
#define TWI_QUEUE_LENGTH (TWI_QUEUE_DISPLAY_OFFSET + NUMBER_OF_DISPLAY_DIGITS)  // 12
const uint8_t TWI_SLA_W_BYTE = (TWI_EXT_ADDR << 1);


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initTWI();
/* Initializes the TWI hardware
 * This should be called before transmitting
 *
 * Initialization involves configuring the interface's bit rate and interrupt, and disabling the clock.
 *
 * Note that due to the interface's clock being disabled, it must be enabled before transmission.
 */

void beginI2CTransmission();
/* Begins an I2C transmission using the TWI hardware
 *
 * This function enables the interface's clock and transmits a START condition. It also resets the
 * TWI queue pointer.
 *
 * Queue data and queue length should be ready before the first TWI interrupt.
 *
 * Affects TWI_Queue_Pointer
 */

#endif
