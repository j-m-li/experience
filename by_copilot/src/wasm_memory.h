#ifndef WASM_MEMORY_H
#define WASM_MEMORY_H

#define WASM_MEMORY_MAX 65536

typedef struct {
    unsigned int size;
    unsigned int max_size;
    unsigned char data[WASM_MEMORY_MAX];
} WasmMemory;

void wasm_memory_init(WasmMemory* memory, unsigned int initial_size, unsigned int max_size);
unsigned char wasm_memory_load(WasmMemory* memory, unsigned int address);
void wasm_memory_store(WasmMemory* memory, unsigned int address, unsigned char value);

#endif /* WASM_MEMORY_H */