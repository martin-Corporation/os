#pragma once

#include <stdint.h>
#include <asm/io.h>
#include "vga.h"

// Enables the VGA Text Mode cursor.
inline void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
  outb(0x3D4, 0x0A);
  outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

// Disables the VGA Text Mode cursor.
inline void disable_cursor()
{
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);
}

// Updates the VGA Text Mode cursor's position.
inline void update_cursor(int x, int y)
{
  uint16_t pos = y * VGA_WIDTH + x;

  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// Gets the VGA Text Mode cursor's position.
inline uint16_t get_cursor_position()
{
  uint16_t pos = 0;
  outb(0x3D4, 0x0F);
  pos |= inb(0x3D5);
  outb(0x3D4, 0x0E);
  pos |= ((uint16_t)inb(0x3D5)) << 8;
  return pos;
}
