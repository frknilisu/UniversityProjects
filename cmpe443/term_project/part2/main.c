#include "LPC407x_8x_177x_8x.h"

#include "Library/GPIO.h"
#include "Library/Joystick.h"
#include "Library/PWM.h"
#include "Library/LED.h"
#include "Library/PLL.h"
#include "Library/Ultrasonic.h"
#include "Library/SystemTickTimer.h"
#include "Library/ADC.h"
#include "Library/PINS.h"

typedef enum {
	BACKWARD = 0,
	FORWARD = 1,
	STOP = 2
} FB_Direction;

uint8_t stop_flag = 1;		// initially the car should stop
double speed = 0;					// speed change via trimpot

uint32_t diff;						// temp variable to keep difference between light sensor values
uint8_t sign = 0;					// which light sensor value larger than the other

uint8_t backoff = 0;			// 10-30 cm away case flag
uint32_t led_threshold = 0x300;		// threshold to say light sensors give similar results

void init() {
	
	GPIO_DIR_Write(PORT0, LDR_LEFT_MASK, INPUT);		// LDR_LEFT should Analog Input
	GPIO_DIR_Write(PORT0, LDR_RIGHT_MASK, INPUT);		// LDR_RIGHT should Analog Input
	
	GPIO_DIR_Write(PORT0, TRIGGER_MASK, OUTPUT);		// Trigger should Output
	GPIO_DIR_Write(PORT0, ECHO_MASK, INPUT);				// Echo should Analog Input
	
	GPIO_DIR_Write(PORT0, IN1_MASK, OUTPUT);		// In1
	GPIO_DIR_Write(PORT0, IN2_MASK, OUTPUT);		// In2
	GPIO_DIR_Write(PORT0, IN3_MASK, OUTPUT);		// In3
	GPIO_DIR_Write(PORT0, IN4_MASK, OUTPUT);		// In4
	
	SysTick_Init();
	PWM_Init();
	
	PLL_Change_Configuration();
	
	Joystick_Init();
	
	Ultrasonic_Trigger_Timer_Init();
	Ultrasonic_Capture_Timer_Init();
	Ulrasonic_Start_Trigger();
	
	ADC_Init();
	ADC_Start();

	__enable_irq();
}

void forward(){
	/* left motor rotate cc -> IN1 = 0, IN2 = 1
	 * right motor rotate cc -> IN3 = 0, IN4 = 1
	*/
	GPIO_PIN_Write(PORT0, IN1_MASK, LOW); 			// IN1 = 0
	GPIO_PIN_Write(PORT0, IN2_MASK, HIGH); 		// IN2 = 1
	GPIO_PIN_Write(PORT0, IN3_MASK, LOW); 			// IN3 = 0
	GPIO_PIN_Write(PORT0, IN4_MASK, HIGH); 		// IN4 = 1
	
	// frequency to 20 milisecond
	PWM_Cycle_Rate(50);
	
	// full gas -> ENA -> 100, ENB -> 100
	PWM_Write((uint32_t)speed, 5);
	PWM_Write((uint32_t)speed, 6);
	
	// car is moving, not stop
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
	GPIO_PIN_Write(PORT0, IN1_MASK, HIGH); 		// IN1 = 1
	GPIO_PIN_Write(PORT0, IN2_MASK, LOW);		// IN2 = 0
	GPIO_PIN_Write(PORT0, IN3_MASK, HIGH);		// IN3 = 1
	GPIO_PIN_Write(PORT0, IN4_MASK, LOW);		// IN4 = 0
	
	// frequency to 20 milisecond
	PWM_Cycle_Rate(50);
	
	// full gas -> ENA -> 100, ENB -> 100
	PWM_Write((uint32_t)speed, 5);
	PWM_Write((uint32_t)speed, 6);
	
	// car is moving, not stop
	stop_flag = 0;
	
	/* Back-Left and Back-Right LEDs turned on
	 * the other LEDs turned off.
	*/
	PWM_Write(100, 3);		// Back-Left LED = ON
	PWM_Write(100, 4);		// Back-Right LED = ON
	PWM_Write(0, 1);			// Front-Right LED = OFF
	PWM_Write(0, 2);			// Front-Left LED = OFF
}

