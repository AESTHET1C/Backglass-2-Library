# MID2HEAD—.MID to .H File Conversion Script

## About

MID2HEAD is a Python script that reads certain types of MIDI files and converts the data into a proprietary scheme that can be used by the [Backglass 2 User Library](https://github.com/AESTHET1C/Backglass-2-Library/). This allows the Library to play custom sequenced audio.

## Usage

Place the MIDI file in the same directory as MID2HEAD. Run MID2HEAD and input the file name, including the file extension (".mid", ".midi"). If no errors arise, follow the instructions given by the script to start using the generated file. Note that while errors may occur, it does not necessarily mean the output file will be unusable. In the event that the output file cannot be played, follow the advice of the following section.

## Requirements for MIDI files

The verified method to ensure compatibility is to follow this procedure:

* Use OpenMPT version 1.25 or 1.26
* Start a new IT project
* Set the number of channels to 4 (or fewer)
* Set the initial tempo to 150
* Set the Ticks/Row to 1

MIDI can be exported from `File -> Export as MIDI…` using the default settings.

New users of OpenMPT should also verify that an instrument is assigned to each channel containing notes (or those notes will not be played). We suggest "NES VST 1.2" to maintain a similar sound to the Backglass 2 hardware.

-----

For those who wish to use MIDI files obtained through some other means, these files are expected to adhere to several technical specifics, including:

* MIDI format 1 (REQUIRED)
* MIDI Division of 480
* MIDI Tempo of 66667
* Between 2 and 5 MIDI tracks (including timing track) (REQUIRED)

Note that while different division and tempo settings are allowed, they may produce incorrect results.
