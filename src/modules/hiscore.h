/* High Score Module
 *
 * TODO
 * Documentation
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef hiscore
#define hiscore

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "ascii.inc"
#include "display.h"
#include "eeprom.h"
#include "io.h"
#include "timer1.h"
#include "timing.h"

typedef uint8_t byte;


/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

// Length of desired string for inputting name
const unsigned int MAX_CHARACTERS = NUMBER_OF_DISPLAY_DIGITS;

// Blink timing
const unsigned int BLINK_ON_TIME = 100;
const unsigned int BLINK_OFF_TIME = 100;

// Character selection timing
const unsigned int REPEAT_RATE_SLOW = 300;
const unsigned int REPEAT_RATE_FAST = 200;
const byte REPEAT_THRESHOLD = 2;

// Debounce parameters
const unsigned FLIP_DEBOUNCE_TIME = 150;


/////////////////////////
// CHARACTER LIST
/////////////////////////

const char CHARACTER_LIST[] = {
	'A',
	'b',
	'C',
	'd',
	'E',
	'F',
	'G',
	'H',
	'I',
	'J',
	'k',
	'L',
	'M',
	'N',
	'O',
	'P',
	'q',
	'R',
	'S',
	't',
	'U',
	'v',
	'W',
	'X',
	'y',
	'Z',
	' '
};

#define CHARACTER_LIST_LENGTH (sizeof(CHARACTER_LIST) / sizeof(char))


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

byte getHighScorePosition(int score);
/*
 *
 */

char * getPlayerName();
/*
 * Gets the player's name and returns a string
 *
 * This function is blocking and overrides the display
 *
 * The right flipper is used to select characters, and the left is used to confirm selection
 * This repeats until a string of a set length is created.
 *
 * Upon completion of the string, the player can either confirm the name with the left flipper
 * or restart the process with the right flipper.
 *
 * Affects Player_Name[]
 * OUTPUT: Pointer to a character array containing the player name
 */

void insertHighScore(byte position, int score, char name[]);
/*
 *
 */

void resetHighScores();
/*
 *
 */


/////////////////////////
// INTERNAL FUNCTIONS
/////////////////////////

char getCharacter();
/*
 * Gets a single ascii character from the user
 *
 * This function is blocking and overrides the display
 *
 * The right flipper is used to cycle through available characters,
 * and the left flipper is used to confirm selection.
 *
 * The currently selected character is displayed (blinking) on the right-most digit on the display.
 * Upon confirmation, the character becomes solid.
 *
 * Auto-repeat is enabled on the right flipper in two stages.
 *
 * OUTPUT: Selected character
 */


#endif