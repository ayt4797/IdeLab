/*
Title: Lab1
Purpose:
Lab1
Name: Aubrey tarmu
Date: 08-27-21
This file prepared by LJBeato
01/11/2021
*/
#include "msp.h"
typedef unsigned char BOOLEAN;
#define FALSE 0
#define TRUE !FALSE
typedef unsigned char BYTE;
// Main Program
// 1. Init LEDs
// 2. Init Switch
// 3. init state=0
// 4. If SW1 pressed, light LED1
// 5. if SW2 pressed, cycle next color of R,G,B,C,M,Y,W
// 5. If button released

void LED1_Init(void)
{
// configure PortPin for LED1 as port I/O
P1->SEL0 &= ~BIT0;
P1->SEL1 &= ~BIT0;
P1->DIR |=BIT0;
P1->DS &= BIT0;
//P1->OUT |=BIT0;
P1->OUT &=~BIT0;
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
void Switches_Init(void)
{
// configure PortPin for Switch 1 and Switch2 as port I/O
P1->SEL0 &= ~BIT1;
P1->SEL1 &= ~BIT1;
P1->DIR |=BIT1;
P1->DS &= BIT1;
P1->OUT |=BIT1;
P1->SEL0 &= ~BIT4;
P1->SEL1 &= ~BIT4;
P1->DIR |=BIT4;
P1->DS &= BIT4;
P1->OUT |=BIT4;
// configure as input
// enable pullup resistor
}
void Switch1_Init(void){
	P1->SEL0 &= ~BIT1;
	P1->SEL1 &= ~BIT1;
	P1->DIR |=BIT1;
	P1->DS &= BIT1;
	P1->OUT |=BIT1;
}

BOOLEAN LED1_State(void){
	if(P1->OUT)
		return FALSE;
	return TRUE;
}

void LED2_Red() {
	P2->OUT |= BIT0; // RED
	P2->OUT &= ~BIT1;
	P2->OUT &= ~BIT2;
}

void LED2_Blue() {
	P2->OUT &= ~BIT0; // BLUE
	P2->OUT &= ~BIT1;
	P2->OUT |= BIT2;
}

void LED2_Green() {
	P2->OUT &= ~BIT0; // GREEN
	P2->OUT |= BIT1;
	P2->OUT &= ~BIT2;
}

void LED2_Yellow() {
	P2->OUT |= BIT0; // Yellow
	P2->OUT &= ~BIT1;
	P2->OUT |= BIT2;
}

void LED2_Magenta() {
	P2->OUT |= BIT0; // Magenta
	P2->OUT |= BIT1;
	P2->OUT &= ~BIT2;
}

void LED2_Cyan() {
	P2->OUT &= ~BIT0; // Cyan
	P2->OUT |= BIT1;
	P2->OUT |= BIT2;
}

void LED2_Off() {
	P2->OUT &= ~BIT0;
	P2->OUT &= ~BIT1;
	P2->OUT &= ~BIT2;
}

void LED2_White() {
	P2->OUT |= BIT0;
	P2->OUT |= BIT1;
	P2->OUT |= BIT2;
}

void LED1_On() {
	P1->OUT |= BIT0; // TOGGLE LED
}

void LED1_Off() {
	P1->OUT &= ~BIT0; // TOGGLE LED
}

void Switch2_Init(void){
	P1->SEL0 &= ~BIT4;
	P1->SEL1 &= ~BIT4;
	P1->DIR |=BIT4;
	P1->DS &= BIT4;
	P1->OUT |=BIT4;
}
//------------Switch_Input------------
// Read and return the status of Switch1
// Input: none
// return: TRUE if pressed
//
BOOLEAN Switch1_Pressed(void)
{
BOOLEAN retVal = FALSE;
// check if pressed
if((P1->IN & BIT1) == 0)
retVal = TRUE;
return (retVal);
// return TRUE(pressed) or FALSE(not pressed)
}
//------------Switch_Input------------
// Read and return the status of Switch2
// Input: none
// return: TRUE if pressed
//
BOOLEAN Switch2_Pressed(void)
{
BOOLEAN retVal = FALSE;
// check if pressed
if((P1->IN & BIT4) == 0)
retVal = TRUE;
return (retVal);
// return TRUE(pressed) or FALSE(not pressed)
}
