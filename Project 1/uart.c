/*
 * File:        uart.c
 * Purpose:     Provide UART routines for serial IO
 *
 * Notes:
 *
 */

#include "msp.h"
#include "uart.h"  // you need to create this file with the function prototypes

#define BAUD_RATE 115200      //default baud rate
extern uint32_t SystemCoreClock;  // clock rate of MCU

typedef unsigned char BOOLEAN;
#define FALSE 0
#define TRUE  !FALSE
void uart0_init(void)
{
//Set the UART to RESET state (set bit0 of EUSCI_A0->CTLW0 register to '1'
// bit15=0,      no parity bits
// bit14=x,      not used when parity is disabled
// bit13=0,      LSB first
// bit12=0,      8-bit data length
// bit11=0,      1 stop bit
// bits10-8=000, asynchronous UART mode
// bits7-6=11,   clock source to SMCLK
// bit5=0,       reject erroneous characters and do not set flag
// bit4=0,       do not set flag for break characters
// bit3=0,       not dormant
// bit2=0,       transmit data, not address (not used here)
// bit1=0,       do not transmit break (not used here)
// bit0=1,       hold logic in reset state while configuring

// set CTLW0 - hold logic and configure clock source to SMCLK
EUSCI_A0->CTLW0 =BIT0;
EUSCI_A0->CTLW0 |=BIT7; //*
EUSCI_A0->CTLW0 |=BIT6; //*


// baud rate
// N = clock/baud rate = clock_speed/BAUD_RATE
// set BRW register

EUSCI_A0->BRW=SystemCoreClock/BAUD_RATE ;



// clear first and second modulation stage bit fields
// MCTLW register;  
EUSCI_A0->MCTLW =0;
//UCA0MCTLW

// P1.3 = TxD
// P1.2 = RxD
// we will be using P1.2, P1.3 for RX and TX but not in I/O mode, so we set Port1 SEL1=0 and SEL0=1
// set SEL0, SEL1 appropriately
P1->SEL1 |=BIT2;
P1->SEL0 |=BIT3;



// CTLW0 register - release from reset state
EUSCI_A0->CTLW0 &=~BIT0; //*

// disable interrupts (transmit ready, start received, transmit empty, receive full)
// IE register;      
EUSCI_A0->IE&=~BIT0;
EUSCI_A0->IE&=~BIT1;
EUSCI_A0->IE&=~BIT2;
EUSCI_A0->IE&=~BIT3;
}


void uart2_init(void)
{
//Set the UART to RESET state (set bit0 of EUSCI_A0->CTLW0 register to '1'
// bit15=0,      no parity bits
// bit14=x,      not used when parity is disabled
// bit13=0,      LSB first
// bit12=0,      8-bit data length
// bit11=0,      1 stop bit
// bits10-8=000, asynchronous UART mode
// bits7-6=11,   clock source to SMCLK
// bit5=0,       reject erroneous characters and do not set flag
// bit4=0,       do not set flag for break characters
// bit3=0,       not dormant
// bit2=0,       transmit data, not address (not used here)
// bit1=0,       do not transmit break (not used here)
// bit0=1,       hold logic in reset state while configuring

// set CTLW0 - hold logic and configure clock source to SMCLK
EUSCI_A2->CTLW0 =BIT0;
EUSCI_A2->CTLW0 |=BIT7; //*
EUSCI_A2->CTLW0 |=BIT6; //*


// baud rate
// N = clock/baud rate = clock_speed/BAUD_RATE
// set BRW register
EUSCI_A2->BRW=SystemCoreClock/BAUD_RATE; ;



// clear first and second modulation stage bit fields
// MCTLW register;  
EUSCI_A2->MCTLW =0;
//UCA0MCTLW

// P1.3 = TxD
// P1.2 = RxD
// we will be using P1.2, P1.3 for RX and TX but not in I/O mode, so we set Port1 SEL1=0 and SEL0=1
// set SEL0, SEL1 appropriately
P3->SEL1 |=BIT2;
P3->SEL0 |=BIT3;



// CTLW0 register - release from reset state
EUSCI_A2->CTLW0 &=~BIT0; //*

// disable interrupts (transmit ready, start received, transmit empty, receive full)
// IE register;      
EUSCI_A2->IE&=~BIT0;
EUSCI_A2->IE&=~BIT1;
EUSCI_A2->IE&=~BIT2;
EUSCI_A2->IE&=~BIT3;
}

BYTE uart0_getchar(void)
{
BYTE inChar;
// Wait for data
	while(((EUSCI_A0->IFG &BIT0)==0)){
	}
	inChar= EUSCI_A0->RXBUF;

//Return the 8-bit data from the receiver
return(inChar);

}
BYTE uart2_getchar(void)
{
BYTE inChar;
// Wait for data
	while(((EUSCI_A2->IFG &BIT0)==0)){
	}
	inChar= EUSCI_A2->RXBUF;

//Return the 8-bit data from the receiver
return(inChar);

}

void uart0_putchar(char ch)
{
// Wait until transmission of previous bit is complete

	while(((EUSCI_A0->IFG &BIT1)==0)){
	}
	EUSCI_A0->TXBUF=ch;
}
void uart2_putchar(char ch)
{
// Wait until transmission of previous bit is complete

	while(((EUSCI_A2->IFG &BIT1)==0)){
	}
	EUSCI_A2->TXBUF=ch;
}
// send character
// (you need to fill in the ??)

void uart0_put(char *ptr_str)
{
while(*ptr_str != 0)
uart0_putchar(*ptr_str++);
}

void uart2_put(char *ptr_str)
{
while(*ptr_str != 0)
uart2_putchar(*ptr_str++);
}

void put(char *temp){ //prints to both putty & phone
    uart0_put(temp);
    uart2_put(temp);
}

BOOLEAN uart0_dataAvailable(){
	BOOLEAN go = FALSE;//if a character is available, set go = TRUE;
	if((EUSCI_A0->IFG &BIT0)==0){
		go = TRUE;
	}
	return go;
}

BOOLEAN uart2_dataAvailable(){
	BOOLEAN go = FALSE;//if a character is available, set go = TRUE;
	if((EUSCI_A2->IFG &BIT0)==0){
		go = TRUE;
	}
	return go;
}
