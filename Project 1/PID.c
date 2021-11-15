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
#define STANDARD_STRAIGHT_SPEED 30
#define ROCKET_STRAIGHT_SPEED 50
#define TURN_SPEED 25
#define MOTOR_FACTOR 3

short current_leftmost;
short current_rightmost;
short tolerance_right;
short tolerance_left;
short dir;
double error[3]; // [0] - Current error // [1] - Last Error // [2] - 2nd latest error ... and so on
double correction=.0725;

BOOLEAN print_direction = FALSE;
BOOLEAN PID_differential = FALSE;

double kp = 0.0525/90;
double ki = 0.0525/360;
double kd = 0.0525/1440;

BOOLEAN straight_machine[4]; // Straight state machine
BOOLEAN newly_straight;

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
		// Motor assignment later
		return 0; // Straight
	} 
	// Case 2
	else if (current_leftmost >= tolerance_left) {
		if (PID_differential == FALSE) {
			driveMotors_forwardLeft(TURN_SPEED + MOTOR_FACTOR);
			driveMotors_forwardRight(TURN_SPEED - MOTOR_FACTOR);
		}
		return 1; // Turn Right
	} 
	// Case 3
	else if (current_rightmost <= tolerance_right) {
		if (PID_differential == FALSE) {
			driveMotors_forwardLeft(TURN_SPEED - MOTOR_FACTOR);
			driveMotors_forwardRight(TURN_SPEED + MOTOR_FACTOR);
		}
		return 2; // Turn Left
	} 
	// Error Case
	else {
		put("Error! Direction not found\r\n");
		return 3;
	}
}

double verify_limit(double c) {
	if (c > servo_limit_left) {
		return servo_limit_left;
	} else if (c < servo_limit_right) {
		return servo_limit_right;
	} else {
		return c;
	}
}
float get_PID(float prev_pos){
    float new_pos 
			 = prev_pos
			+(kp*(error[0]-error[1])) 
		  + ki*((error[0]-error[1])/2)
		  + kd*(error[0]-2*error[1]+error[2]);
		
		// Shift errors to load newest error into error[0]
    error[2] = error[1];
    error[1] = error[0];

    return new_pos;
}

void steering_adjust() {
	// New method using 3 basic cases
	current_leftmost = 0;
	current_rightmost = 127;
	dir = 0; // 0 = straight, // 1 = turn right // 2 = turn left // 3 = error (straight)
	error[0] = 0; // Ideally error is 0 so straight
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
	
	// Advance all values in the counter
		for (i=0; i<3; i++) { 
			straight_machine[i+1] = straight_machine[i];
		}
	
	// Calculate error - e(t)
	switch(dir) {
		case(0): // Straight!
			straight_machine[0] = TRUE;
			error[0] = 0;
			
		 newly_straight = TRUE; 
			for (i=0; i<4; i++) {
				// If the car hasn't been straight for 4 checks
				if (straight_machine[i] == FALSE) {
					newly_straight = FALSE;
				}
			}
			
			if (newly_straight) {
				// If newly straight, don't go full bore
				driveMotors_setSpeed(STANDARD_STRAIGHT_SPEED);
			} else {
				// If we've been straight for a bit. SEND IT!
				driveMotors_setSpeed(ROCKET_STRAIGHT_SPEED);
				
			}
			
			break;
		case(1): // Turn Right
			// Negative error
		straight_machine[0] = FALSE;
			error[0] = tolerance_left - current_leftmost;
			break;
		case(2): // Turn Left
			// Positive error
		 straight_machine[0] = FALSE;
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
	
 correction = get_PID(correction); // Cacluate correction using PID

	// Verify the correction does not exceed the servo limits
	// If it does, the correction will be clipped
 correction = verify_limit(correction);
	
	if (print_direction) { // Verbose direction
		sprintf(str, "dir=%d", dir);
		put(str);
		sprintf(str, " e[0]=%f", error[0]);
		put(str);
		sprintf(str, " c=%f\r\n", correction);
		put(str);
	}
	servo_move(correction);
}
