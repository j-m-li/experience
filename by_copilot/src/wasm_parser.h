#ifndef WASM_PARSER_H
#define WASM_PARSER_H

#include "wasm_types.h"

int parse_wasm(const unsigned char* bytes, unsigned int length, WasmModule* out_module);

#endif /* WASM_PARSER_H */