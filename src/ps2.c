#include "ps2.h"
#include <asm/io.h>

int get_status_register(int bit) { return inb(0x64) & bit; }
