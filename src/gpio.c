#include "gpio.h"
#include "board.h"

void initLED(void)
{
    // 1. Disable input on pin 5
    REG32(GPIO_BASE, GPIO_INPUT_EN_OFFSET) &= ~LED_MASK;
    // 2. Disable IOF on pin 5 (regular GPIO)
    REG32(GPIO_BASE, GPIO_IOF_EN_OFFSET) &= ~LED_MASK;
    // 3. Enable output on pin 5
    REG32(GPIO_BASE, GPIO_OUTPUT_EN_OFFSET) |= LED_MASK;
    // 4. Optionally set LED pin low initially
    REG32(GPIO_BASE, GPIO_OUTPUT_VAL_OFFSET) &= ~LED_MASK;
}

void ledOn(void)
{
    // Set pin 5 high
    REG32(GPIO_BASE, GPIO_OUTPUT_VAL_OFFSET) |= LED_MASK;
}

void ledOff(void)
{
    // Set pin 5 low
    REG32(GPIO_BASE, GPIO_OUTPUT_VAL_OFFSET) &= ~LED_MASK;
}
