#include "PLL.h"

void PLL_Init() {
	CCLKSEL = 0x01;
	PCLKSEL = 0x04;
	EMCCLKSEL = 0x00;
	USBCLKSEL = 0x00;
	SPIFICLKSEL = 0x00;
}

void PLL_Disable() {
	//Disable PLL
}

void PLL_Feed() {
	//Feed PLL
}

void PLL_Change_Source() {
	//Select the clock source as the OSC_CLK
}

void PLL_Change_Frequency() {
	//Change PLL Frequency to 60MHz
}

void PLL_Enable() {
	//Enable PLL
}

void PLL_Check_State() {
	//Check PLL is Enabled
}

void PLL_Start() {
	//Select the CPU clock as the same clock rate as Main PLL
	
	//Configure PCLK as PCLKDIV = 0x04
	
	EMCCLKSEL = 0x01;
	USBCLKSEL = 0x201;
	SPIFICLKSEL = 0x02;
}

void PLL_Change_Configuration() {
	PLL_Init();
	PLL_Disable();
	PLL_Feed();
	PLL_Change_Source();
	PLL_Change_Frequency();
	PLL_Enable();
	PLL_Feed();
	PLL_Check_State();
	PLL_Start();
}

