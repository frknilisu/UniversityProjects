#include "PWM.h"

void PWM_Init() {
	IOCON_LED |= 0x3;
	IOCON_LED &= ~(1 << 2);
	
	PCONP |= 1 << 5;
	
	PWM0->PCR |= 1 << 11;
	
	PWM0->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * 20 * 1000;
	
	PWM0->MR3 = 0;
	
	PWM0->LER |= 1 << 0 | 1 << 1;
	
	PWM0->TCR |= (1 << 0 | 1 << 3);
	
	PWM_Write(0);
}

void PWM_Cycle_Rate(uint32_t value) {
	PWM0->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * value * 1000;
	
	PWM0->LER |= 1 << 0;
}

void PWM_Write(uint32_t value) {
	uint32_t trueValue;
	
	if((int)value > 100) {
		value = 100;
	}
	else if((int)value < 0) {
		value = 0;
	}
	
	trueValue = (uint32_t)(((PWM0->MR0) * value) / 100);
	
	if (trueValue == PWM0->MR0) {
		trueValue++;
	}	
	PWM0->MR3 = trueValue;
	
	PWM0->LER |= 1 << 3;
}
