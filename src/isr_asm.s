# ISR from https://github.com/nanobyte-dev/nanobyte_os

.code32
.extern isr_handler
.macro ISR_NOERRORCODE num
.globl i686_ISR\num

i686_ISR\num:
  pushl $0
  pushl $\num
  jmp isr_common
.endm

.macro ISR_ERRORCODE num
.globl i686_ISR\num

i686_ISR\num:
  pushl $\num
  jmp isr_common
.endm

.include "src/isrs_gen.inc"

isr_common:
  pusha

  xorl %eax, %eax
  movw %ds, %ax
  pushl %eax

  movw $0x10, %ax
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs

  pushl %esp
  call isr_handler
  addl $4, %esp

  popl %eax
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs

  popa
  addl $8, %esp
  iret
