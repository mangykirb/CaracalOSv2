ENTRY(_start)

SECTIONS
{
    . = 0x1000;

    .text :
    {
        *(.text)
        *(.text.*)
    }

    .usertext ALIGN(4096) :
    {
        __user_text_start = .;
        KEEP(*(.usertext))
        . = ALIGN(4096);
        __user_text_end = .;
    }

    ASSERT(
        (__user_text_end - __user_text_start) == 4096,
        "User code section must occupy exactly one page"
    )

    .rodata :
    {
        *(.rodata)
        *(.rodata.*)
    }

    .data :
    {
        *(.data)
        *(.data.*)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    }

    /DISCARD/ :
    {
        *(.comment)
        *(.note*)
        *(.eh_frame*)
    }
}
