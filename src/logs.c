#include "logs.h"
#include <stdio.h>

const status_t status_map[] = {{"  OK  ", VGA_COLOR_GREEN},
                               {"", VGA_COLOR_BLACK},
                               {" FAIL ", VGA_COLOR_LIGHT_RED}};

void output_writestring(const char *s) {
  terminal_writestring(s);
  serial_writestring(s);
}

void puts_status(const status_t status, const char *message) {
  if (status.text != status_map[STATUS_WAIT].text) {
    putchar('[');
    terminal_setcolor(status.color);
    output_writestring(status.text);
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    putchar(']');
    putchar(' ');
  } else {
    output_writestring("         ");
  }

  puts(message);
}
