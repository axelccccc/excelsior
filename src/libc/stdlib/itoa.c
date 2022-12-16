#include <stddef.h>
#include <stdlib.h>

char* itoa(int n, char* str, int base) {

    if (base < 2 || base > 16) {
        return NULL;
    }

    int i = 0;

    if (n < 0) {
        *str++ = '-';
        n *= -1;
    }

    do {
        int rem = n % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        n /= base;
    } while (n != 0);

    str[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }

    return str;
    
}