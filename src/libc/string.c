#include <string.h>

size_t strspn(const char *cs, const char *ct) {
  size_t n;
  const char *p;
  for (n = 0; *cs; cs++, n++) {
    for (p = ct; *p && *p != *cs; p++)
      ;
    if (!*p)
      break;
  }
  return n;
}

static int char_in_set(char c, const char *set) {
  while (*set) {
    if (c == *set)
      return 1;
    ++set;
  }
  return 0;
}

size_t strcspn(const char *s, const char *reject) {
  const char *p = s;

  while (*p && !char_in_set(*p, reject)) {
    ++p;
  }

  return (size_t)(p - s);
}
