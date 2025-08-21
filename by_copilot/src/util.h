#ifndef UTIL_H
#define UTIL_H

void print_hex(const unsigned char* data, unsigned int length);
unsigned int read_binary_file(const char* filename, unsigned char* buffer, unsigned int maxlen);

#endif /* UTIL_H */