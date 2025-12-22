#include <asm/io.h>

#ifndef __wasm__
uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}
#endif

#ifndef __wasm__
void outb(uint16_t port, uint8_t val) {
  __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
  /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers
   * that fit in 8b. (N constraint). Wider immediate constants would be
   * truncated at assemble-time (e.g. "i" constraint). The outb al, %dx
   * encoding is the only option for all other cases. %1 expands to %dx because
   * port is a uint16_t. %w1 could be used if we had the port number a wider C
   * type */
}
#endif

#ifdef __wasm__
#define MAX_QUEUE 16

typedef struct {
  uint8_t data[MAX_QUEUE];
  int head;
  int tail;
} PortQueue;

// A queue for each port
static PortQueue queues[256];

// Last value written with outb (optional for inspection)
static uint8_t outb_table[256];

void queue_inb_data(uint16_t port, uint8_t value) {
  PortQueue *q = &queues[port];
  int next = (q->tail + 1) % MAX_QUEUE;
  if (next != q->head) { // avoid overflow
    q->data[q->tail] = value;
    q->tail = next;
  }
}

uint8_t get_outb_state(uint16_t port) { return outb_table[port]; }

uint8_t inb(uint16_t port) {
  if (port == 0x64) {
    // status: bit 0 = output buffer full if queue non‑empty
    PortQueue *q = &queues[0x60];
    if (q->head != q->tail) {
      return 0x01;
    }
    return 0x00;
  }

  if (port < 256) {
    PortQueue *q = &queues[port];
    if (q->head != q->tail) {
      uint8_t val = q->data[q->head];
      q->head = (q->head + 1) % MAX_QUEUE;
      return val;
    }
  }
  return 0;
}

void outb(uint16_t port, uint8_t val) { outb_table[port] = val; }
#endif

void io_wait() { outb(0x80, 0); }
