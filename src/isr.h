// ISR from https://github.com/nanobyte-dev/nanobyte_os

#pragma once
#include <stdint.h>

typedef struct registers {
  // in the reverse order they are pushed:
  uint32_t ds; // data segment pushed by us
  uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax; // pusha
  uint32_t interrupt,
      error; // we push interrupt, error is pushed automatically (or our dummy)
  uint32_t eip, cs, eflags, esp, ss; // pushed automatically by CPU
} __attribute__((packed)) registers_t;

typedef void (*isr_handler_t)(registers_t *regs);

void isr_initialize();
void isr_register_handler(int interrupt, isr_handler_t handler);
