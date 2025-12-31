#pragma once

#ifdef __wasm__
extern void asm(const char *instruction);
#endif
