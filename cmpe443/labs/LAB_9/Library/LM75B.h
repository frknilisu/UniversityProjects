#ifndef LM75B_H
#define LM75B_H

#include "LPC407x_8x_177x_8x.h"

#include "I2C.h"

//Write the I2C address of LM75B (7 bits)
#define LM75B_Address_7_Bits 0x00
#define LM75B_Address (LM75B_Address_7_Bits << 1)

void LM75B_Init(void);
int LM75B_Open(void);
int LM75B_Read(void);

#endif
