/*
 * Minimal WASM module parser with detailed section parsing
 * Public domain, C90
 * Parses header, all standard sections, and prints their contents.
 * For educational/demonstration use.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- WASM constants ---- */
#define WASM_MAGIC "\0asm"
#define WASM_VERSION 0x1

/* ---- WASM types ---- */
enum {
    WASM_TYPE_I32 = 0x7F,
    WASM_TYPE_I64 = 0x7E,
    WASM_TYPE_F32 = 0x7D,
    WASM_TYPE_F64 = 0x7C,
    WASM_TYPE_FUNC = 0x60,
    WASM_TYPE_EMPTY = 0x40
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
static unsigned long long read_leb_u64(WasmReader *r) {
    unsigned long long result = 0;
    int shift = 0;
    int b;
    do {
        b = read_u8(r);
        result |= ((unsigned long long)(b & 0x7F)) << shift;
        shift += 7;
    } while ((b & 0x80) && (r->pos < r->len));
    return result;
}
static int read_bytes(WasmReader *r, unsigned char *dst, int n) {
    if (r->pos + n > r->len) return 0;
    memcpy(dst, r->data + r->pos, n);
    r->pos += n;
    return 1;
}

/* ---- Section parsers ---- */
static void parse_type_section(WasmReader *r, int size) {
    int end = r->pos + size;
    unsigned int count = read_leb_u32(r);
    printf("  Types count: %u\n", count);
    for (unsigned int i = 0; i < count && r->pos < end; ++i) {
        int form = read_u8(r); /* Should be 0x60 for func */
        printf("    Type %u: form=0x%X\n", i, form);
        unsigned int param_count = read_leb_u32(r);
        printf("      Params (%u):", param_count);
        for (unsigned int j = 0; j < param_count; ++j) {
            int t = read_u8(r);
            printf(" %s", t==WASM_TYPE_I32?"i32":t==WASM_TYPE_I64?"i64":t==WASM_TYPE_F32?"f32":t==WASM_TYPE_F64?"f64":"other");
        }
        printf("\n");
        unsigned int ret_count = read_leb_u32(r);
        printf("      Results (%u):", ret_count);
        for (unsigned int j = 0; j < ret_count; ++j) {
            int t = read_u8(r);
            printf(" %s", t==WASM_TYPE_I32?"i32":t==WASM_TYPE_I64?"i64":t==WASM_TYPE_F32?"f32":t==WASM_TYPE_F64?"f64":"other");
        }
        printf("\n");
    }
}

static void parse_import_section(WasmReader *r, int size) {
    int end = r->pos + size;
    unsigned int count = read_leb_u32(r);
    printf("  Imports count: %u\n", count);
    for (unsigned int i = 0; i < count && r->pos < end; ++i) {
        unsigned int module_len = read_leb_u32(r);
        char module[256] = {0};
        read_bytes(r, (unsigned char*)module, module_len);
        unsigned int field_len = read_leb_u32(r);
        char field[256] = {0};
        read_bytes(r, (unsigned char*)field, field_len);
        int kind = read_u8(r);
        printf("    Import %u: module='%s' field='%s' kind=%d\n", i, module, field, kind);
        switch (kind) {
        case 0x00: /* func */
            printf("      Func type idx: %u\n", read_leb_u32(r));
            break;
        case 0x01: /* table */
            /* table type */
            r->pos += 4; /* skip for demo */
            break;
        case 0x02: /* memory */
            /* memory type */
            r->pos += 2; /* skip for demo */
            break;
        case 0x03: /* global */
            /* global type */
            r->pos += 2; /* skip for demo */
            break;
        }
    }
}

static void parse_function_section(WasmReader *r, int size) {
    int end = r->pos + size;
    unsigned int count = read_leb_u32(r);
    printf("  Functions count: %u\n", count);
    for (unsigned int i = 0; i < count && r->pos < end; ++i) {
        unsigned int type_idx = read_leb_u32(r);
        printf("    Function %u: type idx %u\n", i, type_idx);
    }
}

static void parse_table_section(WasmReader *r, int size) {
    int end = r->pos + size;
    unsigned int count = read_leb_u32(r);
    printf("  Tables count: %u\n", count);
    for (unsigned int i = 0; i < count && r->pos < end; ++i) {
        int elem_type = read_u8(r);
        printf("    Table %u: elem_type %d\n", i, elem_type);
        /* limits */
        int flags = read_leb_u32(r);
        unsigned int initial = read_leb_u32(r);
        printf("      limits: flags=%d initial=%u", flags, initial);
        if (flags & 1) {
            unsigned int max = read_leb_u32(r);
            printf(" max=%u", max);
        }
        printf("\n");
    }
}

static void parse_memory_section(WasmReader *r, int size) {
    int end = r->pos + size;
    unsigned int count = read_leb_u32(r);
    printf("  Memories count: %u\n", count);
    for (unsigned int i = 0; i < count && r->pos < end; ++i) {
        int flags = read_leb_u32(r);
        unsigned int initial = read_leb_u32(r);
        printf("    Memory %u: flags=%d initial=%u", i, flags, initial);
        if (flags & 1) {
            unsigned int max = read_leb_u32(r);
            printf(" max=%u", max);
        }
        printf("\n");
    }
}

static void parse_global_section(WasmReader *r, int size) {
    int end = r->pos + size;
    unsigned int count = read_leb_u32(r);
    printf("  Globals count: %u\n", count);
    for (unsigned int i = 0; i < count && r->pos < end; ++i) {
        int type = read_u8(r);
        int mut = read_u8(r);
        printf("    Global %u: type=%d mut=%d\n", i, type, mut);
        int opcode = read_u8(r); /* init expr: opcode */
        if (opcode == 0x41) { /* i32.const */
            int val = read_leb_i32(r);
            printf("      Init: i32.const %d\n", val);
        }
        while (1) { /* skip rest of init expr */
            if (read_u8(r) == 0x0B) break;
        }
    }
}

static void parse_export_section(WasmReader *r, int size) {
    int end = r->pos + size;
    unsigned int count = read_leb_u32(r);
    printf("  Exports count: %u\n", count);
    for (unsigned int i = 0; i < count && r->pos < end; ++i) {
        unsigned int name_len = read_leb_u32(r);
        char name[256] = {0};
        read_bytes(r, (unsigned char*)name, name_len);
        int kind = read_u8(r);
        unsigned int idx = read_leb_u32(r);
        printf("    Export %u: name='%s' kind=%d idx=%u\n", i, name, kind, idx);
    }
}

static void parse_start_section(WasmReader *r, int size) {
    unsigned int func_idx = read_leb_u32(r);
    printf("  Start function idx: %u\n", func_idx);
}

static void parse_element_section(WasmReader *r, int size) {
    int end = r->pos + size;
    unsigned int count = read_leb_u32(r);
    printf("  Elements count: %u\n", count);
    for (unsigned int i = 0; i < count && r->pos < end; ++i) {
        unsigned int table_idx = read_leb_u32(r);
        printf("    Element %u: table idx=%u\n", i, table_idx);
        int opcode = read_u8(r); /* usually i32.const */
        if (opcode == 0x41) {
            int val = read_leb_i32(r);
            printf("      Offset: i32.const %d\n", val);
        }
        while (1) { /* skip rest of offset expr */
            if (read_u8(r) == 0x0B) break;
        }
        unsigned int n = read_leb_u32(r);
        printf("      Num function indices: %u\n", n);
        for (unsigned int j = 0; j < n; ++j) {
            unsigned int idx = read_leb_u32(r);
            printf("        Func idx %u\n", idx);
        }
    }
}

static void parse_code_section(WasmReader *r, int size) {
    int end = r->pos + size;
    unsigned int count = read_leb_u32(r);
    printf("  Codes count: %u\n", count);
    for (unsigned int i = 0; i < count && r->pos < end; ++i) {
        unsigned int body_size = read_leb_u32(r);
        int body_start = r->pos;
        unsigned int local_count = read_leb_u32(r);
        printf("    Code %u: body_size=%u locals=%u\n", i, body_size, local_count);
        for (unsigned int j = 0; j < local_count; ++j) {
            unsigned int num = read_leb_u32(r);
            int type = read_u8(r);
            printf("      Local group: num=%u type=%d\n", num, type);
        }
        printf("      Body opcodes:");
        while (r->pos < body_start + body_size) {
            int op = read_u8(r);
            if (op == 0x0B) {
                printf(" END");
                break;
            }
            printf(" 0x%02X", op);
        }
        printf("\n");
    }
}

static void parse_data_section(WasmReader *r, int size) {
    int end = r->pos + size;
    unsigned int count = read_leb_u32(r);
    printf("  Data count: %u\n", count);
    for (unsigned int i = 0; i < count && r->pos < end; ++i) {
        unsigned int memidx = read_leb_u32(r);
        printf("    Data %u: memidx=%u\n", i, memidx);
        int opcode = read_u8(r); /* usually i32.const */
        if (opcode == 0x41) {
            int val = read_leb_i32(r);
            printf("      Offset: i32.const %d\n", val);
        }
        while (1) {
            if (read_u8(r) == 0x0B) break;
        }
        unsigned int size2 = read_leb_u32(r);
        printf("      Bytes: %u\n", size2);
        r->pos += size2; /* skip bytes */
    }
}

/* ---- Section dispatch ---- */
static void parse_section(WasmReader *r, int id, int size) {
    printf("Section %d, size %d, pos %d\n", id, size, r->pos);
    switch (id) {
    case 1: parse_type_section(r, size); break;
    case 2: parse_import_section(r, size); break;
    case 3: parse_function_section(r, size); break;
    case 4: parse_table_section(r, size); break;
    case 5: parse_memory_section(r, size); break;
    case 6: parse_global_section(r, size); break;
    case 7: parse_export_section(r, size); break;
    case 8: parse_start_section(r, size); break;
    case 9: parse_element_section(r, size); break;
    case 10: parse_code_section(r, size); break;
    case 11: parse_data_section(r, size); break;
    default:
        printf("  Custom or unknown section\n");
        r->pos += size;
    }
}

/* ---- Main module parser ---- */
int parse_wasm_module(unsigned char *data, int len) {
    WasmReader rdr;
    rdr.data = data; rdr.len = len; rdr.pos = 0;

    /* Magic */
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
    printf("WASM version: %u\n", version);

    /* Sections */
    while (rdr.pos < rdr.len) {
        int id = read_u8(&rdr);
        unsigned int size = read_leb_u32(&rdr);
        if (rdr.pos + size > rdr.len) {
            printf("Section size out of bounds\n");
            return 0;
        }
        parse_section(&rdr, id, size);
    }
    return 1;
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

    if (!parse_wasm_module(buf, len)) {
        printf("Failed to parse module\n");
        free(buf);
        return 1;
    }
    free(buf);
    return 0;
}

/*
 * Public domain code.
 */