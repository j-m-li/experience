#include "../src/wasm_parser.h"
#include "../include/wasm_types.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    /* Test WASM magic header */
    unsigned char test_bytes[8] = { 0x00, 0x61, 0x73, 0x6D, 0x01, 0x00, 0x00, 0x00 };
    WasmModule m;
    printf("Running parser test...\n");
    if (parse_wasm(test_bytes, 8, &m) == 0) {
        printf("Parser test passed (version: %u)\n", m.version);
        return 0;
    } else {
        printf("Parser test failed\n");
        return 1;
    }
}