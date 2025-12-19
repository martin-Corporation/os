#include "idt.h"
#include "util/binary.h"
#include <stdint.h>

typedef struct idt_entry {
  uint16_t base_low;
  uint16_t segment_selector;
  uint8_t reserved;
  uint8_t flags;
  uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_descriptor {
  uint16_t limit;
  idt_entry_t *ptr;
} __attribute__((packed)) idt_descriptor_t;

idt_entry_t g_idt[256];

idt_descriptor_t g_idt_descriptor = {sizeof(g_idt) - 1, g_idt};

#ifndef __wasm__
void __attribute__((cdecl)) i686_IDT_Load(idt_descriptor_t *idt_descriptor);
#endif

void idt_set_gate(int interrupt, void *base, uint16_t segment_descriptor,
                  uint8_t flags) {
  g_idt[interrupt].base_low = ((uint32_t)base) & 0xFFFF;
  g_idt[interrupt].segment_selector = segment_descriptor;
  g_idt[interrupt].reserved = 0;
  g_idt[interrupt].flags = flags;
  g_idt[interrupt].base_high = ((uint32_t)base >> 16) & 0xFFFF;
}

void idt_enable_gate(int interrupt) {
  FLAG_SET(g_idt[interrupt].flags, IDT_FLAG_PRESENT);
}

void idt_disable_gate(int interrupt) {
  FLAG_UNSET(g_idt[interrupt].flags, IDT_FLAG_PRESENT);
}

void idt_initialize() {
#ifndef __wasm__
  i686_IDT_Load(&g_idt_descriptor);
#endif
}
