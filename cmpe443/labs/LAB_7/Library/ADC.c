#include "ADC.h"

uint32_t ADC_Last = 0;
uint8_t ADC_New_Data_Available = 0;

void ADC_Init() {
	//Change the function value of pin to ADC.
	
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	
	//Change Analog/Digital mode of pin to Analog.
	
	//Turn on ADC.
	
	//Select corresponding AD pins to be sampled and converted. Also, set the CLKDIV and make the A/D converter operational without Burst mode.
	
	//Enable interrupt for corresponding pin.
	
	//Enable ADC_IRQn (Interrupt Request).
}

void ADC_Start () {
	//Write a code for starting A/D conversion.
}

void ADC_Stop() {
	//Write a code for stopping A/D conversion.
}

uint32_t ADC_GetLastValue() {
	ADC_New_Data_Available = 0;
	return ADC_Last;
}

void ADC_IRQHandler() {
	ADC_Stop();
	
	//Write the converted data (only the converted data) to ADC_Last variable.
	
	ADC_New_Data_Available = 1;
}
