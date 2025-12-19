// PIC from https://github.com/nanobyte-dev/nanobyte_os

#include "pic.h"
#include <asm/io.h>

#define PIC1_COMMAND_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_COMMAND_PORT 0xA0
#define PIC2_DATA_PORT 0xA1

// Initialization Control Word 1
// -----------------------------
//  0   IC4     if set, the PIC expects to receive ICW4 during initialization
//  1   SGNL    if set, only 1 PIC in the system; if unset, the PIC is cascaded
//  with slave PICs
//              and ICW3 must be sent to controller
//  2   ADI     call address interval, set: 4, not set: 8; ignored on x86, set
//  to 0 3   LTIM    if set, operate in level triggered mode; if unset, operate
//  in edge triggered mode 4   INIT    set to 1 to initialize PIC 5-7 ignored on
//  x86, set to 0

enum pic_icw1 {
  PIC_ICW1_ICW4 = 0x01,
  PIC_ICW1_SINGLE = 0x02,
  PIC_ICW1_INTERVAL4 = 0x04,
  PIC_ICW1_LEVEL = 0x08,
  PIC_ICW1_INITIALIZE = 0x10
} pic_icw1_t;

// Initialization Control Word 4
// -----------------------------
//  0   uPM     if set, PIC is in 80x86 mode; if cleared, in MCS-80/85 mode
//  1   AEOI    if set, on last interrupt acknowledge pulse, controller
//  automatically performs
//              end of interrupt operation
//  2   M/S     only use if BUF is set; if set, selects buffer master;
//  otherwise, selects buffer slave 3   BUF     if set, controller operates in
//  buffered mode 4   SFNM    specially fully nested mode; used in systems with
//  large number of cascaded controllers 5-7         reserved, set to 0
enum pic_icw4 {
  PIC_ICW4_8086 = 0x1,
  PIC_ICW4_AUTO_EOI = 0x2,
  PIC_ICW4_BUFFER_MASTER = 0x4,
  PIC_ICW4_BUFFER_SLAVE = 0x0,
  PIC_ICW4_BUFFERRED = 0x8,
  PIC_ICW4_SFNM = 0x10,
} pic_icw4_t;

enum pic_cmd {
  PIC_CMD_END_OF_INTERRUPT = 0x20,
  PIC_CMD_READ_IRR = 0x0A,
  PIC_CMD_READ_ISR = 0x0B,
} pic_cmd_t;

void pic_configure(uint8_t offset_pic_1, uint8_t offset_pic_2) {
  // initialization control word 1
  outb(PIC1_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
  io_wait();
  outb(PIC2_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
  io_wait();

  // initialization control word 2 - the offsets
  outb(PIC1_DATA_PORT, offset_pic_1);
  io_wait();
  outb(PIC2_DATA_PORT, offset_pic_2);
  io_wait();

  // initialization control word 3
  outb(PIC1_DATA_PORT,
       0x4); // tell PIC1 that it has a slave at IRQ2 (0000 0100)
  io_wait();
  outb(PIC2_DATA_PORT, 0x2); // tell PIC2 its cascade identity (0000 0010)
  io_wait();

  // initialization control word 4
  outb(PIC1_DATA_PORT, PIC_ICW4_8086);
  io_wait();
  outb(PIC2_DATA_PORT, PIC_ICW4_8086);
  io_wait();

  // clear data registers
  outb(PIC1_DATA_PORT, 0);
  io_wait();
  outb(PIC2_DATA_PORT, 0);
  io_wait();
}

void pic_send_end_of_interrupt(int irq) {
  if (irq >= 8)
    outb(PIC2_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT);
  outb(PIC1_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT);
}

void pic_disable() {
  outb(PIC1_DATA_PORT, 0xFF); // mask all
  io_wait();
  outb(PIC2_DATA_PORT, 0xFF); // mask all
  io_wait();
}

void pic_mask(int irq) {
  uint8_t port;

  if (irq < 8) {
    port = PIC1_DATA_PORT;
  } else {
    irq -= 8;
    port = PIC2_DATA_PORT;
  }

  uint8_t mask = inb(port);
  outb(port, mask | (1 << irq));
}

void pic_unmask(int irq) {
  uint8_t port;

  if (irq < 8) {
    port = PIC1_DATA_PORT;
  } else {
    irq -= 8;
    port = PIC2_DATA_PORT;
  }

  uint8_t mask = inb(port);
  outb(port, mask & ~(1 << irq));
}

uint16_t pic_read_irq_request_register() {
  outb(PIC1_COMMAND_PORT, PIC_CMD_READ_IRR);
  outb(PIC2_COMMAND_PORT, PIC_CMD_READ_IRR);
  return ((uint16_t)inb(PIC2_COMMAND_PORT)) |
         (((uint16_t)inb(PIC2_COMMAND_PORT)) << 8);
}

uint16_t pic_read_in_service_register() {
  outb(PIC1_COMMAND_PORT, PIC_CMD_READ_ISR);
  outb(PIC2_COMMAND_PORT, PIC_CMD_READ_ISR);
  return ((uint16_t)inb(PIC2_COMMAND_PORT)) |
         (((uint16_t)inb(PIC2_COMMAND_PORT)) << 8);
}
