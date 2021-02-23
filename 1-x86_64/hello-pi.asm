section .text
	global _start

	_start:
		jmp msg ; jump & call trick
	print:
		mov rax, 1
		mov rdi, 1
		pop rsi
		mov rdx, 12
		syscall
		mov rax, 60
		xor edi, edi
		syscall
	msg:
		call print
		db "Hello world",0xa
