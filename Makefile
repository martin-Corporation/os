.PHONY: build run clean

AS = i686-elf-as
CC = i686-elf-gcc
LD = i686-elf-gcc

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I src/libc
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

TARGET = myos.bin

BOOT_OBJ = boot.o
SRC_BOOT_OBJ = src/boot.o

all: $(TARGET)

$(TARGET): $(BOOT_OBJ) $(SRC_BOOT_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(BOOT_OBJ): boot.s
	$(AS) $< -o $@

$(SRC_BOOT_OBJ): src/boot.c
	$(CC) -c $< -o $@ $(CFLAGS)

run: $(TARGET)
	qemu-system-i386 -kernel $(TARGET)

clean:
	rm -f $(BOOT_OBJ) $(SRC_BOOT_OBJ) $(TARGET)

build: $(TARGET)
