#include "vga.h"
#include "cursor.h"
#include <stdbool.h>
#include <string.h>

#ifdef __wasm__
#include "wasm/console.h"
#endif

const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

uint8_t vga_entry_color(const vga_color_t fg, const vga_color_t bg) {
  return fg | bg << 4;
}

uint16_t vga_entry(unsigned char uc, uint8_t color) {
  return (uint16_t)uc | (uint16_t)color << 8;
}

void _terminal_initialize_custom_color(const uint8_t color) {
#ifndef __wasm__
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = color;
  terminal_buffer = (uint16_t *)0xB8000;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
#else
  terminal_color = color;
#endif
}

void terminal_initialize() {
  _terminal_initialize_custom_color(
      vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

static void _scroll() {
#ifndef __wasm__
  uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
  uint16_t blank = 0x20 /* space */ | (attribute_byte << 8);

  if (terminal_row >= VGA_HEIGHT) {
    /* shift all rows up */
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
      for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[y * VGA_WIDTH + x] =
            terminal_buffer[(y + 1) * VGA_WIDTH + x];
      }
    }

    /* blank last row */
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = blank;
    }

    terminal_row = VGA_HEIGHT - 1;
  }
#endif
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
#ifndef __wasm__
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
#endif
}

void terminal_putchar(const char c) {
#ifndef __wasm__
  if (c == '\n') {
    terminal_column = 0;
    terminal_row++;

    if (terminal_row >= VGA_HEIGHT) {
      _scroll();
      terminal_row = VGA_HEIGHT - 1;
      terminal_column = 0;
    }

    update_cursor(terminal_column, terminal_row);
    return;
  }

  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
  update_cursor(terminal_column + 1, terminal_row);

  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    terminal_row++;
    if (terminal_row >= VGA_HEIGHT) {
      _scroll();
      terminal_row = VGA_HEIGHT - 1;
      terminal_column = 0;
    }
  }
#else
  console_log_fmt((const char *)(intptr_t)c, 0);
#endif
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; ++i)
    terminal_putchar(data[i]);
}

void terminal_writestring(const char *data) {
#ifndef __wasm__
  terminal_write(data, strlen(data));
#else
  console_log_fmt(data, 0);
#endif
}

#ifdef __wasm__
uint8_t get_terminal_color() { return terminal_color; }
#endif
