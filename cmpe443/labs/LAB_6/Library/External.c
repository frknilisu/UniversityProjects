#include "External.h"

void External_Init() {
	IOCON_PUSH_BUTTON |= 0x01;
	
	EXT->EXTMODE |= 0x01;
	
	NVIC_EnableIRQ(EINT0_IRQn);
}

