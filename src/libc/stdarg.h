// Source - https://stackoverflow.com/a
// Posted by luser droog
// Retrieved 2025-12-01, License - CC BY-SA 3.0

/* Copyright (c) 1992 David I. Bell
 * Permission is granted to use, distribute, or modify this source,
 * provided that this copyright notice remains intact.
 */

/*
 * SIMULATE_STDARG
 *
 * WARNING: This type of stdarg makes assumptions about the stack
 *             that may not be true on your system.  You may want to
 *            define STDARG (if using ANSI C) or VARARGS.
 */

#ifndef __wasm__
typedef char *va_list;
#define va_start(ap, parmn) (void)((ap) = (char *)(&(parmn) + 1))
#define va_end(ap) (void)((ap) = 0)
#define va_arg(ap, type) (((type *)((ap) = ((ap) + sizeof(type))))[-1])
#else
typedef __builtin_va_list va_list;
#define va_start(ap, last_fixed) __builtin_va_start(ap, last_fixed)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)
#endif
