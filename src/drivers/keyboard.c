#include "../logs.h"
#include "keyboard/ps2.h"

void keyboard_initialize() {
  ps2_keyboard_initialize();
  puts_status(status_map[STATUS_OK], "Initialized keyboard driver");
}
