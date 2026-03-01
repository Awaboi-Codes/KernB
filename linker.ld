ENTRY(start)
SECTIONS {
    . = 1M;
    .multiboot : { *(.multiboot) }   /* ← was missing, header got dropped */
    .text      : { *(.text)      }
    .data      : { *(.data)      }
    .bss       : { *(.bss)       }
}