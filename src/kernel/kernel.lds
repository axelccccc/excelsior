ENTRY(main)

PHDRS
{
    headers PT_PHDR;
    headers_load PT_LOAD FILEHDR PHDRS;
    code PT_LOAD;
    data PT_LOAD;
}

SECTIONS
{
    .text 0x600 : ALIGN(0x100)
    {
        *(.text.main)
        *(.text)
    } :code
    .rodata : { *(.rodata) } :data
    .data : { *(.data) } :data
    .bss : { *(.bss) } :data
    /DISCARD/ : { *(.eh_frame) }
}