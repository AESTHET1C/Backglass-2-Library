#include "timer1.h"

void initTimer1() {

	initTWI();
	initAudio();
	initMotor();

	// Configure and enable Timer1 unit
	TCCR1A = (1 << WGM10);
	TCCR1B = ((1 << WGM12) | (1 << CS12) | (1 << CS10));
	TIMSK1 = (1 << TOIE1);
}

ISR(TIMER1_OVF_vect) {

	// Display currently playing audio channels if secret debug mode enabled
	#ifdef AUDIO_CHANNEL_DEBUG
		Decimal_Data[0] = Channel_Enabled_Array[0];
		Decimal_Data[1] = Channel_Enabled_Array[1];
		Decimal_Data[2] = Channel_Enabled_Array[2];
		Decimal_Data[3] = Channel_Enabled_Array[3];
		Display_Decimal_Override = true;
		Display_Update = true;
	#endif

	// Begin the I2C transmission so we can send first byte as soon as possible
	beginI2CTransmission();

	// Queue audio channel notes
	for (byte Channel = 0; Channel < NUMBER_OF_CHANNELS; Channel++) {
		if ((Channel_Enabled_Array[Channel]) && (Channel_Note_Remaining_Array[Channel]-- == 0)){
			TWI_Queue_Data[Channel] = pgm_read_byte(Channel_Pointer_Array[Channel]++);
			Channel_Note_Remaining_Array[Channel] = pgm_read_byte(Channel_Pointer_Array[Channel]++);
			if ((Channel_Note_Remaining_Array[Channel]-- == 0) && (TWI_Queue_Data[Channel] == 0)) {
				if (Channel_Repeat_Array[Channel]) {
					Channel_Pointer_Array[Channel] = Channel_Next_Pointer_Array[Channel];
					TWI_Queue_Data[Channel] = pgm_read_byte(Channel_Pointer_Array[Channel]++);
					Channel_Note_Remaining_Array[Channel] = pgm_read_byte(Channel_Pointer_Array[Channel]++);
				}
				else {
					Channel_Enabled_Array[Channel] = false;
				}
			}
		}
	}
	TWI_Queue_Length = NUMBER_OF_CHANNELS;

	// Handle text scrolling
	if (Display_Queue_Active) {
		if (Display_Scroll_Remaining-- == 0) {

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

	// NOTICE
	// This line is added purely to account for a bug in the Backglass 2 firmware
	// It forces at least 8 bytes to be sent per transmission
	// It should be removed when this bug is fixed
	Voice_Update = true;

	// Queue audio channel voices if needed
	if (Voice_Update || Display_Update) {
		TWI_Queue_Length += NUMBER_OF_CHANNELS;
		Voice_Update = false;
	}

	// Queue display data if needed
	if (Display_Update) {
		for (byte Digit = 0; Digit < NUMBER_OF_DISPLAY_DIGITS; Digit++) {
			uint8_t Temp_Digit = Display_Data[Digit];
			if (Display_Decimal_Override) {
				Temp_Digit = ((Temp_Digit & 0x7F) | (Decimal_Data[Digit] ? 0x80 : 0));
			}
			TWI_Queue_Data[TWI_QUEUE_DISPLAY_OFFSET + Digit] = Temp_Digit;
		}
		TWI_Queue_Length += NUMBER_OF_DISPLAY_DIGITS;
	}

	return;
}
