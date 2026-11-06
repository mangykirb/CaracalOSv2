BITS 32

GLOBAL user_entry
GLOBAL enter_user_mode
GLOBAL syscall_stub

EXTERN syscall_handler
EXTERN user_main
EXTERN user_entry

SECTION .usertext

user_entry:
    call user_main

.hang:
    jmp .hang

SECTION .text

enter_user_mode:
    cli

    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push dword 0x23
    push dword 0x00402000
    push dword 0x02
    push dword 0x1B
    push dword user_entry

    iretd

syscall_stub:
    cld
    pushad

    xor eax, eax
    mov ax, ds
    push eax

    xor eax, eax
    mov ax, es
    push eax

    xor eax, eax
    mov ax, fs
    push eax

    xor eax, eax
    mov ax, gs
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, [esp + 44]
    push eax

    call syscall_handler
    add esp, 4

    pop eax
    mov gs, ax

    pop eax
    mov fs, ax

    pop eax
    mov es, ax

    pop eax
    mov ds, ax

    popad
    iretd
