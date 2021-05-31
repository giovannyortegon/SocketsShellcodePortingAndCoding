	global _start
_start:
	xor eax, eax
	jmp short string

code:
	pop esi
	push byte 15
	push esi
	push byte 1
	mov al, 4
	push eax
	int 0x80

	xor eax, eax
	push eax
	push eax
	mov al, 1
	int 0x80

string:
	call code
	db "Hello, World!", 0x0a
