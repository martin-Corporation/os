#include "hal.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"

void hal_initialize() {
#ifndef __wasm__
  gdt_initialize();
  idt_initialize();
  isr_initialize();
  irq_initialize();
#endif
}
