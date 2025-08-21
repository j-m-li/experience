#ifndef WASM_GLOBALS_H
#define WASM_GLOBALS_H

#define WASM_GLOBALS_MAX 32

typedef struct {
    int values[WASM_GLOBALS_MAX];
    unsigned int count;
} WasmGlobals;

void wasm_globals_init(WasmGlobals* globals);
void wasm_globals_set(WasmGlobals* globals, unsigned int index, int value);
int wasm_globals_get(const WasmGlobals* globals, unsigned int index);

#endif /* WASM_GLOBALS_H */