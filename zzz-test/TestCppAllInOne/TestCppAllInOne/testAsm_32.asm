

.model flat

.code

public _TestAsm
_TestAsm proc

jmp _return

push 0abcdef12h
ret

mov eax, 0abcdef12h
jmp eax
pop eax
nop
nop

push eax
mov eax, 0abcdef12h
jmp eax
nop
nop

_return:
ret

_TestAsm endp

end