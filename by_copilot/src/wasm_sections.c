#include "wasm_sections.h"
#include "wasm_types.h"
#include <stdio.h>
#include <string.h>

/* Helper for value type */
static WasmValueType parse_value_type(unsigned char byte) {
    switch (byte) {
        case 0x7F: return WASM_TYPE_I32;
        case 0x7E: return WASM_TYPE_I64;
        case 0x7D: return WASM_TYPE_F32;
        case 0x7C: return WASM_TYPE_F64;
        default:   return WASM_TYPE_UNKNOWN;
    }
}

int parse_type_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset) {
    unsigned int idx = 0, i, j;
    unsigned int count = 0;
    printf("Parsing TYPE section\n");
    if (length > 0) count = bytes[idx++];
    module->type_count = count;
    for (i = 0; i < count && idx < length; ++i) {
        if (bytes[idx++] != 0x60) continue;
        module->types[i].param_count = bytes[idx++];
        for (j = 0; j < module->types[i].param_count && idx < length; ++j)
            module->types[i].param_types[j] = parse_value_type(bytes[idx++]);
        module->types[i].result_count = bytes[idx++];
        for (j = 0; j < module->types[i].result_count && idx < length; ++j)
            module->types[i].result_types[j] = parse_value_type(bytes[idx++]);
    }
    *offset += length;
    return 0;
}

int parse_import_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset) {
    unsigned int idx = 0, i;
    unsigned int count = 0;
    printf("Parsing IMPORT section\n");
    if (length > 0) count = bytes[idx++];
    module->import_count = count;
    for (i = 0; i < count && idx < length; ++i) {
        memcpy(module->imports[i].module, &bytes[idx], 4); idx += 4;
        memcpy(module->imports[i].name, &bytes[idx], 4); idx += 4;
        module->imports[i].module[4] = '\0';
        module->imports[i].name[4] = '\0';
        module->imports[i].kind = (WasmImportKind)bytes[idx++];
        if (module->imports[i].kind == WASM_IMPORT_FUNC && idx < length)
            module->imports[i].desc.func.type_index = bytes[idx++];
    }
    *offset += length;
    return 0;
}

int parse_function_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset) {
    unsigned int idx = 0, i;
    unsigned int count = 0;
    printf("Parsing FUNCTION section\n");
    if (length > 0) count = bytes[idx++];
    module->function_count = count;
    for (i = 0; i < count && idx < length && i < MAX_FUNCTIONS; ++i)
        module->functions[i].type_index = bytes[idx++];
    *offset += length;
    return 0;
}

int parse_table_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset) {
    unsigned int idx = 0, i;
    unsigned int count = 0;
    printf("Parsing TABLE section\n");
    if (length > 0) count = bytes[idx++];
    module->table_count = count;
    for (i = 0; i < count && idx + 1 < length && i < MAX_TABLES; ++i) {
        module->tables[i].min_size = bytes[idx++];
        module->tables[i].max_size = bytes[idx++];
        module->tables[i].elem_count = 0;
    }
    *offset += length;
    return 0;
}

int parse_memory_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset) {
    unsigned int idx = 0, i;
    unsigned int count = 0;
    printf("Parsing MEMORY section\n");
    if (length > 0) count = bytes[idx++];
    module->memory_count = count;
    for (i = 0; i < count && idx + 1 < length && i < MAX_MEMORIES; ++i)
        wasm_memory_init(&module->memories[i], bytes[idx++], bytes[idx++]);
    *offset += length;
    return 0;
}

int parse_global_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset) {
    printf("Parsing GLOBAL section\n");
    wasm_globals_init(&module->globals);
    *offset += length;
    return 0;
}

int parse_export_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset) {
    unsigned int idx = 0, i;
    unsigned int count = 0;
    printf("Parsing EXPORT section\n");
    if (length > 0) count = bytes[idx++];
    module->export_count = count;
    for (i = 0; i < count && idx + 5 < length && i < MAX_EXPORTS; ++i) {
        memcpy(module->exports[i].name, &bytes[idx], 4); idx += 4;
        module->exports[i].name[4] = '\0';
        module->exports[i].kind = (WasmExportKind)bytes[idx++];
        module->exports[i].index = bytes[idx++];
    }
    *offset += length;
    return 0;
}

int parse_start_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset) {
    printf("Parsing START section\n");
    *offset += length;
    return 0;
}

int parse_element_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset) {
    printf("Parsing ELEMENT section\n");
    *offset += length;
    return 0;
}

