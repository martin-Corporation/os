// GDT from https://github.com/nanobyte-dev/nanobyte_os

#include "gdt.h"
#include <stdint.h>

typedef struct gdt_entry {
  uint16_t limit_low;     // limit (bits 0-15)
  uint16_t base_low;      // base (bits 0-15)
  uint8_t base_middle;    // base (bits 16-23)
  uint8_t access;         // access
  uint8_t flags_limit_hi; // limit (bits 16-19) | flags
  uint8_t base_high;      // base (bits 24-31)
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_descriptor {
  uint16_t limit;   // sizeof(gdt) - 1
  gdt_entry_t *ptr; // address of GDT
} __attribute__((packed)) gdt_descriptor_t;

typedef enum gdt_access {
  GDT_ACCESS_CODE_READABLE = 0x02,
  GDT_ACCESS_DATA_WRITEABLE = 0x02,

  GDT_ACCESS_CODE_CONFORMING = 0x04,
  GDT_ACCESS_DATA_DIRECTION_NORMAL = 0x00,
  GDT_ACCESS_DATA_DIRECTION_DOWN = 0x04,

  GDT_ACCESS_DATA_SEGMENT = 0x10,
  GDT_ACCESS_CODE_SEGMENT = 0x18,

  GDT_ACCESS_DESCRIPTOR_TSS = 0x00,

  GDT_ACCESS_RING0 = 0x00,
  GDT_ACCESS_RING1 = 0x20,
  GDT_ACCESS_RING2 = 0x40,
  GDT_ACCESS_RING3 = 0x60,

  GDT_ACCESS_PRESENT = 0x80,

} gdt_access_t;

typedef enum gdt_flags {
  GDT_FLAG_64BIT = 0x20,
  GDT_FLAG_32BIT = 0x40,
  GDT_FLAG_16BIT = 0x00,

  GDT_FLAG_GRANULARITY_1B = 0x00,
  GDT_FLAG_GRANULARITY_4K = 0x80,
} gdt_flags_t;

// Helper macros
#define GDT_LIMIT_LOW(limit) (limit & 0xFFFF)
#define GDT_BASE_LOW(base) (base & 0xFFFF)
#define GDT_BASE_MIDDLE(base) ((base >> 16) & 0xFF)
#define GDT_FLAGS_LIMIT_HI(limit, flags)                                       \
  (((limit >> 16) & 0xF) | (flags & 0xF0))
#define GDT_BASE_HIGH(base) ((base >> 24) & 0xFF)

#define GDT_ENTRY(base, limit, access, flags)                                  \
  {GDT_LIMIT_LOW(limit),                                                       \
   GDT_BASE_LOW(base),                                                         \
   GDT_BASE_MIDDLE(base),                                                      \
   access,                                                                     \
   GDT_FLAGS_LIMIT_HI(limit, flags),                                           \
   GDT_BASE_HIGH(base)}

gdt_entry_t g_gdt[] = {
    // NULL descriptor
    GDT_ENTRY(0, 0, 0, 0),

    // Kernel 32-bit code segment
    GDT_ENTRY(0, 0xFFFFF,
              (GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE_SEGMENT |
               GDT_ACCESS_CODE_READABLE),
              (GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K)),

    // Kernel 32-bit data segment
    GDT_ENTRY(0, 0xFFFFF,
              (GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA_SEGMENT |
               GDT_ACCESS_DATA_WRITEABLE),
              (GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K)),

};

gdt_descriptor_t g_gdt_descriptor = {sizeof(g_gdt) - 1, g_gdt};

#ifndef __wasm__
void __attribute__((cdecl)) i686_GDT_Load(gdt_descriptor_t *descriptor,
                                          uint16_t codeSegment,
                                          uint16_t dataSegment);
#endif

void gdt_initialize() {
#ifndef __wasm__
  i686_GDT_Load(&g_gdt_descriptor, i686_GDT_CODE_SEGMENT,
                i686_GDT_DATA_SEGMENT);
#endif
}
