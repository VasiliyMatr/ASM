.model tiny					; memory addr model
.code 						; code starts

org 100h					; addresation start addr set
                                    
locals @@

; CONSTS
VID_MEM_SHT	equ 0b800h			; video mem start addr 
LINE_SIZE	equ 80				; num of symbols in str
              
Start:  			; int vector change + old int addr write                                                                                                                            
					mov bx		,	8		; int number
					mov di		,	offset old08Sht ; old int will be written here
					mov cx		,	offset New08Int ; new int addr
					call 			SetNewInt
                		
				; send the command to DOS so that it does not move this file in memory
					mov ax		, 	3100h
					; counting num of 16-byte segments in file
					mov dx		, 	offset EOF
					shr dx		, 	4
					inc dx
			                
					; sys int call             	
					int 21h 

;!!!==================================================================================
;  New sys timer interruption proc - outputs regs values info on screen and jmp to 
;  common DOS interrupt proc addr
;!!!==================================================================================
New08Int		PROC
				; saving registers values
					push ax bx cx dx si di es ds
                                                
				; moving data segment here
					push cs
					pop  ds
                                                                                  
				; bx represent (bx is destr in RepresentHex, so should make it's representation at first and manualy)
					; popping additional bx value for representatoion later
					push bx

					; low part represent
					mov si		,	offset @@bxBuff + 6
					call 			RepresentHex

					; high part represent
					pop bx
					mov bl		,	bh
					dec si
					dec si
					call			RepresentHex

				; all segment regs represent in output buff (except bx)                                               
				IRP reg, <a, c, d>
			
					; low part represent
					mov  si		, 	offset @@&reg&xBuff + 6
					mov  bl		, 	reg&l
					call			RepresentHex

					; high part represent
					mov  bl		,	reg&h 
					dec  si
					dec  si
					call			RepresentHex
			
				ENDM
				
				; calling border proc
				        ;    Xlt  Ylt  Xrb  Yrb                 
					push 00   00   09   05
                        	        ; color info
					mov dx	, 0AB00h
                                                     
					call 		 	MakeBorder

				; calling regs values output proc   
					mov di 		,	2
					mov si		,	2 * LINE_SIZE             
					mov bx		,       offset @@Buffs

					call			Print	
                                                                              
				; popping registers values back
@@PopRegs:				pop ds es di si dx cx bx ax
                   
				; far jmp cmd code + old interrupt offset and segment, which was written here by SetNewInt proc					
					db 00eah
Old08Sht				dw 0000h
Old08Seg				dw 0000h


				; buff with registers values
@@Buffs:

@@axBuff:				db "AX: 0000\"
@@bxBuff:				db "BX: ABBA\"
@@cxBuff:				db "CX: 0000\"
@@dxBuff:				db "DX: 0000", 0                                                                                               			                	
			ENDP 

;!!!=====================================================================================
;  Writes new addr to interruptions vector and writes old value to sended addr in ds seg  
;
;  INPUT	:  di - offset to buff for old addr in ds; bx - int proc number;
;                  cx - new int proc offset
;  OUTPUT	:  no
;
;!!DESTR	:  es, ax, bx - DONT LOZE YOUR DATA!
;!!!=====================================================================================
SetNewInt		PROC

				; addrs setup
                			xor ax		,	ax
					mov es		, 	ax
					shl bx		,	2

				; write old addrs
					mov ax		,	es:[bx]
					mov ds:[di]	,	ax
					mov ax		,	es:[bx + 2]
					mov ds:[di + 2]	,	ax 

				; overwrite int addr					
					cli; interr off

					mov es:[bx]	,	cx
					mov es:[bx + 2]	,	cs

					sti; interr on

					ret

			ENDP                                                       		
 
;!!!====================================================================================================
;  Transfers symbols from buff in ds to video memory; '\' - for line break; 0 - for end of line; 
;  works with global LINE_SIZE const
;
;  INPUT	: bx - buff offset in ds; si - start_str_num * str_size * 2; di - str_start_symbol_num * 2
;  OUTPUT	: no
;
;!!DESTR	: bx, di, si, dx, es - DONT LOZE YOUR DATA!
;!!!====================================================================================================
Print		PROC
                                         
		; names for regs    	
		lineShift 		= si; shift to curr line
		strShift 		= di; shift from curr line begin
		data			= dx; data to send to memory
		currSymbol		= dl; printable symbol code
		currColor		= dh; printable symbol color
		carriage		= bx; printable symbol addr

			;!! inits !!;
				; init val for line breaks
					mov basicShift	, strShift
				; es init
					mov dx		, VID_MEM_SHT
					mov es		, dx                                                                           
				; color init
					mov currColor	, 069h
				
				; curr symbol output
		@@OutSymbol:		mov currSymbol	, ds:[carriage]
				 
				; line break?
		@@TermCheck:		cmp currSymbol	, '\'
					jne @@EOSCheck

					add lineShift	, LINE_SIZE * 2
					mov strShift	, basicShift

					inc carriage
					jmp @@OutSymbol

				; end of str?
		@@EOSCheck:		cmp currSymbol, 0
					je  @@Ret

				; writing data
					add lineShift			, strShift
					mov word ptr es:[lineShift]	, data
					sub lineShift			, strShift
				; inc ptrs
					inc strShift
					inc strShift		
					inc carriage

					jmp @@OutSymbol		

		@@Ret:			ret

		basicShift		dw 0
		
		ENDP
                        
                                 
;!!!===============================================================================================================
;  To get Byte string representation in binary format
;
;  INPUT	: cl - Byte; si - destination offset in ds (there should be enough memory - 8 bytes)
;  OUTPUT	: no
;
;!!DESTR	: cx, si - DONT LOSE YOUR DATA
;!!!===============================================================================================================
RepresentBin	PROC

		; names for registers
                valueCpy		= ch
		value			= cl

		pointer			= si
		
	                                               
		@@Cycle:	; copying value
					mov valueCpy		, value
					
				; less bit check
					and valueCpy		, 1 
					cmp valueCpy		, 0
					je @@ZeroPrint

				; prints
		@@OnePrint:		mov byte ptr ds:[pointer], '1'
					jmp @@CycleEnd
		@@ZeroPrint:            mov byte ptr ds:[pointer], '0'

				; counters update and counter check
		@@CycleEnd:		shr value		, 1
					inc pointer
					cmp value		, 0

					jne @@Cycle

					ret
		
		ENDP	                                     
                
;!!!==================================================================================================
;  To get Byte string representation in HEX format
;
;  INPUT	: bl - Byte; si - destination offset in ds (there should be enough memory - 2 bytes)
;  OUTPUT	: no
;
;!!DESTR	: bx - DON'T LOSE YOUR DATA
;!!!==================================================================================================
RepresentHex	PROC                               
		
		; names for registers
		pointer			= si

		; no name for bl or bx - this register is used in diff roles (for symbol code send, numerals addresation and number transformation)
		
				; copying number                                                       
					mov [@@Copy]	, bl
				; need to move by bx offset later
					xor bh		, bh

				; counting less part 
					and bl		, 0fh
				; getting numeral representation & writing data
					mov bl		, cs:[bx + offset @@Numerals]
					mov ds:[pointer], bl

				; updating data
					inc pointer
					mov bl		, [@@Copy]
				; counting high part
					shr bl		, 4

				; getting numeral representation & writing data
					mov bx		, cs:[bx + offset @@Numerals]
					mov ds:[pointer], bl 

				; pointer restoration
					dec pointer                                                     
                                        
					ret

				; all needed numerals in prop order for HEX format
		@@Numerals:		db		"0123456789ABCDEF"

				; num copy here
		@@Copy			db		0
		
		ENDP

;!!!==================================================================================================
;  To get Word string representation in decimal format
;
;  INPUT	: ax - word; si - destination offset in ds (there should be enough memory - 5 bytes)
;  OUTPUT	: no
;
;!!DESTR	: ax, si, dx, cx - DON'T LOSE YOUR DATA
;!!!==================================================================================================
RepresentDec	PROC

		; names for registers
		pointer			= si
		
		divider			= cx
		number    		= ax
		module			= dl 

					mov divider		, 10 				                                  

				; shift to number end  
					add pointer		, 4

				; dx shuld be 0 for divide operation
					mov dx			, 0

				; dividing and working with module
		@@Cycle:		div divider
					add module		, '0'
					mov ds:[pointer]	, module
					
				; if there is 0 in number - all is done
					cmp number		, 0
					je  @@Ret
				
				; updating pointers and module	
					dec pointer
					mov module		, 0
					jmp @@Cycle
					

		@@Ret:			ret
		
		ENDP

;!!!=======================================================================================================
;  Writes square border with sended left-top and righ-bot corners coords in video memory
;
;  INPUT	: Xlt Ylt Xrb Yrb - coords in stack (this order pushed); dh - color; dl - inside filling flag
;  OUTPUT	: no
;
;!!DESTR	: di, es, ax, cx - DON'T LOSE YOUR DATA
;!!!=======================================================================================================
MakeBorder   	PROC
		
		; names for registers              
		colFlag				= dl
		color				= dh
				
					; getting to video memory
						mov di 				, VID_MEM_SHT
						mov es				, di

						pop ax; saving return addr

					; popping values from stack to local variables
					IRP coord, <Yrb, Xrb, Ylt, Xlt>

						pop di
						mov coord			, di

					ENDM                            
                                    			
                                    		push ax; pushing return addr back on top of stack

					;!!! data calculations !!!;

					; counting width and height
					IRP axis, <X, Y>

						mov di				, axis&rb
						mov brSize&axis			, di
						mov di				, axis&lt
						sub brSize&axis			, di

					ENDM	
	
					; counting shift to left-top corner           
						mov ax				, Ylt
						mov cl				, LINE_SIZE
						mul cl
						add ax				, Xlt

					; writing data to vars (then just will plus other addition shifts to coords)
					IRP corner, <leftTop, righTop, leftBot, righBot>

						mov corner&Shift		, ax

					ENDM   

					; additional shifts calculation and additions

						; horizontal shift                           					
						mov ax				, brSizeX
						add righTopShift		, ax
						add righBotShift		, ax

						; vertical shift
						mov ax				, brSizeY
						mov cl				, LINE_SIZE
						mul cl
						add leftBotShift		, ax
						add righBotShift		, ax

					; multiply x2 (there are color and symbol bytes)
					IRP corner, <leftTop, righTop, leftBot, righBot>

						shl corner&Shift		, 1

					ENDM

					;!!! data calculations ends !!!;

		; now di is used as pointer only
		pointer				= di


		; symbols names
		vertSymbol			= 0cdh
		horizSymbol			= 0bah                                    	
                                                                                
		;!!! printing top and bot lines !!!;
		IRP side, <leftTop, leftBot>    
					
					; ptr init  
		                        	mov pointer			, side&Shift 
					                                               
					; init color
						mov ah				, color
					; top and bot lines symbol
						mov al				, vertSymbol
					; counter setup
						mov cl				, byte ptr brSizeX
						mov ch				, 0

					; shifting counter and pointer  
						inc pointer
						inc pointer
						dec cx

					; cycle
						cld
						repe stosw       

		ENDM                    

		;!!! printing left and right borders !!!;
		IRP side, <leftTop, righTop>

					; ptr init
						mov pointer			, side&Shift
					
					; init color
						mov ah				, color
					; top and bot lines symbol
						mov al				, horizSymbol

					; counter setup
						mov cx				, brSizeY

						add pointer			, LINE_SIZE * 2
						dec cx

		@@BrLoop&side:			mov es:[pointer]		, ax
						add pointer			, LINE_SIZE * 2
						loop				@@BrLoop&side
					

		ENDM                      

					; corners symbols 
		leftTopCr		= 0c9h
		righTopCr		= 0bbh
		leftBotCr		= 0c8h
		righBotCr		= 0bch

					; sending corners
					IRP corner, <righTop, leftBot, righBot, leftTop>

						mov pointer			, corner&Shift
						mov byte ptr es:[pointer]	, corner&Cr 
						mov es:[pointer + 1]		, color

					ENDM
		
		 				cmp colFlag			, 0
						je @@Ret

		                        ;!!! filling in internal space !!!;

					; 2 counters: cl and ch - for lines and cols
						mov cl				, byte ptr brSizeX						
						mov ch				, byte ptr brSizeY

						dec cl
						dec ch

					; pointer was inited in last macro, but additional shift needed
						add pointer			, 2 + LINE_SIZE * 2

		@@FillLoop:			mov byte ptr es:[pointer]	, ' '
						mov byte ptr es:[pointer + 1] 	, color
						
						inc pointer
						inc pointer
						dec cl

						cmp cl				, 0
						jne @@FillLoop

						mov cl				, byte ptr brSizeX
						dec cl

						sub pointer			, brSizeX
						sub pointer			, brSizeX
						add pointer			, LINE_SIZE * 2 + 2
						
						dec ch
						cmp ch				, 0
						jne @@FillLoop						

			           
		@@Ret:				ret

					;!!! local variables !!!;

					; corners coords - inputs
						Xlt 		dw			0
						Ylt 		dw			0
						Xrb 		dw			0
						Yrb 		dw			0

					; border width and height
						brSizeX	        dw			0
						brSizeY		dw			0

					; shifts to corners (in bytes)
						leftTopShift 	dw			0
						righTopShift 	dw			0
						leftBotShift 	dw			0
						righBotShift 	dw			0   		
		ENDP


EOF:
end Start