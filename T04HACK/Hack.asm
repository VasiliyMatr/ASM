.model tiny
.code

org 100h

start:
			xor 	si	,	si

                      	L MACRO letter
				mov 		ah	,	01h
				int 		21h
				cmp 		al	, 	letter
				JNE 		PROGRAMM
                        ENDM

			L "P"
			L "A"
			L "S"
			L "S"
			L "W"
			L "O"
			L "R"
			L "D"
DEFAULT_READ:		L "$"

			jmp	CHECK						                    

PROGRAMM:		mov 	cs:[READEN + si]	,	al
			jmp	DEFAULT_READ
							




READEN:			db "true-password"

CHECK:

end start