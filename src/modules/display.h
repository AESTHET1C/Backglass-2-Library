/* Display Module
 *
 * TODO
 * Documentation
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef display
#define display

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "ascii.inc"

typedef uint8_t byte;

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

const byte FAST_SCROLL = 5;
const byte NORM_SCROLL = 7;
const byte SLOW_SCROLL = 10;

#define DISPLAY_QUEUE_LENGTH 128  // Maximum length of scrolling text string. Completely arbitrary.


/////////////////////////
// MISC CONSTANTS
/////////////////////////

#define NUMBER_OF_DISPLAY_DIGITS 4


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void clearDisplay();
/*
 * Clears the display and stops scrolling text
 * Also clears decimal point overrides
 * This operation is atomic
 *
 * Affects TODO
 */

void displayNumber(int number);
void displayNumber(int number, bool leading_zeros);
/*
 * Displays an integer on the display
 * Defaults to leading zeros being shown
 *
 * Numbers outside the range [-999, 9999] are limited.
 * No action is taken if text is currently scrolling.
 *
 * Affects Display_Update, Display_Data[]
 * INPUT:  Integer to be displayed
 *         [Optional] Should leading zeros be shown?
 */

void displayText(char text[]);
/*
 * Displays an alphanumeric string on the display
 *
 * Text longer than 4 characters will be truncated.
 * No action is taken if text is currently scrolling.
 *
 * Affects Display_Update, Display_Data[]
 * INPUT:  String to be displayed
 */

void scrollNumber(long number, byte scroll_speed);
/*
 *
 */

void scrollText(char text[], byte scroll_speed);
/*
 *
 */

void showNumberOfBalls(byte balls);
/*
 * Displays the number of remaining balls using decimal points on the display
 *
 * This overrides decimal points until clearDisplay() is called.
 *
 * The number of balls is left-justified.
 *
 * Affects TODO
 * INPUT:  Number of remaining balls
 */

void setDecimalPoints(bool a, bool b, bool c, bool d);
void setDecimalPoints(byte decimal_data);

int getRemainingScrollCharacters();

#endif
