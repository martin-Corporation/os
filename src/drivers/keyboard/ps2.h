#pragma once

#include "../irq.h"

int ps2_keyboard_initialize();

#ifdef __wasm__
void ps2_keyboard_handler(registers_t *regs);
#endif
