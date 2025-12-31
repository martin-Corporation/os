#include "../cursor.h"
#include "../product.h"
#include "../serial.h"
#include "../vga.h"
#include <stdio.h>
#include <string.h>

static void terminal_writestring_centered(const char *data, const uint8_t fg) {
  const int gap = (VGA_WIDTH - strlen(data)) / 2;
  const vga_color_t og_color = (vga_color_t)terminal_color;

  for (int i = 0; i < gap; i++) {
    terminal_putchar(' ');
  }

  terminal_setcolor(fg);
  terminal_writestring(data);
  serial_writestring(data);
  terminal_setcolor(og_color);

  for (int i = 0; i < gap; i++) {
    terminal_putchar(' ');
  }
}

void panic(const char *msg) {
  disable_cursor();

  _terminal_initialize_custom_color(
      vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE));

  terminal_writestring("\n\n\n\n\n\n");

  char name_with_space[sizeof(PRODUCT_NAME) + 2];

  strcat(name_with_space, " ");
  strcat(name_with_space, PRODUCT_NAME);
  strcat(name_with_space, " ");

  terminal_writestring_centered(
      name_with_space, vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE));
  terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE));

  printf("\n");

  terminal_writestring_centered("The system or parts of it have crashed.",
                                terminal_color);
  printf("\n");

  terminal_writestring_centered("The error that occured was:", terminal_color);

  terminal_putchar('\n');
  serial_putchar(' ');

  terminal_writestring_centered(msg, terminal_color);

  serial_putchar('\n');

  terminal_writestring_centered("To continue, please restart your computer.",
                                terminal_color);

  printf("\n");

  terminal_writestring_centered(
      "Doing so will result in irreversible loss of unsaved data.",
      terminal_color);

#ifdef __wasm__
  asm("cli");
#endif

#ifndef __wasm__
  for (;;)
    asm("hlt");
#endif
}
