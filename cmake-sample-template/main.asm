INCLUDE main.inc

.code


PUBLIC main_asm

; return the ip following the function call
; void* main_asm()

main_asm PROC

	mov rax, [rsp]
	ret

main_asm ENDP

END
