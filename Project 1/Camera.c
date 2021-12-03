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
#include "Lab1.h"

// line stores the current array of camera data
extern uint16_t line[128];
uint16_t smoothline[128];
uint16_t binline[128];
int center_leftlimit = 0;
int center_rightlimit = 127;
extern BOOLEAN g_sendData;
char str[100];
int parseMode;
extern int i;
int j;
double absolute_white = 0;
double absolute_dark = 0;

///////////////////////////////////////////////////////
//
// NOTE: For the camera, you may want to change the default
//       clock to 48MHz
//
// To do that: Edit system_msp432p401r.c
//             Change:   #define  __SYSTEM_CLOCK    3000000
//             To:       #define  __SYSTEM_CLOCK    48000000 
// ADC will be P4.7 A6
//
// SI Pin will be P5.5 A0
//
// CLK Pin will be P5.4 A1//
//

/**
 * Waits for a delay (in milliseconds)
 * 
 * del - The delay in milliseconds
 */
void ms_delay(int del){
	volatile int i;
	for (i=0; i<del*(48000/6); i++){
		;// Do nothing
	}
}

void bl_put(char *temp){ //prints to both putty & phone
    uart0_put(temp);
    uart2_put(temp);
}


/////////////////////////////////////////////////////
//
// INIT_Camera function
//
//
/////////////////////////////////////////////////////
void INIT_Camera(void)
{
	g_sendData = FALSE;
	ControlPin_SI_Init();
	ControlPin_CLK_Init();
	ADC0_InitSWTriggerCh6();
}

/*
	Plot given line onto the OLED
*/
void plotline(uint16_t in_line[128]) {
	OLED_DisplayCameraData(in_line); // Hand off to oled.c code
}

void bin_plotline(uint16_t in_line[128]) {
	uint16_t temp[128];
	for (i=0; i<128; i++) {
			if (in_line[i] == 1) {
				temp[i] = 15000;
			} else {
				temp[i] = 2000;
			}
	}
	OLED_DisplayCameraData(temp); // Hand off to oled.c code
}

/*
 Preform edge detection on smoothed camera results
*/
void edge_detection(void) {
	int maxval = absolute_white;
	int minval = absolute_dark;
	double thresehold = 1.5; // Thereshold for binary high value (percentage of maxval) // was 3
	// Generate binary result.
	for (i=0; i<128; i++) {
		if (smoothline[i] > ((maxval-minval)/2)+(minval*thresehold)) {
			binline[i] = 1;
		} else {
			binline[i] = 0;
		}
	}
}

/*
	Smooth camera results to remove unaccurate values
*/
void parsedata(void) { // Translated from MATLAB
	short smooth_precision = 2;
	int total;
	// For each value from the camera, avergiate it with the next #x points
	for (i=0; i<128; i++) {
		// If there are not enough points to smooth around. Skip
		 if ((i < smooth_precision) || (i > 127-smooth_precision)) {
			smoothline[i] = line[i];
		} else {
		// There are enough points to smooth. Smooth the response accordingly
			short validPoints = 0;
			if (line[i] != 0) { // If the current point is valid
				total = line[i];
				validPoints++;
			} else { // Current point is invalid
				total = 0;
			}
			for (j=smooth_precision; j > 0; j--) {
					total = total + line[i+j] + line[i-j];
			}
			// With the sum of the index and the near by indicies.
			// The smoothed value is the average of the sum.
			// (2*smooth_precision)+1 is the number of points sampled
			smoothline[i] = total/(2*smooth_precision)+1;
		}
	} // end smoothline for loop
	edge_detection();
}

// If the camera is upside down, the values will look werid.
// Flip the values across the y-axis (center most value).
void cameraUpsidedown (uint16_t line[128]) {
	// Inverse line scan data. (Camera is upside down)
		uint16_t inversed_line[128];
		for (i = 0; i < 128; i++) {
			inversed_line[i] = line[127-i];
		}
		for (i = 0; i < 128; i++) {
			line[i] = inversed_line[i]; // Correct original value
		}
}

void camera_calibration() {
		LED2_White();
//		uart0_put("\r\nCalibrate White - Press Switch1 to record\r\n");
//		while(Switch1_Pressed() == 0) {} // Wait for capture
//		// Get started by getting the whitemost value
//		double acc = 0;
//		for (i = 0; i<128; i++) {
//			acc+= line[i];
//		}
//		absolute_white = acc/128;
//		sprintf(str,"Absolute White value is =%lf\r\n",absolute_white);
//		uart0_put(str);	
//		
//		acc = 0;
//		LED2_Blue();
//		uart0_put("Calibrate Dark - Press Switch2 to record\r\n");
//		while(Switch2_Pressed() == 0) {} // Wait for capture
//		// Now get the darkest value
//		for (i = 0; i<128; i++) {
//			acc+= line[i];
//		}
//		absolute_dark = acc/128;
//		sprintf(str,"Absolute Dark value is =%lf\r\n",absolute_dark);
//		uart0_put(str);
		absolute_white = line[0];
		absolute_dark = line[0];
		for (i=0; i<128; i++) {
			if (line[i] > absolute_white) {
				absolute_white = line[i];
			}
			if (line[i] < absolute_dark) {
				absolute_dark = line[i];
			}
		}
		//	sprintf(str,"Absolute Dark value is =%lf\r\n",absolute_dark);
			//uart0_put(str);
		 // uart2_put(str);
//			sprintf(str,"Absolute White value is =%lf\r\n",absolute_white);
			//uart0_put(str);	
			//uart2_put(str);
			
		
		LED2_Off();
}

void calibrate_center() {
	for (i=0;i<128;i++) {
		if (binline[i] > 0) { // If the left most value has been identified
			center_leftlimit = i;
			break;
		}
	}
//	sprintf(str,"Left most for Center =%d\r\n",center_leftlimit);
	//bl_put(str);
	for (i=127; i>0; i--) {
		if (binline[i] > 0) { // If the left most value has been identified
			center_rightlimit = i;
			break;
		}
	}
//	sprintf(str,"Right most for Center =%d\r\n",center_rightlimit);
	//bl_put(str);
}

BOOLEAN isOffTrack() {
	int offTrack_Thrsehold = 30; // Minimum number of points to be considered on track. -- Was 30
	int acc = 0;
	for (i=0; i<128; i++) {
		acc = acc + binline[i];
	}
	if (acc < offTrack_Thrsehold) {
		return TRUE;
	} else {
		return FALSE;
	}
	
}
