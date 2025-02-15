/* src/start.S */
    .section .startup, "ax"
    .globl _start
_start:
    /* set stack pointer */
    la sp, _stack_top

    /* Optionally copy .data from LMA to VMA and zero .bss */
    /* For a quick test, skip or add later if you use global variables */

    /* call main */
    call main

1:  j 1b   /* loop forever if main() returns */
