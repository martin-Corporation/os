#pragma once

#include <stddef.h>
#include <stdlib.h>

size_t strnlen(const char *s, size_t maxlen);
size_t strlen(const char *str);
int strcmp(const char *s1, const char *s2);
char *strcat(char *dest, const char *src);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t len);
void *memmove(void *dest, const void *src, size_t n);
char *strcpy(char *dest, const char *src);
char *strndup(const char *s, size_t n);
char *strdup(const char *s);
size_t strspn(const char *cs, const char *ct);
size_t strcspn(const char *s, const char *reject);
