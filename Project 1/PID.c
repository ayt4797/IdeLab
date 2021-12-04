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
#define TOLERANCE_FACTOR 0
#define STANDARD_STRAIGHT_SPEED 25
#define MAX_SPEED 30
#define SPEED_GAIN .01
#define TURN_SPEED 10
#define MOTOR_FACTOR 35

short current_leftmost;
short current_rightmost;
short tolerance_right;
short tolerance_left;
short dir;
double error[3]; // [0] - Current error // [1] - Last Error // [2] - 2nd latest error ... and so on
double correction=.0725;
BOOLEAN print_direction = FALSE;
BOOLEAN PID_differential = FALSE;
BOOLEAN print_straight_machine = FALSE;
double kp_left= 0.0525/11;
double kp_right= 0.0525/11;
// double kp = ;
double ki = 0; //0.0525/(26*0.25);
double kd = 0;

int brake_time= 6; //200;
int brake_required = 0; // Length of how many cycles to break for.
double speed= STANDARD_STRAIGHT_SPEED;

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
	current_leftmost = get_current_leftmost();
	current_rightmost = get_current_rightmost();
	
	// Case 1.
	if ((current_leftmost < tolerance_left) && (current_rightmost) > tolerance_right) {
		return 0; // Straight
	}
	// Case 2
	else if (current_leftmost >= tolerance_left) {
		return 1; // Turn Right
	} 
	// Case 3
	
	else if (current_rightmost <= tolerance_right) {
		return 2; // Turn Left
	} 
	// Error Case
		put("Error! Direction not found\r\n");
		return 3;
}

double verify_limit(double c, int dir) {
	
		if (c > servo_limit_left) {
			c = servo_limit_left;
		} else if (c < servo_limit_right) {
			c = servo_limit_right;
		}
	
	switch(dir) {
		case 0: // Straight
			return c;
		case 1: // Right
			if (c <= 0.0725)
				return c;
			else
				return 0.0725;
		case 2: // Left
			if (c >= 0.0725)
				return c;
			else
				return 0.0725;
		default:
			return c;
	}
}

float get_PID(float prev_pos, BOOLEAN left){
	float new_pos;
	if(left){
    new_pos 
			 = prev_pos
			+(kp_left*(error[0]-error[1])) 
		  + ki*((error[0]-error[1])/2)
		  + kd*(error[0]-2*error[1]+error[2]);
		
		// Shift errors to load newest error into error[0]
    error[2] = error[1];
    error[1] = error[0];
	}
	else{
		new_pos 
			 = prev_pos
			+(kp_right*(error[0]-error[1])) 
		  + ki*((error[0]-error[1])/2)
		  + kd*(error[0]-2*error[1]+error[2]);
		
		// Shift errors to load newest error into error[0]
    error[2] = error[1];
    error[1] = error[0];
	}
    return new_pos;
}
double update_speed(double prev_speed){
	if(prev_speed>=MAX_SPEED)
		return prev_speed;
	return (MAX_SPEED-prev_speed)*SPEED_GAIN+prev_speed;;
}
void steering_adjust() {
	// New method using 3 basic cases
	
	//tolerance_right = center_rightlimit+3;
	tolerance_left = TOLERANCE_FACTOR + center_leftlimit;
	tolerance_right = center_rightlimit - TOLERANCE_FACTOR;
	dir = steering_direction(tolerance_left, tolerance_right);
	// Calculate error - e(t)
	switch(dir) {
		case(0): // Straight!
			speed = update_speed(speed);
		;
			put("S");
			break;
		case(1): // Turn Right
			// Negative error
			put("R");
			error[0] = tolerance_left - current_leftmost;
			break;
		case(2): // Turn Left
			// Positive error
		put("L");
			error[0] = tolerance_right - current_rightmost;
			break;
		case(3): // Error case. Something werid is going on.
			error[0] = 0;
			 break;
		default:
			 put("dir out of range");
			 error[0] = 0;
			 break;
	}
	
	if(dir!=0)
		correction = get_PID(correction,0); // Cacluate correction using PID
	else
		correction = .0725;

	// Verify the correction does not exceed the servo limits
	// If it does, the correction will be clipped
 correction = verify_limit(correction, dir);
	
	if (print_direction) { // Verbose direction
		sprintf(str, "dir=%d", dir);
		put(str);
		sprintf(str, " e[0]=%f", error[0]);
		put(str);
		sprintf(str, " c=%f\r\n", correction);
		put(str);
	}
	servo_move(correction);
	
	if (brake_required == 0) {
		brake_required = brake_required - 1;
		driveMotors_stop();
	}
	
	if (brake_required > 0) {
		put("-B\r\n");
		brake_required--;
		driveMotors_brake(100);
	} else {
		switch(dir) {
			case(0): // Straight!
				// If we've been straight longer than the thresehold, full speed!
				driveMotors_setSpeed(speed);

				break;
			case(1): // Turn Right
				if (speed==(MAX_SPEED-1)) {
					// brake(brake_time,dir);
					brake_required = brake_time;

				}
					speed= STANDARD_STRAIGHT_SPEED;

				if (PID_differential == FALSE) {
					driveMotors_forwardLeft(TURN_SPEED + MOTOR_FACTOR);
					driveMotors_forwardRight(TURN_SPEED - MOTOR_FACTOR);
				}
				break;
			case(2): // Turn Left
				if (speed==(MAX_SPEED-1)) {
					// brake(brake_time, dir);
					brake_required = brake_time;
				}
					speed= STANDARD_STRAIGHT_SPEED;

				if (PID_differential == FALSE) {

					driveMotors_forwardLeft(TURN_SPEED - MOTOR_FACTOR);
					driveMotors_forwardRight(TURN_SPEED + MOTOR_FACTOR);
				}
				break;
			case(3): // Error case. Something werid is going on.
				 break;
			default:
				 break;
		}
	}
}
