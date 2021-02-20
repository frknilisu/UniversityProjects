#include "LPC407x_8x_177x_8x.h"

#include "Library/GPIO.h"
#include "Library/Joystick.h"
#include "Library/PWM.h"

typedef enum {
	BACKWARD = 0,
	FORWARD = 1,
	STOP = 2
} FB_Direction;

uint8_t stop_flag = 1;

void init() {
	Joystick_Init();
	
	PWM_Init();
	
	GPIO_DIR_Write(PORT0, (1 << 23), OUTPUT);
	GPIO_DIR_Write(PORT0, (1 << 24), OUTPUT);
	GPIO_DIR_Write(PORT0, (1 << 25), OUTPUT);
	GPIO_DIR_Write(PORT0, (1 << 26), OUTPUT);
}

uint8_t FBward_Dir(){
	if(stop_flag){
		return STOP;
	} else{
		if(GPIO_PIN_Read(PORT0, (1 << 23)) == HIGH && GPIO_PIN_Read(PORT0, (1 << 25)) == HIGH
			&& GPIO_PIN_Read(PORT0, (1 << 24)) == LOW && GPIO_PIN_Read(PORT0, (1 << 26)) == LOW){
				return FORWARD;
		} else if(GPIO_PIN_Read(PORT0, (1 << 23)) == LOW && GPIO_PIN_Read(PORT0, (1 << 25)) == LOW
			&& GPIO_PIN_Read(PORT0, (1 << 24)) == HIGH && GPIO_PIN_Read(PORT0, (1 << 26)) == HIGH){
				return BACKWARD;
		}
	}
}

void forward(){
	/* left motor rotate cc -> IN1 = 0, IN2 = 1
	 * right motor rotate cc -> IN3 = 0, IN4 = 1
	*/
	GPIO_PIN_Write(PORT0, (1 << 23), LOW); 			// IN1 = 0
	GPIO_PIN_Write(PORT0, (1 << 24), HIGH); 		// IN2 = 1
	GPIO_PIN_Write(PORT0, (1 << 25), LOW); 			// IN3 = 0
	GPIO_PIN_Write(PORT0, (1 << 26), HIGH); 		// IN4 = 1
	
	// full gas -> ENA -> 100, ENB -> 100
	PWM_Write(100, 5);
	PWM_Write(100, 6);
	
	stop_flag = 0;
	
	/* Front-Left and Front-Right LEDs turned on
	 * the other LEDs turned off.
	*/
	PWM_Write(100, 1); 		// Front-Right LED = ON
	PWM_Write(100, 2);		// Front-Left LED = ON
	PWM_Write(0, 3);			// Back-Left LED = OFF
	PWM_Write(0, 4);			// Back-Right LED = OFF
}

void backward(){
	/* left motor rotate ccw -> IN1 = 1, IN2 = 0
	 * right motor rotate ccw -> IN3 = 1, IN4 = 0
	*/
	GPIO_PIN_Write(PORT0, (1 << 23), HIGH); 		// IN1 = 1
	GPIO_PIN_Write(PORT0, (1 << 24), LOW);		// IN2 = 0
	GPIO_PIN_Write(PORT0, (1 << 25), HIGH);		// IN3 = 1
	GPIO_PIN_Write(PORT0, (1 << 26), LOW);		// IN4 = 0
	
	// full gas -> ENA -> 100, ENB -> 100
	PWM_Write(100, 5);
	PWM_Write(100, 6);
	
	stop_flag = 0;
	
	/* Back-Left and Back-Right LEDs turned on
	 * the other LEDs turned off.
	*/
	PWM_Write(100, 3);		// Back-Left LED = ON
	PWM_Write(100, 4);		// Back-Right LED = ON
	PWM_Write(0, 1);			// Front-Right LED = OFF
	PWM_Write(0, 2);			// Front-Left LED = OFF
}

void stop(){
	
	PWM_Write(0, 5);
	PWM_Write(0, 6);
	
	stop_flag = 1;
	
	// When robot stops, all the LEDs should be turned off.
	PWM_Write(0, 1);
	PWM_Write(0, 2);
	PWM_Write(0, 3);
	PWM_Write(0, 4);
}

