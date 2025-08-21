#include <stdio.h>
#include "wasm_parser.h"
#include "wasm_interpreter.h"
#include "util.h"

int main(void) {
    printf("WASM interpreter (C90) starting...\n");
    const char *filename = "example.wasm";
    unsigned char buffer[4096];
    unsigned int length = read_binary_file(filename, buffer, sizeof(buffer));
    if (length == 0) {
        printf("Failed to load WASM file: %s\n", filename);
        return 1;
    }
    WasmModule module;
    if (parse_wasm(buffer, length, &module) != 0) {
        printf("Failed to parse WASM binary\n");
        return 1;
    }
    interpret_wasm(&module);
    return 0;
}