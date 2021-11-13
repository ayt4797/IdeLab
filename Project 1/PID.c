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

extern double servo_state_center;
extern double servo_limit_right;
extern double servo_limit_left; 
extern int center_rightlimit;
extern int i;
extern int center_leftlimit;
extern uint16_t binline[128];
extern char str[100];
#define TOLERANCE_LEFT 5
#define TOLERANCE_RIGHT 122
#define TOLERANCE_FACTOR 0

short current_leftmost;
short current_rightmost;
short tolerance_right;
short tolerance_left;
short dir;
double error[3]; // [0] - Current error // [1] - Last Error // [2] - 2nd latest error ... and so on
double correction;

BOOLEAN print_direction = FALSE;

double kp = 0.0525/16;
double ki = 0;
double kd = 0;

short get_current_leftmost() {
	for (i=0; i<127; i++) {
		if (binline[i] == 0) {
			current_leftmost = i;
		} else {
			return current_leftmost;
		}
	}
	return 127;
}

short get_current_rightmost() {
	for (i=127; i>0; i--) {
		if (binline[i] == 0) {
			current_rightmost = i;
		} else {
			return current_rightmost;
		}
	}
	return 0;
}

short steering_direction(short tolerance_left, short tolerance_right) {
	// Case 1.
	if ((current_leftmost < tolerance_left) && (current_rightmost) > tolerance_right) {
		return 0; // Straight
	} 
	// Case 2.
	else if (current_leftmost >= tolerance_left) {
		return 1; // Turn Right
	} 
	// Case 3
	else if (current_rightmost <= tolerance_right) {
		return 2; // Turn Left
	} 
	// Error Case
	else {
		put("Error! Direction not found\r\n");
		return 3;
	}
}

double verify_correction(double c) {
	if (c > servo_limit_left) {
		return servo_limit_left;
	} else if (c < servo_limit_right) {
		return servo_limit_right;
	} else {
		return c;
	}
}

void steering_adjust() {
	// New method using 3 basic cases
	current_leftmost = 0;
	current_rightmost = 127;
	dir = 0; // 0 = straight, // 1 = turn right // 2 = turn left // 3 = error (straight)
	error[0] = 0; // Ideally error is 0 so straight
	correction = servo_state_center;
	tolerance_left = TOLERANCE_FACTOR + center_leftlimit;
	tolerance_right = center_rightlimit - TOLERANCE_FACTOR;
	
//	char* ch = get_gain();
//	if(ch!=0){
//		phone_gain = atof(ch);
//		put(ch);
//		
//	}
//	if(phone_gain){
//		gain = phone_gain;		
//		sprintf(str,"gain: %f, phone_gain : %f\n",gain,phone_gain);
//		put(str);
//	}
//	double kp = 0.0525/gain; // Proportional gain.

	current_leftmost = get_current_leftmost();
	current_rightmost = get_current_rightmost();

	dir = steering_direction(tolerance_left, tolerance_right);
	
	// Calculate error - e(t)
	switch(dir) {
		case(0): // Straight!
			error[0] = 0;
			break;
		case(1): // Turn Right
			// Negative error
			error[0] = tolerance_left - current_leftmost;
			break;
		case(2): // Turn Left
			// Positive error
			error[0] = tolerance_right - current_rightmost;
			break;
		case(3): // Error case. Straight?
			error[0] = 0;
			 break;
		default:
			 put("dir out of range");
			 error[0] = 0;
			 break;
	}
	
 correction = servo_state_center + kp*error[0];

 correction = verify_correction(correction);
	
	if (print_direction) { // Verbose direction
		sprintf(str, "dir=%d", dir);
		put(str);
	}
	servo_move(correction);
}
