
bits 32

global vga_mode13_enable

section .text

vga_mode13_enable:
    pushad

    mov dx, 0x3C4
    xor al, al
    out dx, al

    inc dx
    mov al, 1
    out dx, al

    mov dx, 0x3C2
    mov al, 0x63
    out dx, al

    mov esi, seq_regs
    mov ecx, 4
    mov ebx, 1

.seq:
    mov dx, 0x3C4
    mov al, bl
    out dx, al

    mov dx, 0x3C5
    lodsb
    out dx, al

    inc bl
    loop .seq

    mov dx, 0x3D4
    mov al, 0x11
    out dx, al

    mov dx, 0x3D5
    in al, dx
    and al, 0x7F
    out dx, al

    mov esi, crtc_regs
    xor ebx, ebx
    mov ecx, 25

.crtc:
    mov dx, 0x3D4
    mov al, bl
    out dx, al

    mov dx, 0x3D5
    lodsb
    out dx, al

    inc bl
    loop .crtc

    mov esi, gc_regs
    xor ebx, ebx
    mov ecx, 9

.gc:
    mov dx, 0x3CE
    mov al, bl
    out dx, al

    mov dx, 0x3CF
    lodsb
    out dx, al

    inc bl
    loop .gc

    mov esi, ac_regs
    xor ebx, ebx
    mov ecx, 21

.ac:
    mov dx, 0x3DA
    in al, dx

    mov dx, 0x3C0
    mov al, bl
    out dx, al

    lodsb
    out dx, al

    inc bl
    loop .ac

    mov dx, 0x3DA
    in al, dx

    mov dx, 0x3C0
    mov al, 0x20
    out dx, al

    mov dx, 0x3C4
    xor al, al
    out dx, al

    inc dx
    mov al, 3
    out dx, al

    popad
    ret

section .rodata

seq_regs:
    db 0x01, 0x0F, 0x00, 0x0E

crtc_regs:
    db 0x5F, 0x4F, 0x50, 0x82, 0x54
    db 0x80, 0xBF, 0x1F, 0x00, 0x41
    db 0x00, 0x00, 0x00, 0x00, 0x00
    db 0x00, 0x9C, 0x0E, 0x8F, 0x28
    db 0x40, 0x96, 0xB9, 0xA3, 0xFF

gc_regs:
    db 0x00, 0x00, 0x00, 0x00, 0x00
    db 0x40, 0x05, 0x0F, 0xFF
ac_regs:
    db 0x00, 0x01, 0x02, 0x03
    db 0x04, 0x05, 0x06, 0x07
    db 0x08, 0x09, 0x0A, 0x0B
    db 0x0C, 0x0D, 0x0E, 0x0F
    db 0x41, 0x00, 0x0F, 0x00, 0x00