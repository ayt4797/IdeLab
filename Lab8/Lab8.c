/*
* Rochester Institute of Technology
* Department of Computer Engineering
* CMPE 460  Interfacing Digital Electronics
* LJBeato
* 1/14/2021
*
* Filename: main_timer_template.c
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
#define MAXOUT 16383.0
// The sprintf function seemed to cause a hange in the interrupt service routine.
// I think if we increase the HEAP size, it will work
// change to Heap_Size       EQU     0x00000200 in startup_msp432p401r_uvision.s

extern uint32_t SystemCoreClock;

BOOLEAN Timer1RunningFlag = FALSE;
BOOLEAN Timer2RunningFlag = FALSE;

unsigned long MillisecondCounter = 0;
BOOLEAN flag = FALSE;
double lastPulse=0;
int periodPassed(int START, int analogin){
	if(START==analogin){//a tolerance may be needed
		return 1;
	}
	return 0;
	
}
// Interrupt Service Routine for Timer32-1
void print_data(void){
		char temp[64]; //purely for printing data		
		unsigned int analogIn = ADC_In(); //data recieved
		
		int const START =analogIn;
		
		int n=sprintf(temp,"\r\n data is %i\r\n", analogIn); //the unsigned analog value
		uart0_put(temp);//printing that data for debug purpses
		
		double voltageOut = 3.3*((double)analogIn/MAXOUT);
		int y = sprintf(temp,"voltage %lf",voltageOut);
			uart0_put(temp);
		int x = sprintf(temp,"totalTime: %lu",MillisecondCounter);
		uart0_put(temp);
		if(periodPassed(START,analogIn)){
			double period_ms = MillisecondCounter-lastPulse;
			double period_s= period_ms*1000;
			double freq = 1/(period_s);
			double bpm= (freq)*60;
			lastPulse=MillisecondCounter;
			x = sprintf(temp,"heart rate is  %lf bpm",bpm);
			uart0_put(temp);
		}
		flag = TRUE;
		P1->OUT=0;

}


void Timer32_1_ISR(void)
{
	uart0_put("\r\nDebug Statement");
	if(!flag){
		P1->OUT=BIT0;
		print_data();
	}
	flag = FALSE;

}

//
// Interrupt Service Routine
//
//
//
void Timer32_2_ISR(void)
{

		//MillisecondCounter++;

}

// main
int main(void)
{
	//initializations
	uart0_init();
	uart0_put("\r\nLab5 ADC demo\r\n");

	LED1_Init();
	LED2_Init();
	Timer32_1_Init(&Timer32_1_ISR, SystemCoreClock/2, T32DIV1); // initialize Timer A32-1;
	//Timer32_1_Init(&Timer32_1_ISR, SystemCoreClock/2, T32DIV1); // initialize Timer A32-1;
        
	// Setup Timer32-2 with a .001 second timeout.
	// So use DEFAULT_CLOCK_SPEED/(1/0.001) = SystemCoreClock/1000
	//Timer32_2_Init(&Timer32_2_ISR, SystemCoreClock/1000, T32DIV1); // initialize Timer A32-1;
	TIMER32_CONTROL1 |= BIT7;
	NVIC_EnableIRQ(T32_INT1_IRQn); // Enable IRQ
	TIMER32_CONTROL2 |= BIT7; // stop TIMER 2
	Switch1_Init();
	Switch2_Init();
	ADC0_InitSWTriggerCh6();
	EnableInterrupts();

  while(1)
	{
		WaitForInterrupt();
		
  }
}

