#pragma once

#include "../serial.h"
#include "../vga.h"
#include "stdarg.h"
#include "string.h"
#include <stdbool.h>

inline int puts(const char *s) {
  terminal_writestring(s);
  terminal_writestring("\n");

  serial_writestring(s);
  serial_writestring("\n");

  return strlen(s);
}

inline int putchar(int c) {
#ifdef __wasm__
#include "../wasm/console.h"
  terminal_writestring((const char *)&c);
  serial_writestring((const char *)&c);
#else
  terminal_putchar(c);
  serial_putchar(c);
#endif

  return c;
}

int printf(const char *fmt, ...);
