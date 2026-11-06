#include "../include/cpu/gdt.h"
#include <stdint.h>

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct tss_entry {
    uint32_t prev_tss;

    uint32_t esp0;
    uint16_t ss0;
    uint16_t reserved0;

    uint32_t esp1;
    uint16_t ss1;
    uint16_t reserved1;

    uint32_t esp2;
    uint16_t ss2;
    uint16_t reserved2;

    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;

    uint32_t eax, ecx, edx, ebx;
    uint32_t esp, ebp, esi, edi;

    uint16_t es, reserved3;
    uint16_t cs, reserved4;
    uint16_t ss, reserved5;
    uint16_t ds, reserved6;
    uint16_t fs, reserved7;
    uint16_t gs, reserved8;
    uint16_t ldt, reserved9;

    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed));

static struct gdt_entry gdt[6];
static struct gdt_ptr gp;
static struct tss_entry tss;

static uint8_t ring0_stack[16384]
    __attribute__((aligned(16)));

extern void gdt_flush(uint32_t);
extern void tss_flush(void);

static void gdt_set_gate(
    int i,
    uint32_t base,
    uint32_t limit,
    uint8_t access,
    uint8_t granularity
) {
    gdt[i].base_low = base & 0xFFFF;
    gdt[i].base_middle = (base >> 16) & 0xFF;
    gdt[i].base_high = (base >> 24) & 0xFF;

    gdt[i].limit_low = limit & 0xFFFF;

    gdt[i].granularity =
        ((limit >> 16) & 0x0F) |
        (granularity & 0xF0);

    gdt[i].access = access;
}

void gdt_init(void)
{
    // Clear TSS explicitly.
    uint8_t *p = (uint8_t *)&tss;

    for (unsigned int i = 0; i < sizeof(tss); i++)
        p[i] = 0;

    // Kernel stack for privilege transitions.
    tss.ss0 = 0x10;
    tss.esp0 = (uint32_t)(
        ring0_stack + sizeof(ring0_stack)
    );

    // Disable user-mode I/O bitmap access.
    tss.iomap_base = sizeof(tss);

    gp.limit = sizeof(gdt) - 1;
    gp.base = (uint32_t)&gdt;

    // Null descriptor.
    gdt_set_gate(0, 0, 0, 0, 0);

    // Ring 0 code.
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Ring 0 data.
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Ring 3 code.
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    // Ring 3 data.
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    // TSS descriptor (available 32-bit TSS).
    gdt_set_gate(
        5,
        (uint32_t)&tss,
        sizeof(tss) - 1,
        0x89,
        0x00
    );

    // Load GDT and activate TSS.
    gdt_flush((uint32_t)&gp);
    tss_flush();
}


uint16_t tss_get_tr(void)
{
    uint16_t selector;
    __asm__ volatile ("str %0" : "=r"(selector));
    return selector;
}
