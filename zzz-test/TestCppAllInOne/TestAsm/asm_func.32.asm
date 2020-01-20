
ifdef _X86_32

.model flat

.code

_SetFunc proc public
ret
_SetFunc endp


_RipRelJump proc public
ret
_RipRelJump endp



_CallProc proc public
ret
_CallProc endp



_InspectBytecode proc public
ret
;mov rax, rdx
mov eax, edx
mov ax, dx
mov al, dl
_InspectBytecode endp



StdProc proc stdcall public, arg0:dword, arg1:dword, arg2:dword
mov eax, [ebp + 8]
mov eax, [ebp + 12]
mov eax, [ebp + 16]
ret 12
StdProc endp


endif

end