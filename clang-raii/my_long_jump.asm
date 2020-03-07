

.code

my_jmp_buf STRUCT

        _Frame QWORD ?;
        _Rbx QWORD ?;
        _Rsp QWORD ?;
        _Rbp QWORD ?;
        _Rsi QWORD ?;
        _Rdi QWORD ?;
        _R12 QWORD ?;
        _R13 QWORD ?;
        _R14 QWORD ?;
        _R15 QWORD ?;
        _Rip QWORD ?;
        _MxCsr DWORD ?;
        _FpCsr WORD ?;
        _Spare WORD ?;
        _Xmm6 XMMWORD ?;
        _Xmm7 XMMWORD ?;
        _Xmm8 XMMWORD ?;
        _Xmm9 XMMWORD ?;
        _Xmm10 XMMWORD ?;
        _Xmm11 XMMWORD ?;
        _Xmm12 XMMWORD ?;
        _Xmm13 XMMWORD ?;
        _Xmm14 XMMWORD ?;
        _Xmm15 XMMWORD ?;

my_jmp_buf ENDS


;extern "C" int my_setjmp(jmp_buf env);
public my_setjmp

my_setjmp PROC
    
    mov rax, [rsp] ;save ip 
    mov (my_jmp_buf ptr[rcx])._Rip, rax

    lea rax, [rsp + 8] ;save sp before call this function
    mov (my_jmp_buf ptr[rcx])._Rsp, rax
    mov (my_jmp_buf ptr[rcx])._Frame, rax
    
    ;save gprs
    mov (my_jmp_buf ptr[rcx])._Rbx,rbx  
    mov (my_jmp_buf ptr[rcx])._Rbp,rbp  
    mov (my_jmp_buf ptr[rcx])._Rsi,rsi  
    mov (my_jmp_buf ptr[rcx])._Rdi,rdi  
    mov (my_jmp_buf ptr[rcx])._R12,r12  
    mov (my_jmp_buf ptr[rcx])._R13,r13  
    mov (my_jmp_buf ptr[rcx])._R14,r14  
    mov (my_jmp_buf ptr[rcx])._R15,r15  

    ;save fp and xmm
    stmxcsr     (my_jmp_buf ptr[rcx])._MxCsr
    fnstcw      (my_jmp_buf ptr[rcx])._FpCsr
    movdqa      (my_jmp_buf ptr[rcx])._Xmm6,xmm6  
    movdqa      (my_jmp_buf ptr[rcx])._Xmm7,xmm7  
    movdqa      (my_jmp_buf ptr[rcx])._Xmm8,xmm8  
    movdqa      (my_jmp_buf ptr[rcx])._Xmm9,xmm9  
    movdqa      (my_jmp_buf ptr[rcx])._Xmm10,xmm10
    movdqa      (my_jmp_buf ptr[rcx])._Xmm11,xmm11
    movdqa      (my_jmp_buf ptr[rcx])._Xmm12,xmm12
    movdqa      (my_jmp_buf ptr[rcx])._Xmm13,xmm13
    movdqa      (my_jmp_buf ptr[rcx])._Xmm14,xmm14
    movdqa      (my_jmp_buf ptr[rcx])._Xmm15,xmm15

    xor         rax,rax  
    ret  

my_setjmp ENDP


;extern "C" void my_longjmp(jmp_buf env,  int value);

public my_longjmp

my_longjmp PROC
    
    ;restore fp and xmm
    movdqa      xmm15,(my_jmp_buf ptr[rcx])._Xmm15
    movdqa      xmm14,(my_jmp_buf ptr[rcx])._Xmm14
    movdqa      xmm13,(my_jmp_buf ptr[rcx])._Xmm13
    movdqa      xmm12,(my_jmp_buf ptr[rcx])._Xmm12
    movdqa      xmm11,(my_jmp_buf ptr[rcx])._Xmm11
    movdqa      xmm10,(my_jmp_buf ptr[rcx])._Xmm10
    movdqa      xmm9,(my_jmp_buf ptr[rcx])._Xmm9
    movdqa      xmm8,(my_jmp_buf ptr[rcx])._Xmm8
    movdqa      xmm7,(my_jmp_buf ptr[rcx])._Xmm7
    movdqa      xmm6,(my_jmp_buf ptr[rcx])._Xmm6

    fldcw      (my_jmp_buf ptr[rcx])._FpCsr
    ldmxcsr    (my_jmp_buf ptr[rcx])._MxCsr


    ;restore gprs
    mov r15, (my_jmp_buf ptr[rcx])._R15 
    mov r14, (my_jmp_buf ptr[rcx])._R14
    mov r13, (my_jmp_buf ptr[rcx])._R13  
    mov r12, (my_jmp_buf ptr[rcx])._R12  
    mov rdi, (my_jmp_buf ptr[rcx])._Rdi  
    mov rsi, (my_jmp_buf ptr[rcx])._Rsi  
    mov rbp, (my_jmp_buf ptr[rcx])._Rbp  
    mov rbx, (my_jmp_buf ptr[rcx])._Rbx



    ;retore sp
    mov rsp, (my_jmp_buf ptr[rcx])._Rsp
   
    ;restore ip
    mov rcx, (my_jmp_buf ptr[rcx])._Rip; must be the last instruction as rcx modified
    
    ;return value
    mov rax, rdx 

   jmp rcx
my_longjmp ENDP

END