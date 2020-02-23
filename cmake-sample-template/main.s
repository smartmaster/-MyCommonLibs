INCLUDE main.inc

.code

PUBLIC main_s

; return the ip following the function call
; void* main_s()

main_s PROC

	mov rax, [rsp]
	ret

main_s ENDP

END
