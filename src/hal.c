#include "hal.h"
#include "gdt.h"

void hal_initialize() { gdt_initialize(); }
