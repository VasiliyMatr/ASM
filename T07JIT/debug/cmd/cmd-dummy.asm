section .code
global _start
_start:

    push    'A'
    mov     rsi                 ,   rsp
    mov     rdx                 ,   1

    mov     rax                 ,   1
    syscall

    add     rsp                 ,   8

    push     r8
    push     r9
    push     r10
    push     r11

    mov eax, 0x3C
    xor rdi, rdi
    syscall