#include "wasm_globals.h"
#include <stdio.h>
#include <string.h>

void wasm_globals_init(WasmGlobals* globals) {
    memset(globals->values, 0, sizeof(globals->values));
    globals->count = 0;
}

void wasm_globals_set(WasmGlobals* globals, unsigned int index, int value) {
    if (index >= WASM_GLOBALS_MAX) return;
    globals->values[index] = value;
}

int wasm_globals_get(const WasmGlobals* globals, unsigned int index) {
    if (index >= WASM_GLOBALS_MAX) return 0;
    return globals->values[index];
}