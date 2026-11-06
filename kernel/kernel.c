#include "include/out.h"
#include "include/carac.h"
#include "include/cpu/gdt.h"
#include "include/cpu/idt.h"
#include "include/cpu/paging.h"

extern void enter_user_mode(void);
void kernel_main(void)
{
    asm volatile ("cli");

    clean_screen();
    gdt_init();
    idt_init();
    paging_init();
    print(0, 0, "[ OK ] BOOTLOADER loaded (bootloader/boot.asm)");
    print(0, 1, "[ OK ] Terminal loaded. You can now continue.");
    terminal_load();
}
