

.code 


;void	SmlCVector_Push(PSmlCVector obj, INT_PTR data);
extern SmlCVector_Push :  proc

;SML_DATA_PTR GetRetAddr(PSmlCVector vec, int enabled, int reserved1, int reserved2);
public Sml_AsmGetRetAddr

Sml_AsmGetRetAddr proc

;rax = retaddr
mov rax, [rsp]

;if enabled == 0
;quick return
cmp edx, 0 
je __return

;allocate enough stack space, or the sub proc will overwrite the retaddr
STACK_SIZE_GRA equ 128
sub rsp, STACK_SIZE_GRA

mov rdx, rax
mov [rsp], rcx
mov [rsp + 8], rdx
call SmlCVector_Push

add rsp, STACK_SIZE_GRA

__return:
;rax = retaddr
mov rax, [rsp]
ret


Sml_AsmGetRetAddr endp


;void Sml_PrepareJmpEnd(BYTE* src, BYTE* target, IN OUT volatile long* inited);
extern Sml_PrepareJmpEnd : proc

;void Sml_RunCleanup(BYTE* first, BYTE* last, IN OUT volatile long* inited);
public Sml_AsmRunCleanup

Sml_AsmRunCleanup proc

;allocate enough stack space, or the sub proc will overwrite the retaddr
STACK_SIZE_RC equ 120
sub rsp, STACK_SIZE_RC

mov [rsp + STACK_SIZE_RC - 8], r8
mov [rsp + STACK_SIZE_RC - 16], rdx
mov [rsp + STACK_SIZE_RC - 24], rcx

mov rcx, [rsp + STACK_SIZE_RC - 16] ; src = last
mov rdx, [rsp + STACK_SIZE_RC] ;target = retAddr
mov r8, [rsp + STACK_SIZE_RC - 8]; inited parameter
call Sml_PrepareJmpEnd

cmp  qword ptr [rsp + STACK_SIZE_RC - 24], 0 ;if first == 0
je _return

; add one more 8 to rsp, restore stack, caller allocted paramters stack, 
;call instruction place retaddr on statck
add rsp, STACK_SIZE_RC + 8
jmp qword ptr [rsp - 32] ; jmp to first

_return:
add rsp, STACK_SIZE_RC
ret

Sml_AsmRunCleanup endp



end

