#include <stdio.h>

#include <e820.h>

int e820_entry_cmp(const void* a, const void* b) {
    uint64_t x = ((e820_entry*)a)->base;
    uint64_t y = ((e820_entry*)b)->base;
    if(x > y) return 1;
    if(x < y) return -1;
    return 0;
}

void e820_print_entry(e820_entry entry) {
    printf("Base: 0x%016x\n", entry.base);
    printf("Size: 0x%016x\n", entry.size);
    printf("Type: 0x%08x\n", entry.type);
    printf("ACPI: 0x%08x\n", entry.acpi3);
}