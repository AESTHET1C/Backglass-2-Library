#include "eeprom.h"

char LOAD_TEXT_ARRAY[MAX_TEXT_LENGTH + 1];

void save(int location, int value) {
	uint8_t Low_Byte = (value & 0xFF);
	uint8_t High_Byte = ((value >> 8) & 0xFF);

	eeprom_busy_wait();
	uint8_t Old_Byte = eeprom_read_byte((uint8_t*) location);
	if (Old_Byte != Low_Byte) {
		eeprom_write_byte((uint8_t*) location, Low_Byte);
	}

	eeprom_busy_wait();
	Old_Byte = eeprom_read_byte((uint8_t*) (location + 1));
	if (Old_Byte != High_Byte) {
		eeprom_write_byte((uint8_t*) (location + 1), High_Byte);
	}
	return;
}

void saveText(int location, char text[]) {
	byte Pointer = 0;
	uint8_t Old_Byte = 0;
	while (Pointer < MAX_TEXT_LENGTH) {
		eeprom_busy_wait();
		Old_Byte = eeprom_read_byte((uint8_t*) (location + Pointer));
		if (Old_Byte != text[Pointer]) {
			eeprom_write_byte((uint8_t*) (location + Pointer), text[Pointer]);
		}
		if (text[Pointer] == (char) NULL) {
			break;
		}
	}
	return;
}

int load(int location) {
	eeprom_busy_wait();
	return eeprom_read_word((uint16_t*) location);
}

char * loadText(int location) {
	byte Pointer = 0;
	eeprom_busy_wait();
	while (Pointer < MAX_TEXT_LENGTH) {
		LOAD_TEXT_ARRAY[Pointer] = eeprom_read_byte((uint8_t*) (location + Pointer));
		if (LOAD_TEXT_ARRAY[Pointer] == (char) NULL) {
			return LOAD_TEXT_ARRAY;
		}
		Pointer += 1;
	}
	LOAD_TEXT_ARRAY[MAX_TEXT_LENGTH] = (char) NULL;
	return LOAD_TEXT_ARRAY;
}
