section .text

exit:
    mov rax, 0x3c
    syscall

string_length:
    xor rax, rax
    jmp .loop
.increment:
    inc rax
.loop:
    cmp byte [rdi+rax], 0x0
    je .end
    jmp .increment
.end:
    ret

print_string:
    push rdi
    call string_length
    pop rsi
    mov rdx, rax
    mov rdi, 1
    mov rax, 1
    syscall
    ret

print_char:
    push rdi
    mov rdi, rsp
    call print_string
    pop rdi
    ret

print_newline:
    mov rdi, 10
    call print_char
    ret

print_uint:
    xor rdx, rdx
    mov rax, rdi
    mov rcx, 0xa
    dec rsp
    mov byte [rsp], 0x0
.loop:
    div rcx
    add rdx, 0x30
    dec rsp
    mov byte [rsp], dl
    xor rdx, rdx
    cmp rax, 0x0
    jne .loop
    mov rdi, rsp
    call print_string
    inc rsp
    add rsp, rax
    ret

print_int:
    cmp rdi, 0
    jl  .add_signal
    jmp .end
.add_signal:
    push rdi
    mov rdi, '-'
    call print_char
    pop rdi
    mov rax, rdi
    mov r8, -1
    mul r8
    mov rdi, rax
.end:
    call print_uint
    ret

read_char:
    push 0
    mov rdi, 0x0
    mov rsi, rsp
    mov rdx, 0x1
    mov rax, 0x0
    syscall
    pop rax
    ret

read_word:
    xor rcx, rcx
.iterate:
    push rcx
    push rdi
    push rsi
    call read_char
    pop rsi
    pop rdi
    pop rcx
    cmp rax, 0x20
    je .iterate
    cmp rax, 0x10
    je .iterate
    cmp rax, 0x9
    je .iterate
    cmp rax, 0
    je .end
    mov byte [rdi+rcx], al
    inc rcx
    cmp rcx, rsi
    jb .iterate
    ja .error
.end:
    mov byte [rdi+rcx], 0
    mov rax, rdi
    ret
.error:
    mov rax, 0
    ret

parse_uint:
    xor rax, rax
    xor rdx, rdx
    mov r10, 10
.loop:
    cmp byte [rdi+rcx], 0x30
    jnge .end
    cmp byte [rdi+rcx], 0x39
    jnle .end
    inc rdx
    push rdx
    mul r10
    pop rdx
    mov r8b, [rdi+rcx]
    sub r8b, 0x30
    add rax, r8
    inc rcx
    jmp .loop
.end:
    ret

parse_int:
    mov r10, -1
    cmp byte [rdi], '-'
    jne .end
    push rdi
    push r10
    inc rdi
    call parse_uint
    pop r10
    push rdx
    mul r10
    pop rdx
    inc rdx
    pop rdi
.end:
    ret

string_equals:
    xor rcx, rcx
    push rcx
    call string_length
    pop rcx
    jmp .loop
.increment:
    inc rcx
.loop:
    mov r8b, [rdi+rcx]
    mov r9b, [rsi+rcx]
    cmp r8b, r9b
    jne .not_equals
    cmp rcx, rax
    jl .increment
    mov rax, 1
    ret
.not_equals:
    mov rax, 0
    ret

string_copy:
    call string_length
    cmp rdx, rax
    jnae .error
    xor rcx, rcx
    jmp .loop
.increment:
    inc rcx
.loop:
    mov dl, [rdi+rcx]
    mov [rsi+rcx], dl
    cmp rcx, rdx
    jl .increment
    mov rax, rsi
    ret
.error:
    mov rax, 0
    ret
