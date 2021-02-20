#include "LPC407x_8x_177x_8x.h"

#include "Library/SystemTickTimer.h"
#include "Library/ADC.h"
#include "Library/PWM.h"

void init() {
	SysTick_Init();
	
	PWM_Init();
	PWM_Write(0);
	
	ADC_Init();
	ADC_Start();
}

void SysTick_Handler() {
	ADC_Start();
}

// Under the LAB Lighting condition, Green LED should be turned off.
// When the light intensity is decreased, Green LED brightness will be increased.
// Change the brightness of the LED by using average value of last 10 ADC value.
void update() {

}

int main() {
	init();
	
	while(1) {
		update();
	}
}

