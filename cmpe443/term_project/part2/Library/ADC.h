#ifndef ADC_H
#define ADC_H

#include "LPC407x_8x_177x_8x.h"

#include "SystemStructures.h"

typedef struct {
  volatile	uint32_t CR;
  volatile	uint32_t GDR;
						uint32_t RESERVED0;
  volatile	uint32_t INTEN;
  volatile	uint32_t DR[8];
  volatile	uint32_t STAT;
  volatile	uint32_t TRM;
} ADC_TypeDef;

#define ADC_CLOCK_FREQUENCY 1000000

//Write the IOCON address of Analog Pin
#define	ANALOG_PIN_IOCON_ADDRESS_1	0x4002C068	//P0.26(P18)
#define	ANALOG_PIN_IOCON_ADDRESS_2	0x4002C064	//P0.25(P17)
#define	ANALOG_PIN_IOCON_ADDRESS_3	0x4002C05C	//P0.23(P15)
#define LED_SENSOR_LEFT	*((volatile uint32_t*)(ANALOG_PIN_IOCON_ADDRESS_1))
#define LED_SENSOR_RIGHT	*((volatile uint32_t*)(ANALOG_PIN_IOCON_ADDRESS_2))
#define TRIMPOT		*((volatile uint32_t*)(ANALOG_PIN_IOCON_ADDRESS_3))

//Write the CLKDIV of ADC for given ADC_CLOCK_FREQUENCY.
#define ADC_CLKDIV 14

//Write the max value of ADC.
#define ADC_MAX_VALUE 0xFFF

#define ADC_BASE	0x40034000
#define ADC	((ADC_TypeDef*) ADC_BASE)

extern uint32_t ADC_Left;
extern uint32_t ADC_Right;
extern uint32_t ADC_Trimpot;

void ADC_Init(void);
void ADC_Start(void);
void ADC_Stop(void);
//uint32_t ADC_GetLastRightValue(void);
//uint32_t ADC_GetLastLeftValue(void);
//uint32_t GetTrimpotValue(void);

#endif
