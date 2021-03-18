.model tiny
.code

org 100h
                               
CONTROL_SUM 		equ	0719h
start:           ; cleaning up all needed regs
			xor 		ax	,	ax
			xor		cx	,	cx
			xor 		dx	,	dx

		 ; complicated macro for bulling
		 	
			IRP letter, <q, w, e, r, t, y>

					        ; getting symbol
							mov 		ah	,	01h
							int 		21h
							mov		dl	,	al

						; bulling 
							cmp		dx	,	'&letter&'
							shl		dx	,	1
							jne		bullingSkip_&letter
									
							jmp		Print
			bullingSkip_&letter:

						; counting control sum
							and 		dx	,	2h
							cmp 		dx	,	0
							je		addSkip_&letter
							add		cx	,	ax
			addSkip_&letter:

			ENDM

		; control sum check                                               
			cmp		cx	,	(CONTROL_SUM / 3)
			mov		dx	,	offset OtherStr			
			je		Print
			jmp		Check		                                                                                                         
		
		
Print:		; printing info                            
			int 		21h

		
Exit:		; exit prgramm
			mov ax, 4c00h
			int 21h

                        
; data
	MessageStr:		db		"  lmao, fuck u$$"

	FakeStr			db		"  lmao u r gay$"

	OtherStr:		db		"  biba & boba$$$"                   


Check:		; control sum check and some bulling
			
			mov 		ax	, 	0900h
             			
			cmp		cx	,	CONTROL_SUM / 2
			mov		dx	,	offset MessageStr
			je 		Print
                                                                       
			mov		dx	,	offset FakeStr
			mov		dx	,	offset FakeStr
			je		Print


	Shirm:		db		100h dup (0)

			mov 		dx	,	offset MessageStr
			cmp		cx	,	CONTROL_SUM
			je		NoBulling
			jmp		Print		

NoBulling:		; if we don't need to bull user
			
			mov 		di	,	offset FakeStr
			dec		di
			dec 		di
			
			mov		si	,	[di]
			add		si	,	'e' - '$'
			mov   		cs:[di]	,	si

			dec		di	
			mov		si	,	[di]
			add		si	,	'm' - 'u'
			mov   		cs:[di]	,	si                                                                      		

			jmp		Print

end start
