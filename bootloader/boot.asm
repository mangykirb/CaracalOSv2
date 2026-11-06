
[org 0x7C00]
bits 16
start:
    cld
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov si, msg
.print:
    lodsb
    or al, al
    jz load_kernel
    mov ah, 0x0E
    int 0x10
    jmp .print
load_kernel:
    mov ah, 0x02        
    mov al, 64         
    mov ch, 0
    mov cl, 2           
    mov dh, 0
    mov bx, 0x1000  
    int 0x13
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:protected_mode
gdt_start:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF 
    dq 0x00CF92000000FFFF 
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start


[bits 32]
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov esp, 0x90000
    jmp 0x08:0x1000

msg db "e", 0

times 510-($-$$) db 0
dw 0xAA55