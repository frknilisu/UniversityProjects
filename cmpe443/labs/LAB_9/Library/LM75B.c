#include "LM75B.h"

void LM75B_Init() {
	I2C_Init();
}

int LM75B_Open() {
	if(!I2C_Write(LM75B_Address,0,0,1)) {
		return 1;
	}
	else {
		return 0;
	}
}

int LM75B_Read() {
	int value;
	char buffer[2];
	
	I2C_Write(LM75B_Address,0,1,0);
	
	I2C_Read(LM75B_Address,buffer,2,1);
	
	//Combine the buffers into value variable.
	
	//Return correct temperature value for positive and negative temperature values.
	
	return value;
}
