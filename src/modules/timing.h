/* Timing Module
 *
 * Handles timing functions
 *
 * Almost nothing's here; it's just aliases to Arduino functions.
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef timing
#define timing

#include <arduino.h>

#define getMillis() millis()
#define waitMillis(m) delay(m)

#endif
