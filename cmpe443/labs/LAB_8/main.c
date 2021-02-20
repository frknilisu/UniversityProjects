#include "LPC407x_8x_177x_8x.h"

#include "Library/LED.h"
#include "Library/Serial.h"
#include "Library/ESP8266.h"

void init() {	
	LED1_Init();
	LED2_Init();
	LED3_Init();
	LED4_Init();
	
	LED1_Off();
	LED2_Off();
	LED3_Off();
	LED4_Off();
	
	Serial_Init();
	
	//ESP8266_Init();	
}

void update() {
	
}

int main() {
	init();
	
	while(1) {
		update();
	}
}

