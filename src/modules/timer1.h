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
