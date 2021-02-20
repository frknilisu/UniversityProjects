#ifndef SYSTEM_STRUCTURES_H
#define SYSTEM_STRUCTURES_H

//0x00000000 and 0x000 are the dummby values. Write the correct values.
#define SCR_ADDRESS	 0x00000000
#define SCR	*((volatile uint32_t*)(SCR_ADDRESS))

//0x00000000 and 0x000 are the dummby values. Write the correct values.
#define PCON_ADDRESS	0x00000000
#define PCON	*((volatile uint32_t*)(PCON_ADDRESS))

#define PCONP_ADDRESS	0x400FC0C4
#define PCONP	*((volatile uint32_t*)(PCONP_ADDRESS))

#define PERIPHERAL_CLOCK_FREQUENCY 60000000

#endif