void turn_ccw(){
	/* left motor stop -> IN1 = 0, IN2 = 0
	 * right motor cw -> IN3 = 0, IN4 = 1
	*/
	GPIO_PIN_Write(PORT0, (1 << 23), LOW);			// IN1 = 0
	GPIO_PIN_Write(PORT0, (1 << 24), LOW);			// IN2 = 0
	GPIO_PIN_Write(PORT0, (1 << 25), LOW);			// IN3 = 0
	GPIO_PIN_Write(PORT0, (1 << 26), HIGH);			// IN4 = 1
	
	// full gas -> ENA -> 100, ENB -> 100
	PWM_Write(100, 5);
	PWM_Write(100, 6);
	
	// to blink (2 times in a second)
	PWM_Cycle_Rate(500);
	
	/* Front-Left and Back-Left LEDs blink (2 times in a second)
	 * the other LEDs should be turned off
	*/
	PWM_Write(50, 2);			// Front-Left LED = ON
	PWM_Write(50, 3);			// Back-Left LED = ON
	PWM_Write(0, 1);			// Front-Right LED = OFF
	PWM_Write(0, 4);			// Back-Right LED = OFF
}

void turn_cw(){
	/* left motor rotate cw -> IN1 = 0, IN2 = 1
	 * right motor stop -> IN3 = 0, IN4 = 0
	*/
	GPIO_PIN_Write(PORT0, (1 << 23), LOW); 		// IN1 = 0
	GPIO_PIN_Write(PORT0, (1 << 24), HIGH);			// IN2 = 1
	GPIO_PIN_Write(PORT0, (1 << 25), LOW);			// IN3 = 0
	GPIO_PIN_Write(PORT0, (1 << 26), LOW);			// IN4 = 0
	
	// full gas -> ENA -> 100, ENB -> 100
	PWM_Write(100, 5);
	PWM_Write(100, 6);
	
	// to blink (2 times in a second)
	PWM_Cycle_Rate(500);
	
	/* Front-Right and Back-Right LEDs blink (2 times in a second)
	 * the other LEDs should be turned off
	*/
	PWM_Write(50, 1);			// Front-Right LED = ON
	PWM_Write(50, 4);			// Back-Right LED = ON
	PWM_Write(0, 2);			// Front-Left LED = OFF
	PWM_Write(0, 3);			// Back-Left LED = OFF
}


// When Joystick Up button is pressed, your robot should start to travel in forward direction.
// When Joystick Down button is pressed, your robot should start to travel in backward direction.
// When Joystick Center button is pressed, your robot should stop.
// When Joystick Left button is pressed, your robot should start to rotate counter-clockwise direction 
// When Joystick Right button is pressed, your robot should rotate clockwise direction

// When robot stops, all the LEDs should be turned off.
// When robot travels in forward direction, 
//		Front-Left and Front-Right LEDs should be turned on and the other LEDs should be turned off.
// When robot travels in backward direction, 
// 		Back-Left and Back-Right LEDs should be turned on and the other LEDs should be turned off.
// When robot rotates counter-clockwise direction, 
// 		Front-Left and Back-Left LEDs should blink (2 times in a second) and the other LEDs should be turned off.
// When robot rotates clockwise direction, 
// 		Front-Right and Back-Right LEDs should blink (2 times in a second) and the other LEDs should be turned off

void update() {
	
	if(Joystick_Up_Pressed()){
		// forward direction
		forward();
	} else if(Joystick_Center_Pressed()){
		// stop
		stop();
	} else if(Joystick_Down_Pressed()){
		// backward direction
		backward();
	} else if(Joystick_Left_Pressed()){
		// counter-clockwise direction
		turn_ccw();
	} else if(Joystick_Right_Pressed()){
		// clockwise direction
		turn_cw();
	}
	
}

int main() {
	init();
	
	while(1) {
		update();
		//forward();
	}
}

