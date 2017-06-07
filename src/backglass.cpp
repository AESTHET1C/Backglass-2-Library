#include "backglass.h"

void initBackglass() {
	PRR |= ((1 << PRTIM2) | (1 << PRSPI));

	initTimer1();
	initIO();
	return;
}
