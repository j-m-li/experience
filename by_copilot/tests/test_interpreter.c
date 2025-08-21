#include "../src/wasm_interpreter.h"
#include "../include/wasm_types.h"
#include <stdio.h>

int main(void) {
    WasmModule m;
    m.version = 1;
    printf("Running interpreter test...\n");
    interpret_wasm(&m);
    return 0;
}