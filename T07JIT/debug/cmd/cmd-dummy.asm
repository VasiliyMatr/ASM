section .code
global _start
_start:

            cmp        eax,     ebx
            pop        rdx
            pop        r10

            call        scanDec
            call        scanDec
            call        scanDec
            call        printDec
            call        exit

;========================================
; IN: NO
; OUT: scanned num in rax
;
; USES: RAX, RDX, RCX, RBX, RSI, RDI
;========================================
scanDec:    ; space for input
            sub         rsp,    16

            xor         rdi,    rdi
            mov         rsi,    rsp
            mov         rdx,    10
            mov         rax,    0
            syscall

            ; input border
            mov         rcx,    10
            mov         rbx,    rax
            add         rbx,    rsi
            dec         rbx

            ; for mul
            mov         rax,    0
            mov         rdx,    0

.CALC:      ; calculating number
            cmp         rsi,    rbx
            je          .CALC_END
            mul         rcx

            ; getting next digit
            mov         rdi,    [ rsi ]
            and         rdi,    255
            sub         rdi,    '0'

            add         rax,    rdi
            inc         rsi
            jmp         .CALC
.CALC_END   ; calculating end

            add         rsp,    16

            ret

;========================================
; IN: RAX - num to print
; OUT: NO
;
; USES: RAX, RDX, RCX, RBX
;========================================
printDec:   ; divider & symbols counter
            mov         rcx,    10
            xor         rbx,    rbx

            ; \n
            push        0x0A

.LOOP       ; counting symbols number
            inc         rbx

            ; dividing
            xor         rdx,    rdx
            div         rcx
            add         rdx,    '0'
            push        rdx

            ; dividing end check
            cmp         rax,    0
            jne         .LOOP

.END        ; symbols count end

            mov         rdx,    rbx
            shl         rdx,    3
            inc         rdx
            inc         rbx

            mov         rsi,    rsp
            xor         rdi,    rdi

            mov         rax,    1
            syscall

.STK_FREE   ; free stack
            add         rsp,    8
            dec         rbx
            cmp         rbx,    0
            jne         .STK_FREE

            ret

exit:       ; exiting
            mov         rax,    0x3C
            xor         rdi,    rdi
            syscall