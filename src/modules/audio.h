/* Audio Module
 *
 * TODO
 * Documentation
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

const byte DEFAULT_CHANNEL_VOICE =  0;
const byte DEFAULT_CHANNEL_VOLUME = 63;
const byte DEFAULT_GLOBAL_VOLUME =  255;


/////////////////////////
// MISC CONSTANTS
/////////////////////////

const byte NUMBER_OF_CHANNELS = 4;

// Bit masks
const uint8_t VOICE_MASK =  B11000000;
const uint8_t VOLUME_MASK = B00111111;


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initAudio();
/*
 *
 */

void playAudio(uint8_t audio_clip[]);
/*
 *
 */

void loopAudio(uint8_t audio_clip[]);
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


/////////////////////////
// INTERNAl FUNCTIONS
/////////////////////////

void queueAudioClip(uint8_t audio_clip[], bool loop);
/*
 *
 */
