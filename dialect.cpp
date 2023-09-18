#include "guile_mlir.hpp"
#include "mlir/Dialect/Affine/Passes.h"
#include "mlir/ExecutionEngine/ExecutionEngine.h"
#include "mlir/ExecutionEngine/OptUtils.h"
#include "mlir/IR/AsmState.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/IR/BuiltinDialect.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/ExtensibleDialect.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/Verifier.h"
#include "mlir/InitAllDialects.h"
#include "mlir/Parser/Parser.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Support/TypeID.h"
#include "mlir/Target/LLVMIR/Dialect/LLVMIR/LLVMToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Export.h"
#include "mlir/Transforms/Passes.h"
#include "llvm/Support/CommandLine.h"
#include <cstdlib>
#include <libguile.h>
#include <llvm/Support/CommandLine.h>
#include <memory>
#include <mlir/IR/Types.h>

class SCMDialect : public mlir::Dialect {
public: SCMDialect(mlir::MLIRContext *context): mlir::Dialect ("scheme",context,mlir::TypeID::get<SCMDialect>()){
  initialize();
}
  static llvm::StringRef getDialectNamespace() {
    return "scm";
  }
  void initialize();
};
