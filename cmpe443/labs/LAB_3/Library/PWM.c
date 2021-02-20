#include "PWM.h"

void PWM_Init() {
	//Change the function of the pin in here:
	
	//Turn on PWM
	
	//Enable PWM output for corresponding pin.
	
	//Reset The PWM Timer Counter and The PWM Prescale Counter on the Next Positive Edge of PCLK
	
	//Configure MR0 register for giving pulse every 20 ms.
	
	//Reset TC, when MR0 matches TC.
	
	//Enable PWM Match 0 Latch.
	
	//Enable Counter and PWM and Clear Reset on the PWM
	
	PWM_Write(0);
}

void PWM_Cycle_Rate(uint32_t period_In_Cycles) {
	//Write a formula that changes the MR0 register value for a given parameter.
	
	//Enable PWM Match Register Latch for MR0.
}

void PWM_Write(uint32_t T_ON) {	
	if(T_ON > 100) {
		T_ON = 100;
	}
	
	//Write a formula to calculate the match register for P30 (P1_2) pin.
	
	//If T_ON equal to MR0, there will be 1 Cycle Dropout. In order to prevent this, we increase the T_ON value.
	if (T_ON == PWMX->MR0) {
		T_ON++;
	}
	PWMX->MR1 = T_ON;
	
	//Enable PWM Match Register Latch.
}
