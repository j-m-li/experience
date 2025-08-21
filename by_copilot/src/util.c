#include "util.h"
#include <stdio.h>

void print_hex(const unsigned char* data, unsigned int length) {
    unsigned int i;
    for (i = 0; i < length; ++i) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (length % 16 != 0) printf("\n");
}

unsigned int read_binary_file(const char* filename, unsigned char* buffer, unsigned int maxlen) {
    FILE *f = fopen(filename, "rb");
    unsigned int read_len = 0;
    if (!f) return 0;
    read_len = (unsigned int)fread(buffer, 1, maxlen, f);
    fclose(f);
    return read_len;
}