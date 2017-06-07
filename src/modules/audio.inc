/* Audio clip data
 *
 * Audio clips available to play using the playAudio() and loopAudio() commands
 *
 * Human-friendly macros can be found in notes.inc.
 *
 * Information on the structure of audio clip data can be found in the Theory of Operation document.
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>,
 *            Mike Swann <>
 */

#ifndef audio_inc
#define audio_inc

#include "notes.inc"

const PROGMEM uint8_t VECTREX[] = {
	3, // Number of tracks (1-4)

	// Track 0 configuration
	1,  // Voice type (0-3)
	63, // Volume (0-63)

	// Track 0 data
	NOTE_D4,  EIGHTH_NOTE,
	REST,     SIXTY_FOURTH_NOTE,
	NOTE_D4,  SIXTEENTH_NOTE,
	NOTE_G5,  (HALF_NOTE + QUARTER_NOTE),
	REST, 0,  // End of track data

	// Track 1 configuration
	2,  // Voice type (0-3)
	3,  // Volume (0-63)

	// Track 1 data
	NOTE_D5,  EIGHTH_NOTE,
	REST,     SIXTY_FOURTH_NOTE,
	NOTE_D5,  (SIXTEENTH_NOTE + HALF_NOTE + QUARTER_NOTE),
	REST, 0,  // End of track data

	// Track 2 configuration
	0,  // Voice type (0-3)
	8,  // Volume (0-63)

	// Track 2 data
	REST,     (EIGHTH_NOTE + SIXTY_FOURTH_NOTE + SIXTEENTH_NOTE),
	NOTE_G6,  (HALF_NOTE + QUARTER_NOTE),
	REST, 0,  // End of track data
};

#endif
