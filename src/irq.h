// IRQ from https://github.com/nanobyte-dev/nanobyte_os

#pragma once
#include "isr.h"

typedef void (*irq_handler_t)(registers_t *regs);

void irq_initialize();
void irq_register_handler(int irq, irq_handler_t handler);
