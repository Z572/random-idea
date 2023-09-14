#include "guile_mlir.hpp"
#include <scm/scm.hpp>
#include "mlir/Dialect/Affine/Passes.h"
#include "mlir/ExecutionEngine/ExecutionEngine.h"
#include "mlir/ExecutionEngine/OptUtils.h"
#include "mlir/IR/AsmState.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/Verifier.h"
#include "mlir/InitAllDialects.h"
#include "mlir/Parser/Parser.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Target/LLVMIR/Dialect/LLVMIR/LLVMToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Export.h"
#include "mlir/Transforms/Passes.h"
#include "llvm/Support/CommandLine.h"
#include <cstdlib>
#include <libguile.h>
#include <llvm/Support/CommandLine.h>
#include <mlir/IR/Types.h>
namespace cl = llvm::cl;
static cl::opt<bool> enbac("opt", cl::desc("Enable optimizations"));

namespace scm {
  namespace detail {
    template <>
    struct convert<bool> {
      static bool bool_to_cpp(SCM v) {return scm_to_bool(v);}
      static SCM to_scm(bool v) {return scm_from_bool(v);}
    };
  }
}

extern "C" void init_immer() {
  scm::type<mlir::DialectRegistry>("mlir-dialect-registry")
    .constructor()
    .finalizer();

  scm::type<mlir::MLIRContext>("mlir-context")
    .constructor()
    .finalizer()
    .define("load-all-available-dialects", &mlir::MLIRContext::loadAllAvailableDialects)
    .define("allows-unregistered-dialects", &mlir::MLIRContext::allowsUnregisteredDialects);
  ;
  ;

}
