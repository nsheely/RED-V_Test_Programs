ENTRY(_start)

MEMORY {
    ROM (rx)  : ORIGIN = 0x20000000, LENGTH = 512K
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 16K
}

SECTIONS {
    /* Code and read-only data in Flash (ROM) */
    .text : {
        _start = ABSOLUTE(.);
        KEEP(*(.startup))    /* keep the startup section if we name it .startup */
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > ROM

    /* Initialized data in RAM, but loaded from Flash just after .text */
    .data : AT(ADDR(.text) + SIZEOF(.text)) {
        _sdata = .;            /* start of .data in RAM */
        *(.data*)
        *(.sdata*)
        _edata = .;            /* end of .data */
    } > RAM  /* <-- no AT>ROM here */

    /* Uninitialized data (.bss) in RAM */
    .bss : {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > RAM

    /* End of program, define top of stack */
    _end = .;
    PROVIDE(_stack_top = ORIGIN(RAM) + LENGTH(RAM));
}
