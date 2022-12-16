#include <stdlib.h>
#include <string.h>

int atoi(const char* str) {

    int sign = 1;
    int ret = 0;

    int len = strlen(str);
    for(int i = 0; i < len; i++) {

        if((str[i] >= '0' && str[i] <= '9')) {

            if(str[i-1] == '-')
                sign = -1;

            while(str[i] >= '0' && str[i] <= '9')
                ret = ret * 10 + (str[i++] - '0');

            break;

        }

    }

    return ret * sign;

}