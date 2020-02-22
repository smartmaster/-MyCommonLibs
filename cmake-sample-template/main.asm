INCLUDE main.inc

.code

main_asm PROC

	push 1
	push 2

	pop rax
	pop rax

	ret

main_asm ENDP

END
