; boot.asm
bits 32

section .multiboot
align 8
header_start:
    dd 0xE85250D6
    dd 0
    dd header_end - header_start
    dd -(0xE85250D6 + 0 + (header_end - header_start))

    ; framebuffer tag
    align 8
    dw 5
    dw 0
    dd 20
    dd 1024
    dd 768
    dd 32

    ; end tag
    align 8
    dw 0
    dw 0
    dd 8
header_end:

section .bss
align 16
stack_bottom:
    resb 65536          ; 64KB stack — bigger than before
stack_top:

section .text
global start
extern kernel_main

start:
    ; disable interrupts immediately
    cli

    ; set up stack
    mov esp, stack_top

    ; DO NOT align or push padding — just pass args directly
    ; kernel_main(unsigned int magic, unsigned int mb_info_addr)
    push ebx            ; arg2: multiboot info pointer
    push eax            ; arg1: multiboot magic

    call kernel_main

.hang:
    cli
    hlt
    jmp .hang