section .data
  msg db "Hello world",0xa ; 0xa is '\n'

section .text
  global _start

  _start:
    ; syscall 1 (write)
    ; rdi: unsigned int fd
    ; rsi: const char *msg
    ; rdx: size_t count
    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, 13
    syscall
    ; syscall 60 (exit)
    ; rdi: int error_code
    mov rax, 60
    xor edi, edi
    syscall
