section .data	; Data segment
msg db	"Hello World!", 0x0a;

section .text	; text segment
global _start   ; Default elf entry linking

_start:

; Syscall: write(1, msg, 14)

mov eax, 4
mov ebx, 1
mov ecx, msg
mov edx, 14
int 0x80


; Syscall: exit(0)

mov eax, 1
mov ebx, 0
int 0x80
