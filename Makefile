CC      = gcc
LD      = ld
NASM    = nasm

CFLAGS = -m64 -ffreestanding -fno-builtin -nostdlib -nostartfiles -fno-stack-protector -Wall -Wextra
LDFLAGS = -m elf_x86_64 -T linker.ld

# Automatically find all .c and .asm files in any subdirectory
C_SRCS   := $(shell find . -name "*.c" -not -path "./isodir/*")
ASM_SRCS := $(shell find . -name "*.asm" -not -path "./isodir/*")

# Convert source paths to .o files
C_OBJS   := $(C_SRCS:.c=.o)
ASM_OBJS := $(ASM_SRCS:.asm=.o)
OBJS     := $(C_OBJS) $(ASM_OBJS)
DISK = disk.img

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

$(DISK):
	dd if=/dev/zero of=disk.img bs=1M count=64
	mke2fs -t ext4 -O ^64bit,^metadata_csum,^orphan_file disk.img

# Run in 64-bit QEMU
run: kernel.iso $(DISK)
	qemu-system-x86_64 \
		-drive file=$(DISK),format=raw,id=disk0,if=none \
		-device ide-hd,drive=disk0,bus=ide.0,unit=0 \
		-drive file=kernel.iso,media=cdrom,id=cdrom0,if=none \
		-device ide-cd,drive=cdrom0,bus=ide.1,unit=0 \
		-boot d \
		-d int,cpu_reset,guest_errors \
		-no-reboot

# Cleanup
clean:
	find . -name "*.o" -not -path "./isodir/*" -delete
	rm -f kernel.elf kernel.iso
	rm -rf isodir

.PHONY: all run clean