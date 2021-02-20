#include "PWM.h"

void PWM_Init() {
	//Change the function of the pin in here:
	FRONT_RIGHT_LED |= 0x3;
	FRONT_RIGHT_LED &= ~(1 << 2);
	
	FRONT_LEFT_LED |= 0x3;
	FRONT_LEFT_LED &= ~(1 << 2);
	
	BACK_LEFT_LED |= 0x3;
	BACK_LEFT_LED &= ~(1 << 2);
	
	BACK_RIGHT_LED |= 0x3;
	BACK_RIGHT_LED &= ~(1 << 2);
	
	ENA |= 0x3;
	ENA &= ~(1 << 2);
	
	ENB |= 0x3;
	ENB &= ~(1 << 2);
	
	//Turn on PWM
	PCONP |= (1 << 5);
	
	//Enable PWM output for corresponding pin.
	PWM0 -> PCR |= (1 << 9 | 1 << 10 | 1 << 11 | 1 << 12 | 1 << 13 | 1 << 14);
	
	//Reset The PWM Timer Counter and The PWM Prescale Counter on the Next Positive Edge of PCLK
	PWM0 -> TCR |= (1 << 1);
	
	//Configure MR0 register for giving pulse every 20 ms.
	PWM0 -> MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * 20 * 1000;
	
	//Reset TC, when MR0 matches TC.
	PWM0 -> MCR |= (1 << 1);
	
	//Enable PWM Match 0 Latch.
	PWM0 -> LER |= (1 << 0);
	
	//Enable Counter and PWM and Clear Reset on the PWM
	PWM0 -> TCR |= (1 << 0 | 1 << 3);
	PWM0 -> TCR &= ~(1 << 1);
		
	
	PWM_Write(0, 1);
	PWM_Write(0, 2);
	PWM_Write(0, 3);
	PWM_Write(0, 4);
	PWM_Write(0, 5);
	PWM_Write(0, 6);
}

void PWM_Cycle_Rate(uint32_t period_In_Cycles) {
	//Write a formula that changes the MR0 register value for a given parameter.
	PWM0 -> MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * period_In_Cycles * 1000;
	
	//Enable PWM Match Register Latch for MR0.
	PWM0 -> LER |= (1 << 0);
}

void PWM_Write(uint32_t T_ON, uint32_t channel) {	
	if(T_ON > 100) {
		T_ON = 100;
	}
	
	//Write a formula to calculate the match register for P30 (P1_2) pin.
	T_ON = (PWM0 -> MR0 / 100) * T_ON;
	
	//If T_ON equal to MR0, there will be 1 Cycle Dropout. In order to prevent this, we increase the T_ON value.
	if (T_ON == PWM0->MR0) {
		T_ON++;
	}
	
	switch(channel){
		case 1:
			PWM0 -> MR1 = T_ON;
			break;
		case 2:
			PWM0 -> MR2 = T_ON;
			break;
		case 3:
			PWM0 -> MR3 = T_ON;
			break;
		case 4:
			PWM0 -> MR4 = T_ON;
			break;
		case 5:
			PWM0 -> MR5 = T_ON;
			break;
		case 6:
			PWM0 -> MR6 = T_ON;
			break;
	}
	
	
	//Enable PWM Match Register Latch.
	PWM0 -> LER |= (1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6);
}
