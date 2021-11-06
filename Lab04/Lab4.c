#include<stdio.h>
#include "msp.h"
#include "uart.h"

#define CHAR_COUNT 10      //change this to modify the max. permissible length of a sentence
void LED1_Init(void);
void LED2_Init(void);

int main()
{

	uart0_init();
	uart2_init();
	LED1_Init();
	LED2_Init();
	int character_count=0;
	char ch;
	// write this string to the UART
	uart0_put("\r\n IDE Lab: Instructed by Prof. Louis Beato");    /*Transmit this through UART*/

	uart2_put("\r\n IDE Lab: Instructed by Prof. Louis Beato");    /*Transmit this through UART*/
	char input[255];
	while(1){
		char ch = uart2_getchar();

		if(ch=='0'){
			P2->OUT &=~BIT0;
			P2->OUT &=~BIT1;
			P2->OUT &=~BIT2;
		}
		else if(ch=='1'){//red
			P2->OUT &=~BIT0;
			P2->OUT &=~BIT1;
			P2->OUT &=~BIT2;

			P2->OUT |=BIT0;
		}
		else if(ch=='2'){//blue
			P2->OUT &=~BIT0;
			P2->OUT &=~BIT1;
			P2->OUT &=~BIT2;
			P2->OUT |=BIT2;
		}
		else if(ch=='3'){
			//green
			P2->OUT &=~BIT0;
			P2->OUT &=~BIT1;
			P2->OUT &=~BIT2;
			P2->OUT |=BIT1;		
		}
		else if (ch!='\r'&&ch!='\n')			{
			uart0_put("Invalid Input\n");
		}
		uart0_put(&ch);
	}
}

void LED1_Init(void)
{
// configure PortPin for LED1 as port I/O
P1->SEL0 &= ~BIT0;
P1->SEL1 &= ~BIT0;
P1->DIR |=BIT0;
P1->DS &= BIT0;
P1->OUT |=BIT0;
//P1->OUT &=~BIT0;
// make built-in LED1 LED high drive strength
 // make built-in LED1 out
// turn off LED
}
void LED2_Init(void)
{
// configure PortPin for LED2 as port I/O
P2->SEL0 &= ~BIT0;
P2->DIR |=BIT0;
P2->DS &= BIT0;
//P2->OUT |=BIT0;
P2->SEL0 &= ~BIT1;
P2->DIR |=BIT1;
P2->DS &= BIT1;
//P2->OUT |=BIT1;
P2->SEL0 &= ~BIT2;
P2->DIR |=BIT2;
P2->DS &= BIT2;
//P2->OUT |=BIT2;
// make built-in LED2 LEDs high drive strength
 // make built-in LED2 out
// turn off LED
}
