#include "clock.h"
#include "board.h"

void initSystemClock(void)
{
    // 1. Enable the high-frequency external oscillator (HFXOSC)
    REG32(PRCI_BASE, PRCI_HFXOSCCFG_OFFSET) |= (1u << 30); // set hfxoscen (bit 30)
    // 2. Wait until the oscillator is ready (hfxoscrdy bit, bit 31)
    while ((REG32(PRCI_BASE, PRCI_HFXOSCCFG_OFFSET) & (1u << 31)) == 0) {
        /* wait for hfxoscrdy = 1 (crystal stable) */
    }

    // 3. Use PLL in bypass mode to switch to HFXOSC
    //    (pllrefsel=1, pllbypass=1), then pllsel=1 to select it as hfclk
    uint32_t pllcfg = REG32(PRCI_BASE, PRCI_PLLCFG_OFFSET);
    pllcfg |= (1u << 17);  // pllrefsel = 1 (select HFXOSC)
    pllcfg |= (1u << 18);  // pllbypass = 1 (bypass PLL)
    REG32(PRCI_BASE, PRCI_PLLCFG_OFFSET) = pllcfg;
    REG32(PRCI_BASE, PRCI_PLLCFG_OFFSET) |= (1u << 16);  // pllsel = 1 (select PLL-bypass as hfclk)

    // 4. Disable HFROSC (bit 30) if desired for power savings
    REG32(PRCI_BASE, PRCI_HFROSCCFG_OFFSET) &= ~(1u << 30);
}
