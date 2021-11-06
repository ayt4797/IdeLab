#include "msp.h"
#include "oled.h"
#include "Common.h"
#include "i2c.h"

/*
 * i2c.c
 *
 *
 *  Update to the new register notation
 *	LJBeato
 *	03/23/21
 *
 *  added   listener  09/04/21
 * LJBeato
 */


#
extern uint32_t SystemCoreClock;
int i;


//static BOOLEAN OLED_Display_Active;
// This function writes the RECEIVER address to the i2c bus.
// If a RECEIVER chip is at that address, it should respond to
// this with an "ACK".   This function returns TRUE if an
// ACK was found.  Otherwise it returns FALSE.

//// --------------------- I2C0  ---------------------------------------------

void i2c0_Init(int listenerAddress)
{

	// make sure module is disabled (in reset mode) 
  EUSCI_B0->CTLW0 |= BIT0; // Enable Reset mode. CTLW0.0 <- 1;
	;

	// set appropriate Port.Pins for I2C SDA/SCL
	P1->SEL0 |= BIT6; // PxSel0 = 1  // SEL0 = 1
	P1->SEL0 |= BIT7; // PxSel0 = 1
	P1->SEL1 &= ~BIT6; // PxSel1 = 0 // SEL1 = 0
	P1->SEL1 &= ~BIT7; // PxSel1 = 0
    
    // configure EUSCI_B0->CTLW0 for:

    // bit15      UCA10   - 0 = Own address is a 7-bit address,      1 = Own address is a 10-bit address.
    // bit14      UCSLA10 - 0 = Address LISTENER with 7-bit address, 1 = Address LISTENER with 10-bit address
    // bit13      UCMM    - 0 = Single  MASTER environment,          1 = multi-MASTER environ 
    // bit12      reserved
    // ----------------------------------------------------------------

    // bit11      UCMST   -  0 = LISTENER mode, 1 = MASTER mode
    // bits10-9   UCMODEx - 00 = 3-pin SPI
    // 											01 = 4-pin SPI (MASTER or LISTENER enabled if STE = 1)
    // 											10 = 4-pin SPI (MASTER or LISTENER enabled if STE = 0)
    // 											11 = I2C mode
    // bit8       UCSYNC  - For eUSCI_B always read and write as 1.
	// -----------------------------------------------------------------
	// 
    // bits7-6    UCSSELx - (ignore in LISTENER mode)
    //											00 -  UCLKI
    //											01 - 	ACLK
    //											10 -  SMCLK
    //											11 -  SMCLK
    // bit5       UCTXACK  - 0 = do not acknowledge LISTENER, 1 = acknowledge LISTENER
    // bit4       UCTR     - 0 = LISTENER,                    1 = MASTER
	//

    // bit3       UCTXNACK - 0 = acknowledge normally,  1 = generate NACK
    // bit2       UCTXSTP  - 0 = DO NOT generate STOP,  1 = generate STOP
    // bit1       UCTXSTT  - 0 = DO NOT generate START, 1 = generate START
    // bit0       UCSWRST  - 0 = not RESET,             1 = RESET
	//
	//

	// 7 bit LISTENER and self, 
	EUSCI_B0->CTLW0 &= ~BIT(15); // Ensure we have a 7-bit address
	EUSCI_B0->CTLW0 &= ~BIT(14); // Ensure 7-bit address is being listened
	// single MASTER 
	EUSCI_B0->CTLW0 &= ~BIT(13); // Ensure CTLW0.13, UCMM = 0;
	
	// MASTER mode 
	EUSCI_B0->CTLW0 |= BIT(11); // CTLW0.11, UCMST = 1;
	// I2C mode
	EUSCI_B0->CTLW0 |= 0x0600; // CTLW0.9-10 = 11;
	
	
	// SMCLK mode 
	EUSCI_B0->CTLW0 |= BIT7; // CTLW0.7, UCSSEL1 = 1;
	EUSCI_B0->CTLW0 |= BIT6; // CTLW0.6, UCSSEL0 = 0;
	// don/t acknowledge
	EUSCI_B0->CTLW0 &= ~BIT5; // CTLW0.5, UCTXACK = 0;
	// MASTER 
	EUSCI_B0->CTLW0 |= BIT(4);
	// ack normal
	EUSCI_B0->CTLW0 &= ~BIT3; // CTLW0.3, UCTXNACK = 0;
	// no STOP
	EUSCI_B0->CTLW0 &= ~BIT2; // CTLW0.2, UCTXSTP = 0;
	// no START
	EUSCI_B0->CTLW0 &= ~BIT1; // CTLW0.1, UCTXSTT = 0;
	// stay RESET
	EUSCI_B0->CTLW0 |= BIT0; // CTLW0.0, UCSWRST = 1;

	// set clock: 400 KHz
	EUSCI_B0->BRW = SystemCoreClock / 400000;
    ;
	// initialize RECEIVER address 
	EUSCI_B0->I2CSA = SSD1306_ADDR; // Display slave address - Is this right???
    ;

	// release reset
	// EUSCI_B0->CTLW0 &= ~BIT0; // CTLW0.0, UCSWRST = 0;
    ;
}

void i2c0_put(BYTE *data, unsigned int len)
{
	// enable i2c module, (remove from RESET)
	EUSCI_B0->CTLW0 &= ~BIT0; // RESET, CTLW0.0 = 0
    ;

    // generate start condition and wait for the bus 
  EUSCI_B0->CTLW0 |= BIT1; // GENERATE Start, CTLW0.1 = 1
    ;
    
	// BIT1 of IFG reg is 0 until character has been transmitted, then changes to 1
	// wait until it changes
	while ((EUSCI_B0->IFG & BIT1) == 0);

	// write data byte by byte to i2c, use putchar
   for (i = 0; i<len; i++){
		 i2c0_putchar(data[i]);
	 }

    // force stop
	EUSCI_B0->CTLW0 |= BIT2; // Generate Stop, CTLW0.2 = 1
    ;
    
    // wait for transmission to complete
	while ((EUSCI_B0->IFG & EUSCI_B_IFG_STPIFG) == 0); // break when TX, IFG != 0

	// transmission completed, disable the module (put it back in reset)
	EUSCI_B0->CTLW0 |= BIT0; // RESET, CTLW0.0 = 1;
    ;
}

/* write data on i2c bus */
static void i2c0_putchar(BYTE data)
{
    // write data to TXBUF
	EUSCI_B0->TXBUF = data;
    ;
    
	// wait until byte is transmitted
	while ((EUSCI_B0->IFG & BIT1) == 0); // break when TX, IFG != 0
}

