#include "ime.h"
#include "drivers/keyboard/layouts/us.h"
#include "logs.h"
#include "product.h"
#include "serial.h"
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
  output_writestring("\n");
  terminal_setcolor(VGA_COLOR_WHITE);
  output_writestring(PRODUCT_NAME);
  output_writestring(" ");
  output_writestring(PRODUCT_VERSION);
  terminal_setcolor(VGA_COLOR_LIGHT_GREY);
  output_writestring(" shell (msh)\n");
  output_writestring(PROMPT);
}

static void terminal_backspace() {
  if (terminal_column > strlen(PROMPT)) {
    output_writestring("\b \b");
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
    output_writestring("\n");

    // handle command
    handle_command(prompt_buffer);

    // empty buffer
    prompt_buffer[0] = '\0';

    output_writestring(PROMPT);

    return;
  }

  terminal_putchar(key);
  serial_putchar(key);

  const size_t len = strlen(prompt_buffer);

  if (len < sizeof(prompt_buffer) - 1) {
    prompt_buffer[len] = key;
    prompt_buffer[len + 1] = '\0';
  }
}
