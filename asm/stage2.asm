org 0x0
bits 16
push es ; -> 0x8000
pop ds ; <- data segment is now 0x8000
jmp _kernel

%include "inc/print.inc"

delay1sec:
    mov     cx, 0FH
    mov     dx, 4240H
    mov     ah, 86H
    int     15H
    ret
_kernel:
    cli
    
    mov si, kernel_load_success_msg
    call print

    hlt
kernel_load_success_msg db "Kernal loaded 0x8000", 0xa, 0
times 512-($-$$) db 0