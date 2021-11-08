/*
	* CMPE 460  Interfacing Digital Electronics
	* Original Author: LJBeato
	* Initialy Created: 1/14/2021

	TI CUP PROJECT 1 CODE
	Created: 11/1/2021
	Modified: 11/1/2021
	Authors: Dante Sivo & Aubrey Tarmu
*/

#include <stdio.h>
#include <stdlib.h>
#include "msp.h"
#include "uart.h"
#include "Lab1.h"
#include "Timer32.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"
#include "ControlPins.h"
#include "oled.h"
#include "Camera.h"
#include "TimerA.h"
#include "Motors.h"

// line stores the current array of camera data
extern uint16_t line[128];
uint16_t inversed_line[128];
uint16_t scaled_binline[128];
extern uint16_t smoothline[128];
extern uint16_t binline[128];
extern BOOLEAN g_sendData;
static char str[100];
extern int parseMode;
extern int i;
extern int j;
extern short OLED_Output;
extern double servo_state_center;
extern double servo_limit_right;
extern double servo_limit_left; 
extern int center_rightlimit;
extern int center_leftlimit;
#define LEFT_MOST_TOLERANCE 32
#define RIGHT_MOST_TOLERANCE 96
#define DRIVE_STRAIGHT_SPEED 20
#define PULSE_LENGTH_TOLERANCE 5
//#define SERVO_LIMIT_CENTER 0.0725
//#define SERVO_LIMIT_LEFT 0.1 // 0.085
//#define SERVO_LIMIT_RIGHT 0.0475 //0.045

//#define RIGHT_G 6
//#define LEFT_G 19
#define LEFT_KP 0.00144736842 // (SERVO_LIMIT_LEFT-SERVO_LIMIT_CENTER)/LEFT_G
#define RIGHT_KP 0.00416666666;
////////////////////////////////////////////////////
// Show Camera Output on OLED
////////////////////////////////////////////////////
void OLED_Camera_Debug(short select) {
	switch(select) {
	case 0:
			plotline(line);
			break;
		case 1:
			plotline(smoothline);
			break;
		case 2:
			// adjust binline for camera view.
			bin_plotline(binline);
			break;
		default:
			break;
		}
}

////////////////////////////////////
// Generic Startup
////////////////////////////////////
void put(char *temp){ //prints to both putty & phone
    uart0_put(temp);
    uart2_put(temp);
}

void car_startup() {
	DisableInterrupts();
	uart0_init();
	uart2_init();

	put("\r\nTI CUP Project\r\n");
	put("\r\nINIT LEDs");
	LED1_Init();
	LED2_Init();
  put("- ENABLED\r\n");
  put("OLED DISPLAY");
	OLED_Init();
  put("- ENABLED\r\n");
	
	put("INIT Switches");
	Switches_Init();
	put("- ENABLED\r\n");
	put("Enable Interrupts");
	EnableInterrupts();
  put("- ENABLED\r\n");
	
  put("INIT CAMERA");
	INIT_Camera();
  put("- ENABLED\r\n");
	
	put("INIT DC MOTORS");
	driveMotors_init();
	
	P3->SEL0 &= ~BIT6;
	P3->SEL0 &= ~BIT7;
	P3->SEL1 &= ~BIT6;
	P3->SEL1 &= ~BIT7;
	P3->DIR |= BIT6;
	P3->DIR |= BIT7;
	P3->DS |= BIT6;
	P3->DS |= BIT7;
	P3->OUT |= BIT6;
	P3->OUT |= BIT7;
	uart0_put("- ENABLED\r\n");
	
	driveMotors_stop();
	
	uart0_put("INIT SERVO MOTORS");
	TIMER_A2_PWM_Init(SystemCoreClock/(50*8*2), 0.1, 1);
	uart0_put("- ENABLED\r\n");
	
	servo_right();
	ms_delay(1000);
	servo_left();
	ms_delay(1000);
	servo_center();
	
}
int moving_off_center(){ //darkness is between the two tolerance then we need to turn
	
	for (i=LEFT_MOST_TOLERANCE; i<RIGHT_MOST_TOLERANCE; i++) {
		int pulse_length;
		if(binline[i]==0){
			pulse_length++;
		}
		if(pulse_length>PULSE_LENGTH_TOLERANCE){ //we may see sudden sparks of darkness if the darkness goes on for more than TOLERANCE pixels than it's not a camera error
			return i;
		}
	}
	return 0;
}
int turn_right(){ //returns the point at which the darkness becomes light, indicating how far to the left we are
		short max=0;
		for (int i=0; i<LEFT_MOST_TOLERANCE; i++) {
			if(binline[i]==0){
				max=i;
			}
	} 
	return max;
}
int turn_left(){ //returns the point at which the darkness becomes light, indicating how far to the right we are
		short max=0;
		for (i=RIGHT_MOST_TOLERANCE; i<128; i++) {
			if(binline[i]==0){
				max=i;
			}
	}
	return max;
}
void steering_adjust() {
	int current_leftmost = 0;
	int current_rightmost = 127;
	double error = 0; // Error in P control
	double kp = 0; // Gain of proportional control
	double correction = servo_state_center; // By default
	int dir; //0=straight, 1=left, 2=right;
	if(moving_off_center()){
		current_leftmost=turn_right();
		current_rightmost=turn_left();
		short right_most_adjusted = 128 - current_rightmost; //we need to adjust this because the value will always be greater on the right most b/c it's labeled 0-127
		if(current_leftmost>right_most_adjusted){
			dir = 1;
		}
		else {
			dir=2;
		}
	}else{
		dir=0;
	}
	
	if (dir==1) { // Steer Left!
		kp = LEFT_KP;
		error = center_leftlimit - current_leftmost;
		correction = servo_state_center + (kp*error);
		if (correction > servo_limit_left) {
			correction = servo_limit_left;
		}
	} else if (dir==2) { // Steer Right!
		kp = RIGHT_KP;
		error = (center_rightlimit - current_rightmost);
		correction = servo_state_center + (kp*error);
		if (correction < servo_limit_right) {
			correction = servo_limit_right;
		}
	} else { // Go straight!
		correction = servo_state_center;
		driveMotors_setSpeed(DRIVE_STRAIGHT_SPEED);
	}
	servo_move(correction);
	sprintf(str, "error=%f ", error);
	uart2_put(str);
	sprintf(str, "kp=%f\n\r", kp);
	uart2_put(str);
}

