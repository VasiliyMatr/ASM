section .code
global _start
_start:

    push    'A'
    mov     rsi                 ,   rsp
    mov     rdx                 ,   1

    mov     rax                 ,   1
    syscall

    add     rsp                 ,   8

    mov     r8d                 ,   eax   
    mov     r9d                 ,   eax
    mov     r10d                ,   eax
    mov     r11d                ,   eax
    mov     ebx                 ,   r8d
    mov     ebx                 ,   r9d
    mov     ebx                 ,   r10d
    mov     ebx                 ,   r11d

    mov eax, 0x3C
    xor rdi, rdi
    syscall