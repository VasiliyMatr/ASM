
All:
	nasm -f elf64 print.asm 
	nasm -f elf64 c_in_asm.asm 

	gcc -no-pie -Wall main.c c_in_asm.c c_in_asm.o print.o 
	./a.out