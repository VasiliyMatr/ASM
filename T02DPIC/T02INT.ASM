.model tiny					; memory addr model
.code 						; code starts

org 100h					; addresation start addr set
                                    
locals @@

VID_MEM_SHT	equ 0b800h
                      
Start:  			; going to needed addr
					xor bx		, 	bx
					mov es		, 	bx
					mov bx		,	9 * 4

				; moving info about old addr
					mov ax			, es:[bx]
					mov Old09Sht		, ax
					mov ax			, es:[bx + 2]
					mov Old09Seg		, ax
				
				; moving info in needed addr
					cli; interrupts off
					mov es:[bx]	, 	offset New09Int
					mov ax		, 	cs
					mov es:[bx + 2]	, 	ax
					sti

				; send the command to DOS so that it does not move this file in memory
					mov ax		, 	3100h
					mov dx		, 	offset EOF
					shr dx		, 	4
					inc dx
					int 21h 
			                             	
Exit:					mov ax		, 	4c00h
					int 			21h

New09Int		PROC

					push ax
					push di
					push es

				; doing stuff with data

					in  al		,	60h
					cmp al		, 	'X'
					jne @@PopRegs

					mov di		,	VID_MEM_SHT
					mov es		,	di
					mov di		,	80 * 5 * 2 + 60 * 2
					mov ah		, 	4eh
					mov al		, 	0afh
					mov es:[di]	,	ax

				;!!! DON'T USE IT FOR NOW !!!;
					; sending info to ports
					; keyboard controller
					;	in  al		, 	61h
					;	mov ah		,	al
					;	or  al		,	80h
					;	out 61h		,	al
					;	mov al		, 	ah
					;	out 61h		,	al
					
					; interrupts controller
					;	mov al		,	20h
					;	out 20h		,	al

				; pushing registers values back

@@PopRegs:				pop es
					pop di
					pop ax
                                           
				; far call cmd code + old interrupt shift					
					db 0eah
Old09Sht				dw 0000h
Old09Seg				dw 0000h 
                                           
			
				; returning	
					iret	

			ENDP

EOF:
end Start
		