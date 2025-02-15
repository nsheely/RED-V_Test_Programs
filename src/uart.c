#include "uart.h"
#include "board.h"

void initUART(void)
{
    // 1. Configure GPIO pins 16 & 17 for UART0 (IOF0)
    REG32(GPIO_BASE, GPIO_IOF_SEL_OFFSET) &= ~UART_IOF_MASK; // select IOF0 for pins 16 & 17
    REG32(GPIO_BASE, GPIO_IOF_EN_OFFSET)  |=  UART_IOF_MASK; // enable IOF on pins 16 & 17

    // 2. Set UART0 baud rate divisor for 115200 at 16 MHz => ~139
    REG32(UART0_BASE, UART_DIV_OFFSET) = 139;

    // 3. Enable TX and RX
    REG32(UART0_BASE, UART_TXCTRL_OFFSET) = (1u << 0); // TX enable
    REG32(UART0_BASE, UART_RXCTRL_OFFSET) = (1u << 0); // RX enable
}

void uartSendChar(char c)
{
    while ((int32_t)REG32(UART0_BASE, UART_TXDATA_OFFSET) < 0) {
        /* wait for space in FIFO */
    }
    REG32(UART0_BASE, UART_TXDATA_OFFSET) = (uint8_t)c;
}

void uartSendString(const char *str)
{
    while (*str) {
        uartSendChar(*str);
        str++;
    }
}

char uartReceiveChar(void)
{
    int32_t data;
    do {
        data = (int32_t)REG32(UART0_BASE, UART_RXDATA_OFFSET);
    } while (data < 0); // Wait until data is available

    return (char)(data & 0xFF); // Mask to get lower byte (received character)
}

void uartReceiveString(char *buffer, size_t max_length)
{
    size_t i = 0;
    while (i < max_length - 1)
    {
        char c = uartReceiveChar();

        // Handle Backspace (ASCII 8 or 127)
        if (c == '\b' || c == 127) {
            if (i > 0) {
                i--; // Remove last character
                uartSendString("\b \b"); // Move cursor back, erase character, move back again
            }
        }
        else if (c == '\r' || c == '\n') {
            break; // Stop on Enter
        }
        else {
            buffer[i++] = c;
            uartSendChar(c); // Echo back
        }
    }
    buffer[i] = '\0'; // Null terminate
}
