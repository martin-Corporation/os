#include "vga.h"
#include "panic.h"
#include "product.h"
#include <stdio.h>

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "martinOS needs to be compiled with a ix86-elf compiler"
#endif

void kmain()
{
	terminal_initialize();

	terminal_writestring(name);
	terminal_writestring(" ");
	terminal_writestring(version);
	terminal_writestring("\n");
}
