/**
 * main.c
 */
#include "msp.h"
#include "oled.h"
#include <stdio.h>
#include <string.h>
#include "i2c.h"

extern  unsigned char OLED_clr_data[1024];
extern unsigned char OLED_TEXT_ARR[1024];
extern unsigned char OLED_GRAPH_ARR[1024];
uint16_t lineData[128];

void msdelay(int delay)
{
    int i,j;
    for(i=0;i<delay;i++)
        for(j=0;j<16000;j++);
}

int main(void)
{
	int i;
	int delta;
	OLED_Init();
	OLED_display_on();
	OLED_display_clear();
	OLED_display_on();
    // write "Hello World" on line 1, column 1
	OLED_Print(1, 1, "Hello World");
    // write "How are you?" on line 2, column 2
	OLED_Print(2, 2, "How are you");
    // write "Goodbye" on line 3, column 3
	OLED_Print(3, 3, "Goodbye");
	// (HINT: Use OLED_draw_line)
    
	// create some fake 14-bit camera line data 0 - >2^14 in a 128 short array that utilizes all 14 bits
	for (i = 0; i < 128; i++) {
		lineData[i] = 16384-(i*110);
	}
	
	while(1)
	{
			OLED_write_display(OLED_TEXT_ARR);
			msdelay(500);
			OLED_DisplayCameraData(lineData);
			msdelay(500);

	}
}