int parse_code_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset) {
    unsigned int idx = 0, i;
    unsigned int count = 0;
    printf("Parsing CODE section\n");
    if (length > 0) count = bytes[idx++];
    for (i = 0; i < count && idx < length && i < MAX_FUNCTIONS; ++i) {
        unsigned int code_size = bytes[idx++];
        module->functions[i].code_size = code_size;
        memcpy(module->functions[i].code, &bytes[idx], code_size);
        idx += code_size;
    }
    *offset += length;
    return 0;
}

int parse_data_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset) {
    printf("Parsing DATA section\n");
    *offset += length;
    return 0;
}

/* -- Interpreting functions -- */
void interpret_type_section(const WasmModule* module) {
    printf("=== TYPE section ===\n");
    unsigned int i, j;
    for (i = 0; i < module->type_count; ++i) {
        printf("Type %u: (", i);
        for (j = 0; j < module->types[i].param_count; ++j) {
            printf("%s", j > 0 ? ", " : "");
            switch (module->types[i].param_types[j]) {
                case WASM_TYPE_I32: printf("i32"); break;
                case WASM_TYPE_I64: printf("i64"); break;
                case WASM_TYPE_F32: printf("f32"); break;
                case WASM_TYPE_F64: printf("f64"); break;
                default: printf("unknown");
            }
        }
        printf(") -> ");
        for (j = 0; j < module->types[i].result_count; ++j) {
            switch (module->types[i].result_types[j]) {
                case WASM_TYPE_I32: printf("i32"); break;
                case WASM_TYPE_I64: printf("i64"); break;
                case WASM_TYPE_F32: printf("f32"); break;
                case WASM_TYPE_F64: printf("f64"); break;
                default: printf("unknown");
            }
            if (j < module->types[i].result_count - 1) printf(", ");
        }
        printf("\n");
    }
}

void interpret_import_section(const WasmModule* module) {
    printf("=== IMPORT section ===\n");
    unsigned int i;
    for (i = 0; i < module->import_count; ++i) {
        printf("Import %u: module='%s', name='%s', kind=%u\n", i, module->imports[i].module, module->imports[i].name, module->imports[i].kind);
        if (module->imports[i].kind == WASM_IMPORT_FUNC)
            printf("  Function type index: %u\n", module->imports[i].desc.func.type_index);
    }
}

void interpret_function_section(const WasmModule* module) {
    printf("=== FUNCTION section ===\n");
    unsigned int i;
    for (i = 0; i < module->function_count; ++i)
        printf("Function %u: type index=%u\n", i, module->functions[i].type_index);
}

void interpret_table_section(const WasmModule* module) {
    printf("=== TABLE section ===\n");
    unsigned int i;
    for (i = 0; i < module->table_count; ++i)
        printf("Table %u: min=%u, max=%u\n", i, module->tables[i].min_size, module->tables[i].max_size);
}

void interpret_memory_section(const WasmModule* module) {
    printf("=== MEMORY section ===\n");
    unsigned int i;
    for (i = 0; i < module->memory_count; ++i)
        printf("Memory %u: size=%u, max=%u\n", i, module->memories[i].size, module->memories[i].max_size);
}

void interpret_global_section(const WasmModule* module) {
    printf("=== GLOBAL section ===\n");
    unsigned int i;
    for (i = 0; i < WASM_GLOBALS_MAX; ++i)
        printf("Global %u: value=%d\n", i, module->globals.values[i]);
}

void interpret_export_section(const WasmModule* module) {
    printf("=== EXPORT section ===\n");
    unsigned int i;
    for (i = 0; i < module->export_count; ++i)
        printf("Export %u: name='%s', kind=%u, index=%u\n", i, module->exports[i].name, module->exports[i].kind, module->exports[i].index);
}

void interpret_start_section(const WasmModule* module) {
    printf("=== START section ===\n");
    printf("Start section: (not implemented)\n");
}

void interpret_element_section(const WasmModule* module) {
    printf("=== ELEMENT section ===\n");
    printf("Element section: (not implemented)\n");
}

void interpret_code_section(const WasmModule* module) {
    printf("=== CODE section ===\n");
    unsigned int i, j;
    for (i = 0; i < module->function_count; ++i) {
        printf("Function %u code: size=%u\n", i, module->functions[i].code_size);
        printf("  Code bytes: ");
        for (j = 0; j < module->functions[i].code_size; ++j)
            printf("%02X ", module->functions[i].code[j]);
        printf("\n");
    }
}

void interpret_data_section(const WasmModule* module) {
    printf("=== DATA section ===\n");
    printf("Data section: (not implemented)\n");
}