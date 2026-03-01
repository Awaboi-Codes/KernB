bits 32

section .multiboot
align 4
    dd 0x1BADB002
    dd 0x00
    dd -(0x1BADB002)

section .text
    global start
    extern kernel_main

start:
    cli
    mov esp, stack_top
    call kernel_main
    hlt

section .bss
align 16
    resb 4096 ; 4KB stack
stack_top: