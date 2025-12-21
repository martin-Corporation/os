#include "ime.h"
#include "cursor.h"
#include "drivers/keyboard/layouts/us.h"
#include "product.h"
#include "shell.h"
#include "vga.h"
#include <stdio.h>
#include <string.h>

#define PROMPT "# "

static char prompt_buffer[4096] = "";

int backspace_buffer(char *buf) {
  size_t len = strlen(buf);
  if (len == 0)
    return 0;

  buf[len - 1] = '\0';
  return 1;
}

void ime_initialize() {
  terminal_writestring("\n");
  terminal_setcolor(VGA_COLOR_WHITE);
  terminal_writestring(PRODUCT_NAME);
  terminal_writestring(" ");
  terminal_writestring(PRODUCT_VERSION);
  terminal_setcolor(VGA_COLOR_LIGHT_GREY);
  terminal_writestring(" shell (msh)\n");
  terminal_column = 0;
  terminal_writestring(PROMPT);
}

static void terminal_backspace() {
  if (terminal_column > strlen(PROMPT)) {
    terminal_column--;
    terminal_writestring(" ");
    terminal_column--;
    update_cursor(terminal_column, terminal_row);
  }
}

void handle_packet(kbd_packet_t packet) {
  const char key =
      (packet.shift_pressed ? SHIFT_KEYBOARD_US : KEYBOARD_US)[packet.scancode];

  if (key == 0 || packet.event == KEY_UP)
    return;

  if (packet.scancode == 0x0E) {
    terminal_backspace();
    backspace_buffer(prompt_buffer);
    return;
  }

  if (key == '\n') {
    terminal_writestring("\n");

    // handle buffer
    // ...
    handle_command(&*prompt_buffer);

    // empty buffer
    prompt_buffer[0] = '\0';

    terminal_writestring(PROMPT);

    return;
  }

  terminal_writestring(&key);

  const size_t len = strlen(prompt_buffer);
  if (len < sizeof(prompt_buffer) - 1) {
    prompt_buffer[len] = key;
    prompt_buffer[len + 1] = '\0';
  }
}
