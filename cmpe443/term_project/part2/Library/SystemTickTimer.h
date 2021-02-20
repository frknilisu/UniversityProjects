#ifndef SYSTICK_H
#define SYSTICK_H

#include "LPC407x_8x_177x_8x.h"

#include "SystemStructures.h"

typedef struct
{
	volatile uint32_t STCTRL;
	volatile uint32_t STRELOAD;
	volatile uint32_t STCURR;
	volatile uint32_t STCALIB;
} SYSTICK_TypeDef;

//0x00000000 and 0x000 are the dummby values. Write the correct address for System Tick Timer.
#define SYSTICK_ADDRESS	0xE000E010
#define SYSTICK	((SYSTICK_TypeDef*) SYSTICK_ADDRESS)

void SysTick_Init(void);

#endif
