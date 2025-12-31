#include "drivers/keyboard.h"
#include "hal.h"
#include "ime.h"
#include "logs.h"
#include "product.h"
#include "serial.h"
#include "vga.h"
#include <stdio.h>

#ifdef __linux__
#error                                                                         \
    "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__) && !defined(__wasm__)
#error "martinOS needs to be compiled with a ix86-elf compiler"
#endif

#ifdef __DATE__
#ifdef __TIME__
#define BUILD_DATE __DATE__ " " __TIME__
#else
#define BUILD_DATE __DATE__
#endif
#endif

void kmain() {
  serial_initialize();
  terminal_initialize();
  hal_initialize();

  terminal_writestring("Welcome to ");
  terminal_setcolor(VGA_COLOR_WHITE);
  terminal_writestring(PRODUCT_NAME);
  terminal_writestring(" ");
  terminal_writestring(PRODUCT_VERSION);
  terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
  puts("!");

  printf("Build date: %s\n\n", BUILD_DATE);

  puts_status(status_map[STATUS_OK], "Booted into the kernel");

  keyboard_initialize();
  ime_initialize();

#ifndef __wasm__
  for (;;)
    asm("hlt");
#endif
}
