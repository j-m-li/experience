#include "wasm_interpreter.h"
#include "wasm_sections.h"
#include <stdio.h>

void interpret_wasm(const WasmModule* module) {
    printf("=== Interpreting WASM Module ===\n");
    interpret_type_section(module);
    interpret_import_section(module);
    interpret_function_section(module);
    interpret_table_section(module);
    interpret_memory_section(module);
    interpret_global_section(module);
    interpret_export_section(module);
    interpret_start_section(module);
    interpret_element_section(module);
    interpret_code_section(module);
    interpret_data_section(module);
    printf("=== Interpretation Complete ===\n");
}