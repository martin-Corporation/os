// IRQ from https://github.com/nanobyte-dev/nanobyte_os

#include "irq.h"
#include "isr.h"
#include "pic.h"
#include <asm/io.h>
#include <stddef.h>

#define PIC_REMAP_OFFSET 0x20

irq_handler_t g_irq_handlers[16];

void i686_IRQ_Handler(registers_t *regs) {
  int irq = regs->interrupt - PIC_REMAP_OFFSET;

  // uint8_t pic_isr = pic_read_in_service_register();
  // uint8_t pic_irr = pic_read_irq_request_register();

  // printf("IRQ %d  ISR=%x  IRR=%x...\n", irq, pic_isr, pic_irr);

  if (g_irq_handlers[irq] != NULL) {
    // handle IRQ
    g_irq_handlers[irq](regs);
  } else {
    // printf("Unhandled IRQ %d  ISR=%x  IRR=%x...\n", irq, pic_isr, pic_irr);
  }

  // send EOI
  pic_send_end_of_interrupt(irq);
}

void irq_initialize() {
  pic_configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);

  // register ISR handlers for each of the 16 irq lines
  for (int i = 0; i < 16; i++)
    isr_register_handler(PIC_REMAP_OFFSET + i, i686_IRQ_Handler);

  // enable interrupts
  enable_interrupts();
}

void irq_register_handler(int irq, irq_handler_t handler) {
  g_irq_handlers[irq] = handler;
}
