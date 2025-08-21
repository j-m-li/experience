#ifndef WASM_INTERPRETER_H
#define WASM_INTERPRETER_H

#include "wasm_types.h"

void interpret_wasm(const WasmModule* module);

#endif /* WASM_INTERPRETER_H */