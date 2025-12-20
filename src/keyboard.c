#include "keyboard.h"
#include "irq.h"
#include "logs.h"
#include "pic.h"
#include <asm/io.h>
#include <stdio.h>

#define PS2_CMD 0x64
#define PS2_DATA 0x60

static volatile int shift_pressed = 0;

static void keyboard_handler(registers_t *regs) {
  (void)regs;
  uint8_t sc = inb(PS2_DATA);
  if (sc == 0x2A || sc == 0x36)
    shift_pressed = 1;
  else if (sc == 0xAA || sc == 0xB6)
    shift_pressed = 0;
}

static void ps2_wait_write_ready() {
  while (inb(PS2_CMD) & 0x02)
    ;
}

static void ps2_wait_output() {
  while (!(inb(PS2_CMD) & 0x01))
    ;
}

void keyboard_initialize() {

  outb(PS2_CMD, 0xAD);
  ps2_wait_write_ready();
  outb(PS2_CMD, 0xA7);
  ps2_wait_write_ready();
  while (inb(PS2_CMD) & 0x01)
    inb(PS2_DATA);
  ps2_wait_write_ready();
  outb(PS2_CMD, 0x20);
  ps2_wait_output();
  uint8_t config = inb(PS2_DATA);
  config |= 0x01;
  ps2_wait_write_ready();
  outb(PS2_CMD, 0x60);
  ps2_wait_write_ready();
  outb(PS2_DATA, config);
  ps2_wait_write_ready();
  outb(PS2_DATA, 0xF4);
  ps2_wait_output();
  if (inb(PS2_DATA) != 0xFA)
    return;
  outb(PS2_CMD, 0xAE);
  pic_unmask(1);
  irq_register_handler(1, keyboard_handler);
  puts_status(status_map[STATUS_OK], "Initialized keyboard driver");
}
