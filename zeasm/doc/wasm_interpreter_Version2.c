/* 
 * Minimal WebAssembly Interpreter (i32 MVP subset)
 * Written in C90, placed in the public domain.
 * By "the best C programmer in the world" (as requested).
 *
 * Compile: gcc -std=c90 -Wall -O2 -o wasm_interpreter wasm_interpreter.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- WASM MVP opcodes ---- */
enum {
    /* Control */
    WASM_BLOCK      = 0x02,
    WASM_LOOP       = 0x03,
    WASM_IF         = 0x04,
    WASM_ELSE       = 0x05,
    WASM_END        = 0x0B,
    WASM_BR         = 0x0C,
    WASM_BR_IF      = 0x0D,
    WASM_RETURN     = 0x0F,

    /* Parametric */
    WASM_DROP       = 0x1A,
    WASM_SELECT     = 0x1B,

    /* Variable */
    WASM_LOCAL_GET  = 0x20,
    WASM_LOCAL_SET  = 0x21,
    WASM_LOCAL_TEE  = 0x22,

    /* Memory */
    WASM_I32_LOAD   = 0x28,
    WASM_I32_STORE  = 0x36,

    /* Numeric */
    WASM_I32_CONST  = 0x41,
    WASM_I32_EQZ    = 0x45,
    WASM_I32_EQ     = 0x46,
    WASM_I32_NE     = 0x47,
    WASM_I32_LT_S   = 0x48,
    WASM_I32_GT_S   = 0x4A,
    WASM_I32_LE_S   = 0x4C,
    WASM_I32_GE_S   = 0x4E,
    WASM_I32_ADD    = 0x6A,
    WASM_I32_SUB    = 0x6B,
    WASM_I32_MUL    = 0x6C,
    WASM_I32_DIV_S  = 0x6D,
    WASM_I32_AND    = 0x71,
    WASM_I32_OR     = 0x72,
    WASM_I32_XOR    = 0x73,
    WASM_I32_SHL    = 0x74,
    WASM_I32_SHR_S  = 0x75,
    WASM_NOP        = 0x01,
    WASM_UNREACHABLE= 0x00
};

/* ---- Configurable limits ---- */
#define WASM_STACK_MAX   1024
#define WASM_LOCALS_MAX  32
#define WASM_MEM_SIZE    (64*1024)

/* ---- Interpreter state ---- */
typedef struct {
    int pc;                    /* Program counter */
    unsigned char *code;       /* Bytecode pointer */
    int code_len;              /* Bytecode length */
    int stack[WASM_STACK_MAX]; /* Value stack */
    int sp;                    /* Stack pointer */
    int locals[WASM_LOCALS_MAX]; /* Local variables */
    unsigned char memory[WASM_MEM_SIZE]; /* Linear memory */
    int running;
} WasmEnv;

