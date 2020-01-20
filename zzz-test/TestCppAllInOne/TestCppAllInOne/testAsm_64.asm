

.code

public TestAsm

TestAsm proc

jmp _return


jmp qword ptr[rsp]
jmp qword ptr[rsp-8]
jmp qword ptr[rsp-16]
jmp qword ptr[rsp-24]
jmp qword ptr[rsp-32]

jmp qword ptr[0abcdef1234567890h]


push 0abcdef12h
ret

int 3
int 3
mov rax, 0abcdef1234567890h
jmp rax
pop rax
nop
nop

int 3
int 3

push rax
mov rax, 0abcdef1234567890h
jmp rax
nop
nop

int 3
int 3

_return:
ret

TestAsm endp

end