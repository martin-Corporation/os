#include "../serial.h"
#include "../vga.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static const char g_hex_chars[] = "0123456789abcdef";

int printf_unsigned(unsigned long long number, int radix) {
  char buffer[65];
  int pos = 0;
  int printed = 0;

  do {
    unsigned long long rem = number % radix;
    number /= radix;
    buffer[pos++] = g_hex_chars[(int)rem];
  } while (number > 0);

  while (--pos >= 0) {
    putchar(buffer[pos]);
    printed++;
  }
  return printed;
}

int printf_signed(long long number, int radix) {
  if (number < 0) {
    putchar('-');
    return printf_unsigned((unsigned long long)(-number), radix) + 1;
  }
  return printf_unsigned((unsigned long long)number, radix);
}

#define PRINTF_STATE_NORMAL 0
#define PRINTF_STATE_LENGTH 1
#define PRINTF_STATE_LENGTH_SHORT 2
#define PRINTF_STATE_LENGTH_LONG 3
#define PRINTF_STATE_SPEC 4

#define PRINTF_LENGTH_DEFAULT 0
#define PRINTF_LENGTH_SHORT_SHORT 1
#define PRINTF_LENGTH_SHORT 2
#define PRINTF_LENGTH_LONG 3
#define PRINTF_LENGTH_LONG_LONG 4

int printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  int printed = 0;
  int state = PRINTF_STATE_NORMAL;
  int length = PRINTF_LENGTH_DEFAULT;
  int radix = 10;
  bool sign = false;
  bool number = false;

  while (*fmt) {
    switch (state) {
    case PRINTF_STATE_NORMAL:
      if (*fmt == '%') {
        state = PRINTF_STATE_LENGTH;
      } else {
        putchar(*fmt);
        printed++;
      }
      break;

    case PRINTF_STATE_LENGTH:
      switch (*fmt) {
      case 'h':
        length = PRINTF_LENGTH_SHORT;
        state = PRINTF_STATE_LENGTH_SHORT;
        break;
      case 'l':
        length = PRINTF_LENGTH_LONG;
        state = PRINTF_STATE_LENGTH_LONG;
        break;
      default:
        goto handle_spec;
      }
      break;

    case PRINTF_STATE_LENGTH_SHORT:
      if (*fmt == 'h') {
        length = PRINTF_LENGTH_SHORT_SHORT;
        state = PRINTF_STATE_SPEC;
      } else {
        goto handle_spec;
      }
      break;

    case PRINTF_STATE_LENGTH_LONG:
      if (*fmt == 'l') {
        length = PRINTF_LENGTH_LONG_LONG;
        state = PRINTF_STATE_SPEC;
      } else {
        goto handle_spec;
      }
      break;

    case PRINTF_STATE_SPEC:
    handle_spec:
      switch (*fmt) {
      case 'c': {
        const char ch = (char)va_arg(args, int);
        putchar(ch);
        printed++;
        break;
      }
      case 's': {
        const char *s = va_arg(args, const char *);
        terminal_writestring(s);
        serial_writestring(s);
        printed += (int)strlen(s);
        break;
      }
      case '%': {
        putchar('%');
        printed++;
        break;
      }
      case 'd':
      case 'i':
        radix = 10;
        sign = true;
        number = true;
        break;
      case 'u':
        radix = 10;
        sign = false;
        number = true;
        break;
      case 'X':
      case 'x':
      case 'p':
        radix = 16;
        sign = false;
        number = true;
        break;
      case 'o':
        radix = 8;
        sign = false;
        number = true;
        break;
      default:
        break;
      }

      if (number) {
        if (sign) {
          switch (length) {
          case PRINTF_LENGTH_SHORT_SHORT:
          case PRINTF_LENGTH_SHORT:
          case PRINTF_LENGTH_DEFAULT:
            printed += printf_signed(va_arg(args, int), radix);
            break;
          case PRINTF_LENGTH_LONG:
            printed += printf_signed(va_arg(args, long), radix);
            break;
          case PRINTF_LENGTH_LONG_LONG:
            printed += printf_signed(va_arg(args, long long), radix);
            break;
          }
        } else {
          switch (length) {
          case PRINTF_LENGTH_SHORT_SHORT:
          case PRINTF_LENGTH_SHORT:
          case PRINTF_LENGTH_DEFAULT:
            printed += printf_unsigned(va_arg(args, unsigned int), radix);
            break;
          case PRINTF_LENGTH_LONG:
            printed += printf_unsigned(va_arg(args, unsigned long), radix);
            break;
          case PRINTF_LENGTH_LONG_LONG:
            printed += printf_unsigned(va_arg(args, unsigned long long), radix);
            break;
          }
        }
      }

      state = PRINTF_STATE_NORMAL;
      length = PRINTF_LENGTH_DEFAULT;
      radix = 10;
      sign = false;
      number = false;
      break;
    }

    fmt++;
  }

  va_end(args);
  return printed;
}
