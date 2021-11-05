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
int start =-1;
unsigned long MillisecondCounter = 0;
BOOLEAN flag = FALSE;
int tolerance = 3;
double lastPulse=0;
int state=0;
int prevAnalog=0;
int peak=0;
int periodPassed(int analogin, char* temp){
	if(state==0 && analogin>prevAnalog){
			state=1;
	}
	if(state==1 && analogin<prevAnalog){
		peak = prevAnalog;
		state=2;
	}
	//int y = sprintf(temp,"state %d\r\n",state);
	//uart0_put(temp);
	prevAnalog=analogin;
		if(state==2){
			state=0;
			return 1;
		}
		else return 0;
}
// Interrupt Service Routine for Timer32-1
void print_data(void){
		char temp[64]; //purely for printing data		
		unsigned int analogIn = ADC_In(); //data recieved
		
		if(start==-1){

				start =analogIn;
	}
		//int n=sprintf(temp,"\r\n data is %i\r\n", analogIn); //the unsigned analog value
		//uart0_put(temp);//printing that data for debug purpses
		
		//double voltageOut = 3.3*((double)analogIn/MAXOUT);
		//int x = sprintf(temp,"totalTime: %lu",MillisecondCounter);
		uart0_put(temp);
		if(periodPassed(analogIn,temp)){
			double period_ms =(MillisecondCounter-lastPulse);
			double period_s= period_ms/1000.0;
			double freq = 1.0/period_s;
			double bpm= freq*60;
			int y = sprintf(temp,"freq %lf\r\n",freq);
			uart0_put(temp);
			lastPulse=MillisecondCounter;
			int x = sprintf(temp,"heart rate is  %lf bpm\r\n",bpm);
			uart0_put(temp);
		}
		flag = TRUE;
		P1->OUT=0;

}


void Timer32_1_ISR(void)
{
	if(!flag){
		P1->OUT=BIT0;
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

		MillisecondCounter++;

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
	Timer32_2_Init(&Timer32_2_ISR, SystemCoreClock/1000, T32DIV1); // initialize Timer A32-1;
	TIMER32_CONTROL1 |= BIT7;
	NVIC_EnableIRQ(T32_INT1_IRQn); // Enable IRQ
	TIMER32_CONTROL2 |= BIT7; // stop TIMER 2
	Switch1_Init();
	Switch2_Init();
	ADC0_InitSWTriggerCh6();
	EnableInterrupts();

  while(1)
	{
		print_data();
		
  }
}

