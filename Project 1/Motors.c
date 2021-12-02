#include "msp.h"
#include "uart.h"
#include "TimerA.h"

double servo_state_center = 0.0725; 
double servo_limit_left = 0.1; // 0.085
double servo_limit_right= 0.0475; //0.045
short DC_right_forward = 4;
short DC_right_reverse = 3;
short DC_left_forward = 2;
short DC_left_reverse = 1;

void driveMotors_stop() {
	TIMER_A0_PWM_DutyCycle(0,DC_left_reverse);
	TIMER_A0_PWM_DutyCycle(0,DC_left_forward);
	TIMER_A0_PWM_DutyCycle(0,DC_right_reverse);
	TIMER_A0_PWM_DutyCycle(0,DC_right_forward);
}

// Init All DC Motors
void driveMotors_init() {
	TIMER_A0_PWM_Init(SystemCoreClock/20000, 0.2, DC_left_reverse);
	TIMER_A0_PWM_Init(SystemCoreClock/20000, 0.2, DC_left_forward);
	TIMER_A0_PWM_Init(SystemCoreClock/20000, 0.2, DC_right_reverse);
	TIMER_A0_PWM_Init(SystemCoreClock/20000, 0.2, DC_right_forward);
	driveMotors_stop();
}

// Center servo steering
void servo_center() {
	TIMER_A2_PWM_DutyCycle(servo_state_center,1); // Center Servo
}

void servo_right() {
	TIMER_A2_PWM_DutyCycle(servo_limit_right,1); // Center Servo
}

void servo_left() {
	TIMER_A2_PWM_DutyCycle(servo_limit_left,1); // Center Servo
}

void servo_move(double posisition) {
	TIMER_A2_PWM_DutyCycle(posisition,1); // Center Servo
}
// Set motor speed - from a percentage. 
// 1 is 100%, 0 is 0%
void driveMotors_setSpeed(double percentage) {
	if (percentage > 0) {
		TIMER_A0_PWM_DutyCycle(percentage/100,DC_right_forward);
		TIMER_A0_PWM_DutyCycle(percentage/100,DC_left_forward);
		TIMER_A0_PWM_DutyCycle(0,DC_left_reverse);
		TIMER_A0_PWM_DutyCycle(0,DC_right_reverse);
	} else if (percentage < 0){
		TIMER_A0_PWM_DutyCycle(percentage/100,DC_left_reverse);
		TIMER_A0_PWM_DutyCycle(percentage/100,DC_right_reverse);
		TIMER_A0_PWM_DutyCycle(0,DC_left_forward);
		TIMER_A0_PWM_DutyCycle(0,DC_right_forward);
	} else {
		driveMotors_stop();
	}
}

void driveMotors_brake(double brake_per) {
		TIMER_A0_PWM_DutyCycle(brake_per/100,DC_left_reverse);
		TIMER_A0_PWM_DutyCycle(brake_per/100,DC_right_reverse);
		TIMER_A0_PWM_DutyCycle(0,DC_left_forward);
		TIMER_A0_PWM_DutyCycle(0,DC_right_forward);
}

void driveMotors_forwardRight(double percentage) {
	TIMER_A0_PWM_DutyCycle(percentage/100,DC_right_forward);
}

void driveMotors_forwardLeft(double percentage) {
	TIMER_A0_PWM_DutyCycle(percentage/100,DC_left_forward);
}
