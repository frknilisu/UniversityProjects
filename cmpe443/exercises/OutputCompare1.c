
typedef struct
{
	volatile uint32_t IR;
	volatile uint32_t TCR;
	volatile uint32_t TC;
	volatile uint32_t PR;
	volatile uint32_t PC;
	volatile uint32_t MCR;
	volatile uint32_t MR0;
	volatile uint32_t MR1;
	volatile uint32_t MR2;
	volatile uint32_t MR3;
	volatile uint32_t CCR;
	volatile uint32_t CR0;
	volatile uint32_t CR1;
	uint32_t reserved_1[2];
	volatile uint32_t EMR;
	uint32_t reserved_2[12];
	volatile uint32_t CTCR;
}TIMER_Type

#define TIMER2 ((TIMER_TYPE *)(_____))
#define TOFF _____
#define TON _____
#define CLOCKDIV _____

static uint32_t count;

void TIMER2_IRQInit(void)
{
//Turn on TIMER2 module: PCTIM2, bit22
	PCONP |= 1<<22;
//Out of reset, PCLK=APB/4, but system initialisation code might change it. If necessary, change PCLK by modifying PCLKSEL register
//TIMER2 module is driven by PCLK
	TIMER2 ->CTCR &= ~3;
//Set mode of P0_7 as T2_MAT1 (IOCON_P0_7[2:0]=3)
	value = IOCON_P0_7;
	value |= 3;
	value &=~4;
	IOCON_P0_7 = value; 
//Determine the PCLK
//Enable Timer Counter
	TIMER2->TCR |= 1;
//Determine Prescalar values PR and PC if necessary
	TIMER2->PR = CLOCKDIV;
//Set T2_MAT1 for the MATCH Interrupt and continue 
//MR1I=1, MR1R=0,MR1S=0 (bit3,4,5)
	value =TIMER2->MCR
	value |= 8;
	value &=~(0x30)
	TIMER2->MCR=value;	
//Set P0_7=T2_MAT1 to zero initially, i.e. EM1=0 (bit1)
	TIMER2->EMR &= ~2;
//Set the MATCH value on MR1
	TIMER2->MR1=TIMER2->TC+TOFF;
//Configure P0_7=T2_MAT1 so that it will toggle when there is a match on MR1, i.e. EMC1=3 (bits 7,6)
	TIMER2->EMR |= 0xC0;
//Clear MAT1 interrupt
	TIMER2->IR |= 2;
//Enable TIMER2_IRQ;
	NVIC_EnableIRQ(TIMER2_IRQ);
	NVIC_SetPriority(TIMER2_IRQ, 2);
 	NVIC_ClearPendingIRQ(TIMER2_IRQ);
}

void DisableTIMER2IRQ(void)
{ 
	 	NVIC_ClearPendingIRQ(TIMER2_IRQ);
		NVIC_DisableIRQ(TIMER2_IRQ);
}

void TIMER2_IRQHandler(void)
{

	if(count==0)
	{
		TIMER2->MR1 += TON;
		count++;
//Clear MAT1 interrupt
		TIMER2->IR |= 2;
	 	NVIC_ClearPendingIRQ(TIMER2_IRQ);
	}
	else
	{
		DisableTIMER2IRQ();
		count=0;
	}
}	

void main (void)
{
	TIMER2_IRQInit();
	__enable_irq();
	while(1)
	{
		__WFI();
	}
}