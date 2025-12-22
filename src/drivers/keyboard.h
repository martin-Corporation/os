#pragma once

#include "../irq.h"

void keyboard_initialize();
void keyboard_handler(registers_t *regs);
