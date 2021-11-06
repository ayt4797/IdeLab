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

void steering_adjust() {
	int current_leftmost = 0;
	int current_rightmost = 127;
	double error = 0; // Error in P control
	double kp = 0; // Gain of proportional control
	short right_gain = 6; // Difference in error achieves max response at this value
	short left_gain = 6;
	double correction = servo_state_center; // By default
	for (i=0;i<128;i++) {
		if (binline[i] > 0) { // If the left most value has been identified
			current_leftmost = i;
			break;
		}
	}
	for (i=127; i>0; i--) {
		if (binline[i] > 0) { // If the left most value has been identified
			current_rightmost = i;
			break;
		}
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
	}
	
	servo_move(correction);
	sprintf(str,"Error=%lf\n\r",error);
	put(str);
	sprintf(str,"kp=%lf\n\r",kp);
	put(str);
	sprintf(str,"Correction=%lf\n\r",correction);
	put(str);
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
	sprintf(str,"%i\n\r",-2); // end value
	put(str);
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
		
		// Fun Math Here to stop car
		
		
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
