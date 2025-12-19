#pragma once

#include <string.h>

#ifdef __wasm__
extern void js_console_log_args(int32_t ptr, int32_t len, int32_t use_console);

inline void console_log_fmt(const char *str, int32_t use_console) {
  const int len = (int)strlen(str);
  js_console_log_args((int32_t)str, len, use_console);
}
#endif
