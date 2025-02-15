#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// FE310 memory-mapped base addresses
#define GPIO_BASE     0x10012000UL
#define UART0_BASE    0x10013000UL
#define PRCI_BASE     0x10008000UL

// GPIO register offsets
#define GPIO_INPUT_EN    0x04  // input enable for pins
#define GPIO_OUTPUT_EN   0x08  // output enable for pins
#define GPIO_OUTPUT_VAL  0x0C  // output values for pins
#define GPIO_IOF_EN      0x38  // IO function enable
#define GPIO_IOF_SEL     0x3C  // IO function select

// UART0 register offsets
#define UART_TXDATA   0x00  // Transmit data (TX FIFO)
#define UART_RXDATA   0x04  // Receive data
#define UART_TXCTRL   0x08  // Transmit control
#define UART_RXCTRL   0x0C  // Receive control
#define UART_DIV      0x18  // Baud rate divisor

// PRCI (clocking) register offsets
#define PRCI_HFROSCCFG  0x00  // HF oscillator config
#define PRCI_HFXOSCCFG  0x04  // HF crystal config
#define PRCI_PLLCFG     0x08  // PLL config
#define PRCI_PLLOUTDIV  0x0C  // PLL output divider

// Handy macros to access registers
#define REG32(base, offset) (*(volatile uint32_t *)((base) + (offset)))

// LED and UART pin definitions
#define LED_PIN    5    // FE310 GPIO 5 for on-board LED
#define UART0_RX_PIN 16 // FE310 GPIO 16 -> UART0 RX (not used for TX-only)
#define UART0_TX_PIN 17 // FE310 GPIO 17 -> UART0 TX

// Bit masks for LED and UART pins
#define LED_MASK        (1u << LED_PIN)
#define UART_RX_MASK    (1u << UART0_RX_PIN)
#define UART_TX_MASK    (1u << UART0_TX_PIN)
#define UART_IOF_MASK   (UART_RX_MASK | UART_TX_MASK)

int main(void) {
    // 1. Configure system clock to use external 16 MHz crystal for stability
    // Enable the high-frequency external oscillator (HFXOSC)
    REG32(PRCI_BASE, PRCI_HFXOSCCFG) |= (1u << 30);                   // set hfxoscen
    while ((REG32(PRCI_BASE, PRCI_HFXOSCCFG) & (1u << 31)) == 0) {
        /* wait for hfxoscrdy = 1 (crystal stable) */
    }
    // Use PLL in bypass mode to switch to HFXOSC directly
    uint32_t pllcfg = REG32(PRCI_BASE, PRCI_PLLCFG);
    pllcfg |= (1u << 17);  // pllrefsel = 1 (select HFXOSC as PLL reference)
    pllcfg |= (1u << 18);  // pllbypass = 1 (bypass PLL, use ref clock directly)
    REG32(PRCI_BASE, PRCI_PLLCFG) = pllcfg;
    REG32(PRCI_BASE, PRCI_PLLCFG) |= (1u << 16);  // pllsel = 1 (select PLL (bypass) as hfclk)
    // Now hfclk (core clock) is the 16 MHz external crystal. Disable HFROSC for power if desired:
    REG32(PRCI_BASE, PRCI_HFROSCCFG) &= ~(1u << 30);  // turn off HFROSC (optional)

    // 2. Configure GPIO for LED (pin 5) as output
    REG32(GPIO_BASE, GPIO_INPUT_EN)  &= ~LED_MASK;  // disable input on pin 5
    REG32(GPIO_BASE, GPIO_IOF_EN)    &= ~LED_MASK;  // disable IOF on pin 5 (no alternate function)
    REG32(GPIO_BASE, GPIO_OUTPUT_EN) |=  LED_MASK;  // enable output on pin 5

    // 3. Configure UART0 pins (GPIO 16 & 17) for IOF0 (UART0 RX/TX) and UART0 settings
    REG32(GPIO_BASE, GPIO_IOF_SEL) &= ~UART_IOF_MASK; // select IOF0 for 16 & 17 (UART0 is IOF0)
    REG32(GPIO_BASE, GPIO_IOF_EN)  |=  UART_IOF_MASK; // enable IOF on pins 16 & 17

    // Set UART0 baud rate divisor for 115200 baud.
    // For 16 MHz clock, divisor = 16e6 / 115200 = 139. (0.08% error)
    REG32(UART0_BASE, UART_DIV) = 139;  // 115200 baud (0.08% error)

    // Configure UART0 TX control: enable TX, 1 stop bit (nstop=0), no Tx interrupt (we poll)
    REG32(UART0_BASE, UART_TXCTRL) = (1u << 0);  // txen = 1 (bit 0 enables transmitter)
    // (RX not used, but we could enable it similarly if needed: RXCTRL bit0 = rxen)

    // 4. Main loop: Blink LED and send UART message every 1 second
    const char message[] = "Hello from RED-V!\r\n";
    while (1) {
        // Turn LED on
        REG32(GPIO_BASE, GPIO_OUTPUT_VAL) |= LED_MASK;   // set pin 5 high
        // Send message over UART0
        for (size_t i = 0; i < sizeof(message) - 1; ++i) {
            // Wait if UART FIFO is full (txdata returns <0 if full)
            while ((int32_t)REG32(UART0_BASE, UART_TXDATA) < 0) {
                /* wait for space in FIFO */
            }
            // Write next character to TX FIFO
            REG32(UART0_BASE, UART_TXDATA) = (uint8_t)message[i];
        }
        // (Optional) Wait until FIFO fully empties before toggling LED off or delaying,
        // to ensure all data is sent:
        // while ((int32_t)REG32(UART0_BASE, UART_TXDATA) < 0) { /* wait */ }

        // Simple delay ~1 second (busy loop)
        for (volatile uint32_t d = 0; d < 2000000; ++d) { }  // ~1 sec delay at 16 MHz

        // Turn LED off
        REG32(GPIO_BASE, GPIO_OUTPUT_VAL) &= ~LED_MASK;  // set pin 5 low
        // Another ~1 sec delay
        for (volatile uint32_t d = 0; d < 2000000; ++d) { }
    }

    return 0;
}
