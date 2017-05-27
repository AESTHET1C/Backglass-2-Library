#include "backglass.h"
#include "modules/io.h"

void initBackglass() {
	PRR |= ((1 << PRTIM2) | (1 << PRSPI));

	initIO();
	return;
}
