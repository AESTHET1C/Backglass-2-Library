#include "timer1.h"
#include "motor.h"

void initTimer1() {

	initMotor();

	// Configure and enable Timer1 unit
	TCCR1A = ((1 << COM1B1) | (1 << WGM10));
	TCCR1B = ((1 << WGM12) | (1 << CS12) | (1 << CS10));
	TIMSK1 = (1 << TOIE);
}

ISR(TIMER1_OVF_vect) {
	// TODO
	// Calculate new note values
	// Queue TWI packet

	/*
	volatile bool Voice_Update = false;

	uint8_t TWI_Packet_Array[12];
	byte TWI_Packet_Length = 0;
	byte TWI_Packet_Pointer = 0;
	*/

	// Send audio channel notes
	for (byte channel = 0; channel < NUMBER_OF_GLOBAL_CHANNELS; channel++) {
		// TODO
	}

	// Handle scrolling text
	if (Display_Queue_Active) {
		if (Display_Queue_Remaining-- == 0) {

			// Scroll next digit
			Display_Data[0] = Display_Data[1];
			Display_Data[1] = Display_Data[2];
			Display_Data[2] = Display_Data[3];
			Display_Data[3] = Display_Queue_Data[Display_Queue_Pointer++];

			// Update display metadata
			Display_Update = true;

			// Update Scrolling metadata
			Display_Scroll_Remaining = Display_Scroll_Delay;

			// End scrolling if needed
			if (Display_Queue_Pointer >= Display_Queue_Length) {
				Display_Queue_Active = false;
			}
		}
	}

	// [Potentially] send audio channel voices
	if (Voice_Update | Display_Update) {
		for (byte channel = 0; channel < NUMBER_OF_GLOBAL_CHANNELS; channel++) {
			// TODO
		}
	}

	// [Potentially] send display data
	if (Display_Update) {
		for (byte digit = 0; digit < NUMBER_OF_DIGITS; digit++) {
			uint8_t Temp_Digit = Display_Data[digit];
			if (Display_Decimal_Override) {
				Temp_Digit = ((Temp_Digit & 0xFE) | (Decimal_Data[digit] ? 1 : 0));
			}
			TWI_Packet_Array[(NUMBER_OF_GLOBAL_CHANNELS + NUMBER_OF_DISPLAY_DIGITS) + digit] = Temp_Digit;
		}
	}
}