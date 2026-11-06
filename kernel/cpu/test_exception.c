void test_divide_by_zero(void)
{
    asm volatile (
        "xor %%edx, %%edx\n"
        "mov $10, %%eax\n"
        "xor %%ecx, %%ecx\n"
        "div %%ecx\n"
        ::: "eax", "ecx", "edx"
    );
}
