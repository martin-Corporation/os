#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum kbd_packet_event { KEY_DOWN, KEY_UP } kbd_packet_event_t;

typedef struct kbd_packet {
  const uint8_t scancode;
  const int shift_pressed;
  const kbd_packet_event_t event;
} kbd_packet_t;

void handle_packet(kbd_packet_t packet);
void ime_initialize();
