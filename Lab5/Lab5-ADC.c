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
// The sprintf function seemed to cause a hange in the interrupt service routine.
// I think if we increase the HEAP size, it will work
// change to Heap_Size       EQU     0x00000200 in startup_msp432p401r_uvision.s

extern uint32_t SystemCoreClock;

BOOLEAN Timer1RunningFlag = FALSE;
BOOLEAN Timer2RunningFlag = FALSE;

unsigned long MillisecondCounter = 0;
BOOLEAN flag = FALSE;


// Interrupt Service Routine for Timer32-1
void Timer32_1_ISR(void)
{
	uart0_put("Debug Statement");
	if(!flag){
		flag=TRUE;
		P1->OUT=BIT0;
	}
	else{
		flag = FALSE;
		P1->OUT=0;
	}
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
	char temp[64];
	unsigned int analogIn = 0;
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
		if(flag){
			analogIn = ADC_In();
			int n=sprintf(temp,"\r\n data is %i\r\n", analogIn);
			uart0_put(temp);
		}
		
  }
}

