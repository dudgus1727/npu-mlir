//===------- RegisterEverything.h - Register all dialects and passes ------===//
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//===----------------------------------------------------------------------===//

#ifndef npuMLIR_C_REGISTEREVERYTHING_H
#define npuMLIR_C_REGISTEREVERYTHING_H

#include "mlir-c/IR.h"
#include "mlir-c/Support.h"

#ifdef __cplusplus
extern "C" {
#endif

// Registers all dialects with a context.
MLIR_CAPI_EXPORTED void npuRegisterAllDialects(MlirDialectRegistry registry);

// Register all translations to LLVM IR for dialects that can support it.
MLIR_CAPI_EXPORTED void npuRegisterAllTranslations(MlirContext context);

// Registers all passes for symbolic access with the global registry.
MLIR_CAPI_EXPORTED void npuRegisterAllPasses();

#ifdef __cplusplus
}
#endif

#endif // npuMLIR_C_REGISTEREVERYTHING_H
