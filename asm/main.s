add:                ; assume 2 or more integer arguments
    pop rdx         ; pop return address
    pop rdi         ; copy # of args to RDI
    pop rax         ; copy first arg out
    dec rdi         ; dec # of args left
    pop rbx         ; copy second arg out
    dec rdi         ; dec # of args left
add_loop:
    add rax, rbx    ; compute sum
    cmp 0, rdi      ; check for remaining args
    je ret_lbl      ; if none, jmp to return label
    pop rbx         ; otherwise, pop next arg and jmp back to loop
    jmp add_loop
ret_lbl:
    push rax        ; push answer to stack
    push rdx        ; push return addr back to ret works
    ret


cons_local:
    pop rdx         ; pop return address - require only 2 args
    pop rax         ; pop value arg
    lea rsp, rdi    ; get value of first LL item
    push rdi        ; push ptr to first item to stack
    push rax        ; push value
    push rdx        ; push return address
    ret


cons_non_local:
    pop rdx
    pop

cdr_local:                            ; assume LL is on stack
    pop rdx
    pop rax
    pop rax
    push rax
    push rdx
    ret
