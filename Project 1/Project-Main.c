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
#include "PID.h"

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
BOOLEAN oled_wave;
char phone_input[2];
int phone_count =0;	
double gain = 16.0f;

short speed_profile = 0;
// 0 - Super Wicked Fast
// 1 - Moderate
// 2 - Slow and Steady
short speed_profile_length = 3;

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
	OLED_Print(1, 1, "Initalizing...");
	
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
	
	OLED_Print(1,1,"Right         ");
	servo_right();
	ms_delay(400);
	
	OLED_Print(1,1,"Left          ");
	servo_left();
	ms_delay(400);
	
	OLED_Print(1,1,"Straight");
	servo_center();
	
}

void mode_select() {
	OLED_display_on();
	OLED_Print(1, 1, "Select Mode!");
	sprintf(str,"Config:%i\r\n",speed_profile);
	put(str);
	OLED_Print(2, 2, str);
	while (Switch2_Pressed() == FALSE) {
		if (Switch1_Pressed()) {
			ms_delay(700);
			if (speed_profile == speed_profile_length) {
				speed_profile = 0;
			} else {
				speed_profile++;
			}
			sprintf(str,"Config:%i\r\n",speed_profile);
			put(str);
			OLED_Print(2, 2, str);
		}
	}
	OLED_Print(1, 1, "Mode Selected!");
}

/////////////////////////////////////////////////////
// main function
/////////////////////////////////////////////////////
int main(void)
{
	// Generic Initializations
	i = 0;
	j = 0;
	printCameraOutput = FALSE; // Show Camera Output on Terminal
	// OLED_Output - Show Camera Values on OLED Display
	// 0 - Analog unfilter data
	// 1 - Smooth filtered data
	// 2 - Binarized Data (1/0)
	OLED_Output = 0;
	oled_wave = TRUE;
	
	// Preform generic initalizations
	car_startup();
	camera_calibration();
	
	// Select Race Mode. Profiles need to be filled out.
	put("Select Mode\r\n");
	mode_select();
	
	put("Press Switch1 to calibrate center\r\n");
	OLED_Print(1, 1, "Press SW1       ");
	OLED_Print(3,3, "Allign Center");
	while (Switch1_Pressed() == 0) {}; // Determine the center point of the track on a straight portion.
	OLED_Print(3,3, "                 ");
	while (g_sendData == FALSE) {}; // wait until data is able to be parsed.
	parsedata();
	calibrate_center();
	g_sendData = FALSE;
		
	put("Use Switch 1 to index through the OLED modes\r\n");
	put("Use Switch 2 for Start/E-Stop!\r\n");
	OLED_Print(1,1, "Calibrating...    ");
		ms_delay(1000);
	
	while (Switch2_Pressed() != 0) {}; // Use a button to wait to drive the car
	ms_delay(1000);		
	P2->OUT &= ~BIT0; // No Light
	P2->OUT &= ~BIT1;
	P2->OUT &= ~BIT2;
	driveMotors_setSpeed(20); // 5% forward
	put("Oh boy! Time to drive!\r\n");
//	OLED_DisplayCameraData(line);
	OLED_Print(1,1,"Starting Drive!");
	while(1)
	{
		//get_gain();
		//continue;
		if (g_sendData == TRUE) {
			if (printCameraOutput) {
			for (i=0; i<127; i++) {
					if (binline[i] == 1) {
						uart0_putchar('O');
					} else {
						uart0_putchar('-');
				}
			}
		}
		if (oled_wave) {
			OLED_Camera_Debug(OLED_Output);
		}
	} 
		
		parsedata(); // Binary Edge Detection
		//

		if (oled_wave) {
			if (Switch1_Pressed()) {
				if (OLED_Output < 2) {
					OLED_Output++;
				} else {
					OLED_Output = 0;
				}
				ms_delay(1000);
			}
		}
		g_sendData = FALSE; // Ready for next signal.
		//
		
		if (Switch2_Pressed() || isOffTrack()) {
			if (oled_wave) {
				OLED_display_clear();
				OLED_display_off();
			}
			driveMotors_stop();
			break;
		}

		// Steering adjustment. Based on how far out the wheels are. Adjust left or right
		//old_steering_adjust();
		steering_adjust();
	}
	//sprintf(str,"gain: %f, phone_gain : %f\n",gain,phone_gain);
	//put(str);

}
