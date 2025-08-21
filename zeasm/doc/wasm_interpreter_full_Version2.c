/*
 * Minimal WASM interpreter with integrated full module parser (public domain, C90)
 * - Parses module header, sections, and code
 * - Interprets i32 arithmetic functions from WASM bytecode
 *
 * Features:
 * - Reads and decodes all sections in a WASM module
 * - Builds function index and code tables
 * - Interprets code of i32-only functions (MVP subset)
 * - Prints module structure
 *
 * Attribution: None required. Public domain.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- WASM constants ---- */
#define WASM_MAGIC "\0asm"
#define WASM_VERSION 0x1
#define MAX_FUNCS 128
#define MAX_TYPES 32
#define MAX_CODE 128
#define MAX_LOCALS 32
#define MAX_STACK 1024

/* ---- WASM types ---- */
enum {
    WASM_TYPE_I32 = 0x7F,
    WASM_TYPE_FUNC = 0x60,
    WASM_TYPE_EMPTY = 0x40
};

/* ---- Section IDs ---- */
enum {
    SEC_TYPE = 1,
    SEC_IMPORT = 2,
    SEC_FUNCTION = 3,
    SEC_TABLE = 4,
    SEC_MEMORY = 5,
    SEC_GLOBAL = 6,
    SEC_EXPORT = 7,
    SEC_START = 8,
    SEC_ELEMENT = 9,
    SEC_CODE = 10,
    SEC_DATA = 11
};

/* ---- Reader ---- */
typedef struct {
    unsigned char *data;
    int len;
    int pos;
} WasmReader;

static int read_u8(WasmReader *r) {
    if (r->pos >= r->len) return -1;
    return r->data[r->pos++];
}
static unsigned int read_u32(WasmReader *r) {
    unsigned int val = 0;
    int i;
    for (i = 0; i < 4; ++i) {
        int b = read_u8(r);
        if (b < 0) return 0;
        val |= b << (i*8);
    }
    return val;
}
static unsigned int read_leb_u32(WasmReader *r) {
    unsigned int result = 0, shift = 0;
    int b;
    do {
        b = read_u8(r);
        result |= (b & 0x7F) << shift;
        shift += 7;
    } while ((b & 0x80) && (r->pos < r->len));
    return result;
}
static int read_leb_i32(WasmReader *r) {
    int result = 0, shift = 0;
    int b;
    do {
        b = read_u8(r);
        result |= (b & 0x7F) << shift;
        shift += 7;
    } while ((b & 0x80) && (r->pos < r->len));
    if ((shift < 32) && (b & 0x40)) result |= (~0U << shift);
    return result;
}
static int read_bytes(WasmReader *r, unsigned char *dst, int n) {
    if (r->pos + n > r->len) return 0;
    memcpy(dst, r->data + r->pos, n);
    r->pos += n;
    return 1;
}

/* ---- Module structures ---- */
typedef struct {
    int param_types[8];
    int param_count;
    int ret_types[2];
    int ret_count;
} WasmFuncType;

typedef struct {
    int type_idx[MAX_FUNCS];
    int count;
} WasmFunctionSection;

typedef struct {
    unsigned char *body[MAX_CODE];
    int body_size[MAX_CODE];
    int count;
    int local_count[MAX_CODE];
    int local_types[MAX_CODE][MAX_LOCALS];
    int local_nums[MAX_CODE][MAX_LOCALS];
} WasmCodeSection;

typedef struct {
    WasmFuncType types[MAX_TYPES];
    int type_count;
    WasmFunctionSection func_sec;
    WasmCodeSection code_sec;
} WasmModule;

/* ---- WASM interpreter state ---- */
typedef struct {
    int stack[MAX_STACK];
    int sp;
    int locals[MAX_LOCALS];
    unsigned char *code;
    int code_len;
    int pc;
    int running;
} WasmEnv;

/* ---- Section parsers ---- */
static void parse_type_section(WasmReader *r, int size, WasmModule *mod) {
    int end = r->pos + size;
    unsigned int count = read_leb_u32(r);
    mod->type_count = count > MAX_TYPES ? MAX_TYPES : count;
    for (unsigned int i = 0; i < count && i < MAX_TYPES && r->pos < end; ++i) {
        int form = read_u8(r); /* Should be 0x60 for func */
        unsigned int param_count = read_leb_u32(r);
        mod->types[i].param_count = param_count;
        for (unsigned int j = 0; j < param_count; ++j)
            mod->types[i].param_types[j] = read_u8(r);
        unsigned int ret_count = read_leb_u32(r);
        mod->types[i].ret_count = ret_count;
        for (unsigned int j = 0; j < ret_count; ++j)
            mod->types[i].ret_types[j] = read_u8(r);
    }
}

static void parse_function_section(WasmReader *r, int size, WasmModule *mod) {
    int end = r->pos + size;
    unsigned int count = read_leb_u32(r);
    mod->func_sec.count = count > MAX_FUNCS ? MAX_FUNCS : count;
    for (unsigned int i = 0; i < count && i < MAX_FUNCS && r->pos < end; ++i)
        mod->func_sec.type_idx[i] = read_leb_u32(r);
}

