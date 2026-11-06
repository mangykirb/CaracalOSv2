#include <stdint.h>

static inline __attribute__((always_inline))
void syscall(uint32_t number)
{
    __asm__ volatile (
        "int $0x80"
        :
        : "a"(number)
        : "memory"
    );
}

__attribute__((section(".usertext"), used))
void user_main(void)
{
    syscall(1);

    for (;;)
        __asm__ volatile ("pause");
}
