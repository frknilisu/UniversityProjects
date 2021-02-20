#include "ADC.h"

uint32_t ADC_Left = 0;
uint32_t ADC_Right = 0;
uint32_t ADC_Trimpot = 0;

void ADC_Init() {
	//Change the function value of pin to ADC.
	LED_SENSOR_LEFT |= 0x1;
	LED_SENSOR_LEFT &= ~(0x3 << 1);
	
	LED_SENSOR_RIGHT |= 0x1;
	LED_SENSOR_RIGHT &= ~(0x3 << 1);
	
	TRIMPOT |= 0x1;
	TRIMPOT &= ~(0x3 << 1);
	
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	LED_SENSOR_LEFT &= ~(0x3 << 3);
	LED_SENSOR_RIGHT &= ~(0x3 << 3);
	TRIMPOT &= ~(0x3 << 3);
	
	//Change Analog/Digital mode of pin to Analog.
	LED_SENSOR_LEFT &= ~(1 << 7);
	LED_SENSOR_RIGHT &= ~(1 << 7);
	TRIMPOT &= ~(1 << 7);
	
	//Turn on ADC.
	PCONP |= (1 << 12);
	
	//Select corresponding AD pins to be sampled and converted. Also, set the CLKDIV and make the A/D converter operational without Burst mode.
	ADC->CR |= (1 << 0 | 1 << 2 | 1 << 3);			// adc channel 0, 2 and 3 for 2 light sensors and trimpot
	ADC->CR |= (ADC_CLKDIV) << 8;				// clkdiv 14
	ADC->CR |= (1 << 21);				// operational mode
	ADC->CR &= ~(0x7 << 24);		// START bit = 000
	ADC->CR |= (1 << 16);				// Burst mode on
	
	//Enable interrupt for corresponding pin.
	ADC->INTEN |= (1 << 0 | 1 << 2 | 1 << 3);
	
	//Enable ADC_IRQn (Interrupt Request).
	NVIC_EnableIRQ(ADC_IRQn);
}


void ADC_Start () {
	//Write a code for starting A/D conversion.
	NVIC_ClearPendingIRQ(ADC_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_Stop() {
	//Write a code for stopping A/D conversion.
	NVIC_DisableIRQ(ADC_IRQn);
}

void ADC_IRQHandler() {
	ADC_Stop();
	
	if((ADC->DR[2] >> 31) & 0x1){			// Right Light Sensor conversion is DONE
		ADC_Right = (ADC->DR[2] >> 4) & ADC_MAX_VALUE;
	}
	
	if((ADC->DR[3] >> 31) & 0x1){			// Left Light Sensor conversion is DONE
		ADC_Left = (ADC->DR[3] >> 4) & ADC_MAX_VALUE;
	}
	
	if((ADC->DR[0] >> 31) & 0x1){			// Trimpot conversion is DONE
		ADC_Trimpot = (ADC->DR[0] >> 4) & ADC_MAX_VALUE;
	}
	
}
