CC      := i686-elf-gcc
AS      := i686-elf-as
LD      := i686-elf-gcc

CFLAGS  := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I src/libc
LDFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

TARGET  := myos.bin

C_SOURCES := $(shell find src -type f -name "*.c")
ASM_SOURCES := boot.s \
               $(shell find src -type f -name "*.s")

C_OBJECTS   := $(C_SOURCES:.c=.o)
ASM_OBJECTS := $(ASM_SOURCES:.s=.o)

OBJECTS := $(C_OBJECTS) $(ASM_OBJECTS)

.PHONY: all wasm clean run build

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

run: $(TARGET)
	qemu-system-i386 -kernel $(TARGET) -serial stdio

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f os.wasm

build: $(TARGET)
