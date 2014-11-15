BITS 32 	; tell nasm that its 32 bit code

call mark_below  ;
db "Hello, world!", 0x0a, 0x0d   ; 

mark_below:
;  ssize_t write(int fd, const void *buf, size_t count);

pop ecx
mov eax, 4
mov ebx, 1
mov edx, 15
int 0x80


; void exit(int status)

mov eax, 1
mov ebx, 0
int 0x80
