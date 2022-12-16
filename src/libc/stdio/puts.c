#include <stdio.h>
#include <string.h>

int puts(const char* str) {
    return printf("%s\n", str);
    // int str_len = strlen(str);
    // for(int i = 0; i < str_len; ++i) {
    //     putchar(str[i]);
    // }
    // putchar('\n');
    // return 1;
}