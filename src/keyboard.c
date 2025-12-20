#include "keyboard.h"
#include "irq.h"
#include "logs.h"
#include "pic.h"
#include "ps2.h"
#include <asm/io.h>
#include <stdio.h>

#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64

static volatile int shift_pressed = 0;

static void keyboard_handler(registers_t *regs) {
  (void)regs;

  if (inb(KBD_STATUS_PORT) & 0x01) {
    uint8_t scancode = inb(KBD_DATA_PORT);

    if (scancode == 250 && shift_pressed == 0) {
      printf("ACK byte\n");
      return;
    }

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
  const int status = inb(0x64);
  printf("PS/2 controller status: as %cd: %d, as %cc: %c\n", '%', status, '%',
         status);

  outb(0x60, 0xF4);
  if (inb(0x60) != 0xFA)
    return puts_status(status_map[STATUS_FAIL],
                       "Failed to initialize keyboard");

  outb(0x64, 0xAD);

  const int res_1 = inb(0x64);

  outb(0x64, 0xA7);

  const int res_2 = inb(0x64);

  printf("res_1 (outb(0x64, 0xAD)): %d\n", res_1);
  printf("res_2 (outb(0x64, 0xA7)): %d\n", res_2);

  // drain keyboard (bit 1 i think)
  outb(0x64, 0xAE);

  while (get_status_register(0x01))
    ;

  printf("get_status_register(0x00): %d\n", get_status_register(0x00));

  // This bootloops/hangs:
  // while (inb(0x60))
  //   ;

  const int status_port_before = inb(0x64);
  outb(0x64, 0x20);
  outb(0x64, 0x60);
  const int status_port_now = inb(0x64);

  printf("status port before: %d; status port now: %d\n", status_port_before,
         status_port_now);

  irq_register_handler(1, keyboard_handler);
  pic_unmask(1);
}
