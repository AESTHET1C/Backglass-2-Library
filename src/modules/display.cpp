#include "display.h"

// Display structure variables
volatile bool Display_Update = false;
volatile bool Display_Decimal_Override = false;
volatile uint8_t Display_Data[NUMBER_OF_DISPLAY_DIGITS] = {0, 0, 0, 0};
volatile bool Decimal_Data[NUMBER_OF_DISPLAY_DIGITS] = {false, false, false, false};

// Scrolling metadata variables
byte Display_Scroll_Delay = 0;
volatile byte Display_Scroll_Remaining = 0;

// Queue structure variables
volatile bool Display_Queue_Active = false;
volatile uint8_t Display_Queue_Data[DISPLAY_QUEUE_LENGTH];
unsigned int Display_Queue_Length = 0;
volatile unsigned int Display_Queue_Pointer = 0;


void clearDisplay() {
	cli();
	Display_Queue_Active = false;
	Display_Decimal_Override = false;
	for (byte Digit = 0; Digit < NUMBER_OF_DISPLAY_DIGITS; Digit++) {
		Display_Data[Digit] = 0;
	}
	Display_Update = true;
	sei();
	return;
}

void displayNumber(int number) {
	displayNumber(number, true);
	return;
}

void displayNumber(int number, bool leading_zeros) {
	if (!Display_Queue_Active) {

		// Constrain input value
		if (number > 9999) {
			number = 9999;
		}
		else if (number < -999) {
			number = -999;
		}

		// Determine sign
		bool Negative = false;
		if (number < 0) {
			Negative = true;
			number = -number;
		}

		// Fill out buffer array
		uint8_t Digit_Buffer[NUMBER_OF_DISPLAY_DIGITS];
		byte Digits_Remaining = NUMBER_OF_DISPLAY_DIGITS;
		while (Digits_Remaining > 0) {
			Digit_Buffer[--Digits_Remaining] = pgm_read_byte((ASCII_DATA + '0' - ASCII_TABLE_OFFSET) + (number % 10));
			number /= 10;
		}

		// Handle leading zeros and sign
		if (leading_zeros) {
			// Add negative sign, if needed
			if (Negative) {
				Digit_Buffer[0] = pgm_read_byte(ASCII_DATA - ASCII_TABLE_OFFSET + '-');
			}
		}
		else {
			// Remove and count leading zeros
			byte Leading_Zeros = 0;
			uint8_t ASCII_ZERO = pgm_read_byte(ASCII_DATA + '0' - ASCII_TABLE_OFFSET);
			while (Leading_Zeros < (NUMBER_OF_DISPLAY_DIGITS - 1)) {
				if (Digit_Buffer[Leading_Zeros] == ASCII_ZERO) {
					Digit_Buffer[Leading_Zeros] = 0;
					Leading_Zeros++;
				}
				else {
					break;
				}
			}

			// Add negative sign, if needed
			if (Negative) {
				Digit_Buffer[Leading_Zeros - 1] = pgm_read_byte(ASCII_DATA - ASCII_TABLE_OFFSET + '-');
			}
		}

		// Copy buffer array to Display_Data array (atomic operation)
		cli();
		for (byte Digit = 0; Digit < NUMBER_OF_DISPLAY_DIGITS; Digit++) {
			Display_Data[Digit] = Digit_Buffer[Digit];
		}
		sei();
		Display_Update = true;
	}
	return;
}

void displayText(char text[]) {
	if (!Display_Queue_Active) {

		// Create and fill buffer array
		uint8_t Digit_Buffer[NUMBER_OF_DISPLAY_DIGITS];
		byte Digit_Buffer_Pointer = 0;
		byte Text_Pointer = 0;
		char Current_Character = text[Text_Pointer];
		while (Current_Character) {

			// Map input ascii text to display data and add to display queue array
			if (Current_Character < ASCII_TABLE_OFFSET) {
				Digit_Buffer[Digit_Buffer_Pointer] = 0;
			}
			else {
				Digit_Buffer[Digit_Buffer_Pointer] = pgm_read_byte(ASCII_DATA - ASCII_TABLE_OFFSET + Current_Character);
			}

			// Read in next character
			Current_Character = text[++Text_Pointer];

			// If the next character is a period, tack that onto the previous character if possible
			if ((Current_Character == '.') && !(Digit_Buffer[Digit_Buffer_Pointer] & 0x80)) {
				Digit_Buffer[Digit_Buffer_Pointer] |= 0x80;
				Current_Character = text[++Text_Pointer];
			}

			// Move on to the next digit
			Digit_Buffer_Pointer += 1;

			// Quit if the buffer array is full
			if (Digit_Buffer_Pointer >= NUMBER_OF_DISPLAY_DIGITS) {
				break;
			}
		}

		// Blank all unused digits in the buffer
		while (Digit_Buffer_Pointer < NUMBER_OF_DISPLAY_DIGITS) {
			Digit_Buffer[Digit_Buffer_Pointer++] = 0;
		}

		// Copy buffer array to Display_Data array (atomic operation)
		cli();
		for (byte Digit = 0; Digit < NUMBER_OF_DISPLAY_DIGITS; Digit++) {
			Display_Data[Digit] = Digit_Buffer[Digit];
		}
		sei();
		Display_Update = true;
	}
	return;
}

