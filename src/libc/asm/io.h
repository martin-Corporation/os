#pragma once

#include <stdint.h>

inline void outb(uint16_t port, uint8_t val) {
#ifndef __wasm__
  __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
#endif
  /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers
   * that fit in 8b. (N constraint). Wider immediate constants would be
   * truncated at assemble-time (e.g. "i" constraint). The outb al, %dx
   * encoding is the only option for all other cases. %1 expands to %dx because
   * port is a uint16_t. %w1 could be used if we had the port number a wider C
   * type */
}

inline uint8_t inb(uint16_t port) {
#ifndef __wasm__
  uint8_t ret;
  __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
#else
  return -1;
#endif
}

inline void io_wait() { outb(0x80, 0); }
void __attribute__((cdecl)) asm_panic();
void __attribute__((cdecl)) enable_interrupts();
