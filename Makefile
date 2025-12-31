CC            := i686-elf-gcc
AS            := i686-elf-as
LD            := i686-elf-gcc
GRUB_MKRESCUE := i686-elf-grub-mkrescue

CFLAGS        := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I src/libc
LDFLAGS       := -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

TARGET        := myos.bin
ISO           := myos.iso
WASM          := os.wasm

C_SOURCES     := $(shell find src -type f -name "*.c")
ASM_SOURCES   := boot.s \
                $(shell find src -type f -name "*.s")

C_OBJECTS     := $(C_SOURCES:.c=.o)
ASM_OBJECTS   := $(ASM_SOURCES:.s=.o)

OBJECTS       := $(C_OBJECTS) $(ASM_OBJECTS)

.PHONY: all iso wasm clean run build

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^ -lgcc

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $< -o $@

wasm:
	chmod +x ./compile-wasm.sh
	./compile-wasm.sh

iso: $(TARGET)
	cp $(TARGET) iso/boot/
	$(GRUB_MKRESCUE) -o $(ISO) iso/

run: $(TARGET)
	qemu-system-i386 -kernel $(TARGET) -serial stdio

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f $(WASM)
	rm -f iso/boot/$(TARGET)
	rm -f $(ISO)

build: $(TARGET)
