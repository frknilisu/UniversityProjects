#include "Ultrasonic.h"

void Ultrasonic_Trigger_Timer_Init() {
	//Give the Correct Function Values to IOCON_TRIGGER
	
	//Enable Timer2.
	
	//This code changes the mode of Timer2 to Timer Mode.
	TIMER2->CTCR = 0x00;
	
	//This code disables Timer Counter and Prescale Counter for Timer2.
	TIMER2->TCR &= ~(1 << 0);
	
	//This code resets Timer Counter and Prescale Counter for Timer2.
	TIMER2->TCR |= (1 << 1);
	
	//Change PR Register value for 1 microsecond incrementing
	
	//Write the Correct Configuration for EMR (Toggle Output Value and Initial value is HIGH)
	
	//Enable TIMER2_IRQn (Interrupt Request).
	
	//Set Priority Timer2 IRQ as 5.
	
	//Clear pendings for Timer2.
}

void Ultrasonic_Capture_Timer_Init() {
	//Give the Correct Function Values to IOCON_ECHO
	
	//Enable Timer3
	
	//This code changes the mode of Timer3 to Timer Mode.
	TIMER3->CTCR = 0x00;
	
	//This code disables Timer Counter and Prescale Counter for Timer3.
	TIMER3->TCR &= ~(1 << 0);
	
	//This code resets Timer Counter and Prescale Counter for Timer3.
	TIMER3->TCR |= (1 << 1);
	
	//Change PR Register value for 1 microsecond incrementing
	
	//Write the Correct Value for Getting Interrupt when Rising Edge Occur
	
	//Remove the reset on counters of Timer3.
	
	//Enable Timer3 Counter and Prescale Counter for counting.
}

void Ulrasonic_Start_Trigger() {
	//Give correct value to MR3 Register for 10 microsecond
	
	//Enable interrupt for MR3 register, if MR3 register matches the TC.
	
	//Remove the reset on counters of Timer2.
	
	//Enable Timer2 Counter and Prescale Counter for counting.
}

uint8_t isUltrasonicSensorTriggerEnded = 0;
uint8_t ultrasonicSensorEdgeCount = 0;

void TIMER2_IRQHandler() {
	if(isUltrasonicSensorTriggerEnded == 0) {
		//Change MR3 Register Value for Suggested Waiting
		
		
		isUltrasonicSensorTriggerEnded = 1;
		
		ultrasonicSensorEdgeCount = 0;
		
		//Clear pendings for Timer3.
		
		//Enable TIMER3_IRQn (Interrupt Request).
	}
	else {
		TIMER2->MR3 = 10;
		isUltrasonicSensorTriggerEnded = 0;
	}
	
	//Clear IR Register Flag for Corresponding Interrupt
	
	
	TIMER2->TC = 0;
}

uint32_t ultrasonicSensorRisingTime = 0;
uint32_t ultrasonicSensorFallingTime = 0;
uint32_t ultrasonicSensorDuration = 0;
uint32_t ultrasonicSensorDistance = 0;

void TIMER3_IRQHandler() {
	if(ultrasonicSensorEdgeCount == 0) {
		
		//Store the rising time into ultrasonicSensorRisingTime variable
		
		
		TIMER3->CCR |= (1 << 1) | (1 << 2);
		ultrasonicSensorEdgeCount = 1;
	}
	else if(ultrasonicSensorEdgeCount == 1){
		
		//Store the falling time into ultrasonicSensorFallingTime variable
		
		
		TIMER3->CCR |= (1 << 0) | (1 << 2);
		
		ultrasonicSensorEdgeCount = 2;
		
		//Clear pendings for Timer3.
		
		//Disable TIMER3_IRQn (Interrupt Request).
	}
	
	TIMER3->IR = 1 << 4;
}

