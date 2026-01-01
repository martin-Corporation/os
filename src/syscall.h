#pragma once

typedef enum syscall {
  SYSCALL_TEST = 5,
} syscall_t;

void syscall_initialize();

#ifdef __wasm__
#include "isr.h"
void syscall_handler(registers_t *regs);
#endif
