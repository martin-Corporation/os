#include <stdarg.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC push_options
#pragma GCC optimize("-fomit-frame-pointer")

long syscall(long number, ...) {
  va_list ap;
  va_start(ap, number);

  long a1 = va_arg(ap, long);
  long a2 = va_arg(ap, long);
  long a3 = va_arg(ap, long);
  long a4 = va_arg(ap, long);
  long a5 = va_arg(ap, long);
  long a6 = va_arg(ap, long);

  va_end(ap);

#ifndef __wasm__
  register long _num asm("eax") = number;
  register long _arg1 asm("ebx") = a1;
  register long _arg2 asm("ecx") = a2;
  register long _arg3 asm("edx") = a3;
  register long _arg4 asm("esi") = a4;
  register long _arg5 asm("edi") = a5;
  register long _arg6 asm("ebp") = a6;

  asm volatile("int $0x80"
               : "+r"(_num)
               : "r"(_arg1), "r"(_arg2), "r"(_arg3), "r"(_arg4), "r"(_arg5),
                 "r"(_arg6)
               : "memory");

  return _num;
#else
#include "../../isr.h"
#include "../../syscall.h"

  registers_t regs = {.eax = number,
                      .ebx = a1,
                      .ecx = a2,
                      .edx = a3,
                      .esi = a4,
                      .edi = a5,
                      .ebp = a6,
                      .esp = 0,
                      .eip = 0,
                      .eflags = 0,
                      .cs = 0,
                      .ds = 0,
                      .ss = 0,
                      .interrupt = 128,
                      .error = 0};

  syscall_handler(&regs);
  return regs.eax;
#endif
}

#pragma GCC pop_options
#pragma GCC diagnostic pop
