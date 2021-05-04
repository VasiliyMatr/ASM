section .code
global _start
_start:

            mov         eax,    131
            call        printDec

            mov         eax,    0x3C
            xor         rdi,    rdi
            syscall

;========================================
; IN: NO
; OUT: scaned num in eax
;
; USES: EAX, EDX, ECX, EBX
;========================================
scanDec:
            sub         rsp,    8
            

;========================================
; IN: EAX - num to print
; OUT: NO
;
; USES: EAX, EDX, ECX, EBX
;========================================
printDec:
            mov         rcx,    10
            xor         ebx,    ebx

.LOOP       ; counting symbols number
            inc         ebx

            ; dividing
            xor         rdx,    rdx
            div         ecx
            add         rdx,    '0'
            push        rdx

            ; dividing end check
            cmp         eax,    0
            jne         .LOOP

.END        ; symbols count end

            mov         edx,    ebx
            shl         edx,    3
            inc         edx

            mov         rsi,   rsp

            mov         rax,    1
            syscall

.STK_FREE   ; free stack
            add         rsp,    8
            dec         ebx
            cmp         ebx,    0
            jne         .STK_FREE

            ret