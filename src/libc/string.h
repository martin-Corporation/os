#pragma once

#include <stddef.h>
#include <stdlib.h>

inline size_t strnlen(const char *s, size_t maxlen) {
  size_t i = 0;
  while (i < maxlen && s[i] != '\0') {
    i++;
  }
  return i;
}

inline size_t strlen(const char *str) {
  size_t len = 0;

  while (str[len])
    len++;

  return len;
}

inline int strcmp(const char *s1, const char *s2) {
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }

  return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

inline char *strcat(char *dest, const char *src) {
  while (*dest != '\0') {
    dest++;
  }

  while (*src != '\0') {
    *dest = *src;
    dest++;
    src++;
  }

  *dest = '\0';

  return dest;
}

inline void *memcpy(void *dest, const void *src, size_t n) {
  char *csrc = (char *)src;
  char *cdest = (char *)dest;

  for (size_t i = 0; i < n; i++) {
    cdest[i] = csrc[i];
  }

  return dest;
}

inline void *memset(void *s, int c, size_t len) {
  unsigned char *p = (unsigned char *)s;

  while (len--) {
    *p++ = (unsigned char)c;
  }

  return s;
}

inline char *strcpy(char *dest, const char *src) {
  if (dest == NULL || src == NULL)
    return NULL;

  char *temp = dest;
  while ((*dest++ = *src++) != '\0')
    ;
  return temp;
}

inline char *strndup(const char *s, size_t n) {
  size_t len = strnlen(s, n);
  char *d = (char *)malloc(len + 1);
  if (!d)
    return NULL;
  memcpy(d, s, len);
  d[len] = '\0';
  return d;
}

inline char *strdup(const char *s) { return strndup(s, strlen(s)); }

size_t strspn(const char *cs, const char *ct);
size_t strcspn(const char *s, const char *reject);
char *strtok(const char *s, const char *delim);