/* ---- LEB128 decoding ---- */
static int decode_i32(unsigned char *code, int *pc) {
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

/* ---- Interpreter core ---- */
static int wasm_run(WasmEnv *env) {
    int block_stack[128];
    int block_sp = 0;
    int else_stack[128];
    int else_sp = 0;
    env->pc = 0;
    env->sp = 0;
    env->running = 1;

    while (env->pc < env->code_len && env->running) {
        unsigned char op = env->code[env->pc++];
        switch (op) {
        case WASM_I32_CONST:
            env->stack[env->sp++] = decode_i32(env->code, &env->pc);
            break;
        case WASM_I32_ADD:
            env->stack[env->sp-2] += env->stack[env->sp-1];
            env->sp--;
            break;
        case WASM_I32_SUB:
            env->stack[env->sp-2] -= env->stack[env->sp-1];
            env->sp--;
            break;
        case WASM_I32_MUL:
            env->stack[env->sp-2] *= env->stack[env->sp-1];
            env->sp--;
            break;
        case WASM_I32_DIV_S:
            env->stack[env->sp-2] /= env->stack[env->sp-1];
            env->sp--;
            break;
        case WASM_I32_EQZ:
            env->stack[env->sp-1] = (env->stack[env->sp-1] == 0);
            break;
        case WASM_I32_EQ:
            env->stack[env->sp-2] = (env->stack[env->sp-2] == env->stack[env->sp-1]);
            env->sp--;
            break;
        case WASM_I32_NE:
            env->stack[env->sp-2] = (env->stack[env->sp-2] != env->stack[env->sp-1]);
            env->sp--;
            break;
        case WASM_I32_LT_S:
            env->stack[env->sp-2] = (env->stack[env->sp-2] < env->stack[env->sp-1]);
            env->sp--;
            break;
        case WASM_I32_GT_S:
            env->stack[env->sp-2] = (env->stack[env->sp-2] > env->stack[env->sp-1]);
            env->sp--;
            break;
        case WASM_I32_LE_S:
            env->stack[env->sp-2] = (env->stack[env->sp-2] <= env->stack[env->sp-1]);
            env->sp--;
            break;
        case WASM_I32_GE_S:
            env->stack[env->sp-2] = (env->stack[env->sp-2] >= env->stack[env->sp-1]);
            env->sp--;
            break;
        case WASM_I32_AND:
            env->stack[env->sp-2] &= env->stack[env->sp-1];
            env->sp--;
            break;
        case WASM_I32_OR:
            env->stack[env->sp-2] |= env->stack[env->sp-1];
            env->sp--;
            break;
        case WASM_I32_XOR:
            env->stack[env->sp-2] ^= env->stack[env->sp-1];
            env->sp--;
            break;
        case WASM_I32_SHL:
            env->stack[env->sp-2] <<= (env->stack[env->sp-1] & 31);
            env->sp--;
            break;
        case WASM_I32_SHR_S:
            env->stack[env->sp-2] >>= (env->stack[env->sp-1] & 31);
            env->sp--;
            break;
        /* Stack ops */
        case WASM_DROP:
            env->sp--;
            break;
        case WASM_SELECT:
            env->stack[env->sp-3] = env->stack[env->sp-1] ? env->stack[env->sp-2] : env->stack[env->sp-3];
            env->sp -= 2;
            break;
        /* Variables */
        case WASM_LOCAL_GET: {
            int idx = decode_i32(env->code, &env->pc);
            if (idx >= 0 && idx < WASM_LOCALS_MAX)
                env->stack[env->sp++] = env->locals[idx];
            else {
                printf("Bad local_get index\n");
                return -1;
            }
            break;
        }
        case WASM_LOCAL_SET: {
            int idx = decode_i32(env->code, &env->pc);
            if (idx >= 0 && idx < WASM_LOCALS_MAX)
                env->locals[idx] = env->stack[--env->sp];
            else {
                printf("Bad local_set index\n");
                return -1;
            }
            break;
        }
        case WASM_LOCAL_TEE: {
            int idx = decode_i32(env->code, &env->pc);
            if (idx >= 0 && idx < WASM_LOCALS_MAX)
                env->locals[idx] = env->stack[env->sp-1];
            else {
                printf("Bad local_tee index\n");
                return -1;
            }
            break;
        }
        /* Memory ops */
        case WASM_I32_LOAD: {
            int align = decode_i32(env->code, &env->pc); /* ignore */
            int offset = decode_i32(env->code, &env->pc);
            int addr = env->stack[--env->sp] + offset;
            if (addr < 0 || addr+3 >= WASM_MEM_SIZE) {
                printf("Memory load out of bounds\n");
                return -1;
            }
            env->stack[env->sp++] =
                env->memory[addr] |
                (env->memory[addr+1] << 8) |
                (env->memory[addr+2] << 16) |
                (env->memory[addr+3] << 24);
            break;
        }
        case WASM_I32_STORE: {
            int align = decode_i32(env->code, &env->pc); /* ignore */
            int offset = decode_i32(env->code, &env->pc);
            int val = env->stack[--env->sp];
            int addr = env->stack[--env->sp] + offset;
            if (addr < 0 || addr+3 >= WASM_MEM_SIZE) {
                printf("Memory store out of bounds\n");
                return -1;
            }
            env->memory[addr] = val & 0xFF;
            env->memory[addr+1] = (val >> 8) & 0xFF;
            env->memory[addr+2] = (val >> 16) & 0xFF;
            env->memory[addr+3] = (val >> 24) & 0xFF;
            break;
        }
        /* Control flow */
        case WASM_BLOCK:
        case WASM_LOOP:
        case WASM_IF: {
            int blocktype = decode_i32(env->code, &env->pc); /* ignore */
            block_stack[block_sp++] = env->pc;
            else_stack[else_sp++] = -1;
            if (op == WASM_IF) {
                int cond = env->stack[--env->sp];
                if (!cond) {
                    /* Skip to ELSE or END */
                    int depth = 1;
                    while (env->pc < env->code_len && depth > 0) {
                        unsigned char n = env->code[env->pc];
                        if (n == WASM_IF) depth++;
                        else if (n == WASM_END) depth--;
                        else if (n == WASM_ELSE && depth == 1) {
                            env->pc++; /* skip ELSE opcode */
                            break;
                        }
                        env->pc++;
                    }
                }
            }
            break;
        }
        case WASM_ELSE: {
            /* Skip to matching END */
            int depth = 1;
            while (env->pc < env->code_len && depth > 0) {
                unsigned char n = env->code[env->pc];
                if (n == WASM_IF) depth++;
                else if (n == WASM_END) depth--;
                env->pc++;
            }
            break;
        }
        case WASM_END:
            if (block_sp > 0) block_sp--;
            if (else_sp > 0) else_sp--;
            break;
        case WASM_BR: {
            int label = decode_i32(env->code, &env->pc);
            /* For MVP: jump to END of block at depth 'label' */
            int depth = label;
            int pc2 = env->pc, d = 0;
            while (pc2 < env->code_len) {
                unsigned char n = env->code[pc2];
                if (n == WASM_BLOCK || n == WASM_LOOP || n == WASM_IF) d++;
                else if (n == WASM_END) {
                    if (d == depth) break;
                    d--;
                }
                pc2++;
            }
            env->pc = pc2 + 1;
            break;
        }
        case WASM_BR_IF: {
            int label = decode_i32(env->code, &env->pc);
            int cond = env->stack[--env->sp];
            if (cond) {
                int depth = label;
                int pc2 = env->pc, d = 0;
                while (pc2 < env->code_len) {
                    unsigned char n = env->code[pc2];
                    if (n == WASM_BLOCK || n == WASM_LOOP || n == WASM_IF) d++;
                    else if (n == WASM_END) {
                        if (d == depth) break;
                        d--;
                    }
                    pc2++;
                }
                env->pc = pc2 + 1;
            }
            break;
        }
        case WASM_RETURN:
            env->running = 0;
            break;
        case WASM_UNREACHABLE:
            printf("Unreachable executed\n");
            return -1;
        case WASM_NOP:
            break;
        default:
            printf("Unknown opcode: 0x%02X at pc=%d\n", op, env->pc-1);
            return -1;
        }
    }
    return env->sp > 0 ? env->stack[env->sp - 1] : 0;
}

/* ---- Example usage ---- */
int main(void) {
    /* Example: WASM code for (7 + 5) * 2 */
    unsigned char wasm_code[] = {
        WASM_I32_CONST, 7,
        WASM_I32_CONST, 5,
        WASM_I32_ADD,
        WASM_I32_CONST, 2,
        WASM_I32_MUL,
        WASM_END
    };
    WasmEnv env;
    memset(&env, 0, sizeof(env));
    env.code = wasm_code;
    env.code_len = sizeof(wasm_code);

    int result = wasm_run(&env);
    printf("Result: %d\n", result); /* Should print 24 */
    return 0;
}

/*
 * This code is dedicated to the public domain.
 * You may use it for any purpose, modify, redistribute, or embed it.
 * Attribution is appreciated but not required.
 */