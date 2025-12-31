#pragma once

#include <stdint.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void io_wait();

#ifdef __wasm__
void queue_inb_data(uint16_t port, uint8_t value);
uint8_t get_outb_state(uint16_t port);
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);
#else
void outw(uint16_t port, uint16_t val);
uint16_t inw(uint16_t port);
void outl(uint16_t port, uint32_t val);
uint32_t inl(uint16_t port);
void __attribute__((cdecl)) asm_panic();
void __attribute__((cdecl)) enable_interrupts();
#endif
