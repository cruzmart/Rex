#include "backend/rex_backend.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/LLVMIR/LLVMTypes.h"


BackEnd::BackEnd() : loc(mlir::UnknownLoc::get(&context)) {
    // Load Dialects.
    context.loadDialect<mlir::LLVM::LLVMDialect>();
    context.loadDialect<mlir::func::FuncDialect>();
    context.loadDialect<mlir::arith::ArithDialect>();
    context.loadDialect<mlir::scf::SCFDialect>();
    context.loadDialect<mlir::cf::ControlFlowDialect>();
    context.loadDialect<mlir::memref::MemRefDialect>();

    // Initialize the MLIR context
    builder = std::make_shared<mlir::OpBuilder>(&context);
    module = mlir::ModuleOp::create(builder->getUnknownLoc());
    builder->setInsertionPointToStart(module.getBody());

    // initialize the types

    types.i32 = builder->getI32Type();
    types.b1 = builder->getI1Type();
    types.f32 = builder->getF32Type();  // For 32-bit float
    types.c8 = builder->getI8Type();
    types.ptr = mlir::LLVM::LLVMPointerType::get(&context);
}

int BackEnd::lowerDialects() {
    // Set up the MLIR pass manager to iteratively lower all the Ops
    mlir::PassManager pm(&context);

    // Lower Func dialect to LLVM
    pm.addPass(mlir::createConvertFuncToLLVMPass());

    // Lower SCF to CF (ControlFlow)
    pm.addPass(mlir::createConvertSCFToCFPass());

    // Lower Arith to LLVM
    pm.addPass(mlir::createArithToLLVMConversionPass());

    // Lower MemRef to LLVM
    pm.addPass(mlir::createFinalizeMemRefToLLVMConversionPass());

    // Lower CF to LLVM
    pm.addPass(mlir::createConvertControlFlowToLLVMPass());

    // Finalize the conversion to LLVM dialect
    pm.addPass(mlir::createReconcileUnrealizedCastsPass());

    // Run the passes
    if (mlir::failed(pm.run(module))) {
        llvm::errs() << "Pass pipeline failed\n";
        return 1;
    }
    return 0;
}

void BackEnd::dumpLLVM(std::ostream &os, bool debug) {
    if (debug) {
        module.dump();
    }

    // emit module
    if (mlir::failed(mlir::verify(module))) {
        module.emitError("module failed to verify");
        return;
    }

    if (lowerDialects() < 0) { return; }
}


void BackEnd::setupPrintf() {
    // Create a function declaration for printf, the signature is:
    //   * `i32 (ptr, ...)`
    mlir::Type intType = mlir::IntegerType::get(&context, 32);
    auto ptrTy = mlir::LLVM::LLVMPointerType::get(&context);
    auto llvmFnType = mlir::LLVM::LLVMFunctionType::get(intType, ptrTy,
                                                        /*isVarArg=*/true);

    // Insert the printf function into the body of the parent module.
    builder->create<mlir::LLVM::LLVMFuncOp>(loc, "printf", llvmFnType);
}

int BackEnd::emitMain(){
    auto funcType = builder->getFunctionType({}, {types.i32});

    auto func = builder->create<mlir::func::FuncOp>(loc, "main", funcType);

    auto &entryBlock = *func.addEntryBlock();
    builder->setInsertionPointToStart(&entryBlock);

    auto zero = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);

    builder->create<mlir::func::ReturnOp>(loc, mlir::ValueRange{zero});

    return 0;
}