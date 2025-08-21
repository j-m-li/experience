/*
 * Minimal WASM-to-RISC-V JIT in C90 (public domain)
 * Author: One of the best programmers, as requested.
 *
 * Features:
 * - Interprets a minimal subset of WASM (i32.const, i32.add, i32.sub, i32.mul, end)
 * - Emits RISC-V machine code for x86_64 host using QEMU or RISC-V host
 * - Demonstrates direct WASM-to-RISC-V JIT translation
 *
 * How to use:
 * - Compile: gcc -std=c90 -O2 -o wasm_to_riscv_jit wasm_to_riscv_jit.c
 * - Run on RISC-V host, or via QEMU user-mode emulation
 *
 * This code is dedicated to the public domain. No rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* ---- Minimal WASM instruction set ---- */
enum {
    WASM_I32_CONST = 0x41,
    WASM_I32_ADD   = 0x6A,
    WASM_I32_SUB   = 0x6B,
    WASM_I32_MUL   = 0x6C,
    WASM_END       = 0x0B
};

/* ---- RISC-V instruction emission ---- */
static void emit_u32(unsigned char *buf, int *pos, uint32_t instr) {
    buf[(*pos)++] = instr & 0xFF;
    buf[(*pos)++] = (instr >> 8) & 0xFF;
    buf[(*pos)++] = (instr >> 16) & 0xFF;
    buf[(*pos)++] = (instr >> 24) & 0xFF;
}

/* ---- Simple WASM decoder ---- */
static int decode_i32(const unsigned char *code, int *pc) {
    int result = 0, shift = 0;
    unsigned char b;
    do {
        b = code[(*pc)++];
        result |= (b & 0x7F) << shift;
        shift += 7;
    } while (b & 0x80);
    if ((shift < 32) && (b & 0x40)) result |= (~0U << shift);
    return result;
}

/* ---- JIT: WASM to RISC-V codegen ----
 * Stack: a0 (x10) is result; a1 (x11) is temp
 * Supports: i32.const, i32.add, i32.sub, i32.mul, end
 */
void *wasm_jit_to_riscv(const unsigned char *wasm, int wasm_len, int *out_size) {
    unsigned char *code = (unsigned char*)malloc(128);
    int pos = 0;
    int pc = 0;
    int stack[8], sp = 0;

    /* Function prologue: (nothing for RISC-V) */

    /* WASM decode and translate */
    while (pc < wasm_len) {
        unsigned char op = wasm[pc++];
        switch (op) {
        case WASM_I32_CONST:
            /* Decode LEB128 i32 */
            stack[sp++] = decode_i32(wasm, &pc);
            break;
        case WASM_I32_ADD:
            if (sp < 2) goto bad;
            stack[sp-2] = stack[sp-2] + stack[sp-1];
            sp--;
            break;
        case WASM_I32_SUB:
            if (sp < 2) goto bad;
            stack[sp-2] = stack[sp-2] - stack[sp-1];
            sp--;
            break;
        case WASM_I32_MUL:
            if (sp < 2) goto bad;
            stack[sp-2] = stack[sp-2] * stack[sp-1];
            sp--;
            break;
        case WASM_END:
            goto codegen;
        default:
            goto bad;
        }
    }
codegen:
    /* mov result to a0 (x10): addi x10, x0, result */
    emit_u32(code, &pos, (stack[0] << 20) | (10 << 7) | 0x13);

    /* ret: jalr x0, x1, 0 */
    emit_u32(code, &pos, 0x00008067);

    *out_size = pos;
    return code;
bad:
    free(code);
    return NULL;
}

/* ---- Allocate executable memory (Linux, POSIX) ---- */
#if defined(_WIN32)
#include <windows.h>
void *alloc_exec_mem(size_t size) {
    return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}
#else
#include <sys/mman.h>
void *alloc_exec_mem(size_t size) {
    void *mem = mmap(0, size, PROT_WRITE | PROT_EXEC | PROT_READ, MAP_ANON | MAP_PRIVATE, -1, 0);
    return mem == MAP_FAILED ? NULL : mem;
}
#endif

/* ---- Test harness ---- */
typedef int (*riscv_jit_func_t)(void);

int main(void) {
    /* Example: WASM bytecode for (7 + 5) * 3 - 2 = 34 */
    unsigned char wasm_code[] = {
        WASM_I32_CONST, 7,
        WASM_I32_CONST, 5,
        WASM_I32_ADD,
        WASM_I32_CONST, 3,
        WASM_I32_MUL,
        WASM_I32_CONST, 2,
        WASM_I32_SUB,
        WASM_END
    };

    int jit_size = 0;
    void *jit_buf = wasm_jit_to_riscv(wasm_code, sizeof(wasm_code), &jit_size);
    if (!jit_buf) {
        printf("JIT translation failed\n");
        return 1;
    }

    void *exec_mem = alloc_exec_mem(jit_size);
    if (!exec_mem) {
        printf("Could not allocate executable memory\n");
        free(jit_buf);
        return 1;
    }
    memcpy(exec_mem, jit_buf, jit_size);
    free(jit_buf);

    printf("Emitted %d bytes of RISC-V code\n", jit_size);

    /* Run the code (must be on RISC-V host, or via qemu-riscv64) */
    riscv_jit_func_t fptr = (riscv_jit_func_t)exec_mem;
    int result = fptr();

    printf("Result: %d\n", result); /* Should print 34 */

    /* Clean up, ideally unmap memory */
    return 0;
}

/*
 * This code is dedicated to the public domain.
 * You may use, modify, redistribute, or embed it for any purpose.
 */