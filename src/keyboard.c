#include "keyboard.h"
#include "irq.h"
#include "pic.h"
#include <asm/io.h>
#include <stdio.h>

#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64

static volatile int shift_pressed = 0;

static void keyboard_handler(registers_t *regs) {
  (void)regs;

  if (inb(KBD_STATUS_PORT) & 0x01) {
    uint8_t scancode = inb(KBD_DATA_PORT);

    if (scancode == 0x2A || scancode == 0x36) {
      shift_pressed = 1;
    } else if (scancode == 0xAA || scancode == 0xB6) {
      shift_pressed = 0;
    }

    if (scancode == 0xE0) {
      return;
    }

    printf("Scancode: %d; shift_pressed: %d\n", scancode, shift_pressed);
  }
}

void keyboard_initialize() {
  irq_register_handler(1, keyboard_handler);
  pic_unmask(1);
}
