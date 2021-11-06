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

// line stores the current array of camera data
extern uint16_t line[128];
uint16_t smoothline[128];
uint16_t binline[128];
extern BOOLEAN g_sendData;
char str[100];
int parseMode;
extern int i;
int j;
short OLED_Output;
BOOLEAN debugReporting = FALSE;

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
void plotline(uint16_t line[128]) {
	OLED_DisplayCameraData(line); // Hand off to oled.c code
}

/*
 Preform edge detection on smoothed camera results
*/
void edge_detection(void) {
	int maxval = 0;
	double thresehold = .50; // Thereshold for binary high value (percentage of maxval)
	// Determine the max value found by the camera
	for (i=0; i<128; i++) {
		if (smoothline[i] > maxval) {
			maxval = smoothline[i];
		}
	}
	// Generate binary result.
	for (i=0; i<128; i++) {
		if (smoothline[i] > maxval*thresehold) {
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
			total = line[i];
			for (j=smooth_precision; j > 0; j--) {
				total = total + line[i+j] + line[i-j];
			}
			// With the sum of the index and the near by indicies.
			// The smoothed value is the average of the sum.
			// (2*smooth_precision)+1 is the number of points sampled
			smoothline[i] = total/((2*smooth_precision)+1);
		}
	} // end smoothline for loop
	edge_detection();
}
