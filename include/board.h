#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// FE310 memory-mapped base addresses
#define GPIO_BASE     0x10012000UL
#define UART0_BASE    0x10013000UL
#define PRCI_BASE     0x10008000UL

// GPIO register offsets
#define GPIO_INPUT_EN_OFFSET    0x04
#define GPIO_OUTPUT_EN_OFFSET   0x08
#define GPIO_OUTPUT_VAL_OFFSET  0x0C
#define GPIO_IOF_EN_OFFSET      0x38
#define GPIO_IOF_SEL_OFFSET     0x3C

// UART0 register offsets
#define UART_TXDATA_OFFSET   0x00  // Transmit data (TX FIFO)
#define UART_RXDATA_OFFSET   0x04  // Receive data
#define UART_TXCTRL_OFFSET   0x08  // Transmit control
#define UART_RXCTRL_OFFSET   0x0C  // Receive control
#define UART_DIV_OFFSET      0x18  // Baud rate divisor

// PRCI (clocking) register offsets
#define PRCI_HFROSCCFG_OFFSET  0x00
#define PRCI_HFXOSCCFG_OFFSET  0x04
#define PRCI_PLLCFG_OFFSET     0x08
#define PRCI_PLLOUTDIV_OFFSET  0x0C

// Handy macro to access registers
#define REG32(base, offset) (*(volatile uint32_t *)((base) + (offset)))

// LED and UART pin definitions
#define LED_PIN    5    // FE310 GPIO 5 for on-board LED
#define UART0_RX_PIN 16 // FE310 GPIO 16 -> UART0 RX
#define UART0_TX_PIN 17 // FE310 GPIO 17 -> UART0 TX

// Bit masks for LED and UART pins
#define LED_MASK        (1u << LED_PIN)
#define UART_RX_MASK    (1u << UART0_RX_PIN)
#define UART_TX_MASK    (1u << UART0_TX_PIN)
#define UART_IOF_MASK   (UART_RX_MASK | UART_TX_MASK)

#endif // BOARD_H
