CC      = gcc
LD      = ld
NASM    = nasm

CFLAGS  = -m64 -ffreestanding -fno-builtin -nostdlib -nostartfiles -Wall -Wextra
LDFLAGS = -m elf_x86_64 -T linker.ld

# Automatically find all .c and .asm files in any subdirectory
C_SRCS   := $(shell find . -name "*.c" -not -path "./isodir/*")
ASM_SRCS := $(shell find . -name "*.asm" -not -path "./isodir/*")

# Convert source paths to .o files
C_OBJS   := $(C_SRCS:.c=.o)
ASM_OBJS := $(ASM_SRCS:.asm=.o)
OBJS     := $(C_OBJS) $(ASM_OBJS)

all: kernel.iso

# Compile C (64-bit)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble (64-bit ELF)
%.o: %.asm
	$(NASM) -f elf64 $< -o $@

# Link (64-bit ELF)
kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

# ISO
kernel.iso: kernel.elf
	mkdir -p isodir/boot/grub
	cp kernel.elf isodir/boot/kernel.elf
	cp boot/grub/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso isodir
	@echo "Built kernel.iso"

# Run in 64-bit QEMU
run: kernel.iso
	qemu-system-x86_64 -cdrom kernel.iso

# Cleanup
clean:
	find . -name "*.o" -not -path "./isodir/*" -delete
	rm -f kernel.elf kernel.iso
	rm -rf isodir

.PHONY: all run clean