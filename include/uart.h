#ifndef UART_H
#define UART_H

#include <stddef.h>

void initUART(void);
void uartSendString(const char *str);
void uartSendChar(char c);
char uartReceiveChar(void);
void uartReceiveString(char *buffer, size_t max_length);

#endif // UART_H
