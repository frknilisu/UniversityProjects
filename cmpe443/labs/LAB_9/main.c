#include "LPC407x_8x_177x_8x.h"

#include "Library/Serial.h"
#include "Library/LM75B.h"

#include <stdio.h>

void init() {	
	Serial_Init();
	
	LM75B_Init();
	
	LM75B_Open();
}

//Send the temperature data to PC (Temperature sensor value is 00)
void update() {
}

int main() {
	init();
	
	while(1) {
		update();
	}
}

