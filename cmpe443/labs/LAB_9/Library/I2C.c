#include "I2C.h"

void I2C_Init() {
	//Turn on I2C
	PCONP |= 1 << 7;
	
	I2C->SCLL = I2CDutyCycle;
	I2C->SCLH = I2CDutyCycle;
	
	//Clear Assert acknowledge,  I2C interrupt, START flag and I2C interface
	
	//In Initialization routine, Write Correct value to CONSET register for Master only functions
	
	//Change the functionality of the I2C_SDA_PIN.
	
	//Change the functionality of the I2C_SCL_PIN.
}

int I2C_Start() {
	int status = 0;
	
	I2C->CONCLR = (1 << 5)
							| (1 << 4)
							| (1 << 3)
							| (1 << 2);
	
	//In Start Master Transmit function, Write Correct Value to CONSET register
	
	I2C_Wait_SI();
	
	status = I2C->STAT;
	
	I2C->CONCLR = (1 << 5)
							| (0 << 4)
							| (0 << 3)
							| (0 << 2);
	
	return status;
}

int I2C_Stop() {
	int timeout = 0;

	//Write the correct value to CONSET for transmitting a STOP condition in master mode
	
	
	I2C->CONCLR = (0 << 5)
							| (0 << 4)
							| (1 << 3)
							| (0 << 2);
	

	while(I2C->CONSET & (1 << 4)) {
			timeout ++;
			if (timeout > 100000) {
				return 1;
			}
	}

	return 0;
}

int I2C_Wait_SI() {
	int timeout = 0;
	
	//Fill the condition in the while statement for waiting until I2C state changes
	while (1) {
			timeout++;
			if (timeout > 100000) {
				return -1;
			}
	}
	return 0;
}

int I2C_Write(uint32_t address, char* data, int length, int stop) {
	int written = I2C_WriteData(address, data, length, stop);
	return length != written;
}

int I2C_WriteData(uint32_t address,const char* data,int length, int stop) {
	int index;
	int status = I2C_Start();
	
	//Stop and Return -1 when A START or a repeated START condition has not been transmitted.

	
	//Write the correct address for writing data to I2C device.
	//The address of the device is different while reading and writing.
	status = I2C_Do_Write(address);
	
	//Stop and Return -1 When First Data Byte is not Transmitted

	
	for (index = 0; index < length; index++) {
		status = I2C_Do_Write(data[index]);
		//Stop and Return the index variable when Data is not Transmitted
		
	}
	
	if (stop) {
		I2C_Stop();
	}
    
	return length;
}

int I2C_Do_Write(int value) {
	//Write the value the a register for transmitting the data.
    
	I2C->CONCLR = (0 << 5)
							| (0 << 4)
							| (1 << 3)
							| (0 << 2);
	
	I2C_Wait_SI();
	return I2C->STAT;
}

int I2C_Read(uint32_t address, char* data, int length, int stop) {
	int readed = I2C_ReadData(address, data, length, stop);
	return length != readed;
}

int I2C_ReadData(uint32_t address,char* data,int length, int stop) {
	int index;
	int value;
	int status = I2C_Start();
	
	if ((status != 0x10) && (status != 0x08)) {
		I2C_Stop();
		return -1;
	}
	
	//Write the correct address for reading data from I2C device.
	//The address of the device is different while reading and writing.
	status = I2C_Do_Write(address);
	
	//Stop and Return -1 when ACK is not received
	
	for (index = 0; index < (length - 1); index++) {
		value = I2C_Do_Read(0);
		status = I2C->STAT;
		
		//Stop and Return index when Data is not received
		
		
		
		data[index] = (char) value;
	}
	
	value = I2C_Do_Read(1);
	status = I2C->STAT;
	
	//Stop and Return length - 1 When the Last Data is not received

	
	
	data[index] = (char) value;
	
	if (stop) {
		I2C_Stop();
	}

	return length;
}

int I2C_Do_Read(int isLast) {
	if(isLast) {
		I2C->CONCLR = (0 << 5)
								| (0 << 4)
								| (0 << 3)
								| (1 << 2);
		
	}
	else {
		I2C->CONSET = (0 << 5)
								| (0 << 4)
								| (0 << 3)
								| (1 << 2);
	}
	
	I2C->CONCLR = (0 << 5)
							| (0 << 4)
							| (1 << 3)
							| (0 << 2);
    
	I2C_Wait_SI();
    
	return (I2C->DAT & 0xFF);
}