static void parse_code_section(WasmReader *r, int size, WasmModule *mod) {
    int end = r->pos + size;
    unsigned int count = read_leb_u32(r);
    mod->code_sec.count = count > MAX_CODE ? MAX_CODE : count;
    for (unsigned int i = 0; i < count && i < MAX_CODE && r->pos < end; ++i) {
        unsigned int body_size = read_leb_u32(r);
        int body_start = r->pos;
        unsigned int local_count = read_leb_u32(r);
        mod->code_sec.local_count[i] = local_count;
        for (unsigned int j = 0; j < local_count && j < MAX_LOCALS; ++j) {
            unsigned int num = read_leb_u32(r);
            int type = read_u8(r);
            mod->code_sec.local_nums[i][j] = num;
            mod->code_sec.local_types[i][j] = type;
        }
        int code_len = body_start + body_size - r->pos;
        mod->code_sec.body_size[i] = code_len;
        mod->code_sec.body[i] = malloc(code_len);
        read_bytes(r, mod->code_sec.body[i], code_len);
    }
}

/* ---- Main WASM module parser ---- */
int parse_wasm_module(unsigned char *data, int len, WasmModule *mod) {
    WasmReader rdr;
    rdr.data = data; rdr.len = len; rdr.pos = 0;
    memset(mod, 0, sizeof(WasmModule));

    if (rdr.len < 8 || memcmp(rdr.data, WASM_MAGIC, 4) != 0) {
        printf("Not a WASM file\n");
        return 0;
    }
    rdr.pos = 4;
    unsigned int version = read_u32(&rdr);
    if (version != WASM_VERSION) {
        printf("Unsupported WASM version: %u\n", version);
        return 0;
    }

    while (rdr.pos < rdr.len) {
        int id = read_u8(&rdr);
        unsigned int size = read_leb_u32(&rdr);
        int before = rdr.pos;
        switch (id) {
        case SEC_TYPE:
            parse_type_section(&rdr, size, mod);
            break;
        case SEC_FUNCTION:
            parse_function_section(&rdr, size, mod);
            break;
        case SEC_CODE:
            parse_code_section(&rdr, size, mod);
            break;
        default:
            rdr.pos += size; /* skip other sections */
        }
        if (rdr.pos != before + size) rdr.pos = before + size;
    }
    return 1;
}

/* ---- WASM interpreter (i32 MVP) ---- */
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

static int wasm_run_func(WasmModule *mod, int fidx, int *params, int param_count) {
    if (fidx < 0 || fidx >= mod->code_sec.count) return 0;
    WasmEnv env;
    memset(&env, 0, sizeof(env));
    env.code = mod->code_sec.body[fidx];
    env.code_len = mod->code_sec.body_size[fidx];
    env.sp = 0;
    env.pc = 0;
    env.running = 1;

    /* setup params as locals */
    for (int i = 0; i < param_count && i < MAX_LOCALS; ++i) env.locals[i] = params[i];

    while (env.pc < env.code_len && env.running) {
        unsigned char op = env.code[env.pc++];
        switch (op) {
        case 0x41: /* i32.const */
            env.stack[env.sp++] = decode_i32(env.code, &env.pc);
            break;
        case 0x20: /* local.get */
            {
                int idx = decode_i32(env.code, &env.pc);
                env.stack[env.sp++] = env.locals[idx];
            }
            break;
        case 0x21: /* local.set */
            {
                int idx = decode_i32(env.code, &env.pc);
                env.locals[idx] = env.stack[--env.sp];
            }
            break;
        case 0x6A: /* i32.add */
            env.stack[env.sp-2] += env.stack[env.sp-1]; env.sp--;
            break;
        case 0x6B: /* i32.sub */
            env.stack[env.sp-2] -= env.stack[env.sp-1]; env.sp--;
            break;
        case 0x6C: /* i32.mul */
            env.stack[env.sp-2] *= env.stack[env.sp-1]; env.sp--;
            break;
        case 0x6D: /* i32.div_s */
            env.stack[env.sp-2] /= env.stack[env.sp-1]; env.sp--;
            break;
        case 0x45: /* i32.eqz */
            env.stack[env.sp-1] = (env.stack[env.sp-1] == 0);
            break;
        case 0x46: /* i32.eq */
            env.stack[env.sp-2] = (env.stack[env.sp-2] == env.stack[env.sp-1]); env.sp--;
            break;
        case 0x47: /* i32.ne */
            env.stack[env.sp-2] = (env.stack[env.sp-2] != env.stack[env.sp-1]); env.sp--;
            break;
        case 0x0B: /* end */
            env.running = 0;
            break;
        default:
            printf("Unknown opcode: 0x%02X at pc=%d\n", op, env.pc-1);
            env.running = 0;
            break;
        }
    }
    return env.sp > 0 ? env.stack[env.sp-1] : 0;
}

/* ---- Example usage ---- */
int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s file.wasm\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        printf("Cannot open file\n");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *buf = (unsigned char*)malloc(len);
    fread(buf, 1, len, f);
    fclose(f);

    WasmModule mod;
    if (!parse_wasm_module(buf, len, &mod)) {
        printf("Failed to parse module\n");
        free(buf);
        return 1;
    }
    printf("Parsed module: types=%d, funcs=%d, code=%d\n",
        mod.type_count, mod.func_sec.count, mod.code_sec.count);

    /* Run all functions with zero params (for demo) */
    for (int i = 0; i < mod.code_sec.count; ++i) {
        printf("Running function %d:\n", i);
        int result = wasm_run_func(&mod, i, NULL, 0);
        printf("  Result: %d\n", result);
    }

    /* Cleanup */
    for (int i = 0; i < mod.code_sec.count; ++i)
        free(mod.code_sec.body[i]);
    free(buf);
    return 0;
}

/*
 * Public domain code.
 */