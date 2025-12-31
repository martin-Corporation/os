#pragma once

#include "vga.h"
#include <asm/io.h>
#include <stdint.h>

// Enables the VGA Text Mode cursor.
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

// Disables the VGA Text Mode cursor.
void disable_cursor();

// Updates the VGA Text Mode cursor's position.
void update_cursor(int x, int y);

// Gets the VGA Text Mode cursor's position.
uint16_t get_cursor_position();
