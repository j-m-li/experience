#ifndef WASM_INSTRUCTIONS_H
#define WASM_INSTRUCTIONS_H

#include "wasm_types.h"

/* Example opcodes (WASM spec v1) */
#define WASM_OP_UNREACHABLE 0x00
#define WASM_OP_NOP         0x01
/* Add more opcode defines as needed */

void wasm_execute_instruction(unsigned char opcode, WasmModule* module);

#endif /* WASM_INSTRUCTIONS_H */