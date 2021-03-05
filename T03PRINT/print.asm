SECTION .data
; format str for print proc
    formatStr:      db 'Hello $cworld!', 10, 0

; Addrs table for fast jumps depend on symbol 
    JMPAddrs:                               dq bruhPrint.BinHandler   ; 'b' - binary format 1 byte
                                            dq bruhPrint.CharHandler  ; 'c' - char
                                            dq bruhPrint.DecHandler   ; 'd' - decimal format 2 bytes
                    times ('g' - 'd')       dq bruhPrint.Return       ; ('d', 'h') - not supported  
                                            dq bruhPrint.HexHandler   ; 'h' - hexidecimal format 1 byte
                    times ('r' - 'h')       dq bruhPrint.Return       ; ('h', 's') - not supported
                                            dq bruhPrint.StringHandler; 's' - string format

SECTION .code
     GLOBAL bruhPrint

;=========================================================================================
;  Printf function  
;
;  INPUT:   __stdcall as usual printf
;  OUTPUT:  NO
;
;  DESTR:   RAX, RBX, RCX, RDX, RSI, RDI
;=========================================================================================

; Names for registers
    %define handlerAddr     RBX ; - handler lable addr will be in this register
    %define symbol           AL ; - synbol will be in this register
    %define formatAddr      RSI ; - addr of parseble symbol in format str
    %define argsCounter     RDI ; - arguments counter

; Names for consts 
    END_OF_STR_     EQU  0      ; - end of str
    FORMAT_TERM_    EQU '$'     ; - format output term symbol
; Names for syscall consts
    STD_IO_MODE_    EQU  4      ; - input/output syscall mode
    STD_OUT_        EQU  1      ; - output func is used

    %macro SYS_PRINT 0 
                ;   MOV     RCX                 ,   POINTER
                ;   MOV     RDX                 ,   LEN

                    MOV     RAX                 ,   STD_IO_MODE_
                    MOV     RBX                 ,   STD_OUT_
                    INT     80h
    %endmacro

bruhPrint:                

                    ; making __cdecl format
                        POP     RBX

                        PUSH R9
                        PUSH R8
                        PUSH RCX
                        PUSH RDX
                        PUSH RSI
                        PUSH RDI
                        
                        PUSH    RBX

                    ; getting str addr & init arguments counter
                        MOV     formatAddr          ,   [RSP + 8]
                        MOV     argsCounter         ,   2

    ; Checking next symbol - common, format or END_OF_STR_
    .Cycle:         ; RAX is used later as offset => it should contain only symbol
                        XOR     RAX                 ,   RAX
                    ; Symbol
                        MOV     symbol              ,   [formatAddr]
                    ; Format symbol check
                        CMP     symbol              ,   FORMAT_TERM_
                        JE      .Format
                    ; EOS check
                        CMP     symbol              ,   END_OF_STR_     
                        JE      .Return

        ; Just need to output symbol
        .Common:        MOV     RCX                 ,   formatAddr
                        MOV     RDX                 ,   1
                        SYS_PRINT

                        INC     formatAddr
                        JMP     .Cycle

        ; Format output needed
        .Format:    ; Getting next symbol
                        INC     formatAddr
                        MOV     symbol              ,   [formatAddr]
                    ; Symbol  limits check
                        CMP     symbol              ,   'b'
                        JL      .Return
                        CMP     symbol              ,   's'
                        JA      .Return
                    ; Skipping format symbol
                        INC     formatAddr

                    ; Getting handler addr and jump to handler - [8 * symbol-code + base addr]
                        MOV     handlerAddr         ,   [8 * RAX + (JMPAddrs - 8 * 'b')]
                        JMP     handlerAddr

        ; Handlers for diff output formats macro
    %macro HANDLE_DEF 1

        .%1Handler: MOV     RAX                 ,   [RSP + argsCounter * 8]
                        INC     argsCounter

                        CALL    Represent%1
                        SYS_PRINT

                        JMP     .Cycle

    %endmacro

         HANDLE_DEF Bin
         HANDLE_DEF Hex
         HANDLE_DEF Dec

        .CharHandler:   MOV     RAX                 ,   [RSP + argsCounter * 8]
                        INC     argsCounter

                        MOV     [RepresentBuff]     ,   AL

                        MOV     RCX                 ,   RepresentBuff
                        MOV     RDX                 ,   1
                        SYS_PRINT

                        JMP     .Cycle


        .StringHandler: MOV     RCX                 ,   [RSP + argsCounter * 8] 
                        INC     argsCounter
                        MOV     RDX                 ,   [RSP + argsCounter * 8]
                        INC     argsCounter

                        SYS_PRINT

                        JMP     .Cycle

    .Return:            POP     RBX   
                        ADD     RSP                 ,   6 * 8
                        PUSH    RBX         
    
                        RET

