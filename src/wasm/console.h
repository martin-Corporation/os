#pragma once

#ifdef __wasm__
#include <stdint.h>
extern void js_console_log_args(int32_t ptr, int32_t len, int32_t use_console);

void console_log_fmt(const char *str, int32_t use_console);
#endif
