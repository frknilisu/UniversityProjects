#include "LPC407x_8x_177x_8x.h"

#define GPIO_ADDRESS	0x20098000

//RGB_GPIO
#define RGB_GPIO_DIR_ADDRESS	(GPIO_ADDRESS + 0x020)
#define RGB_GPIO_DIR *((volatile uint32_t*)(RGB_GPIO_DIR_ADDRESS))
	
#define RGB_GPIO_PIN_ADDRESS	(GPIO_ADDRESS + 0x034)
#define RGB_GPIO_PIN *((volatile uint32_t*)(RGB_GPIO_PIN_ADDRESS))

// MASK (For example for P1.20, mask is 1 << 20) (Change ((uint32_t) 1 << 0))
#define RGB_RED_PIN_MASK					((uint32_t) 1 << 0)
#define RGB_GREEN_PIN_MASK				((uint32_t) 1 << 0)
#define RGB_BLUE_PIN_MASK					((uint32_t) 1 << 0)

//JOYSTICK_GPIO
#define JOYSTICK_GPIO_DIR_ADDRESS	(GPIO_ADDRESS + 0x0A0)
#define JOYSTICK_GPIO_DIR *((volatile uint32_t*)(JOYSTICK_GPIO_DIR_ADDRESS))
	
#define JOYSTICK_GPIO_PIN_ADDRESS	(GPIO_ADDRESS + 0x0B4)
#define JOYSTICK_GPIO_PIN *((volatile uint32_t*)(JOYSTICK_GPIO_PIN_ADDRESS))

//Initialize the RGB-LED (Change the Direction of the Pins Output and Change the Color of the RGB as Black)
void RGB_LED_Init() {
}

//Initialize the Joystick (Change the Direction of the Pins Input, If needed)
void Joystick_Init() {
}

//Write a code for changing the color of RGB LED when Joystick is pressed.
//	JOYSTICK Left = RGB Red
//	JOYSTICK Up = RGB Green
//	JOYSTICK Down = RGB Blue
//	JOYSTICK Center = RGB Magenta
//	JOYSTICK Right = RGB Cyan
//	JOYSTICK (Default) = RGB Black
void Check_Joystick_Pressed() {
}

void init() {
	RGB_LED_Init();
	Joystick_Init();
}

void update() {
	Check_Joystick_Pressed();
}

int main() {
	init();

	while(1) {
		update();
	}
}
