#ifndef WASM_SECTIONS_H
#define WASM_SECTIONS_H

#include "wasm_types.h"

#define WASM_SECTION_TYPE      1
#define WASM_SECTION_IMPORT    2
#define WASM_SECTION_FUNCTION  3
#define WASM_SECTION_TABLE     4
#define WASM_SECTION_MEMORY    5
#define WASM_SECTION_GLOBAL    6
#define WASM_SECTION_EXPORT    7
#define WASM_SECTION_START     8
#define WASM_SECTION_ELEMENT   9
#define WASM_SECTION_CODE      10
#define WASM_SECTION_DATA      11

int parse_type_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset);
int parse_import_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset);
int parse_function_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset);
int parse_table_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset);
int parse_memory_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset);
int parse_global_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset);
int parse_export_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset);
int parse_start_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset);
int parse_element_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset);
int parse_code_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset);
int parse_data_section(const unsigned char* bytes, unsigned int length, WasmModule* module, unsigned int* offset);

void interpret_type_section(const WasmModule* module);
void interpret_import_section(const WasmModule* module);
void interpret_function_section(const WasmModule* module);
void interpret_table_section(const WasmModule* module);
void interpret_memory_section(const WasmModule* module);
void interpret_global_section(const WasmModule* module);
void interpret_export_section(const WasmModule* module);
void interpret_start_section(const WasmModule* module);
void interpret_element_section(const WasmModule* module);
void interpret_code_section(const WasmModule* module);
void interpret_data_section(const WasmModule* module);

#endif /* WASM_SECTIONS_H */