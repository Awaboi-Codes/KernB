bits 32

MAGIC    equ 0x1BADB002
FLAGS    equ 0
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .text
global start
extern kernel_main

start:
    cli
    mov esp, stack_top

; -------------------------
; enable PAE
; -------------------------
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

; -------------------------
; load page tables
; -------------------------
    mov eax, pml4_table
    mov cr3, eax

; -------------------------
; enable long mode
; -------------------------
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

; -------------------------
; enable paging
; -------------------------
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

; load 64-bit GDT
    lgdt [gdt_descriptor]

; jump to 64-bit code
    jmp 0x08:long_mode_start

; =========================
; 64-bit code
; =========================

bits 64
long_mode_start:

    mov rsp, stack_top
    call kernel_main

.hang:
    hlt
    jmp .hang

; =========================
; GDT
; =========================

section .data

gdt_start:
    dq 0
    dq 0x00AF9A000000FFFF
    dq 0x00AF92000000FFFF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start          ; ← dd not dq (we're still 32-bit when lgdt runs)

; =========================
; Page tables (identity map first 2MB)
; =========================

align 4096
pml4_table:
    dq pdpt_table + 3
    times 511 dq 0

align 4096
pdpt_table:
    dq pd_table + 3
    times 511 dq 0

align 4096
pd_table:
    dq 0x0000000000000083
    times 511 dq 0

; =========================
; Stack
; =========================

section .bss
align 16
stack_bottom:
    resb 8192
stack_top: