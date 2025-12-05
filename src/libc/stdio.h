#pragma once

#include "../vga.h"
#include "string.h"

inline int puts(char *s)
{
  terminal_writestring(s);
  terminal_writestring("\n");

  return strlen(s);
}