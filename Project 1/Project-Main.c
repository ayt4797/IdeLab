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
BOOLEAN printCameraOutput;
#define TOLERANCE_LEFT 5
#define TOLERANCE_RIGHT 122

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

void new_steering_adjust() {
	// New method using 3 basic cases
	short tolerance_factor = 5;
	int current_leftmost = 0;
	int current_rightmost = 127;
	int tolerance_left = tolerance_factor + center_leftlimit;
	int tolerance_right = center_rightlimit - tolerance_factor;
	short dir = 0; // 0 = straight, // 1 = turn right // 2 = turn left // 3 = error (straight)
	double error = 0; // Ideally error is 0 so straight
	double correction = servo_state_center;
	double kp = 0.0525/6; // Proportional gain.
	
	for (i=0; i<127; i++) {
		if (binline[i] == 0) {
			current_leftmost = i;
		} else {
			break;
		}
	}
	
	for (i=127; i>0; i--) {
		if (binline[i] == 0) {
			current_rightmost = i;
		} else {
			break;
		}
	}
	
	// Case 1.
	if ((current_leftmost < tolerance_left) && (current_rightmost) > tolerance_right) {
		dir = 0; // Straight
	} 
	// Case 2.
	else if (current_leftmost >= tolerance_left) {
		dir = 1; // Turn Right
	} 
	// Case 3
	else if (current_rightmost <= tolerance_right) {
		dir = 2; // Turn Left
	} 
	// Error Case
	else {
		put("Error! Direction not found\r\n");
		dir = 3;
	}
	
	// Calculate error - e(t)
	switch(dir) {
		case(0): // Straight!
			error = 0;
			break;
		case(1): // Turn Right
			// Negative error
			error = tolerance_left - current_leftmost;
			break;
		case(2): // Turn Left
			// Positive error
			error = tolerance_right - current_rightmost;
			break;
		case(3): // Error case. Straight?
			error = 0;
			 break;
		default:
			 put("dir out of range");
			 error = 0;
			 break;
	}
	
	correction = servo_state_center + kp*error;
	if (correction > servo_limit_left) {
		correction = servo_limit_left;
	} else if (correction < servo_limit_right) {
		correction = servo_limit_right;
	}
	sprintf(str, "dir=%f", (double)dir);
	put(str);
	sprintf(str, " error=%f", error);
	put(str);
	sprintf(str, " correction=%f\r\n", correction);
	put(str);
	servo_move(correction);
}

void old_steering_adjust() {
	int current_leftmost = 0;
	int current_rightmost = 127;
	double error = 0; // Error in P control
	double kp = 0; // Gain of proportional control
	short right_gain = 6; // Difference in error achieves max response at this value
	short left_gain = 19;
	double correction = servo_state_center; // By default
	char temp[128];

	// Find all Rising and Falling edges in the code.
	short edges[128];
	short num_edges = 0;
	BOOLEAN found_rising = FALSE;
	for (i=0; i<128; i++) {
		if ((binline[i] == 1) && (found_rising == FALSE)) { // Rising Edge Found!
			edges[num_edges] = i;
			num_edges++;
			found_rising = TRUE;
		} else if ((binline[i] != 1) && (found_rising == TRUE)) { // Falling Edge Found!
			if (i>0) {
				edges[num_edges] = i-1; // The last index was the falling edge.
			} else {
				edges[num_edges] = 0;
			}
			num_edges++;
			found_rising=FALSE;
		}
	}
	for (i=0; i<num_edges; i++) {
		sprintf(str, "%d, ", edges[i]);
		put(str);
	}
	put("\r\n");
	short pulse_lengths[num_edges/2];
	
	if(num_edges%2 == 0 && num_edges > 2) { // Number of edges found were even. Multiple pulses found
		for (i=0; i<(num_edges/2); i++) {
			pulse_lengths[i] = edges[(2*i)+1] - edges[2*i];
			sprintf(str, "Pulse=%d", pulse_lengths[i]);
			put(str);
		}
		put("\r\n");
		short longest_pulse = 0;
		for (i=0; i<(num_edges/2); i++) {
			if (pulse_lengths[longest_pulse] > pulse_lengths[i]) {
				longest_pulse = i;
			}
		}
		current_leftmost = edges[longest_pulse*2];
		current_rightmost = edges[(longest_pulse*2)+1];
	} else if (num_edges%2 == 1) { // Number of edges found are odd. Anaylze differently
		current_leftmost = edges[0];
		current_rightmost = edges[num_edges-1];
		put("B");
	} else if (num_edges == 2) { // Only two edges were found
		current_leftmost = edges[0];
		current_rightmost = edges[1];
	}
	if(current_leftmost<TOLERANCE_LEFT){
		current_leftmost = 127;
	}
	if(current_rightmost>TOLERANCE_RIGHT){
		current_rightmost = 0;
	}
	if (current_leftmost < center_leftlimit) { // Steer Left!
		kp = (servo_limit_left-servo_state_center)/left_gain;
		error = center_leftlimit - current_leftmost;
		correction = servo_state_center + (kp*error);
		if (correction > servo_limit_left) {
			correction = servo_limit_left;
		}
	} else if (current_rightmost > center_rightlimit) { // Steer Right!
		kp = (servo_state_center - servo_limit_right)/right_gain;
		error = (center_rightlimit - current_rightmost);
		correction = servo_state_center + (kp*error);
		if (correction < servo_limit_right) {
			correction = servo_limit_right;
		}
	} else { // Go straight!
		correction = servo_state_center;
		driveMotors_setSpeed(20);
	}
	servo_move(correction);
}

/////////////////////////////////////////////////////
// main function
/////////////////////////////////////////////////////
int main(void)
{
	// Generic Initializations
	i = 0;
	j = 0;
	printCameraOutput = TRUE; // Show Camera Output on Terminal
	// OLED_Output - Show Camera Values on OLED Display
	// 0 - Analog unfilter data
	// 1 - Smooth filtered data
	// 2 - Binarized Data (1/0)
	OLED_Output = 2;
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
		if (g_sendData == TRUE) 
		{
			LED1_On(); // LED ON = DATA TRANSFER
			if (printCameraOutput) {
			for (i=0; i<127; i++) {
					if (binline[i] == 1) {
						uart0_putchar('O');
					} else {
						uart0_putchar('-');
				}
			}
			uart0_put("\r\n");
		}
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
		//old_steering_adjust();
		new_steering_adjust();
	}
}