/////////////////////////////////////////////////////
// main function
/////////////////////////////////////////////////////
int main(void)
{
	// Generic Initializations
	i = 0;
	j = 0;
	
	// OLED_Output - Show Camera Values on OLED Display
	// 0 - Analog unfilter data
	// 1 - Smooth filtered data
	// 2 - Binarized Data (1/0)
	OLED_Output = 0;
	sprintf(str,"OLED Mode=%d\n\r",OLED_Output);
	put(str);
	
	// Preform generic initalizations
	car_startup();
	camera_calibration();
	
	put("Press Switch1 to calibrate center\r\n");
	while (Switch1_Pressed() == 0) {}; // Determine the center point of the track on a straight portion.
	while (g_sendData == FALSE) {}; // wait until data is able to be parsed.
	parsedata();
	calibrate_center();
	g_sendData = FALSE;
		
	put("Use Switch 1 to index through the OLED modes\r\n");
	put("Use Switch 2 for Start/E-Stop!\r\n");
	ms_delay(1000);
	
	while (Switch2_Pressed() != 0) {}; // Use a button to wait to drive the car
	ms_delay(1000);
	driveMotors_setSpeed(20); // 5% forward
	put("Oh boy! Time to drive!\r\n");
	OLED_Print(1, 1, "press left button for green");
	OLED_Print(2, 2, "press right button for red");
//	OLED_DisplayCameraData(line);
	while(1)
	{
		cameraUpsidedown(line);
		if (g_sendData == TRUE) 
		{
			LED1_On(); // LED ON = DATA TRANSFER
		}
		
		parsedata(); // Binary Edge Detection
		LED1_Off();
		
		OLED_Camera_Debug(OLED_Output);
		if (Switch1_Pressed()) {
			if (OLED_Output < 2) {
				OLED_Output++;
			} else {
				OLED_Output = 0;
			}
			sprintf(str,"OLED Mode=%d\n\r",OLED_Output);
			put(str);
			ms_delay(1000);
		}

		P2->OUT &= ~BIT0; // No Light
		P2->OUT &= ~BIT1;
		P2->OUT &= ~BIT2;
		g_sendData = FALSE; // Ready for next signal.
		LED1_Off();
		
		if (Switch2_Pressed() || isOffTrack()) {
			OLED_display_clear();
			OLED_display_off();
			driveMotors_stop();
			break;
		}

		// Steering adjustment. Based on how far out the wheels are. Adjust left or right
		steering_adjust();
	}
}
