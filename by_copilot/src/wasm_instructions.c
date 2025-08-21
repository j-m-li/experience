#include "wasm_instructions.h"
#include <stdio.h>

void wasm_execute_instruction(unsigned char opcode, WasmModule* module) {
    /* Stub: handle a few example opcodes */
    switch (opcode) {
        case WASM_OP_NOP:
            printf("Executed NOP\n");
            break;
        case WASM_OP_UNREACHABLE:
            printf("Executed UNREACHABLE (trap)\n");
            break;
        default:
            printf("Unknown opcode: 0x%02X\n", opcode);
            break;
    }
}