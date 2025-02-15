#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "clock.h"
#include "gpio.h"
#include "uart.h"

void shell(void)
{
    char buffer[32];

    uartSendString("\r\nSimple Shell Ready!\r\n");
    uartSendString("Commands: echo <text>, led on, led off\r\n");

    while (1)
    {
        uartSendString("\r\n> "); // Prompt
        uartReceiveString(buffer, sizeof(buffer));

        // Handle "echo" command
        if (strncmp(buffer, "echo ", 5) == 0)
        {
            uartSendString("\r\n");
            uartSendString(buffer + 5); // Echo user input
            uartSendString("\r\n");
        }
        // Handle "led on" command
        else if (strcmp(buffer, "led on") == 0)
        {
            ledOn();
            uartSendString("\r\nLED turned ON\r\n");
        }
        // Handle "led off" command
        else if (strcmp(buffer, "led off") == 0)
        {
            ledOff();
            uartSendString("\r\nLED turned OFF\r\n");
        }
        else
        {
            uartSendString("\r\nUnknown command!\r\n");
        }
    }
}

int main(void)
{
    initSystemClock();
    initLED();
    initUART();

    shell(); // Start the simple shell

    return 0;
}
