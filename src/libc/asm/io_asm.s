.text
  .code32
  .globl asm_panic
  .globl enable_interrupts

asm_panic:
  cli
  hlt

enable_interrupts:
  sti
  ret