void scrollNumber (long number, byte scroll_speed) {
	if (!Display_Queue_Active) {

		// Clear display (atomic operation)
		cli();
		for (byte Digit = 0; Digit < NUMBER_OF_DISPLAY_DIGITS; Digit++) {
			Display_Data[Digit] = 0;
		}
		sei();

		// Determine sign
		bool Negative = false;
		if (number < 0) {
			Negative = true;
			number = -number;
		}

		// Fill out buffer array
		uint8_t Digit_Buffer[DISPLAY_QUEUE_LENGTH - NUMBER_OF_DISPLAY_DIGITS];
		unsigned int Digit_Buffer_Pointer = (DISPLAY_QUEUE_LENGTH - NUMBER_OF_DISPLAY_DIGITS);
		while (Digit_Buffer_Pointer > 0) {
			Digit_Buffer[--Digit_Buffer_Pointer] = pgm_read_byte((ASCII_DATA + '0' - ASCII_TABLE_OFFSET) + (number % 10));
			number /= 10;
			if (number == 0) {
				break;
			}
		}

		// Add negative sign, if needed
		if (Negative) {
			Digit_Buffer[--Digit_Buffer_Pointer] = pgm_read_byte(ASCII_DATA - ASCII_TABLE_OFFSET + '-');
		}

		// Queue up the number
		Display_Queue_Pointer = 0;
		while (Digit_Buffer_Pointer < (DISPLAY_QUEUE_LENGTH - NUMBER_OF_DISPLAY_DIGITS)) {
			Display_Queue_Data[Display_Queue_Pointer++] = Digit_Buffer[Digit_Buffer_Pointer++];
		}

		// Pad display data queue with blank characters (to scroll number off display)
		for (byte Digit = 0; Digit < NUMBER_OF_DISPLAY_DIGITS; Digit++) {
			Display_Queue_Data[Display_Queue_Pointer++] = 0;
		}

		// Set up queue metadata
		Display_Queue_Length = Display_Queue_Pointer;
		Display_Queue_Pointer = 0;
		Display_Scroll_Delay = scroll_speed;
		Display_Scroll_Remaining = scroll_speed;

		// Start the scrolling magic
		Display_Queue_Active = true;

	}
	return;
}

void scrollText(char text[], byte scroll_speed) {
	if (!Display_Queue_Active) {

		// Clear display (atomic operation)
		cli();
		for (byte Digit = 0; Digit < NUMBER_OF_DISPLAY_DIGITS; Digit++) {
			Display_Data[Digit] = 0;
		}
		sei();

		// Queue up the text string
		Display_Queue_Pointer = 0;
		unsigned int Text_Pointer = 0;
		char Current_Character = text[Text_Pointer];
		while (Current_Character) {

			// Map input ascii text to display data and add to display queue array
			if (Current_Character < ASCII_TABLE_OFFSET) {
				Display_Queue_Data[Display_Queue_Pointer] = 0;
			}
			else {
				Display_Queue_Data[Display_Queue_Pointer] = pgm_read_byte(ASCII_DATA - ASCII_TABLE_OFFSET + Current_Character);
			}

			// Update display queue metadata
			Display_Queue_Pointer += 1;
			Text_Pointer += 1;

			// Quit if the queue is full
			if (Display_Queue_Pointer >= (DISPLAY_QUEUE_LENGTH - NUMBER_OF_DISPLAY_DIGITS)) {
				break;
			}

			// Load next ascii text character
			Current_Character = text[Text_Pointer];

			// If the next ascii text character is a period, tack that onto the previous character if possible
			if ((Current_Character == '.') && !(Display_Queue_Data[Display_Queue_Pointer - 1] & 0x80)) {
				Display_Queue_Data[Display_Queue_Pointer - 1] |= 0x80;
				Current_Character = text[++Text_Pointer];
			}
		}

		// Pad display data queue with blank characters (to scroll text off display)
		for (byte Digit = 0; Digit < NUMBER_OF_DISPLAY_DIGITS; Digit++) {
			Display_Queue_Data[Display_Queue_Pointer++] = 0;
		}

		// Set up queue metadata
		Display_Queue_Length = Display_Queue_Pointer;
		Display_Queue_Pointer = 0;
		Display_Scroll_Delay = scroll_speed;
		Display_Scroll_Remaining = scroll_speed;

		// Start the scrolling magic
		Display_Queue_Active = true;
	}
	return;
}

void showNumberOfBalls(byte balls) {
	if (balls > 4) {
		balls = 4;
	}
	return setDecimalPoints(~(0xFF << balls));
}

void setDecimalPoints(bool a, bool b, bool c, bool d) {
	cli();
	Decimal_Data[0] = a;
	Decimal_Data[1] = b;
	Decimal_Data[2] = c;
	Decimal_Data[3] = d;
	sei();
	Display_Decimal_Override = true;
	Display_Update = true;
	return;
}

void setDecimalPoints(byte decimal_data) {
	bool a = ((decimal_data & B00000001) > 0);
	bool b = ((decimal_data & B00000010) > 0);
	bool c = ((decimal_data & B00000100) > 0);
	bool d = ((decimal_data & B00001000) > 0);
	return setDecimalPoints(a, b, c, d);
}

int getRemainingScrollCharacters() {
	if (Display_Queue_Active) {
		return (Display_Queue_Length - Display_Queue_Pointer);
	}
	return 0;
}
