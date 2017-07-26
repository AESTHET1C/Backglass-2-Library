# Backglass 2 User Library

## About

The Backglass 2 User Library gives programmers easy access to advanced behaviors on the [Backglass 2](https://github.com/AESTHET1C/Backglass-2/) pinball controller board using the Arduino IDE. It offers a complete and cohesive set of functions to handle all sensing and output functionality, including advanced audio sequencing and character display on the Backglass 2 pinball controller board.

Functionality of this library includes:

* Sensing bumper hits, and 'RAMP' and 'LOSE' sensor activations
* Setting bumper LED states
* Setting motor state and speed
* Playing and/or looping audio clips
* Displaying scrolling text, remaining number of balls, and other information
* Saving and reading scores and other data to and from non-volatile memory
* Communicating debugging information over serial
* Miscellaneous timing and state-related routines

For additional details on functionality, read through the API overview below.

## Installation

This library can be downloaded from GitHub by clicking the green "Clone or download" button and selecting "Download ZIP".

The library can be imported into the Arduino IDE by selecting `Sketch` in the toolbar, then `Include Library -> Add .ZIP Library...` and browsing to the downloaded ZIP file.

`#include <backglass.h>` must then be added to the top of any programs that use this library.

To add additional audio clips, place the clip's .h file in the sketch folder and include it at the top of the sketch. (`#include "filename.h"`)

## API

`void initBackglass()` – Starts up behind-the-scenes code for the Backglass 2 and configures necessary peripherals. This must be called at the top of `void setup()`.

### Sensors and LEDs

`bool readSensor(sensor_t sensor)` – Reads and returns the specified sensor. Valid arguments are:

* RAMP
* LOSE
* FLIP_L
* FLIP_R
* BUMP_0
* BUMP_1
* BUMP_2

`void setLED(led_t led, bool state)` – Sets the specified LED to either on or off. Valid arguments are:

* BUMP_0
* BUMP_1
* BUMP_2

### Motor

`void setMotor(bool state)` – Sets the motor to either on or off.

`void changeMotorSpeed(uint8_t speed)` – Changes the speed at which the motor turns. The default value is 127.

### Speaker

>#### Some notes about Backglass's audio
>The Backglass Firmware plays audio using 4 *channels*. When an audio clip is played, each of that clip's *tracks* are assigned to unused channels, and are played at the same time. If there are not enough free channels, some of the clip's tracks may not be played. If you are having issues where an audio clip will not play correctly and one or more clips are already playing, check that you are not trying to play more than 4 tracks at a time.
>
>Each of Backglass's channels have independent *voice* control.
>The voice refers to the timbre of the note, and can be between 0 and 3. These are defined as so:
>* Voice 0: 12.5% duty cycle – shrillest sound
>* Voice 1: 25% duty cycle
>* Voice 2: 37.5% duty cycle
>* Voice 3: 50% duty cycle – smoothest sound

`void playTone(byte note, byte duration)` – Plays a given note for a given amount of time. See *notes.inc* for a full list of options.

`void playTone(byte note, byte duration, byte voice, byte volume)` – Same as above, but with the ability to specify the tone's voice and volume. The default voice and volume are 0 and 255, respectively.

`void playAudio(uint8_t audio_clip[])` – Starts playback of the specified audio clip. Included audio clips are:

* VECTREX (3 tracks)
* GAME_OVER (2 tracks)
* COIN (1 track)
* MEGALOVANIA (4 tracks) (song)
* JAMMIN (3 tracks) (song)
* BUBBLEMAN (2 tracks) (song)

`void playAudio(uint8_t audio_clip[], byte volume)` – Same as above, but with the ability to specify playback volume. Default volume is 255.

`void loopAudio(uint8_t audio_clip[])` – Starts playback of the specified audio clip and endlessly loops. Valid arguments are identical to `playAudio()`.

`void loopAudio(uint8_t audio_clip[], byte volume)` – Same as above, but with the ability to specify playback volume. Default volume is 255.

`void stopAudio()` – Stops all audio playback, including looping clips.

`void setVolume(byte volume)` – Sets the maximum speaker volume. The default value is 255.

### Display

`void clearDisplay()` – Clears the display and stops any scrolling text.

`void displayNumber(int number)` – Displays a number with leading zeros. Range is limited from -999 to +9999.

`void displayNumber(int number, bool leading_zeros)` – Same as above, but with the option for leading zeros.

`void displayText(char text[])` – Displays a short alphanumeric string.

`void scrollNumber(long number, byte scroll_speed)` – Clears the display and starts scrolling a number at the specified speed. Scrolling begins off-screen and ends off-screen. No action is taken if text is already scrolling. Scroll speed presets include:

* SLOW_SCROLL
* NORM_SCROLL
* FAST_SCROLL

`void scrollText(char text[], byte scroll_speed)` – Same as above, but displays an alphanumeric string rather than a number.

`void showNumberOfBalls(byte balls)` – Overrides the four decimal points to show the given number of balls. The override is cancelled upon calling `
clearDisplay()`.

`void setDecimalPoints(bool a, bool b, bool c, bool d)` – Overrides the four decimal points. The override is cancelled upon calling `
clearDisplay()`.

`void setDecimalPoints(byte decimal_data)` – Same as above, but using a byte input rather than four booleans. LSB is right-most decimal point.

`int getRemainingScrollCharacters()` – Returns the remaining number of characters to scroll.

### Serial

**TODO**

### Storage

`void saveNumber(int location, int value)` – Saves a value to the specified location in EEPROM. Pre-defined locations include:

* HIGH_SCORE[0] - HIGH_SCORE[9]
* GAME_PLAYS

`void saveText(int location, char text[])` – Saves a short text string (up to 16 characters) to the specified location in EEPROM. Pre-defined locations include:

* HIGH_SCORE_NAME[0] - HIGH_SCORE_NAME[9]

`int loadNumber(int location)` – Loads a value from the specified location in EEPROM. Pre-defined locations are identical to `saveNumber()`.

`char * loadText(int location)` – Loads a short text string from the specified location in EEPROM. Pre-defined locations are identical to `saveText()`.

### Timing

`long getMillis()` – Returns the number of elapsed milliseconds since power-on. Alias of Arduino's `millis()` function.

`void waitMillis(long milliseconds)` – Waits for the given number of milliseconds, blocking other code from running. Should be used sparingly. Alias of Arduino's `delay()` function.
