/* Timer1 Module
 *
 * Handles the Timer1 interrupt routine and links supporting functions
 *
 * This includes display and motor control and output, along with audio sequencing.
 *
 * The Timer1 unit runs at approximately 61 Hz, and output B is used as a fast PWM output.
 * Audio sequencing uses a queue structure to play through audio clips stared in FLASH memory.
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef timer1
#define timer1

#include "audio.h"
#include "display.h"
#include "motor.h"
#include "twi.h"

// Secret debug mode enable
//#define AUDIO_CHANNEL_DEBUG

/////////////////////////
// GLOBAL VARIABLES
/////////////////////////

// Audio
extern volatile uint8_t const * Channel_Pointer_Array[];
extern volatile uint8_t Channel_Note_Remaining_Array[];
extern uint8_t const * Channel_Next_Pointer_Array[];

extern volatile bool Channel_Enabled_Array[];
extern bool Channel_Repeat_Array[];
extern volatile bool Voice_Update;

// Display
extern volatile bool Display_Update;
extern volatile bool Display_Decimal_Override;
extern volatile uint8_t Display_Data[];
extern volatile bool Decimal_Data[];

extern byte Display_Scroll_Delay;
extern volatile byte Display_Scroll_Remaining;

extern volatile bool Display_Queue_Active;
extern volatile uint8_t Display_Queue_Data[];
extern unsigned int Display_Queue_Length;
extern volatile unsigned int Display_Queue_Pointer;


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initTimer1();
/*
 * Initializes timer1 unit and all related functionality
 * Should be called at startup
 *
 * This involves configuring the timer1 unit, interrupt routine, output pins, and data structures.
 */

#endif
