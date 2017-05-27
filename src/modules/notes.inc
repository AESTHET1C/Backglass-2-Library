/* Note definitions
 *
 * Enumerates note frequencies and durations for use by audio functions
 *
 * Note duration naming is based upon an assumption of ~114 BPM with x/4 time.
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef notes_inc
#define notes_inc

// Notes
const uint8_t REST =     0;
const uint8_t NOTE_C0 =  1;
const uint8_t NOTE_Db0 = 2;
const uint8_t NOTE_D0 =  3;
const uint8_t NOTE_Eb0 = 4;
const uint8_t NOTE_E0 =  5;
const uint8_t NOTE_F0 =  6;
const uint8_t NOTE_Gb0 = 7;
const uint8_t NOTE_G0 =  8;
const uint8_t NOTE_Ab0 = 9;
const uint8_t NOTE_A0 =  10;
const uint8_t NOTE_Bb0 = 11;
const uint8_t NOTE_B0 =  12;
const uint8_t NOTE_C1 =  13;
const uint8_t NOTE_Db1 = 14;
const uint8_t NOTE_D1 =  15;
const uint8_t NOTE_Eb1 = 16;
const uint8_t NOTE_E1 =  17;
const uint8_t NOTE_F1 =  18;
const uint8_t NOTE_Gb1 = 19;
const uint8_t NOTE_G1 =  20;
const uint8_t NOTE_Ab1 = 21;
const uint8_t NOTE_A1 =  22;
const uint8_t NOTE_Bb1 = 23;
const uint8_t NOTE_B1 =  24;
const uint8_t NOTE_C2 =  25;
const uint8_t NOTE_Db2 = 26;
const uint8_t NOTE_D2 =  27;
const uint8_t NOTE_Eb2 = 28;
const uint8_t NOTE_E2 =  29;
const uint8_t NOTE_F2 =  30;
const uint8_t NOTE_Gb2 = 31;
const uint8_t NOTE_G2 =  32;
const uint8_t NOTE_Ab2 = 33;
const uint8_t NOTE_A2 =  34;
const uint8_t NOTE_Bb2 = 35;
const uint8_t NOTE_B2 =  36;
const uint8_t NOTE_C3 =  37;
const uint8_t NOTE_Db3 = 38;
const uint8_t NOTE_D3 =  39;
const uint8_t NOTE_Eb3 = 40;
const uint8_t NOTE_E3 =  41;
const uint8_t NOTE_F3 =  42;
const uint8_t NOTE_Gb3 = 43;
const uint8_t NOTE_G3 =  44;
const uint8_t NOTE_Ab3 = 45;
const uint8_t NOTE_A3 =  46;
const uint8_t NOTE_Bb3 = 47;
const uint8_t NOTE_B3 =  48;
const uint8_t NOTE_C4 =  49;
const uint8_t NOTE_Db4 = 50;
const uint8_t NOTE_D4 =  51;
const uint8_t NOTE_Eb4 = 52;
const uint8_t NOTE_E4 =  53;
const uint8_t NOTE_F4 =  54;
const uint8_t NOTE_Gb4 = 55;
const uint8_t NOTE_G4 =  56;
const uint8_t NOTE_Ab4 = 57;
const uint8_t NOTE_A4 =  58;
const uint8_t NOTE_Bb4 = 59;
const uint8_t NOTE_B4 =  60;
const uint8_t NOTE_C5 =  61;
const uint8_t NOTE_Db5 = 62;
const uint8_t NOTE_D5 =  63;
const uint8_t NOTE_Eb5 = 64;
const uint8_t NOTE_E5 =  65;
const uint8_t NOTE_F5 =  66;
const uint8_t NOTE_Gb5 = 67;
const uint8_t NOTE_G5 =  68;
const uint8_t NOTE_Ab5 = 69;
const uint8_t NOTE_A5 =  70;
const uint8_t NOTE_Bb5 = 71;
const uint8_t NOTE_B5 =  72;
const uint8_t NOTE_C6 =  73;
const uint8_t NOTE_Db6 = 74;
const uint8_t NOTE_D6 =  75;
const uint8_t NOTE_Eb6 = 76;
const uint8_t NOTE_E6 =  77;
const uint8_t NOTE_F6 =  78;
const uint8_t NOTE_Gb6 = 79;
const uint8_t NOTE_G6 =  80;
const uint8_t NOTE_Ab6 = 81;
const uint8_t NOTE_A6 =  82;
const uint8_t NOTE_Bb6 = 83;
const uint8_t NOTE_B6 =  84;
const uint8_t NOTE_C7 =  85;
const uint8_t NOTE_Db7 = 86;
const uint8_t NOTE_D7 =  87;
const uint8_t NOTE_Eb7 = 88;
const uint8_t NOTE_E7 =  89;
const uint8_t NOTE_F7 =  90;
const uint8_t NOTE_Gb7 = 91;
const uint8_t NOTE_G7 =  92;
const uint8_t NOTE_Ab7 = 93;
const uint8_t NOTE_A7 =  94;
const uint8_t NOTE_Bb7 = 95;
const uint8_t NOTE_B7 =  96;
const uint8_t NOTE_C8 =  97;
const uint8_t NOTE_Db8 = 98;
const uint8_t NOTE_D8 =  99;
const uint8_t NOTE_Eb8 = 100;
const uint8_t NOTE_E8 =  101;
const uint8_t NOTE_F8 =  102;
const uint8_t NOTE_Gb8 = 103;
const uint8_t NOTE_G8 =  104;
const uint8_t NOTE_Ab8 = 105;
const uint8_t NOTE_A8 =  106;
const uint8_t NOTE_Bb8 = 107;
const uint8_t NOTE_B8 =  108;

// Full written note lengths
const uint8_t HUNDRED_TWENTY_EIGTH_NOTE = 1;
const uint8_t SIXTY_FOURTH_NOTE =         2;
const uint8_t THIRTY_SECOND_NOTE =        4;
const uint8_t SIXTEENTH_NOTE =            8;
const uint8_t EIGHTH_NOTE =               16;
const uint8_t QUARTER_NOTE =              32;
const uint8_t HALF_NOTE =                 64;
const uint8_t WHOLE_NOTE =                128;

// Shortened written note lengths
const uint8_t 128TH_NOTE = 1;
const uint8_t 64TH_NOTE =  2;
const uint8_t 32ND_NOTE =  4;
const uint8_t 16TH_NOTE =  8;
const uint8_t 8TH_NOTE =   16;

// Shorthand note lengths
const uint8_t 128_NOTE = 1;
const uint8_t 64_NOTE =  2;
const uint8_t 32_NOTE =  4;
const uint8_t 16_NOTE =  8;
const uint8_t 8_NOTE =   16;
const uint8_t 4_NOTE =   32;
const uint8_t 2_NOTE =   64;
const uint8_t 1_NOTE =   128;

#endif
