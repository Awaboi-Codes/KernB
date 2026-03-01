# Makefile for KernB
ASM    = nasm
CXX    = g++
LD     = ld

CXXFLAGS = -m32 -ffreestanding -fno-exceptions -fno-rtti \
           -fno-stack-protector -nostdlib -O2 -Wall -Wextra \
           -Ikernel -Ifs \
           -mno-sse -mno-sse2 -mno-mmx -mno-avx \
           -mno-80387 -msoft-float

LDFLAGS  = -m elf_i386 -T linker.ld --oformat=elf32-i386

KERNEL  = kernel.bin
ISO     = kernb.iso

# Auto-collect all .cpp files from kernel/ and fs/
CPP_SRCS = $(wildcard kernel/*.cpp) $(wildcard fs/*.cpp)
CPP_OBJS = $(CPP_SRCS:.cpp=.o)

.PHONY: all clean iso run-iso

all: $(KERNEL)

kernel/%.o: kernel/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

fs/%.o: fs/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

boot.o: kernel/boot.asm
	$(ASM) -f elf32 kernel/boot.asm -o boot.o

$(KERNEL): boot.o $(CPP_OBJS)
	$(LD) $(LDFLAGS) boot.o $(CPP_OBJS) -o $(KERNEL)

disk.img:
	dd if=/dev/zero of=disk.img bs=1M count=10

iso: $(KERNEL)
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o $(ISO) iso

run-iso: iso disk.img
	qemu-system-i386 -cdrom $(ISO) -hda disk.img -vga std -m 64M -d int,cpu_reset -no-reboot

clean:
	rm -f boot.o kernel/*.o fs/*.o $(KERNEL) $(ISO)
	rm -rf iso