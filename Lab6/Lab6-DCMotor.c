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

void LED2_Init(void)
{
// configure PortPin for LED2 as port I/O
P2->SEL0 &= ~BIT0;
P2->DIR |=BIT0;
P2->DS &= BIT0;
P2->OUT &= ~BIT0;
P2->SEL0 &= ~BIT1;
P2->DIR |=BIT1;
P2->DS &= BIT1;
P2->OUT &= ~BIT1;
P2->SEL0 &= ~BIT2;
P2->DIR |=BIT2;
P2->DS &= BIT2;
P2->OUT &= ~BIT2;
P2->OUT &= ~(BIT0|BIT1|BIT2);
// make built-in LED2 LEDs high drive strength
// make built-in LED2 out
// turn off LED
}

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
	LED2_Init();
	uart0_put("Running... \n\r");
	TIMER_A0_PWM_Init(SystemCoreClock/20000, .2, 1);
	TIMER_A0_PWM_Init(SystemCoreClock/20000, 0.2, 2);
	TIMER_A0_PWM_Init(SystemCoreClock/20000, 0.2, 3);
	TIMER_A0_PWM_Init(SystemCoreClock/20000, 0.2, 4);
	
	TIMER_A2_PWM_Init(SystemCoreClock/(50*8*2), 0.1, 1);
	
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

	// Part 1 - UNCOMMENT THIS
	// Generate 20% duty cycle at 10kHz
	
	//Part 2 - UNCOMMENT THIS
	for(;;)  //loop forever
	{
		//uint16_t dc = 0;
		//uint16_t freq = 10000; // Frequency = 10 kHz 
		//uint16_t dir = 0;
		//char c = 48;
		int i=0;
		
		TIMER_A2_PWM_DutyCycle(.0625,1);//right
		delay1(1000);
		TIMER_A2_PWM_DutyCycle(.075,1);//center
		delay1(1000);
		TIMER_A2_PWM_DutyCycle(.0875,1); //left
		delay1(1000);	
		TIMER_A2_PWM_DutyCycle(.075,1);//center
		delay1(1000);
		
		// 0 to 100% duty cycle in forward direction
		P2->OUT |= BIT0; // RED
		P2->OUT &= ~BIT1;
		P2->OUT &= ~BIT2;
		for (i=0; i<100; i++) {
		    // RAMP UP CL BOTTOM RIGHT WHEEL
			double d = ((double)i)/100;
			TIMER_A0_PWM_DutyCycle(d,4);

			delay1(100);
		}
		
		// 100% down to 0% duty cycle in the forward direction
		P2->OUT &= ~BIT0; // GREEN
		P2->OUT |= BIT1;
		P2->OUT &= ~BIT2;
		for (i=100; i>=0; i--) {
		    // RAMP DOWN CL BOTTOM RIGHT WHEEL
						double d = ((double)i)/100;
			TIMER_A0_PWM_DutyCycle(d,4);
			delay1(100);
		}
		
		// 0 to 100% duty cycle in reverse direction
		P2->OUT &= ~BIT0; // BLUE
		P2->OUT &= ~BIT1;
		P2->OUT |= BIT2;
		for (i=0; i<100; i++) {
		    // RAMP UP CCL BOTTOM RIGHT WHEEL
						double d = ((double)i)/100;
			TIMER_A0_PWM_DutyCycle(d,3);
			delay1(100);
		}
		
		// 100% down to 0% duty cycle in the reverse direction
		P2->OUT |= BIT0; // Whites
		P2->OUT |= BIT1;
		P2->OUT |= BIT2;
		for (i=100; i>=0; i--) {
		    // RAMP DOWN CCL BOTTOM RIGHT WHEEL
						double d = ((double)i)/100;
			TIMER_A0_PWM_DutyCycle(d,3);
			delay1(100);
		}
		
		// 0 to 100% duty cycle in forward direction
		P2->OUT |= BIT0; // Magenta
			P2->OUT |= BIT1;
			P2->OUT &= ~BIT2;
		for (i=0; i<100; i++) {
		    // RAMP UP CCL BOTTOM LEFT WHEEL
			double d = ((double)i)/100;
			TIMER_A0_PWM_DutyCycle(d,2);

			delay1(100);
		}
		
		// 100% down to 0% duty cycle in the forward direction
		P2->OUT |= BIT0; // Yellow
			P2->OUT &= ~BIT1;
			P2->OUT |= BIT2;
		for (i=100; i>=0; i--) {
		    // RAMP DOWN CCL BOTTOM LEFT WHEEL
						double d = ((double)i)/100;
			TIMER_A0_PWM_DutyCycle(d,2);
			delay1(100);
		}
		
		// 0 to 100% duty cycle in reverse direction
		P2->OUT &= ~BIT0; // Cyan
			P2->OUT |= BIT1;
			P2->OUT |= BIT2;
		for (i=0; i<100; i++) {
		    // RAMP UP CL BOTTOM LEFT WHEEL
						double d = ((double)i)/100;
			TIMER_A0_PWM_DutyCycle(d,1);
			delay1(100);
		}
		
		// 100% down to 0% duty cycle in the reverse direction
		P2->OUT &= ~BIT0; // No Light
		P2->OUT &= ~BIT1;
		P2->OUT &= ~BIT2;
		for (i=100; i>=0; i--) {
		    // RAMP DOWN CL BOTTOM LEFT WHEEL
						double d = ((double)i)/100;
			TIMER_A0_PWM_DutyCycle(d,1);
			delay1(100);
		}
	}
}
