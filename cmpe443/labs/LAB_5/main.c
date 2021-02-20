#include "LPC407x_8x_177x_8x.h"

#include "Library/GPIO.h"
#include "Library/PLL.h"
#include "Library/LED.h"
#include "Library/Ultrasonic.h"

void PLL_Test_Init() {
	GPIO_DIR_Write(PORT1,1 << 11, OUTPUT);
	//PLL_Change_Configuration();
}

void Ulrasonic_Test_Init() {
	Ultrasonic_Trigger_Timer_Init();
	Ultrasonic_Capture_Timer_Init();
	
	LED1_Init();
	LED2_Init();
	LED3_Init();
	LED4_Init();
	
	LED1_Off();
	LED2_Off();
	LED3_Off();
	LED4_Off();
	
	Ulrasonic_Start_Trigger();
}

const uint32_t PLL_TEST_MILISECOND = 500;

void PLL_Test() {
	int i;
	GPIO_PIN_Write(PORT1,1 << 11, HIGH);
	for(i=0;i<PLL_TEST_MILISECOND*24000;i++);
	GPIO_PIN_Write(PORT1,1 << 11, LOW);
	for(i=0;i<PLL_TEST_MILISECOND*24000;i++);
}

// When Ultrasonic Sensor detects obstacles which are closer than 7cm, turn on all the LEDs.
// When Ultrasonic Sensor detects obstacles which are in the range of 7cm and 12cm , turn on  LED1, LED2 and LED3.
// When Ultrasonic Sensor detects obstacles which are in the range of 12cm and 20cm , turn on  LED1 and LED2.
// When Ultrasonic Sensor detects obstacles which are in the range of 20cm and 30cm , turn on  LED1.
// When Ultrasonic Sensor detects obstacles which are far from 30 cm, turn off all the LEDs.
void Ultrasonic_Test() {
	
	
	__WFI();
}

void init() {
	PLL_Test_Init();
	//Ulrasonic_Test_Init();
}

void update() {
	PLL_Test();
	//Ultrasonic_Test();
}

int main() {
	init();
	
	while(1) {
		update();
	}
}

