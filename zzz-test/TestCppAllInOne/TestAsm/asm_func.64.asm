
ifdef _X86_64

.data
func qword 0

.code 


;//number starting from 1
setvar macro number:req, reg:req, stacksize:req
mov qword ptr [rsp + stacksize - number * sizeof(qword)], reg
endm

getvar macro reg:req, number:req, stacksize:req
mov reg, qword ptr [rsp + stacksize - number * sizeof(qword)]
endm


;//number starting from 1
setparam macro number:req, reg:req, stacksize:req
mov qword ptr [rsp + stacksize + number * sizeof(qword)], reg
endm

getparam macro reg:req, number:req, stacksize:req
mov reg, qword ptr [rsp + stacksize + number * sizeof(qword)]
endm


;//number starting from 1
setsubparam macro number:req, reg:req
mov qword ptr [rsp + (number - 1) * sizeof(qword)], reg
endm

getsubparam macro reg:req, number:req
mov reg, qword ptr [rsp + (number - 1) * sizeof(qword)]
endm


STACK_SIZE equ 16*8



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SetFunc proc public

mov qword ptr [func], rcx
ret

SetFunc endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RipRelJump proc public
lea rax, [func]
jmp qword ptr [func]
ret

RipRelJump endp 


CallProc proc public

mov [rsp - 8], rcx
jmp qword ptr [rsp - 8]
ret

CallProc endp


InspectBytecode proc public
ret

mov rax, rdx
mov eax, edx
mov ax, dx
mov al, dl

InspectBytecode endp


StdProc proc public
call InspectBytecode
ret
StdProc endp

endif

end