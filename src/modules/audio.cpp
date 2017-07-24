#include "audio.h"

// Queue structure variables
volatile uint8_t const * Channel_Pointer_Array[NUMBER_OF_CHANNELS];
volatile uint8_t Channel_Note_Remaining_Array[NUMBER_OF_CHANNELS];
uint8_t const * Channel_Next_Pointer_Array[NUMBER_OF_CHANNELS];

// Queue status variables
volatile bool Channel_Enabled_Array[NUMBER_OF_CHANNELS];
bool Channel_Repeat_Array[NUMBER_OF_CHANNELS];
volatile bool Voice_Update = false;

// Channel configuration variables
byte Global_Volume = DEFAULT_GLOBAL_VOLUME;
byte Voice_Array[NUMBER_OF_CHANNELS];
byte Volume_Array[NUMBER_OF_CHANNELS];


void initAudio() {
	const uint8_t Default_Channel_Volume = scaleVolume(scaleVolume(DEFAULT_CHANNEL_VOLUME, DEFAULT_GLOBAL_VOLUME), MAX_VOICE_VOLUME);
	const uint8_t Default_Channel_Configuration = (((Default_Channel_Volume << 6) & VOICE_MASK) | (DEFAULT_CHANNEL_VOLUME & VOLUME_MASK));

	for (byte Channel = 0; Channel < NUMBER_OF_CHANNELS; Channel++) {
		// Queue status variables
		Channel_Enabled_Array[Channel] = false;

		// Channel configuration variables
		Voice_Array[Channel] = DEFAULT_CHANNEL_VOICE;
		Volume_Array[Channel] = DEFAULT_CHANNEL_VOLUME;

		// Channel voice data
		TWI_Queue_Data[TWI_QUEUE_VOICE_OFFSET + Channel] = Default_Channel_Configuration;
	}
	return;
}

void playTone(byte note, byte duration) {
	return playTone(note, duration, DEFAULT_CHANNEL_VOICE, DEFAULT_CLIP_VOLUME);
}

void playTone(byte note, byte duration, byte voice, byte volume) {
	byte Current_Channel = 0;

	cli();

	// Find first free track
	while (Current_Channel < NUMBER_OF_CHANNELS) {
		if (!(Channel_Enabled_Array[Current_Channel])) {
			break;
		}
		else {
			Current_Channel += 1;
		}
	}

	// Return if all channels are in use
	if (Current_Channel == NUMBER_OF_CHANNELS) {
		sei();
		return;
	}

	// Update channel configuration variables
	Voice_Array[Current_Channel] = voice;
	Volume_Array[Current_Channel] = (volume / 4);

	// Update channel voice
	uint8_t Current_Voice_Volume = scaleVolume(scaleVolume(volume, Global_Volume), MAX_VOICE_VOLUME);
	TWI_Queue_Data[TWI_QUEUE_VOICE_OFFSET + Current_Channel] = ((Voice_Array[Current_Channel] & VOICE_MASK) | ((Current_Voice_Volume << 2) & VOLUME_MASK));
	Voice_Update = true;

	// Update channel note
	TWI_Queue_Data[Current_Channel] = note;

	// Initialize dummy queue structure for channel
	Channel_Enabled_Array[Current_Channel] = true;
	Channel_Pointer_Array[Current_Channel] = NULL_TERMINATOR;
	Channel_Repeat_Array[Current_Channel] = false;
	Channel_Note_Remaining_Array[Current_Channel] = duration;

	sei();
	return;
}

void playAudio(const uint8_t audio_clip[]) {
	return queueAudioClip(audio_clip, false, DEFAULT_CLIP_VOLUME);
}

void playAudio(const uint8_t audio_clip[], byte volume) {
	return queueAudioClip(audio_clip, false, volume);
}

void loopAudio(const uint8_t audio_clip[]) {
	return queueAudioClip(audio_clip, true, DEFAULT_CLIP_VOLUME);
}

void loopAudio(const uint8_t audio_clip[], byte volume) {
	return queueAudioClip(audio_clip, true, volume);
}

void stopAudio() {
	cli();
	for (byte Channel = 0; Channel < NUMBER_OF_CHANNELS; Channel++) {
		TWI_Queue_Data[Channel] = REST;
		Channel_Enabled_Array[Channel] = false;
	}
	sei();
	return;
}

void setVolume(byte volume) {
	Global_Volume = volume;
	uint8_t Temp_Voice_Configuration_Array[NUMBER_OF_CHANNELS];

	for (byte Channel = 0; Channel < NUMBER_OF_CHANNELS; Channel++) {
		uint8_t Current_Channel_Volume = scaleVolume(scaleVolume(volume, Volume_Array[Channel]), MAX_VOICE_VOLUME);
		Temp_Voice_Configuration_Array[Channel] = (((Voice_Array[Channel] << 6) & VOICE_MASK) | (Current_Channel_Volume & VOLUME_MASK));
	}

	byte Temp_TWI_Queue_Pointer = TWI_QUEUE_VOICE_OFFSET;
	cli();
	for (byte Channel = 0; Channel < NUMBER_OF_CHANNELS; Channel++) {
		TWI_Queue_Data[Temp_TWI_Queue_Pointer++] = Temp_Voice_Configuration_Array[Channel];
	}
	sei();

	Voice_Update = true;
	return;
}

void queueAudioClip(const uint8_t audio_clip[], bool loop, byte volume) {
	byte Number_Of_Tracks = pgm_read_byte(audio_clip++);
	byte Current_Channel = 0;

	cli();

	for (byte Track = 0; Track < Number_Of_Tracks; Track++) {

		// Get first free channel
		while (Current_Channel < NUMBER_OF_CHANNELS) {
			if (!(Channel_Enabled_Array[Current_Channel])) {
				break;
			}
			else {
				Current_Channel += 1;
			}
		}

		// Return if all channels are in use
		if (Current_Channel == NUMBER_OF_CHANNELS) {
			break;
		}

		// Update channel configuration variables
		Voice_Array[Current_Channel] = pgm_read_byte(audio_clip++);
		Volume_Array[Current_Channel] = pgm_read_byte(audio_clip++);

		// Update channel voice
		uint8_t Current_Voice_Volume = scaleVolume(scaleVolume(scaleVolume(volume, Volume_Array[Current_Channel]), Global_Volume), MAX_VOICE_VOLUME);
		TWI_Queue_Data[TWI_QUEUE_VOICE_OFFSET + Current_Channel] = ((Voice_Array[Current_Channel] & VOICE_MASK) | ((Current_Voice_Volume << 2) & VOLUME_MASK));
		Voice_Update = true;

		// Initialize queue structure for channel
		Channel_Enabled_Array[Current_Channel] = true;
		Channel_Pointer_Array[Current_Channel] = audio_clip;
		Channel_Next_Pointer_Array[Current_Channel] = audio_clip;
		Channel_Repeat_Array[Current_Channel] = loop;
		Channel_Note_Remaining_Array[Current_Channel] = 0;

		// Scan to next track if needed
		if ((Track + 1) < Number_Of_Tracks) {
			while (true) {
				if (pgm_read_byte(audio_clip++) == REST) {
					if (pgm_read_byte(audio_clip++) == 0) {
						break;
					}
				}
				else {
					audio_clip += 1;
				}
			}
		}
	}

	sei();
	return;
}

uint8_t scaleVolume(uint8_t volume_a, uint8_t volume_b) {
	return ((((uint16_t) volume_a + 1) * volume_b) >> 8);
}
