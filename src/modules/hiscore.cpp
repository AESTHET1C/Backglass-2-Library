#include "hiscore.h"

char Player_Name[MAX_CHARACTERS + 1];


char * getPlayerName() {
	Restart:

	// Prepare data structure
	for (int Character = 0; Character <= MAX_CHARACTERS; Character++) {
		Player_Name[Character] = 0;
	}

	clearDisplay();

	byte Current_Character = 0;

	// Get data from player
	while (Current_Character < MAX_CHARACTERS) {
		for (byte Current_Digit = 0; Current_Digit < 3; Current_Digit++) {
			Display_Data[Current_Digit] = Display_Data[Current_Digit + 1];
		}
		Player_Name[Current_Character] = getCharacter();

		// Check that left flipper isn't pressed before continuing
		waitMillis(FLIP_DEBOUNCE_TIME);
		if (readSensor(FLIP_L)) {
			while (readSensor(FLIP_L)) {
				// Do nothing
			}
		}
		waitMillis(FLIP_DEBOUNCE_TIME);

		Current_Character += 1;
	}

	// Wait for player confirmation
	while(!readSensor(FLIP_L) && !readSensor(FLIP_R)) {
		// Do nothing
	}
	if (readSensor(FLIP_R)) {
		goto Restart;
	}

	return Player_Name;
}

char getCharacter() {

	// Character selection state variables
	bool Increment_Current_Character = false;
	byte Current_Character = 0;
	uint8_t Current_Digit = ASCII_DATA[CHARACTER_LIST[Current_Character] - ASCII_TABLE_OFFSET];
	unsigned int Current_Character_Repeats = 0;
	long Current_Character_Last_Change_Time = 0;

	// Blink state variables
	bool Display_Current_Digit = true;
	long Current_Digit_Blink_Start = getMillis();

	// Flipper state variables
	bool Right_Flipper_Just_Pressed = false;
	bool Right_Flipper_Currently_Pressed = false;
	bool Right_Flipper_Last_Pressed = false;
	long Right_Flipper_Debounce_Start = 0;

	while (!(readSensor(FLIP_L))) {
		long Current_Time = getMillis();

		// Handle digit blinking
		if ((Current_Time - Current_Digit_Blink_Start) > (Display_Current_Digit ? BLINK_ON_TIME : BLINK_OFF_TIME)) {
			Display_Current_Digit = !Display_Current_Digit;
			Current_Digit_Blink_Start = Current_Time;
		}

		// Reset Right_Flipper_Just_Pressed
		Right_Flipper_Just_Pressed = false;

		// Handle flipper debounce
		if ((Current_Time - Right_Flipper_Debounce_Start) >= FLIP_DEBOUNCE_TIME) {
			if (readSensor(FLIP_R) != Right_Flipper_Last_Pressed) {
				Right_Flipper_Last_Pressed = readSensor(FLIP_R);
				Right_Flipper_Debounce_Start = Current_Time;
				Right_Flipper_Currently_Pressed = readSensor(FLIP_R);
				if (Right_Flipper_Currently_Pressed) {
					Right_Flipper_Just_Pressed = true;
				}
			}
		}

		// Handle character selection
		if (Right_Flipper_Currently_Pressed == false) {
			Current_Character_Repeats = 0;
			Current_Character_Last_Change_Time = Current_Time;
		}
		else {
			unsigned int Current_Repeat_Rate = ((Current_Character_Repeats > REPEAT_THRESHOLD) ? REPEAT_RATE_FAST : REPEAT_RATE_SLOW);

			if (Right_Flipper_Just_Pressed) {
				Current_Character_Last_Change_Time = Current_Time;
				Current_Character_Repeats = 1;
				Increment_Current_Character = true;
			}
			else if ((Current_Time - Current_Character_Last_Change_Time) > Current_Repeat_Rate) {
				Current_Character_Repeats += 1;
				Current_Character_Last_Change_Time = Current_Time;
				Increment_Current_Character = true;
			}

			if (Increment_Current_Character) {
				Increment_Current_Character = false;
				Current_Character += 1;
				if (Current_Character >= CHARACTER_LIST_LENGTH) {
					Current_Character = 0;
				}
				if (CHARACTER_LIST[Current_Character] == ' ') {
					Current_Digit = B00001000;
				}
				else {
					Current_Digit = pgm_read_byte(ASCII_DATA - ASCII_TABLE_OFFSET + CHARACTER_LIST[Current_Character]);
				}
			}
		}

		// Update the display (atomic operation)
		cli();
		Display_Data[3] = (Display_Current_Digit ? Current_Digit : 0);
		sei();
		Display_Update = true;
	}

	// Update the display, showing the selected digit (atomic operation)
	if (CHARACTER_LIST[Current_Character] == ' ') {
		cli();
		Display_Data[3] = B00000000;
		sei();
	}
	else {
		cli();
		Display_Data[3] = Current_Digit;
		sei();
	}
	Display_Update = true;

	return CHARACTER_LIST[Current_Character];
}
