/* Define memory regions */
MEMORY
{
  flash (rx) : ORIGIN = 0x20010000, LENGTH = 512K
  ram (rwx)  : ORIGIN = 0x80000000, LENGTH = 16K
}

/* Define stack and heap */
__stack_size = 4K;
__heap_size = 2K;
_stack_top = ORIGIN(ram) + LENGTH(ram);

SECTIONS {
  .text : { *(.text) *(.rodata*) } > flash
  .bss : { *(.bss) *(COMMON) } > ram
  .data : { *(.data) } > ram AT> flash
  .heap (NOLOAD) : {
      end = .;
      __heap_start = .;
  } > ram
}
