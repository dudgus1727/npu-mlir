//===------- RegisterEverything.cpp - Register all MLIR entities ----------===//
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

#include "npu-mlir-c/RegisterEverything.h"
#include "npu-mlir-c/InitAll.h"
#include "mlir-c/RegisterEverything.h"

#include "Target/LLVMIR/Dialect/Gemmini/GemminiToLLVMIRTranslation.h"
#include "mlir/CAPI/IR.h"
#include "mlir/Dialect/DLTI/DLTI.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/InitAllDialects.h"
#include "mlir/InitAllExtensions.h"
#include "mlir/InitAllPasses.h"
#include "mlir/Target/LLVMIR/Dialect/All.h"
#include "mlir/Target/LLVMIR/Dialect/Builtin/BuiltinToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Dialect/LLVMIR/LLVMToLLVMIRTranslation.h"

using namespace npu;
using namespace mlir;

namespace npu {
void registernpuToLLVMIRTranslation();
}

void npuRegisterAllDialects(MlirDialectRegistry registry) {
  // Register all Dialects from UPSTREAM MLIR
  mlir::registerAllDialects(*unwrap(registry));
  mlir::registerAllExtensions(*unwrap(registry));

  // Register all Dialects from npu MLIR
  mlir::npu::registerAllDialects(*unwrap(registry));
}

void npuRegisterAllTranslations(MlirContext context) {
  auto &ctx = *unwrap(context);
  mlir::DialectRegistry registry;
  // Register all Translations from UPSTREAM MLIR
  registry.insert<DLTIDialect, func::FuncDialect>();
  mlir::registerAllToLLVMIRTranslations(registry);

  // Register all Translations from npu MLIR
  registerGemminiDialectTranslation(registry);

  ctx.appendDialectRegistry(registry);
}

void npuRegisterAllPasses() {
  // Register all Passes from UPSTREAM MLIR
  mlir::registerAllPasses();

  // Register all Passes from npu MLIR
  mlir::npu::registerAllPasses();
}
