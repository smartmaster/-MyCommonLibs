INCLUDE main.inc

.code

main_s PROC

	push 1
	push 2

	pop rax
	pop rax

	ret

main_s ENDP

END
