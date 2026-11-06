
#include <stdint.h>
#include "../include/cpu/idt.h"

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr idtr;

extern uint32_t isr_stub_table[];
extern void idt_flush(uint32_t);
extern void syscall_stub(void);

static void idt_set_gate(
    uint8_t n,
    uint32_t base,
    uint16_t selector,
    uint8_t flags
) {
    idt[n].base_low = base & 0xFFFF;
    idt[n].base_high = (base >> 16) & 0xFFFF;
    idt[n].selector = selector;
    idt[n].zero = 0;
    idt[n].flags = flags;
}

void idt_init(void)
{
    for (int i = 0; i < 256; i++)
        idt_set_gate(i, 0, 0, 0);

    // CPU exceptions.
    for (int i = 0; i < 32; i++)
        idt_set_gate(i, isr_stub_table[i], 0x08, 0x8E);

    // User-callable system call.
    // Present + DPL 3 + 32-bit interrupt gate.
    idt_set_gate(
        0x80,
        (uint32_t)syscall_stub,
        0x08,
        0xEE
    );

    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t)&idt;

    idt_flush((uint32_t)&idtr);
}
