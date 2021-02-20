#include "SystemTickTimer.h"

void SysTick_Init() {
	//Load a value to System Tick Timer for 10 ms
	SYSTICK->STRELOAD = (CPU_CLOCK_FREQUENCY/100) - 1;
	//SYSTICK->STRELOAD = 1199999;
	//Change the current value of System Tick Timer to zero 
	SYSTICK->STCURR = 0;
	//Enable System Tick Timer and System Tick Timer Interrupt and Select CPU as its clock.
	SYSTICK->STCTRL = 7;
	// Enable SysTick_IRQn(Interrupt Request)
	NVIC_EnableIRQ(SysTick_IRQn);
}

