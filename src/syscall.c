#include "syscall.h"
#include "idt.h"
#include "isr.h"
#include <stdio.h>

void syscall_handler(registers_t *regs) {
  const syscall_t syscall = (syscall_t)regs->eax;

  switch (syscall) {
  case SYSCALL_TEST: {
    puts("Test syscall");
    regs->eax = 0;
    break;
  }

  default: {
    regs->eax = -1;
    break;
  }
  }
}

extern void i686_ISR128();

void syscall_initialize() {
  idt_set_gate(0x80, i686_ISR128, 0x08, 0x8E);
  isr_register_handler(0x80, syscall_handler);
}
