# IDT from https://github.com/nanobyte-dev/nanobyte_os

.text
  .code32
  .globl i686_IDT_Load

i686_IDT_Load:
  pushl %ebp
  movl %esp, %ebp

  movl 8(%ebp), %eax
  lidt (%eax)

  movl %ebp, %esp
  popl %ebp
  ret
