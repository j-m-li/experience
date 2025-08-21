#ifndef WASM_TYPES_H
#define WASM_TYPES_H

#include "../src/wasm_memory.h"
#include "../src/wasm_globals.h"

#define MAX_FUNCTIONS     32
#define MAX_CODE_SIZE     1024
#define MAX_TYPES         16
#define MAX_IMPORTS       16
#define MAX_EXPORTS       16
#define MAX_TABLES        8
#define MAX_MEMORIES      4
#define MAX_PARAMS        8
#define MAX_RESULTS       4
#define WASM_GLOBALS_MAX  32

typedef enum {
    WASM_TYPE_I32 = 0x7F,
    WASM_TYPE_I64 = 0x7E,
    WASM_TYPE_F32 = 0x7D,
    WASM_TYPE_F64 = 0x7C,
    WASM_TYPE_UNKNOWN = 0x00
} WasmValueType;

typedef struct {
    unsigned int param_count;
    WasmValueType param_types[MAX_PARAMS];
    unsigned int result_count;
    WasmValueType result_types[MAX_RESULTS];
} WasmFuncType;

typedef enum {
    WASM_IMPORT_FUNC = 0,
    WASM_IMPORT_TABLE = 1,
    WASM_IMPORT_MEMORY = 2,
    WASM_IMPORT_GLOBAL = 3
} WasmImportKind;

typedef struct {
    char module[32];
    char name[32];
    WasmImportKind kind;
    union {
        struct { unsigned int type_index; } func;
    } desc;
} WasmImport;

typedef enum {
    WASM_EXPORT_FUNC = 0,
    WASM_EXPORT_TABLE = 1,
    WASM_EXPORT_MEMORY = 2,
    WASM_EXPORT_GLOBAL = 3
} WasmExportKind;

typedef struct {
    char name[32];
    WasmExportKind kind;
    unsigned int index;
} WasmExport;

typedef struct {
    unsigned int type_index;
    unsigned int code_offset;
    unsigned int code_size;
    unsigned char code[MAX_CODE_SIZE];
} WasmFunction;

typedef struct {
    unsigned int min_size;
    unsigned int max_size;
    unsigned int elem_count;
    unsigned int elems[MAX_CODE_SIZE];
} WasmTable;

typedef struct {
    unsigned int version;
    WasmFuncType types[MAX_TYPES];
    unsigned int type_count;
    WasmImport imports[MAX_IMPORTS];
    unsigned int import_count;
    WasmFunction functions[MAX_FUNCTIONS];
    unsigned int function_count;
    WasmTable tables[MAX_TABLES];
    unsigned int table_count;
    WasmMemory memories[MAX_MEMORIES];
    unsigned int memory_count;
    WasmGlobals globals;
    WasmExport exports[MAX_EXPORTS];
    unsigned int export_count;
} WasmModule;

#endif /* WASM_TYPES_H */