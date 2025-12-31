#include "console.h"

#ifdef __wasm__
#include <stdint.h>
#include <string.h>
void console_log_fmt(const char *str, int32_t use_console) {
  const int len = (int)strlen(str);
  // FIXME: js_console_log_args doesn't work
  // js_console_log_args((int32_t)str, len, use_console);
}
#endif
