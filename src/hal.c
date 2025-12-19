#include "hal.h"

void hal_initialize() {
#ifndef __wasm__
#include "gdt.h"
  gdt_initialize();
#endif
}
