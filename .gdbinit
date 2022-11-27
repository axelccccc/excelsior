layout asm
layout regs
focus cmd
set architecture i8086
target remote localhost:26000
set disassembly-flavor intel
b *0x7c00