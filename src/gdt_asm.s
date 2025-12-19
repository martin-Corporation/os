# GDT from https://github.com/nanobyte-dev/nanobyte_os

.text
    .code32

    .globl i686_GDT_Load
i686_GDT_Load:

    pushl %ebp
    movl %esp, %ebp

    movl 8(%ebp), %eax
    lgdt (%eax)

    movl 12(%ebp), %eax
    pushl %eax
    pushl $reload_cs
    ret

reload_cs:

    movw 16(%ebp), %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    movl %ebp, %esp
    popl %ebp
    ret
