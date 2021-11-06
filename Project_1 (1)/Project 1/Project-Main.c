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

// line stores the current array of camera data
extern uint16_t line[128];
uint16_t inversed_line[128];
extern uint16_t smoothline[128];
extern uint16_t binline[128];
extern BOOLEAN g_sendData;
static char str[100];
extern int parseMode;
extern int i;
extern int j;
extern short OLED_Output;
extern BOOLEAN debugReporting;
BOOLEAN useOled = 1;
/////////////////////////////////////////////////////
// simple delay function
/////////////////////////////////////////////////////
void myDelay(void)
{
	volatile int j = 0;
	for (j = 0; j < 800000; j++)
	{
		;
	}
}

/////////////////////////////////////////////////////
//
// main function
//
//
/////////////////////////////////////////////////////
void put(char* temp){ //prints to both putty & phone
	uart0_put(temp);
	uart2_put(temp);
}
int main(void)
{
	//initializations
	i = 0;
	j = 0;
	debugReporting = FALSE;
	DisableInterrupts();
	uart0_init();
	uart2_init();
	uart0_put("\r\nLab5 CAMERA demo\r\n");
	uart0_put("\r\nINIT LEDs");
	LED1_Init();
	LED2_Init();
  uart0_put("- ENABLED\r\n");
  uart0_put("\r\nOLED DISPLAY");
	OLED_Init();
  uart0_put("- ENABLED\r\n");
	// remember that we double the desired frequency because we need to account
	// for the toggle from hi to low.
	//
	//Output_MCLK();
	
	// uart0_put("\r\nINIT ADC\r\n");	
	uart0_put("\r\nINIT Switch 2");
	Switch2_Init();
	uart0_put("\r\nEnable Interrupts\r\n");
	EnableInterrupts();
  uart0_put("- ENABLED\r\n");
	//	EnableSysTickTimer();
	// Select type of camera result to display on OLED
  uart0_put("\r\nINIT CAMERA");
	INIT_Camera();
  uart0_put("- ENABLED\r\n");
	while(1)
	{
		// Inverse line scan data. (Camera is upside down)
		for (i = 0; i < 128; i++) {
			inversed_line[i] = line[127-i];
		}
		for (i = 0; i < 128; i++) {
			line[i] = inversed_line[i]; // Correct original value
		}
		
		if (g_sendData == TRUE) 
		{
			LED1_On(); // LED ON = DATA TRANSFER
			// send the array over uart
			sprintf(str,"%i\n\r",-1); // start value
			uart0_put(str);
			
			if (debugReporting) { // Analog UART Reporting
				for (i = 0; i < 128; i++) 
				{
					sprintf(str,"%i\n\r", line[i]);
					uart0_put(str);
				}
			}
			
			parsedata(); // Binary Edge Detection
			
			switch(2) {
				case 0:
					plotline(line);
					break;
				case 1:
					plotline(smoothline);
					break;
				case 2:
					// adjust binline for camera view.
					// '0' at height 10 pixels
				  // '1' at height 100 pixels
					for (i=0; i<128; i++) {
						if (binline[i] == 1) {
							binline[i] = 15000*binline[i];
						} else {
							binline[i] = 2000;
						}
						
					}
					plotline(binline);
					break;
				default:
					break;
			}
			    // Debugging
			if(useOled==1){
				OLED_Print(1, 1, "press left button for green");
				sprintf(str,"%i\n\r",-2); // end value
				uart0_put(str);
				OLED_Print(2, 2, "press right button for red");
				//OLED_DisplayCameraData(line);
			}
			if(useBluetooth==1){
				uart2_put("debug");
				
			}
			LED1_Off();
			g_sendData = FALSE; // LED OFF = DATA TRANSFER COMPLETE
		}
		// do a small delay
		//myDelay();
	}
}
