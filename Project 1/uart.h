#ifndef _UART_
#define _UART_


typedef unsigned char BYTE;
typedef unsigned char BOOLEAN;

void uart0_putchar(char ch);

void uart0_init(void);

void uart0_put(char *ptr_str);

BYTE uart0_getchar(void);

void uart2_putchar(char ch);

void uart2_init(void);

void uart2_put(char *ptr_str);
void put(char *temp);
BYTE uart2_getchar(void);
BOOLEAN uart0_dataAvailable(void);
BOOLEAN uart2_dataAvailable(void);

#endif
