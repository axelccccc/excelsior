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
    .text 0x9000 :  ALIGN(4K) { *(.text) } :code
    .rodata :       ALIGN(4K) { *(.rodata) } :data
    .data :         ALIGN(4K) { *(.data) } :data
    .bss :          ALIGN(4K) { *(.bss) } :data
    /* /DISCARD/ : { *(.eh_frame) } */
}