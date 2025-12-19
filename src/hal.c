#include "hal.h"
#include "idt.h"
#include "irq.h"

void hal_initialize() {
#ifndef __wasm__
#include "gdt.h"
  gdt_initialize();
  idt_initialize();
  isr_initialize();
  irq_initialize();
#endif
}
