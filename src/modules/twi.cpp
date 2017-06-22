#include "twi.h"

// Queue structure variables
volatile uint8_t TWI_Queue_Data[TWI_QUEUE_LENGTH];
volatile byte TWI_Queue_Length = 0;
byte TWI_Queue_Pointer = 0;

void initTWI() {
	TWBR = (((F_CPU / TWI_BIT_RATE) - 16) / 2);
	TWCR |= (1 << TWIE);
	PRR |= (1 << PRTWI);
	return;
}

void beginI2CTransmission() {
	PRR &= ~(1 << PRTWI);
	TWCR |= ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN));
	TWI_Queue_Pointer = 0;
	return;
}

ISR(TWI_vect) {
	if (TWI_Queue_Pointer < TWI_Queue_Length) {
		switch TWSR {
			case TWI_START_TRANSMITTED:
			case TWI_REPEAT_START_TRANSMITTED:
				// Send slave address byte
				TWDR = TWI_SLA_W_BYTE;
				break;
			case TWI_ADDR_TRANSMITTED:
			case TWI_DATA_TRANSMITTED:
				// Send data byte
				TWCR &= ~((1 << TWINT) | (1 << TWSTA));

				TWDR = TWI_Queue_Data[TWI_Queue_Pointer++];
				break;
			default:  // Some form of error
				if (TWI_Queue_Pointer > 0) {                       // Repeat last data byte
					TWDR = TWI_Queue_Data[TWI_Queue_Pointer++ - 1];
				}
				else {                                             // Restart transmission
					TWI_Queue_Pointer = 0;
					TWCR |= (1 << TWSTA);
				}
				break;
		}
	}
	else if (TWI_Queue_Pointer == TWI_Queue_Length) {  // Stop transmission
		TWCR |= (1 << TWSTO);
		TWI_Queue_Pointer += 1;
	}
	else {                                             // Power down TWI
		PRR |= (1 << PRTWI);
		TWCR &= ~(1 << TWEN);
	}

	// Clear interrupt flag to begin sending byte
	TWCR |= (1 << TWINT);
	return;
}
