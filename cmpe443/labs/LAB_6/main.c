#include "LPC407x_8x_177x_8x.h"

#include "Library/SystemStructures.h"
#include "Library/GPIO.h"
#include "Library/PLL.h"
#include "Library/External.h"

void PLL_Test_Init() {
	GPIO_DIR_Write(PORT1,1 << 11, OUTPUT);
	External_Init();
	//PLL_Change_Configuration();
}

const uint32_t PLL_TEST_MILISECOND = 500;

void PLL_Test() {
	int i;
	GPIO_PIN_Write(PORT1,1 << 11, HIGH);
	for(i=0;i<PLL_TEST_MILISECOND*24000;i++);
	GPIO_PIN_Write(PORT1,1 << 11, LOW);
	for(i=0;i<PLL_TEST_MILISECOND*24000;i++);
}

void EINT0_IRQHandler() {
	
	EXT->EXTINT |= 0x01;
}

void init() {
	PLL_Test_Init();
}

void update() {
	PLL_Test();
}

int main() {
	init();
	
	while(1) {
		update();
	}
}

