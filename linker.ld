/* linker.ld — place kernel at 1MB, standard for x86 bare metal */
ENTRY(start)

SECTIONS {
    . = 0x100000;  /* load at 1MB */

    .text : {
        *(.multiboot)   /* multiboot header must be first */
        *(.text)
    }

    .rodata : { *(.rodata*) }
    .data   : { *(.data)   }
    .bss    : {
        *(COMMON)
        *(.bss)
    }
}
