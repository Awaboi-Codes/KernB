CC      = gcc
LD      = ld
NASM    = nasm
CFLAGS  = -m32 -ffreestanding -fno-builtin -nostdlib
LDFLAGS = -m elf_i386 -T linker.ld

# Automatically find all .c and .asm files in any subdirectory
C_SRCS   := $(shell find . -name "*.c" -not -path "./isodir/*")
ASM_SRCS := $(shell find . -name "*.asm" -not -path "./isodir/*")

# Convert source paths to .o files
C_OBJS   := $(C_SRCS:.c=.o)
ASM_OBJS := $(ASM_SRCS:.asm=.o)
OBJS     := $(C_OBJS) $(ASM_OBJS)

all: kernel.iso

# Compile any .c file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble any .asm file
%.o: %.asm
	$(NASM) -f elf32 $< -o $@

# Link
kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

# ISO
kernel.iso: kernel.elf
	mkdir -p isodir/boot/grub
	cp kernel.elf isodir/boot/kernel.elf
	cp boot/grub/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso isodir
	@echo "Built kernel.iso"

# Run
run: kernel.iso
	qemu-system-i386 -cdrom kernel.iso

# Cleanup
clean:
	find . -name "*.o" -not -path "./isodir/*" -delete
	rm -f kernel.elf kernel.iso
	rm -rf isodir

.PHONY: all run clean