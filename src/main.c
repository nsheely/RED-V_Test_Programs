#include <stdint.h>

// Base addresses for FE310-G002 peripherals (from FE310 manual)
#define PRCI_BASE    0x10008000UL        // PRCI (clock/power management) base
#define GPIO_BASE    0x10012000UL        // GPIO controller base

// PRCI register offsets (FE310-G002)
#define PRCI_HFXOSCCFG  0x04  // High-frequency external osc config (enable/ready bits)
#define PRCI_PLLCFG     0x08  // PLL configuration (contains pllrefsel, pllsel, pllbypass bits)

// GPIO register offsets (FE310-G002)
#define GPIO_IOF_EN     0x38  // IO function enable register
#define GPIO_OUTPUT_EN  0x08  // Output enable register
#define GPIO_OUTPUT_VAL 0x0C  // Output value register

// LED pin for SparkFun RED-V (FE310 GPIO5, labeled D13 on board)
#define LED_PIN    5

// Macro to access 32-bit registers at given address (volatile for hardware registers)
#define REG32(base, offset) (*((volatile uint32_t *)((base) + (offset))))

int main(void) {
    // 1. Enable external 16MHz oscillator (HFXOSC)
    REG32(PRCI_BASE, PRCI_HFXOSCCFG) |= (1 << 30);           // set HFXOSC enable bit (bit 30)
    while ((REG32(PRCI_BASE, PRCI_HFXOSCCFG) & (1 << 31)) == 0) {
        ; // wait until HFXOSC ready bit (bit 31) is set
    }

    // 2. Select HFXOSC as clock source, keep PLL bypassed
    uint32_t pllcfg = REG32(PRCI_BASE, PRCI_PLLCFG);
    pllcfg |= (1 << 18);   // PLLBYPASS = 1 (keep PLL off)
    pllcfg &= ~(1 << 16);  // PLLSEL = 0 (stay with bypassed clock)
    pllcfg |= (1 << 17);   // PLLREFSEL = 1 (select external crystal as ref)
    REG32(PRCI_BASE, PRCI_PLLCFG) = pllcfg;
    // (At this point, the CPU should be running from the 16MHz external crystal)

    // 3. GPIO configuration for LED (GPIO5 on RED-V)
    REG32(GPIO_BASE, GPIO_IOF_EN) &= ~(1 << LED_PIN);    // disable IOF for pin 5 (use as GPIO)
    REG32(GPIO_BASE, GPIO_OUTPUT_VAL) &= ~(1 << LED_PIN);  // set output low (LED off initially)
    REG32(GPIO_BASE, GPIO_OUTPUT_EN) |=  (1 << LED_PIN);   // enable output driver on pin 5

    // 4. Main loop: toggle LED with a delay
    while (1) {
        // Toggle LED pin
        REG32(GPIO_BASE, GPIO_OUTPUT_VAL) ^= (1 << LED_PIN);  // invert GPIO5 output bit
        // Simple delay loop (wastes CPU cycles to create a visible blink interval)
        for (volatile uint32_t i = 0; i < 1000000; ++i) { }   // adjust loop count as needed
    }

    return 0; // (Should never reach here)
}
