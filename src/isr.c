// ISR from https://github.com/nanobyte-dev/nanobyte_os

#include "isr.h"
#include "idt.h"
#include <asm/io.h>
#include <stddef.h>
#include <stdio.h>

isr_handler_t g_ISRHandlers[256];

static const char *const g_exceptions[] = {"Divide by zero error",
                                           "Debug",
                                           "Non-maskable Interrupt",
                                           "Breakpoint",
                                           "Overflow",
                                           "Bound Range Exceeded",
                                           "Invalid Opcode",
                                           "Device Not Available",
                                           "Double Fault",
                                           "Coprocessor Segment Overrun",
                                           "Invalid TSS",
                                           "Segment Not Present",
                                           "Stack-Segment Fault",
                                           "General Protection Fault",
                                           "Page Fault",
                                           "",
                                           "x87 Floating-Point Exception",
                                           "Alignment Check",
                                           "Machine Check",
                                           "SIMD Floating-Point Exception",
                                           "Virtualization Exception",
                                           "Control Protection Exception ",
                                           "",
                                           "",
                                           "",
                                           "",
                                           "",
                                           "",
                                           "Hypervisor Injection Exception",
                                           "VMM Communication Exception",
                                           "Security Exception",
                                           ""};

void isr_initialize_gates();

void isr_initialize() {
  isr_initialize_gates();
  for (int i = 0; i < 256; i++)
    idt_enable_gate(i);

  idt_disable_gate(0x80);
}

void __attribute__((cdecl)) isr_handler(registers_t *regs) {
  if (g_ISRHandlers[regs->interrupt] != NULL)
    g_ISRHandlers[regs->interrupt](regs);

  else if (regs->interrupt >= 32)
    printf("Unhandled interrupt %d!\n", regs->interrupt);

  else {
    printf("Unhandled exception %d %s\n", regs->interrupt,
           g_exceptions[regs->interrupt]);

    printf("  eax=%x  ebx=%x  ecx=%x  edx=%x  esi=%x  edi=%x\n", regs->eax,
           regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi);

    printf("  esp=%x  ebp=%x  eip=%x  eflags=%x  cs=%x  ds=%x  ss=%x\n",
           regs->esp, regs->ebp, regs->eip, regs->eflags, regs->cs, regs->ds,
           regs->ss);

    printf("  interrupt=%x  errorcode=%x\n", regs->interrupt, regs->error);

    printf("KERNEL PANIC!\n");
    asm_panic();
  }
}

void isr_register_handler(int interrupt, isr_handler_t handler) {
  g_ISRHandlers[interrupt] = handler;
  idt_enable_gate(interrupt);
}
