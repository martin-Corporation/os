#pragma once

#include "../serial.h"
#include "../vga.h"
#include "stdarg.h"
#include "string.h"
#include <stdbool.h>

int puts(const char *s);
int putchar(int c);
int printf(const char *fmt, ...);
