#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>

const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;

#include "cursor.h"

/* Hardware text mode color constants. */
enum vga_color
{
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
  return fg | bg << 4;
}

inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
  return (uint16_t)uc | (uint16_t)color << 8;
}

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

inline void _terminal_initialize_custom_color(const enum vga_color color)
{
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = color;
  terminal_buffer = (uint16_t *)0xB8000;

  for (size_t y = 0; y < VGA_HEIGHT; y++)
  {
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}

inline void terminal_initialize()
{
  _terminal_initialize_custom_color(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

inline void terminal_setcolor(uint8_t color)
{
  terminal_color = color;
}

inline void _scroll()
{
  uint8_t attributeByte = (0 /* black */ << 4) | (15 /* white */ & 0x0F);
  uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

  if (terminal_row >= VGA_HEIGHT)
  {
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++)
    {
      for (size_t x = 0; x < VGA_WIDTH; x++)
      {
        terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
      }
    }

    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
      terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = blank;
    }

    terminal_row = VGA_HEIGHT - 1;
  }
}

inline void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

inline void terminal_putchar(char c)
{
  if (c == '\n')
  {
    terminal_column = 0;
    terminal_row++;

    if (terminal_row >= VGA_HEIGHT)
    {
      _scroll();
      terminal_row = VGA_HEIGHT - 1;
      terminal_column = 0;
    }

    update_cursor(terminal_column, terminal_row);

    return;
  }

  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
  update_cursor(terminal_column + 1, terminal_row);

  if (++terminal_column == VGA_WIDTH)
  {
    terminal_column = 0;
    terminal_row++;
    if (terminal_row >= VGA_HEIGHT)
    {
      _scroll();
      terminal_row = VGA_HEIGHT - 1;
      terminal_column = 0;
    }
  }
}

inline void terminal_write(const char *data, size_t size)
{
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

inline void terminal_writestring(const char *data)
{
  terminal_write(data, strlen(data));
}
