SECTION .data

SECTION .code
     GLOBAL callBruh
     extern bruhPrint

callBruh:
        CALL    bruhPrint

        RET