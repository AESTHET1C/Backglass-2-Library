#include "timer1.h"
#include "twi.h"
#include "motor.h"

void initTimer1() {

	initTWI();
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

	// Begin the I2C transmission so we can send first byte as soon as possible
	beginI2CTransmission();

	// queue audio channel notes
	for (byte channel = 0; channel < NUMBER_OF_GLOBAL_CHANNELS; channel++) {
		// TODO
	}
	TWI_Queue_Length = 4;

	// Handle text scrolling
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

	// [Potentially] queue audio channel voices
	if (Voice_Update | Display_Update) {
		for (byte channel = 0; channel < NUMBER_OF_GLOBAL_CHANNELS; channel++) {
			// TODO
		}
		TWI_Queue_Length += 4;
	}

	// [Potentially] queue display data
	if (Display_Update) {
		for (byte digit = 0; digit < NUMBER_OF_DIGITS; digit++) {
			uint8_t Temp_Digit = Display_Data[digit];
			if (Display_Decimal_Override) {
				Temp_Digit = ((Temp_Digit & 0xFE) | (Decimal_Data[digit] ? 1 : 0));
			}
			TWI_Queue_Data[(NUMBER_OF_GLOBAL_CHANNELS + NUMBER_OF_DISPLAY_DIGITS) + digit] = Temp_Digit;
		}
		TWI_Queue_Length += 4;
	}
}