SEGMENT .data
                    ; Represented values buff
                        RepresentBuff:  times (64) db 0   
SEGMENT .code

;!!!===============================================================================================================
;  To get Byte string representation in binary format (works with RepresentBuff)
;
;  INPUT     : RAX - Byte;
;  OUTPUT    : RDX - num of symbols; RCX - ptr to represent begin (can have less symbols)
;
;  DESTR     : RAX, RBX, RCX, RDX - DONT LOSE YOUR DATA
;!!!===============================================================================================================
; Consts
    SHT_TO_END      EQU 64

; Names for registers
    %define valueCpy    RBX
    %define value       RAX

    %define buffSh      RDX
    %define buffPtr     RCX
RepresentBin:           

                    ; offset init
                        MOV     buffSh              ,   0
                                                   
    .Cycle:         ; add one symbol shift
                        DEC     buffSh

                    ; copying value
                        MOV     valueCpy            ,   value
                    
                    ; less bit check
                        AND     valueCpy            ,   1 
                        CMP     valueCpy            ,   0
                        JE      .ZeroPrint

                    ; prints
        .OnePrint:      MOV     [buffSh + RepresentBuff + SHT_TO_END]  ,   BYTE '1'
                        JMP     .CycleEnd

        .ZeroPrint:     MOV     [buffSh + RepresentBuff + SHT_TO_END]  ,   BYTE '0'

                    ; value check
        .CycleEnd       SHR     value               ,   1
                        CMP     value               ,   0

                        JNE     .Cycle

                    ; str pointer setup
                        NEG     buffSh
                        MOV     buffPtr             ,   RepresentBuff + SHT_TO_END
                        SUB     buffPtr             ,   buffSh
                        
                        RET   

;!!!==================================================================================================
;  To get Byte string representation in HEX format (works with RepresentBuff)
;
;  INPUT     : RAX - byte;
;  OUTPUT    : RDX - num of symbols; RCX - ptr to represent begin
;
;  DESTR     : RAX, RBX, RDX, RCX - DON'T LOSE YOUR DATA
;!!!==================================================================================================
; Names for registers
    %define numOfSymb   RDX
    %define pointer     RCX
    %define value       RAX
    %define currByte    BL
RepresentHex:

                    ; ptr to buff
                        MOV     pointer             ,   RepresentBuff
                    ; shift in buff
                        MOV     numOfSymb           ,   0
                    ; will be used as addr shift
                        MOV     RBX                 ,   0

.Cycle:             ; getting numeral representation & writing data
                        MOV     BL                              ,   AL
                        AND     BL                              ,   0xF
                        MOV     BL                              ,   [RBX + Numerals]
                        MOV     [pointer + numOfSymb + 15]      ,   BL

                    ; shifting value
                        SHR     RAX                 ,   4
                        DEC     numOfSymb           

                    ; end of cycle check
                        CMP     RAX                 ,   0
                        JNE     .Cycle    

                        ADD     pointer             ,   16
                        ADD     pointer             ,   numOfSymb
                        NEG     numOfSymb
                        RET    

SEGMENT .data
                    ; all needed numerals in prop order for HEX format
                        Numerals:       db "0123456789ABCDEF"  
SEGMENT .code                

;!!!==================================================================================================
;  To get Word string representation in decimal format (works with RepresentBuff)
;
;  INPUT     : AX - word;
;  OUTPUT    : RDX - num of symbols; RCX - ptr to represent begin
;
;  DESTR     : RAX, RBX, RCX, RDX - DON'T LOSE YOUR DATA
;!!!==================================================================================================
; Consts 
    NUM_OF_DIG      EQU 5

; Names for registers
    %define buffSh      RBX
    
    %define divider     CX
    %define number      AX
    %define modul       DL 
RepresentDec:
                        MOV     divider             ,   10                                                   

                    ; shift to number end  
                        MOV     buffSh              ,   0

                    ; dx shuld be 0 for divide operation
                        MOV     DX                  ,   0

                    ; dividing and working with module
    .Cycle:             DEC     buffSh
                        DIV     divider
                        ADD     modul               ,   '0'
                        MOV     [buffSh + RepresentBuff + NUM_OF_DIG]  ,   modul
                        
                    ; if there is 0 in number - all is done
                        CMP     number              ,   0
                        JE      .Return
                    
                    ; updating pointers and module
                        MOV     modul               ,   0
                        JMP     .Cycle

    .Return:            NEG     buffSh
                        MOV     RDX                 ,   buffSh
                        MOV     RCX                 ,   RepresentBuff + NUM_OF_DIG
                        SUB     RCX                 ,   buffSh

                        RET