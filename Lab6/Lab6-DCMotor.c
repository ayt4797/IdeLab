/*
 * Main module for testing the PWM Code for the K64F
 * 
 * Author:  
 * Created:  
 * Modified: Carson Clarke-Magrab <ctc7359@rit.edu> 
 * LJBeato
 * 2021
 */

#include "msp.h"
#include "uart.h"
#include "TimerA.h"

/**
 * Waits for a delay (in milliseconds)
 * 
 * del - The delay in milliseconds
 */
void delay1(int del){
	volatile int i;
	for (i=0; i<del*50; i++){
		;// Do nothing
	}
}

int main(void) {
	// Initialize UART and PWM
	// INSERT CODE HERE

	// Print welcome over serial
	uart0_init();
	uart0_put("Running... \n\r");
	TIMER_A0_PWM_Init(SystemCoreClock/20000, .2, 1);
	TIMER_A0_PWM_Init(SystemCoreClock/20000, 0.2, 2);
	TIMER_A0_PWM_Init(SystemCoreClock/20000, 0.2, 3);
	TIMER_A0_PWM_Init(SystemCoreClock/20000, 0.2, 4);
	
	TIMER_A2_PWM_Init(SystemCoreClock/(50*8*2), 0.1, 1);
	for(;;){
			TIMER_A2_PWM_DutyCycle(.05,1);
		delay1(10);
			TIMER_A2_PWM_DutyCycle(.075,1);
		delay1(10);
			TIMER_A2_PWM_DutyCycle(.01,1);
		delay1(10);
				TIMER_A2_PWM_DutyCycle(.075,1);
		delay1(10);
		
	}
	// Part 1 - UNCOMMENT THIS
	// Generate 20% duty cycle at 10kHz
	
	//Part 2 - UNCOMMENT THIS
	for(;;)  //loop forever
	{
		uint16_t dc = 0;
		uint16_t freq = 10000; // Frequency = 10 kHz 
		uint16_t dir = 0;
		char c = 48;
		int i=0;
		
		// 0 to 100% duty cycle in forward direction
		for (i=0; i<100; i++) {
		    // INSERT CODE HERE
			double d = ((double)i)/100;
			TIMER_A0_PWM_DutyCycle(d,4);

			delay1(100);
		}
		
		// 100% down to 0% duty cycle in the forward direction
		for (i=100; i>=0; i--) {
		    // INSERT CODE HERE
						double d = ((double)i)/100;
			TIMER_A0_PWM_DutyCycle(d,4);

			delay1(100);
		}
		
		// 0 to 100% duty cycle in reverse direction
		for (i=0; i<100; i++) {
		    // INSERT CODE HERE
						double d = ((double)i)/100;
			TIMER_A0_PWM_DutyCycle(d,1);

			delay1(100);
		}
		
		// 100% down to 0% duty cycle in the reverse direction
		for (i=100; i>=0; i--) {
		    // INSERT CODE HERE
						double d = ((double)i)/100;
			TIMER_A0_PWM_DutyCycle(d,1);

			delay1(100);
		}

	}
	return 0;
}



