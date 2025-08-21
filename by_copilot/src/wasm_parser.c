#include "wasm_parser.h"
#include "wasm_types.h"
#include "wasm_sections.h"
#include <stdio.h>
#include <string.h>

int parse_wasm(const unsigned char* bytes, unsigned int length, WasmModule* out_module) {
    unsigned int offset = 0;
    memset(out_module, 0, sizeof(WasmModule));
    if (length < 8 || bytes[0] != 0x00 || bytes[1] != 0x61 || bytes[2] != 0x73 || bytes[3] != 0x6D)
        return 1;

    out_module->version = bytes[4] | (bytes[5] << 8) | (bytes[6] << 16) | (bytes[7] << 24);
    offset = 8;
    while (offset < length) {
        unsigned char section_id = bytes[offset++];
        unsigned int section_size = bytes[offset++];
        unsigned int section_start = offset;
        switch (section_id) {
            case WASM_SECTION_TYPE:    parse_type_section(bytes + section_start, section_size, out_module, &offset); break;
            case WASM_SECTION_IMPORT:  parse_import_section(bytes + section_start, section_size, out_module, &offset); break;
            case WASM_SECTION_FUNCTION:parse_function_section(bytes + section_start, section_size, out_module, &offset); break;
            case WASM_SECTION_TABLE:   parse_table_section(bytes + section_start, section_size, out_module, &offset); break;
            case WASM_SECTION_MEMORY:  parse_memory_section(bytes + section_start, section_size, out_module, &offset); break;
            case WASM_SECTION_GLOBAL:  parse_global_section(bytes + section_start, section_size, out_module, &offset); break;
            case WASM_SECTION_EXPORT:  parse_export_section(bytes + section_start, section_size, out_module, &offset); break;
            case WASM_SECTION_START:   parse_start_section(bytes + section_start, section_size, out_module, &offset); break;
            case WASM_SECTION_ELEMENT: parse_element_section(bytes + section_start, section_size, out_module, &offset); break;
            case WASM_SECTION_CODE:    parse_code_section(bytes + section_start, section_size, out_module, &offset); break;
            case WASM_SECTION_DATA:    parse_data_section(bytes + section_start, section_size, out_module, &offset); break;
            default: offset += section_size; break;
        }
    }
    return 0;
}