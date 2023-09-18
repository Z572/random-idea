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
#include <scm/scm.hpp>
namespace cl = llvm::cl;
static cl::opt<bool> enbac("opt", cl::desc("Enable optimizations"));

// class ConstantOp : public mlir::Op ConstantOp,
//                    mlir::OpTrait::ZeroOperands,
//                    mlir::OpTrait::OneResult,
//                    mlir::OpTrait::OneTypedResult<mlir::TensorType>:mlir::impl  {
// public:
//   using mlir::Op;
//   static llvm::StringRef getOperationName() {return "gm.constant"};
//   mlir::DenseElementsAttr getValue();

// };

namespace scm {
  namespace detail {
  template <> struct convert<bool> {
    static bool to_cpp(SCM v) { return scm_to_bool(v); }
    static SCM to_scm(bool v) { return scm_from_bool(v); }
  };
  } // namespace detail
} // namespace scm

extern "C" void init_mlir() {
  scm::type<mlir::DialectRegistry>("mlir-dialect-registry")
      .constructor()
      .finalizer();
  scm::type<llvm::SMLoc>("llvm-smloc").constructor();
  scm::type<mlir::Dialect>("mlir-dialect")
      .define("get-namespace", &mlir::Dialect::getNamespace)
      .define("allows-unknown-operations",
              &mlir::Dialect::allowsUnknownOperations);
  scm::type<mlir::TypeID>("mlir-typeid")
    .constructor();
  scm::type<mlir::BuiltinDialect>("mlir-builtin-dialect").finalizer();
  scm::type<mlir::Type>("mlir-type").define("is-F128?", &mlir::Type::isF128);
  scm::type<mlir::Block>("mlir-block")
      .define("is-entry-block?", &mlir::Block::isEntryBlock);
  scm::type<mlir::OpBuilder>("mlir-opbuilder");
  scm::type<mlir::OpState>("mlir-opstate")
      .define("use-empty?", &mlir::OpState::use_empty);
  scm::type<mlir::MLIRContext>("mlir-context")
      .maker()
      .finalizer()
      .define("load-all-available-dialects",
              &mlir::MLIRContext::loadAllAvailableDialects)
      .define("allows-unregistered-dialects",
              &mlir::MLIRContext::allowsUnregisteredDialects)
      .define("append-dialect-registry",
              [](mlir::MLIRContext &self, mlir::DialectRegistry &o) {
                self.appendDialectRegistry(o);
              })
      .define("operation-registered?",
              [](mlir::MLIRContext &self, scm::val str) {
                return self.isOperationRegistered(
                    llvm::StringRef(scm_to_utf8_string(str)));
              })
      .define("multithreading?", &mlir::MLIRContext::isMultithreadingEnabled)
      .define("set-multithreading!", &mlir::MLIRContext::enableMultithreading)
      .define("get-num-threads", &mlir::MLIRContext::getNumThreads);
  scm::type<mlir::Builder>("mlir-builder")
      .constructor(
                   [](mlir::MLIRContext &context) { return mlir::Builder(&context); });
  scm::type<mlir::PassManager>("mlir-passmanager")
    // .constructor([&](mlir::MLIRContext &context,scm::val nest,std::string&str) { return mlir::PassManager(&context ,mlir::PassManager::Nesting(scm_to_int8(nest)),llvm::StringRef(str)); })
    ;
}
