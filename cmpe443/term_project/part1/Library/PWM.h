#ifndef PWM_H
#define PWM_H

#include "LPC407x_8x_177x_8x.h"

#include "SystemStructures.h"

//Correctly define PWM_TypeDef struct.
typedef struct
{
	volatile	uint32_t IR;
	volatile	uint32_t TCR;
	volatile	uint32_t TC;
	volatile	uint32_t PR;
	volatile	uint32_t PC;
	volatile	uint32_t MCR;
  volatile	uint32_t MR0;
  volatile	uint32_t MR1;
	volatile	uint32_t MR2;
	volatile	uint32_t MR3;
	volatile	uint32_t CCR;
	volatile	uint32_t CR0;
	volatile	uint32_t CR1;
						uint32_t RESERVED0[3];
	volatile	uint32_t MR4;
	volatile	uint32_t MR5;
	volatile	uint32_t MR6;
	volatile	uint32_t PCR;
	volatile	uint32_t LER;
						uint32_t RESERVED1[3];
						uint32_t RESERVED2[4];
	volatile	uint32_t CTCR;
} PWM_TypeDef;

//0x00000000 is a dummy value, write the correct address
#define IOCON_LED_ADDRESS_1	0x4002C088
#define IOCON_LED_ADDRESS_2	0x4002C08C
#define IOCON_LED_ADDRESS_3	0x4002C094
#define IOCON_LED_ADDRESS_4	0x4002C098

#define IOCON_EN_ADDRESS_A	0x4002C09C
#define IOCON_EN_ADDRESS_B	0x4002C0AC

#define FRONT_RIGHT_LED	*((volatile uint32_t*)(IOCON_LED_ADDRESS_1))
#define FRONT_LEFT_LED	*((volatile uint32_t*)(IOCON_LED_ADDRESS_2))
#define BACK_LEFT_LED	*((volatile uint32_t*)(IOCON_LED_ADDRESS_3))
#define BACK_RIGHT_LED	*((volatile uint32_t*)(IOCON_LED_ADDRESS_4))
	
#define ENA	*((volatile uint32_t*)(IOCON_EN_ADDRESS_A))
#define ENB	*((volatile uint32_t*)(IOCON_EN_ADDRESS_B))


#define PWM0_BASE	0x40014000
#define PWM1_BASE	0x40018000

#define PWM0	((PWM_TypeDef*) PWM0_BASE)
#define PWM1	((PWM_TypeDef*) PWM1_BASE)

//0x00000000 is a dummy value, write the PWM0_BASE or PWM1_BASE in that location
#define PWMX	((PWM_TypeDef*) PWM0_BASE)

void PWM_Init(void);
void PWM_Cycle_Rate(uint32_t period_In_Cycles);
void PWM_Write(uint32_t T_ON, uint32_t channel);

#endif
