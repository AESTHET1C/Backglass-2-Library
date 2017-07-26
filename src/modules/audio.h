/* Audio Module
 *
 * TODO
 * Documentation
 *
 * Some notes on terminology:
 *
 *	 The audio/visual coprocessor offers four "channels" of audio.
 *
 *	 These channels each have a configurable waveform (selectable duty cycle). The term "voice" refers
 *	 to this selection of waveform, while the term "voice configuration" refers to the combination of
 *	 a channel's waveform selection and volume.
 *
 *	 An audio "clip" is a pre-determined sequence of notes and rests. These clips may have between
 *	 1 and 4 "tracks", which are assigned to available channels when a clip is played.
 *
 * All tracks of an audio clip start playing simultaneously, but loop independently of each other.
 * For a multi-track audio clip to loop properly, all tracks must be of identical duration.
 *
 * If there are not enough free channels to play all tracks in an audio clip, the highest-numbered
 * track(s) of the audio clip are not played.
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef audio
#define audio

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "twi.h"
#include "audio.inc"
#include "notes.inc"

typedef uint8_t byte;

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

const byte DEFAULT_CHANNEL_VOICE =  0;
const byte DEFAULT_CHANNEL_VOLUME = 255;  // Scales the volume of individual channels within an audio clip
const byte DEFAULT_CLIP_VOLUME = 255;     // Scales the volume of all channels of an audio clip
const byte DEFAULT_GLOBAL_VOLUME =  255;  // Scales the volume of all channels of all audio


/////////////////////////
// MISC CONSTANTS
/////////////////////////

#define NUMBER_OF_CHANNELS 4

// Bit masks
const uint8_t VOICE_MASK =  B00000011;
const uint8_t VOLUME_MASK = B11111100;

const uint8_t MAX_VOICE_VOLUME = 63;  // The maximum volume any voice configuration may hold

const PROGMEM uint8_t NULL_TERMINATOR[] = {0, 0};


/////////////////////////
// GLOBAL VARIABLES
/////////////////////////

// TWI
extern volatile uint8_t TWI_Queue_Data[];
extern volatile byte TWI_Queue_Length;
extern byte TWI_Queue_Pointer;


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initAudio();
/*
 *
 */

void playTone(byte note, byte duration);
void playTone(byte note, byte duration, byte voice, byte volume);
/*
 *
 */

void playAudio(const uint8_t audio_clip[]);
void playAudio(const uint8_t audio_clip[], byte volume);
/*
 *
 */

void loopAudio(const uint8_t audio_clip[]);
void loopAudio(const uint8_t audio_clip[], byte volume);
/*
 *
 */

void stopAudio();
/*
 *
 */

void setVolume(byte volume);
/*
 *
 */

bool audioPlaying();
/*
 *
 */


/////////////////////////
// INTERNAl FUNCTIONS
/////////////////////////

void queueAudioClip(const uint8_t audio_clip[], bool loop, byte volume);
/*
 *
 */

uint8_t scaleVolume(uint8_t volume_a, uint8_t volume_b);
/*
 * Scales an 8-bit volume by another 8-bit volume
 *
 * This can be used scale local volume levels by global volume levels, and vice versa.
 * The output level will always be lower than or equal to the higher of the two inputs.
 *
 * INPUT:  First volume
 *         Second volume
 * OUTPUT: Scaled volume
 */

#endif
