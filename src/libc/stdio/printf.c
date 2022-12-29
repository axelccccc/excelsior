/**
 * @file printf.c
 * @author axelccccc (github.com/axelccccc)
 * @brief printf implementation
 * @version 0.1
 * @date 2022-12-14
 * 
 * Copyright (c) 2022 axelccccc
 *
 * TODO: 'f': handle INF and NAN (arch-specific)
 * TODO: implement length modifiers
 * TODO: implement 'g', 'G', 'a', 'A', 'p'
 * TODO: handle exceeding max written chars 
 *       (INT_MAX, setting errno to EOVERFLOW)
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// minimal print used for kernel printf
int print(const char* data, size_t data_len) {

    size_t written_chars = 0;

    for(size_t i = 0; i < data_len; ++i) {
        putchar(data[i]);
        ++written_chars;
    }

    return written_chars;
}

int printf(const char* fmt, ...) {

    va_list ptr;
    va_start(ptr, fmt);

    size_t written_chars = 0;

    int fmt_len = strlen(fmt);

    for(int i = 0; i < fmt_len;) {

        if(fmt[i] == '%') {

            if(fmt[i++ + 1] == '%') {

                putchar(fmt[i++]);

            } else {

                bool justify_left = false;
                bool always_sign = false;
                bool space_sign = false;
                bool alt_type = false;
                char pad_char = ' ';

                int pad = 0;
                int precision = -2; // -2 signifies no precision was specified
                // int length_modifier = 0;

                int pad_len = 0; // Effective padding length

                // Options for behavior of converison

                for(;;) {
                    switch(fmt[i++]) {
                        case '-': // Left-justify
                            justify_left = true;
                            break;
                        case '+': // Always output sign
                            always_sign = true;
                            break;
                        case ' ': // Space if no sign
                            space_sign = true;
                            break;
                        case '#': // Use alternative form of conversion
                            alt_type = true;
                            break;
                        case '0': // Pad nums with zeros instead of spaces
                            pad_char = '0';
                            break;
                        default:
                            --i;
                            goto fmt_opts_end;
                            break;
                    }
                }

                fmt_opts_end:

                // Field width

                if((fmt[i] >= '0' && fmt[i] <= '9') || fmt[i] == '*') {
                    if(fmt[i] == '*')
                        pad = -1; // -1 signifies an arg is passed for the value
                    else {
                        pad = atoi(&(fmt[i]));
                        while(fmt[i] >= '0' && fmt[i] <= '9')
                            ++i;
                    }
                }

                // Conversion precision

                if( fmt[i] == '.' && 
                    ((fmt[i+1] >= '0' && fmt[i+1] <= '9') || fmt[i+1] == '*')) {

                    if(fmt[++i] == '*')
                        precision = -1; // -1 signifies an arg is passed for the value
                    else {
                        precision = atoi(&(fmt[i]));
                        while(fmt[i] >= '0' && fmt[i] <= '9')
                            ++i;
                    }
                    
                }

                // Length modifier

                // switch(fmt[i]) {
                //     case 'h':
                //         break;
                //     case 'l':
                //         break;
                //     default:
                //         break;
                // }

                // Conversion format specifier

                if(fmt[i] == 'c') {

                    char c = va_arg(ptr, int);
                    pad_len = pad - 1 < 0 ? 0 : pad - 1;
                    if(justify_left) {
                        putchar(c);
                        for(int j = 0; j < pad_len; ++j) putchar(' ');
                    } else {
                        for(int j = 0; j < pad_len; ++j) putchar(' ');
                        putchar(c);
                    }

                    written_chars += pad_len + 1;

                } else if(fmt[i] == 's') {

                    char* str = va_arg(ptr, char*);
                    int str_len = strlen(str);
                    pad_len = pad - str_len;
                    if(pad_len < 0) pad_len = 0;
                    if(justify_left) {
                        for(int j = 0; j < str_len; ++j) putchar(str[j]);
                        for(int j = 0; j < pad_len; ++j) putchar(' ');
                    } else {
                        for(int j = 0; j < pad_len; ++j) putchar(' ');
                        for(int j = 0; j < str_len; ++j) putchar(str[j]);
                    }

                    written_chars += pad_len + str_len;

                } else if(fmt[i] == 'd' || fmt[i] == 'i') {

                    int val = va_arg(ptr, int);
                    int num_len = 0;
                    char sign_char = 0;

                    // Determine number length
                    int val_copy = val;
                    while(val_copy) {
                        val_copy /= 10;
                        num_len++;
                    }
                    // 0 is 1 digit or 0 if precision is 0
                    if(!num_len && precision != 0) num_len = 1;

                    char num_str[num_len + 1]; // +1: \0
                    num_str[num_len] = '\0';

                    // Add correct sign
                    if(val < 0) {
                        sign_char = '-';
                        val *= -1; // We use positive val now
                    } else if(always_sign)
                        sign_char = '+';
                    else if(space_sign)
                        sign_char = ' ';

                    // Padding with 0 is ignored if a precision
                    // is specified or if the integer is left-justified
                    if(precision != -2)
                        pad_char = ' ';

                    // Adjust padding according to total num length
                    // (bool)sign_char is 0 if no sign, 1 otherwise
                    if(num_len >= precision) {
                        pad_len = pad - num_len - (bool)sign_char;
                        precision = 0;
                    } else {
                        pad_len = pad - precision - (bool)sign_char;
                        precision -= num_len; // Actual number of padding 0s
                    }
                    if(pad_len < 0) pad_len = 0;

                    // Transform the number into a string
                    for(int j = num_len; j >= 0; --j) {
                        num_str[j-1] = val % 10 + '0';
                        val /= 10;
                    }

                    if(justify_left) {
                        if(sign_char != 0)
                            putchar(sign_char);
                        for(int j = 0; j < num_len; ++j)   putchar(num_str[j]);
                        for(int j = 0; j < pad_len; ++j)   putchar(' ');
                        written_chars += pad_len + num_len + (bool)sign_char;
                    } else {
                        for(int j = 0; j < pad_len; ++j)   putchar(pad_char); // '0' or ' '
                        if(sign_char != 0)
                            putchar(sign_char);
                        for(int j = 0; j < precision; ++j) putchar('0');      // precision
                        for(int j = 0; j < num_len; ++j)   putchar(num_str[j]);
                        written_chars += pad_len + num_len + precision + (bool)sign_char;
                    }

                } else if(fmt[i] == 'u') {

                    unsigned int val = va_arg(ptr, unsigned int);

                    int num_len = 0;

                    // Determine number length
                    unsigned int val_copy = val;
                    while(val_copy) {
                        val_copy /= 10;
                        num_len++;
                    }
                    // 0 is 1 digit or 0 if precision is 0
                    if(!num_len && precision != 0) num_len = 1;

                    char num_str[num_len + 1]; // +1: \0
                    num_str[num_len] = '\0';

                    // Padding with 0 is ignored if a precision
                    // is specified or if the integer is left-justified
                    if(precision != -2)
                        pad_char = ' ';

                    // Adjust padding according to total num length
                    // (bool)sign_char is 0 if no sign, 1 otherwise
                    if(num_len >= precision) {
                        pad_len = pad - num_len;
                        precision = 0;
                    } else {
                        pad_len = pad - precision;
                        precision -= num_len; // Actual number of padding 0s
                    }
                    if(pad_len < 0) pad_len = 0;

                    // Transform the number into a string
                    for(int j = num_len; j >= 0; --j) {
                        num_str[j-1] = val % 10 + '0';
                        val /= 10;
                    }

                    if(justify_left) {
                        for(int j = 0; j < num_len; ++j)   putchar(num_str[j]);
                        for(int j = 0; j < pad_len; ++j)   putchar(' ');
                        written_chars += pad_len + num_len;
                    } else {
                        for(int j = 0; j < pad_len; ++j)   putchar(pad_char); // '0' or ' '
                        for(int j = 0; j < precision; ++j) putchar('0');      // precision
                        for(int j = 0; j < num_len; ++j)   putchar(num_str[j]);
                        written_chars += pad_len + num_len + precision;
                    }
                    
                } else if(fmt[i] == 'x' || fmt[i] == 'X') {

                    bool upper = fmt[i] == 'X';

                    char* hex_chars[2]  = {
                        "0123456789abcdef",
                        "0123456789ABCDEF"
                        };
                    char* hex_prefix = "xX";

                    unsigned int val = va_arg(ptr, unsigned int);

                    int num_len = 0;

                    // Determine number length
                    unsigned int val_copy = val;
                    while(val_copy) {
                        val_copy /= 16;
                        num_len++;
                    }
                    // 0 is 1 digit or 0 if precision is 0
                    if(!num_len && precision != 0) num_len = 1;
                    

                    char num_str[num_len + 1]; // +1: \0
                    num_str[num_len] = '\0';

                    // Padding with 0 is ignored if a precision
                    // is specified or if the integer is left-justified
                    if(precision != -2)
                        pad_char = ' ';

                    // Adjust padding according to total num length
                    // (bool)sign_char is 0 if no sign, 1 otherwise
                    if(num_len >= precision) {
                        pad_len = pad - num_len - (alt_type * 2); // alt_type: 0 or 2 ('' or "0x")
                        precision = 0;
                    } else {
                        pad_len = pad - precision - (alt_type * 2);
                        precision -= num_len; // Actual number of padding 0s
                    }
                    if(pad_len < 0) pad_len = 0;

                    // Transform the number into a string
                    for(int j = num_len; j >= 0; --j) {
                        num_str[j-1] = hex_chars[upper][val % 16];
                        val /= 16;
                    }

                    if(justify_left) {
                        if(alt_type) {
                            putchar('0');
                            putchar(hex_prefix[upper]);
                        }
                        for(int j = 0; j < num_len; ++j)   putchar(num_str[j]);
                        for(int j = 0; j < pad_len; ++j)   putchar(' ');
                        written_chars += pad_len + num_len;
                    } else {
                        for(int j = 0; j < pad_len; ++j)   putchar(pad_char); // '0' or ' '
                        if(alt_type) {
                            putchar('0');
                            putchar(hex_prefix[upper]);
                        }
                        for(int j = 0; j < precision; ++j) putchar('0');      // precision
                        for(int j = 0; j < num_len; ++j)   putchar(num_str[j]);
                        written_chars += pad_len + num_len + precision;
                    }
                    
                    if(alt_type) written_chars += 2;

                } else if(fmt[i] == 'o') {
                    
                    unsigned int val = va_arg(ptr, unsigned int);

                    int num_len = 0;

                    // Determine number length
                    unsigned int val_copy = val;
                    while(val_copy) {
                        val_copy /= 8;
                        num_len++;
                    }
                    // 0 is 1 digit or 0 if precision is 0
                    if(!num_len && precision != 0) num_len = 1;
                    

                    char num_str[num_len + 1]; // +1: \0
                    num_str[num_len] = '\0';

                    // Padding with 0 is ignored if a precision
                    // is specified or if the integer is left-justified
                    if(precision != -2)
                        pad_char = ' ';

                    // Adjust padding according to total num length
                    // (bool)sign_char is 0 if no sign, 1 otherwise
                    if(num_len >= precision) {
                        pad_len = pad - num_len - (alt_type * 1); // alt_type: 0 or 1 ('' or "0")
                        precision = 0;
                    } else {
                        pad_len = pad - precision - (alt_type * 1);
                        precision -= num_len; // Actual number of padding 0s
                    }
                    if(pad_len < 0) pad_len = 0;

                    // Transform the number into a string
                    for(int j = num_len; j >= 0; --j) {
                        num_str[j-1] = val % 8 + '0';
                        val /= 8;
                    }

                    if(justify_left) {
                        if(alt_type) {
                            putchar('0');
                        }
                        for(int j = 0; j < num_len; ++j)   putchar(num_str[j]);
                        for(int j = 0; j < pad_len; ++j)   putchar(' ');
                        written_chars += pad_len + num_len;
                    } else {
                        for(int j = 0; j < pad_len; ++j)   putchar(pad_char); // '0' or ' '
                        if(alt_type) {
                            putchar('0');
                        }
                        for(int j = 0; j < precision; ++j) putchar('0');      // precision
                        for(int j = 0; j < num_len; ++j)   putchar(num_str[j]);
                        written_chars += pad_len + num_len + precision;
                    }

                    if(alt_type) written_chars += 1;
                    
                } else if(fmt[i] == 'f' || fmt[i] == 'F') {

                    double val = va_arg(ptr, double);

                    char sign_char = 0;

                    // Determine number length
                    int num_len = 0;
                    int val_copy = val;
                    while(val_copy) {
                        val_copy /= 10;
                        num_len++;
                    }
                    // 0 is 1 digit
                    if(!num_len) num_len = 1;

                    // Adjust precision
                    if(precision == -2) precision = 6;

                    // Add correct sign
                    if(val < 0) {
                        sign_char = '-';
                        val *= -1; // We use positive val now
                    } else if(always_sign)
                        sign_char = '+';
                    else if(space_sign)
                        sign_char = ' ';

                    // Adjust padding according to total number length
                    // (bool)sign_char is 0 if no sign, 1 otherwise
                    pad_len = pad - num_len - precision - ((precision > 0) || alt_type) - (bool)sign_char;  // '.' if dec_len > 0 + sign
                    if(pad_len < 0) pad_len = 0;

                    // Transform the number into a string
                    char num_str[num_len + 1]; // +1: '\0'
                    num_str[num_len] = '\0';

                    // Transform the decimal part into a string
                    char dec_str[precision + 1]; // +1: '\0'
                    dec_str[precision] = '\0';

                    // Transform the number into a string
                    val_copy = val;
                    for(int j = num_len; j >= 0; --j) {
                        num_str[j-1] = (int)val_copy % 10 + '0';
                        val_copy /= 10;
                    }

                    // Transform the decimal part into a string
                    // and trunk to precision
                    double dec_copy = val - (int)val;
                    for(int j = 0; j < precision; ++j) {
                        dec_copy *= 10;
                        // Round at last digit of precision
                        if(j == precision-1 && dec_copy - (int)dec_copy >= 0.5) {
                            dec_str[j] = (int)(++dec_copy) + '0';
                            if(dec_str[j] > '9') {
                                for(int k = precision-1; k >= 0; --k) {
                                    if(dec_str[k] >= '9') {
                                        dec_str[k] = '0';
                                    } else {
                                        ++dec_str[k];
                                        break;
                                    }
                                    if(k == 0 && dec_str[k] == '0') {
                                        // carry to num_str
                                        for(int l = num_len-1; l >= 0; --l) {
                                            if(num_str[l] == '9') {
                                                num_str[l] = '0';
                                            } else {
                                                ++num_str[l];
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        } else {
                            dec_str[j] = (int)(dec_copy) + '0'; // Round
                        }
                        dec_copy -= (int)dec_copy;
                    }
                    
                    // TODO: handle INF & NAN

                    if(justify_left) {
                        if(sign_char != 0)
                            putchar(sign_char);
                        for(int j = 0; j < num_len; ++j)     putchar(num_str[j]);
                        if(alt_type || precision)            putchar('.');
                        for(int j = 0; j < precision; ++j)   putchar(dec_str[j]);
                        for(int j = 0; j < pad_len; ++j)     putchar(' ');
                    } else {
                        for(int j = 0; j < pad_len; ++j)     putchar(pad_char); // '0' or ' '
                        if(sign_char != 0)
                            putchar(sign_char);
                        for(int j = 0; j < num_len; ++j)     putchar(num_str[j]);
                        if(alt_type || precision)            putchar('.');
                        for(int j = 0; j < precision; ++j)   putchar(dec_str[j]);
                    }

                    written_chars += pad_len + num_len + precision + (bool)sign_char + ((precision > 0) || alt_type);
                    
                } else if(fmt[i] == 'e' || fmt[i] == 'E') {

                    double val = va_arg(ptr, double);

                    char sign_char = 0;

                    int exponent = 0;
                    
                    // Integer part is 1 char
                    int num_len = 1;

                    // Adjust precision
                    if(precision == -2) precision = 6;

                    // Add correct sign
                    if(val < 0) {
                        sign_char = '-';
                        val *= -1; // We use positive val now
                    } else if(always_sign)
                        sign_char = '+';
                    else if(space_sign)
                        sign_char = ' ';

                    // // Adjust padding according to total number length
                    // // (bool)sign_char is 0 if no sign, 1 otherwise
                    // pad_len = pad - num_len - precision - ((precision > 0) || alt_type);  // '.' if dec_len > 0 + sign

                    // Get value of exponent and put number in correct form
                    if(val != 0) {
                        while(val < 1) {
                            val *= 10;
                            --exponent;
                        }
                        while(val >= 10) {
                            val /= 10;
                            ++exponent;
                        }
                    }

                    // Transform the number into a string
                    char num_int = (int)val + '0';

                    // Transform the decimal part into a string
                    char dec_str[precision + 1]; // +1: '\0'
                    dec_str[precision] = '\0';

                    // Transform the decimal part into a string
                    // and trunk to precision
                    double dec_copy = val - (int)val;
                    for(int j = 0; j < precision; ++j) {
                        dec_copy *= 10;
                        // Round at last digit of precision
                        if(j == precision-1 && dec_copy - (int)dec_copy >= 0.5) {
                            dec_str[j] = (int)(++dec_copy) + '0';
                            if(dec_str[j] > '9') {
                                for(int k = precision-1; k >= 0; --k) {
                                    if(dec_str[k] >= '9') {
                                        dec_str[k] = '0';
                                    } else {
                                        ++dec_str[k];
                                        break;
                                    }
                                    if(k == 0 && dec_str[k] == '0') {
                                        // carry to num_str
                                        if(num_int == '9') {
                                            num_int = '1'; // Not 0: translated to exponent
                                            ++exponent;
                                        } else {
                                            ++num_int;
                                        }
                                    }
                                }
                            }
                        } else {
                            dec_str[j] = (int)(dec_copy) + '0'; // Round
                        }
                        dec_copy -= (int)dec_copy;
                    }
                    
                    // Transform the exponent into a string
                    char exp_str[4]; // +1: '\0'
                    exp_str[3] = '\0';
                    if(exponent < 0) {
                        exp_str[0] = '-';
                        exponent *= -1;
                    } else {
                        exp_str[0] = '+';
                    }
                    exp_str[2] = exponent % 10 + '0';
                    exp_str[1] = exponent / 10 + '0';

                    // Calculate padding
                    int pad_len = 0;
                    pad_len = pad - num_len - precision - 1 - (bool)sign_char - 4; // 1: '.', sign, 4: 'e+00'
                    if(pad_len < 0) pad_len = 0;


                    if(justify_left) {
                        if(sign_char != 0)
                            putchar(sign_char);
                        putchar(num_int);
                        if(alt_type || precision)            putchar('.');
                        for(int j = 0; j < precision; ++j)   putchar(dec_str[j]);
                        putchar(fmt[i]); // 'e' or 'E'
                        for(int j = 0; j < 3; ++j)           putchar(exp_str[j]);
                        for(int j = 0; j < pad_len; ++j)     putchar(' ');
                    } else {
                        for(int j = 0; j < pad_len; ++j)     putchar(pad_char); // '0' or ' '
                        if(sign_char != 0)
                            putchar(sign_char);
                        putchar(num_int);
                        if(alt_type || precision)            putchar('.');
                        for(int j = 0; j < precision; ++j)   putchar(dec_str[j]);
                        putchar(fmt[i]); // 'e' or 'E'
                        for(int j = 0; j < 3; ++j)           putchar(exp_str[j]);
                    }

                    written_chars += pad_len + num_len + precision + 1 + 4 + (bool)sign_char; // 1: '.', 4: 'e+00'
                    
                }
                
                ++i;
                
            }
            
        } else { // fmt[i] == '%'
            putchar(fmt[i++]);
        }
        
    }
    
    va_end(ptr);
    return written_chars;
    
}