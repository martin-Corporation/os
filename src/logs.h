#pragma once

#include "serial.h"
#include "vga.h"
#include <stddef.h>

typedef struct status {
  const char *text;
  const vga_color_t color;
} status_t;

#define STATUS_OK 0
#define STATUS_WAIT 1
#define STATUS_FAILED 2

extern const status_t status_map[];

#ifdef __wasm__
extern const size_t status_map_size;
#endif

void output_writestring(const char *s);
void puts_status(const status_t status, const char *message);
