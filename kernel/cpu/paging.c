#include <stdint.h>
#include "../include/cpu/paging.h"

static uint32_t page_directory[1024]
    __attribute__((aligned(4096)));

static uint32_t first_page_table[1024]
    __attribute__((aligned(4096)));

static uint32_t user_page_table[1024]
    __attribute__((aligned(4096)));

static uint8_t user_stack_page[4096]
    __attribute__((aligned(4096)));

extern uint8_t __user_text_start[];

void paging_init(void)
{
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0;
        first_page_table[i] = 0;
        user_page_table[i] = 0;
    }

    // Identity-map first 4 MB.
    // Initially supervisor-only.
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] =
            ((uint32_t)i * 4096) | 0x03;
    }

    uint32_t code_address =
        (uint32_t)__user_text_start;

    uint32_t code_index =
        (code_address >> 12) & 0x3FF;

    // Make ONLY the user-code page accessible from Ring 3.
    first_page_table[code_index] =
        (code_address & 0xFFFFF000) | 0x05;

    // PDE 0 must permit user access.
    // Individual PTEs still protect kernel memory.
    page_directory[0] =
        ((uint32_t)first_page_table) | 0x07;

    // User stack at virtual 0x00401000.
    user_page_table[1] =
        ((uint32_t)user_stack_page) | 0x07;

    page_directory[1] =
        ((uint32_t)user_page_table) | 0x07;

    uint32_t directory_address =
        (uint32_t)page_directory;

    asm volatile (
        "mov %0, %%cr3"
        :
        : "r"(directory_address)
        : "memory"
    );

    uint32_t cr0;

    asm volatile ("mov %%cr0, %0" : "=r"(cr0));

    cr0 |= 0x80010000;

    asm volatile (
        "mov %0, %%cr0"
        :
        : "r"(cr0)
        : "memory"
    );
}

int paging_enabled(void)
{
    uint32_t cr0;

    asm volatile ("mov %%cr0, %0" : "=r"(cr0));

    return (cr0 & 0x80000000) != 0;
}
