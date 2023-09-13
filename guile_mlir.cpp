#include <cstdlib>
#include <libguile.h>
#include "guile_mlir.hpp"
#include <scm/scm.hpp>
#include <mlir/IR/Types.h>
#include "libguile/modules.h"
#include "libguile/numbers.h"
#include "libguile/scm.h"
#include "mlir/Dialect/Affine/Passes.h"
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
namespace cl = llvm::cl;
namespace guile_mlir {

Guile_mlir::Guile_mlir() {
    number = 6;
}

int Guile_mlir::get_number() const {
  return number;
}


}


class mm {
    std::size_t count_ = 0;
public:
    std::size_t get() const { return count_; }
    void tick() { ++count_; }
};

template <int I>
void func()
{
    auto port = scm_current_warning_port();
    scm_puts("~~ func", port);
    scm_display(scm_from_int(I), port);
    scm_newline(port);
}

static cl::opt<bool> enbac("opt",cl::desc("Enable optimizations"));

extern "C" void init_immer() {
  mlir::MLIRContext context;
  scm::type<mm>("mlir")
    .constructor()
    .define("get", &mm::get);

    scm::group("mlir")
      .define("registerAsmPrinterCLOptions",&mlir::registerAsmPrinterCLOptions)
      .define("func1", func<1>)
      .define("ParseCommandLineOptions", [](
                                            scm::val argc,
                                            scm::val argv,
                                            scm::args usedouble){

        return 1;
});
}
