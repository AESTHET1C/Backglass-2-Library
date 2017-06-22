/* Backglass 2 User Library
 *
 * A cohesive user-oriented library for the Backglass 2 Pinball Controller Board
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef backglass
#define backglass

#include "modules/io.h"
#include "modules/timer1.h"
#include "modules/eeprom.h"

/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initBackglass();
/*
 * Initializes all Backglass 2 modules
 * Must be called at start of setup()
 *
 * Also disables SPI and Timer2 devices to save power.
 */

#endif