void turn_ccw(){
	/* left motor stop -> IN1 = 0, IN2 = 0
	 * right motor cw -> IN3 = 0, IN4 = 1
	*/
	GPIO_PIN_Write(PORT0, IN1_MASK, LOW);			// IN1 = 0
	GPIO_PIN_Write(PORT0, IN2_MASK, LOW);			// IN2 = 0
	GPIO_PIN_Write(PORT0, IN3_MASK, LOW);			// IN3 = 0
	GPIO_PIN_Write(PORT0, IN4_MASK, HIGH);			// IN4 = 1
	
	// to blink (2 times in a second)
	PWM_Cycle_Rate(500);
	
	// full gas -> ENA -> 100, ENB -> 100
	PWM_Write(100, 5);
	PWM_Write(100, 6);
	
	// car is moving, not stop
	stop_flag = 0;
	
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
	GPIO_PIN_Write(PORT0, IN1_MASK, LOW); 		// IN1 = 0
	GPIO_PIN_Write(PORT0, IN2_MASK, HIGH);			// IN2 = 1
	GPIO_PIN_Write(PORT0, IN3_MASK, LOW);			// IN3 = 0
	GPIO_PIN_Write(PORT0, IN4_MASK, LOW);			// IN4 = 0
	
	// to blink (2 times in a second)
	PWM_Cycle_Rate(500);
	
	// full gas -> ENA -> 100, ENB -> 100
	PWM_Write(100, 5);
	PWM_Write(100, 6);
	
	// car is moving, not stop
	stop_flag = 0;
	
	/* Front-Right and Back-Right LEDs blink (2 times in a second)
	 * the other LEDs should be turned off
	*/
	PWM_Write(50, 1);			// Front-Right LED = ON
	PWM_Write(50, 4);			// Back-Right LED = ON
	PWM_Write(0, 2);			// Front-Left LED = OFF
	PWM_Write(0, 3);			// Back-Left LED = OFF
}

void stop(){
	
	// zero gas -> ENA -> 0, ENB -> 0
	PWM_Write(0, 5);
	PWM_Write(0, 6);
	
	// the car is stopping
	stop_flag = 1;
	
	// When robot stops, all the LEDs should be turned off.
	PWM_Write(0, 1);
	PWM_Write(0, 2);
	PWM_Write(0, 3);
	PWM_Write(0, 4);
}

// A/D Conversion start on Systick Time
void SysTick_Handler() {
	ADC_Start();
}

// go to deep power down mode
void deep_power_down_on(){
	SCR |= (1 << 1);
	PCON |= (0x3);
	__WFI();
}

// get trimpot value and scale the motor speed between 50% - 100% Duty Cycle according to this value
void calc_speed(){
	speed = 1.0*ADC_Trimpot*50/0xFFF + 50;
}

void update_speed(){
	// gas -> ENA -> speed, ENB -> speed
	PWM_Write((uint32_t)speed, 5);
	PWM_Write((uint32_t)speed, 6);
}

// return mean of given array that contains n elements
// will use to calculate mean of last 10 ultrasonicDistance
double mean(int n, int dists[]){
	int sum = 0, i;
	for(i=0;i<n;i++)
		sum+=dists[i];
	return ((double)sum/n);
}

// up -> forward()
// down -> backward()
// right -> clockwise
// left -> counter-clockwise
// center -> stop
void joystick_controller(){
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


uint32_t counter = 0;
double average_distance = 0;
int32_t distances[10];

// When Joystick Up button is pressed, your robot should start to travel in forward direction.
// When Joystick Center button is pressed, your robot should stop.

// When robot stops, all the LEDs should be turned off.
// When robot travels in forward direction, 
//		Front-Left and Front-Right LEDs should be turned on and the other LEDs should be turned off.
// When robot travels in backward direction, 
// 		Back-Left and Back-Right LEDs should be turned on and the other LEDs should be turned off.
// When robot rotates counter-clockwise direction, 
// 		Front-Left and Back-Left LEDs should blink (2 times in a second) and the other LEDs should be turned off.
// When robot rotates clockwise direction, 
// 		Front-Right and Back-Right LEDs should blink (2 times in a second) and the other LEDs should be turned off
void controller(){
	
	calc_speed();		// calculate speed according to TrimpotValue
	update_speed();
	
	if(Joystick_Up_Pressed()){
		// forward direction
		forward();
	} else if(Joystick_Center_Pressed()){
		// go to deep power down mode
		deep_power_down_on();
	} else if(!stop_flag){		// if the car is moving
		
		// ULTRASONIC SENSOR Control Region
		
		// get last 10 distance from ultrasonic sensor and calculate mean
		if(counter < 10){
			distances[counter] = ultrasonicSensorDistance;
			counter++;
		} else {
			counter = 0;
			average_distance = mean(10, distances);
		}
		
		if(average_distance > 8.0 && average_distance < 12.0 && !backoff){		// obstacle close 10 cm to car -> go backward direction
			backoff = 1;
			backward();
		} else if(average_distance < 30.0 && backoff){		// obstacle away from between 10-30 cm, continue to go backward
			backward();
		} else {			
			
			// LIGHT SENSORS Control Region
			
			forward();
			backoff = 0;
			
			// get absoulute difference between light sensors
			diff = ADC_Left < ADC_Right ? (ADC_Right - ADC_Left) : (ADC_Left - ADC_Right);
			
			// which sensor value larger
			sign = ADC_Left < ADC_Right;
			
			if(diff < led_threshold){			// small difference means sensor's values similar
				//forward();
			} else {
				if(sign){		// right larger than left -> turn clockwise
					turn_cw();
				} else {		// left larger than right -> turn counter-clockwise
					turn_ccw();
				}
			}
		}
	
		__WFI();
		
	}
}

void update() {
	
	//PLL_Test();
	//Ultrasonic_Test();
	//joystick_controller();
	//LED_Sensor_Test();
	//deep_power_down_on();
	
	controller();
	
}

int main() {
	init();
	//stop();			// stop car on start
	
	while(1) {
		update();
	}
}

