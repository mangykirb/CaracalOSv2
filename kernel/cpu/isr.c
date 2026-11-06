
#include <stdint.h>
#include "../include/out.h"
static void debug_char(char c)
{
    asm volatile ("outb %0, $0xE9" :: "a"(c));
}

static void debug_string(const char *s)
{
    while (*s)
        debug_char(*s++);
}

static void debug_hex(uint32_t value)
{
    const char hex[] = "0123456789ABCDEF";

    debug_string("0x");

    for (int i = 7; i >= 0; i--)
        debug_char(hex[(value >> (i * 4)) & 15]);
}

void syscall_handler(uint32_t number)
{
    if (number == 1) {
        const char *msg =
            "Hello from CaracalOS Ring 3!";

        volatile uint16_t *vga =
            (volatile uint16_t *)0xB8000;

        for (int i = 0; msg[i]; i++) {
            vga[12 * 80 + i] =
                (0x0A << 8) | msg[i];
        }

        debug_string("[SYSCALL] Ring 3 says hello!\n");
    }
}

__attribute__((noreturn))
void isr_handler(uint32_t vector, uint32_t error, uint32_t eip)
{
    debug_string("\n[ CARACAL KERNEL PANIC ]\n");
    debug_string("Exception: ");
    debug_hex(vector);
    debug_string("\nError: ");
    debug_hex(error);
    debug_string("\nEIP (fault instruction): ");
    debug_hex(eip);
    debug_string("\nCR2 (fault address): ");
    // if qemu DOESNT exist, then we print it on screen
    print(0, 0, "SEVERE ERROR!                                                                ");
    print(0, 1, "KERNEL PANIC!                                                                ");
    
    uint32_t cr2;
    asm volatile ("mov %%cr2, %0" : "=r"(cr2));
    debug_hex(cr2);

    debug_string("\n");

    volatile uint16_t *vga =
        (volatile uint16_t *)0xB8000;

    const char *msg = "[ CARACAL KERNEL PANIC ]";

    for (int i = 0; msg[i]; i++)
        vga[i] = (0x4F << 8) | msg[i];

    for (;;)
        asm volatile ("cli; hlt");
}
