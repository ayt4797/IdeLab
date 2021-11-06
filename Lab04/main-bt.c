/*
    Title: CMPE460 Lab4 - Bluetooth Low Energy 
    Description: UART driver to send and receive characters
    Authors: 
	Louis J. Beato
	3/1/2021
*/


#include "msp.h"
#include "uart.h"         // Main serial funcitonality
//#include "led.h"          // OPTIONAL: Could be useful to display info/warnings
#include <stdio.h>        // General funcitonality
#include <string.h>       // Useful for string operations and memset

#define CHAR_COUNT 80     // Size of a console line
#define FALSE 0
#define TRUE !FALSE
int main()
{
    // Initialize LEDs (optional)

    
    // Initialize UART0 and UART2
   	uart0_init();
		uart2_init();
    
    // Display startup message
	uart0_put("\nChatroom has started up!\n\r");    /*Transmit this through UART*/

	uart2_put("\nChatroom has started up!\n\r"); 
    
    // Declare and reset buffers and print prompts
	char pc_input[255] ;
	int pcCount=0;
	int phoneCount=0;
	char phone_input[255] ;
    
    /* Control loop */
    while(1)
    {
        /* Check if there is data available from the USB */
        if ((EUSCI_A0->IFG &BIT0))
        {
					char ch;
					if(pcCount==0)
						uart0_put("PC> ");
            //LED1_On();
            // Retrieve the character
						if(pcCount<255){
							ch = uart0_getchar(); // get the character
							uart0_put(&ch);
							pc_input[pcCount]=ch;
							pcCount++;
						}
            /* If statements to handle the character */
            if (ch == '\n' || ch == '\r') {
                /** 
                 * Enter pressed. Phone terminal may not \r or \n
                 * Manual entering of a character may be needed
                 * Some terminals enter \r then \n. Handle this.
                 */
                /* Handle text coming from the PC */ 
                /*     ON THE PHONE SIDE    */
                // Clear current text

									uart0_putchar('\n');
									uart0_putchar('\r');

									uart2_putchar('\n');
									uart2_putchar('\r');
									uart2_put("PC> ");
									uart2_put(pc_input);
									
                for (int i=0; i<255;i++){
									pc_input[i]=0;
								}
								pcCount=0;

                
            } else if (ch == 0x7f){
                /* 
                 * Backspace detected. Some terminals don't use \b character
                 * Some enter DEL. Use an ASCII table & debugger to figure out 
                 * the correct character to check for
                */
                /* Handle backspace */
								
								if(pcCount>0){
								pcCount--; //need to remove character and backspace character
								pc_input[pcCount] = 0;
								}
								if(pcCount>0){

								pcCount--;
								pc_input[pcCount] = 0;
								}							

                // Check for buffer underflow
            } else {
                /* Non-special character entered */
                
                // Check for buffer overflow
                
            } 
        }
        else if(EUSCI_A2->IFG &BIT0){
					char ch=uart2_getchar();
					phone_input[phoneCount]=ch;
					phoneCount++;
					if(ch=='\n'){
						for (int i=0;i<pcCount+4;i++){
									uart0_putchar(0x7f);
								}
						uart0_put("Phone>");
						uart2_put("Phone>");

						uart0_put(phone_input);
						uart2_put(phone_input);
                for (int i=0; i<255;i++){
									phone_input[i]=0;
								}
								phoneCount=0;
								if(pcCount>0){
									uart0_put("PC> ");
									uart0_put(pc_input);
								}
					}
				}
    }   
}  
