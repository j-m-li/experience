#include "wasm_memory.h"
#include <stdio.h>
#include <string.h>

void wasm_memory_init(WasmMemory* memory, unsigned int initial_size, unsigned int max_size) {
    memory->size = initial_size;
    memory->max_size = max_size;
    memset(memory->data, 0, sizeof(memory->data));
}

unsigned char wasm_memory_load(WasmMemory* memory, unsigned int address) {
    if (address >= memory->size) return 0;
    return memory->data[address];
}

void wasm_memory_store(WasmMemory* memory, unsigned int address, unsigned char value) {
    if (address >= memory->size) return;
    memory->data[address] = value;
}