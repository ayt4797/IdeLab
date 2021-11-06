#ifndef _LAB1_
#define _LAB1_


typedef unsigned char BOOLEAN;
typedef unsigned char BYTE;

void LED1_Init(void);
void LED2_Init(void);
void LED2_Red(void);
void LED2_Blue(void);
void LED2_Green(void);
void LED2_Cyan(void);
void LED2_Magenta(void);
void LED2_Yellow(void);
void LED2_White(void);
void LED2_Off(void);
void Switches_Init(void);
void Switch1_Init(void);
BOOLEAN LED1_State(void);
void Switch2_Init(void);
BOOLEAN Switch1_Pressed(void);
BOOLEAN Switch2_Pressed(void);
void LED1_Off(void);
void LED1_On(void);

#endif
