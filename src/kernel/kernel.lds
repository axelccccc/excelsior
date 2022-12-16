ENTRY(_start)

PHDRS
{
    headers PT_PHDR;
    headers_load PT_LOAD FILEHDR PHDRS;
    code PT_LOAD;
    data PT_LOAD;
}

SECTIONS
{
    . = 0x11000;
    .text :         ALIGN(4K) { *(.text) } :code
    .rodata :       ALIGN(4K) { *(.rodata) } :data
    .data :         ALIGN(4K) { *(.data) } :data
    .bss :          ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    } :data
    /* /DISCARD/ : { *(.eh_frame) } */
}