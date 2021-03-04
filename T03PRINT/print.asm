SECTION .data
; format str for print proc
    formatStr:      db 'Hello $cworld!', 10, 0

; Addrs table for fast jumps depend on symbol 
    JMPAddrs:                               dq myPrint.BinHandler   ; 'b' - binary format 1 byte
                                            dq myPrint.CharHandler  ; 'c' - char
                                            dq myPrint.DecHandler   ; 'd' - decimal format 2 bytes
                    times ('g' - 'd')       dq myPrint.Return       ; ('d', 'h') - not supported  
                                            dq myPrint.HexHandler   ; 'h' - hexidecimal format 1 byte
                    times ('r' - 'h')       dq myPrint.Return       ; ('h', 's') - not supported
                                            dq myPrint.StringHandler; 's' - string format

SECTION .code
     GLOBAL myPrint 
; 
; _start:
;     PUSH    '$'
;     PUSH    formatStr 
; 
;     CALL    myPrint
;     ADD     RSP     ,   8 * 2
; 
;     ; Terminate program
;     MOV     RAX     ,   1               ; 'exit' system call
;     MOV     RBX     ,   0               ; exit with error code 0
;     INT     80h                         ; call the kernel


;=========================================================================================
;  Printf function  
;
;  INPUT:   __cdecl__ - as usual printf
;  OUTPUT:  NO
;
;  DESTR:   RAX, RBX, RCX, RDX, RSI, RDI
;=========================================================================================

; Names for registers
    %define handlerAddr     RBX
    %define symbol           AL
    %define addrsShift      RCX
    %define formatAddr      RSI
    %define argsCounter     RDI

; Names for consts 
    END_OF_STR_     EQU  0 ; end of str code
    FORMAT_TERM_    EQU '$'; format output term symbol
    STD_IO_MODE_    EQU  4 ; input/output syscall mode
    STD_OUT_        EQU  1 ; output func is used

    %macro SYS_PRINT 0 
                ; MOV     RCX                 ,   POINTER
                ; MOV     RDX                 ,   LEN

                    MOV     RAX                 ,   STD_IO_MODE_
                    MOV     RBX                 ,   STD_OUT_
                    INT     80h
    %endmacro

myPrint:                ; TODO: stosw
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

                    ; Getting handler addr and jump to handler - [8 * code + base addr]
                        MOV     handlerAddr         ,   [8 * RAX + (JMPAddrs - 8 * 'b')]
                        JMP     handlerAddr

        ; Handlers for diff output formats
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

    .Return:            RET


SEGMENT .data
                    ; Represented values buff
                        RepresentBuff:  db "0123456789ABCDEFGH"  
SEGMENT .code

;!!!===============================================================================================================
;  To get Byte string representation in binary format (works with RepresentBuff)
;
;  INPUT     : RAX - Byte;
;  OUTPUT    : RDX - num of symbols; RCX - ptr to represent begin (can have less symbols)
;
;  DESTR     : RAX, RCX, RDX - DONT LOSE YOUR DATA
;!!!===============================================================================================================
; CONSTS
    SHT_TO_END      EQU 8

; names for registers
    %define valueCpy    AH
    %define value       AL

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
;  DESTR     : RAX, RDX, RCX - DON'T LOSE YOUR DATA
;!!!==================================================================================================
; names for registers
    %define pointer     RCX
    %define numOfSymb   RDX
    %define Copy        DL
RepresentHex:

                    ; ptr to buff
                        MOV     pointer             ,   RepresentBuff         
                    ; copying number                               
                        MOV     Copy                ,   AL
                    ; getting less part 
                        AND     RAX                 ,   0fh

                    ; getting numeral representation & writing data
                        MOV     AL                  ,   [RAX + Numerals]
                        MOV     [pointer + 1]       ,   AL

                    ; copying data
                        MOV     AL                  ,   Copy
                    ; counting high part
                        SHR     AL                  ,   4

                    ; getting numeral representation & writing data
                        MOV     AL                  ,   [EAX + Numerals]
                        MOV     [pointer]           ,   AL   

                    ; num of symbols
                        MOV     numOfSymb           ,   2                                                    
           
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
; CONSTS 
    NUM_OF_DIG      EQU 5

; names for registers
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