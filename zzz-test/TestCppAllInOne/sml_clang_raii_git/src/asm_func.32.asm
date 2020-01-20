

ifdef X86_32


.model flat
.code 


;//number starting from 0
setvar macro number:req, reg:req, stacksize:req
mov dword ptr [esp + stacksize - (number + 1) * sizeof(dword)], reg
endm

getvar macro reg:req, number:req, stacksize:req
mov reg, dword ptr [esp + stacksize - (number + 1) * sizeof(dword)]
endm


;//number starting from 0
setparam macro number:req, reg:req, stacksize:req
mov dword ptr [esp + stacksize + (number + 1) * sizeof(dword)], reg
endm

getparam macro reg:req, number:req, stacksize:req
mov reg, dword ptr [esp + stacksize + (number + 1) * sizeof(dword)]
endm


;//number starting from 0
setsubparam macro number:req, reg:req
mov dword ptr [esp + number * sizeof(dword)], reg
endm

getsubparam macro reg:req, number:req
mov reg, dword ptr [esp + number * sizeof(dword)]
endm



STACK_SIZE equ 16*4

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;BYTE* Sml_GetRetAddr(SmlCVector* vec_blocks, long initing_local, BYTE** pretaddr);
extern _Sml_GetRetAddr :  proc

;SML_DATA_PTR /*__stdcall*/ Sml_AsmGetRetAddr(PSmlCVector vec, int enabled, int reserved1, int reserved2);
_Sml_AsmGetRetAddr proc public


;allocate enough stack space, or the sub proc will overwrite the retaddr
sub esp, STACK_SIZE

lea eax, [esp + STACK_SIZE]
setsubparam 2, eax


getparam eax, 1, STACK_SIZE
setsubparam 1, eax


getparam eax, 0, STACK_SIZE
setsubparam 0, eax
call _Sml_GetRetAddr

add esp, STACK_SIZE
ret


_Sml_AsmGetRetAddr endp



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;void Sml_LinkAndRunCleanups(LONG localIniting, LONG volatile* globalInited, SmlCVector* vec_jmp, LPBYTE* pretddr);
extern _Sml_LinkAndRunCleanups : proc

;void /*__stdcall*/ Sml_AsmLinkAndRunCleanups(LONG localIniting, LONG volatile* globalInited, SmlCVector* vec_jmp);
_Sml_AsmLinkAndRunCleanups proc public

sub esp, STACK_SIZE

lea eax, [esp + STACK_SIZE]
setsubparam 3, eax


getparam eax, 2, STACK_SIZE
setsubparam 2, eax


getparam eax, 1,  STACK_SIZE
setsubparam 1, eax


getparam eax, 0, STACK_SIZE
setsubparam 0, eax
call _Sml_LinkAndRunCleanups

add esp, STACK_SIZE
ret

_Sml_AsmLinkAndRunCleanups endp

endif

end
