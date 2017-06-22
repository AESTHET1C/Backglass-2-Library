/* EEPROM Module
 *
 * Handles reading and writing to non-volatile memory
 *
 * Two forms of data are able to be used; standard integers, and short character arrays.
 * Character arrays are fixed to maximum size 16.
 *
 * Commonly used values (including high score and number of game plays) are pre-allocated in EEPROM.
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef eeprom
#define eeprom

#include <avr/eeprom.h>
#include <stddef.h>

typedef uint8_t byte;

/////////////////////////
// MISC CONSTANTS
/////////////////////////

#define NUMBER_OF_SCORES 10
#define MAX_TEXT_LENGTH 16

/////////////////////////
// EEPROM ALLOCATIONS
/////////////////////////

// Ints
const byte HIGH_SCORE[NUMBER_OF_SCORES] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
const byte GAME_PLAYS = 20;

// Char arrays
const byte HIGH_SCORE_NAME[NUMBER_OF_SCORES] = {64, 80, 96, 112, 128, 144, 160, 176, 192, 208};


void save(int location, int value);
/*
 * Saves an integer to non-volatile memory
 *
 * This function may block for up to 6.6 milliseconds.
 *
 * INPUT:  Location in EEPROM to save to
 *         Value to save
 */

void saveText(int location, char text[]);
/*
 * Saves a short character array to non-volatile memory
 * Arrays may contain a maximum of 16 characters, excluding the null terminator
 *
 * This function may block for up to (3.3 * characters) milliseconds.
 *
 * INPUT:  Starting location in EEPROM to save to
 *         Array of characters to save
 */

int load(int location);
/*
 * Loads an integer from non-volatile memory
 *
 * INPUT:  Location in EEPROM to load from
 * OUTPUT: Value read from EEPROM
 */

char * loadText(int location);
/*
 * Loads a character array from non-volatile memory
 * Arrays may contain a maximum of 16 characters, excluding the null terminator
 *
 * INPUT:  Starting location in EEPROM to load from
 * OUTPUT: Pointer to array of characters read from EEPROM
 */

#endif
