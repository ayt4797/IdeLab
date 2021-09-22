#include<stdio.h>
#include "msp.h"
#include "uart.h"

#define CHAR_COUNT 10      //change this to modify the max. permissible length of a sentence
void LED1_Init(void);
void LED2_Init(void);



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
