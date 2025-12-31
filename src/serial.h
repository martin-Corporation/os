#pragma once

int serial_initialize();
int serial_received();
char read_serial();
int is_transmit_empty();
void serial_putchar(char a);
void serial_writestring(const char *s);
