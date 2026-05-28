#include "backend/rex_backend.h"
#include "backend/rex_backend_exps.h"
#include "backend/rex_backend_prints.h"
#include "backend/rex_backend_visit.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/LLVMIR/LLVMTypes.h"
#include <memory>


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

    
    types = std::make_shared<TypesHelper>(builder, loc);
    visitor = std::make_shared<IRGen>(builder, module, loc, types);
    visitor->exps = std::make_shared<ExpressionsHelper>(builder, module, loc, types);
    visitor->prints = std::make_shared<PrintHelper>(builder, loc, types);
    
    
    
    setupPrintf();
    setupPrintFormats();
    loadPrints();
    //loadRuntimeFuncs();

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

    // The only remaining dialects in our module after the passes are builtin
    // and LLVM. Setup translation patterns to get them to LLVM IR.
    mlir::registerBuiltinDialectTranslation(context);
    mlir::registerLLVMDialectTranslation(context);
    llvm_module = mlir::translateModuleToLLVMIR(module, llvm_context);

    // Create llvm ostream and dump into the output file
    llvm::raw_os_ostream output(os);
    output << *llvm_module;
}


int BackEnd::emitMain(std::shared_ptr<FileAst> file) {

    // =====================================================
    // 1. Emit all function declarations FIRST
    // =====================================================

    visitor->visitFunctionDecls(file);   // or visitFunctions

    // =====================================================
    // 2. Create main
    // =====================================================

    mlir::Type intType =
        mlir::IntegerType::get(&context, 32);

    auto mainType =
        mlir::LLVM::LLVMFunctionType::get(intType, {}, false);

    auto mainFunc =
        builder->create<mlir::LLVM::LLVMFuncOp>(
            loc, "main", mainType
        );

    auto entry = mainFunc.addEntryBlock();
    auto exitBlock = builder->createBlock(&mainFunc.getBody());

    builder->setInsertionPointToStart(entry);

    visitor->contStack.push_back(exitBlock);

    // =====================================================
    // 3. ONLY statements go into main
    // =====================================================

    visitor->visit(file);

    // safety branch
    if (builder->getInsertionBlock() &&
        !visitor->blockHasTerminator(builder->getInsertionBlock())) {
        builder->create<mlir::LLVM::BrOp>(loc, exitBlock);
    }

    visitor->contStack.pop_back();

    // =====================================================
    // 4. exit block
    // =====================================================

    builder->setInsertionPointToStart(exitBlock);

    mlir::Value zero =
        builder->create<mlir::LLVM::ConstantOp>(
            loc,
            intType,
            builder->getIntegerAttr(intType, 0)
        );

    builder->create<mlir::LLVM::ReturnOp>(loc, zero);

    return 0;
}
void BackEnd::createGlobalString(const char *str, const char *name) {
    auto charType = mlir::IntegerType::get(&context, 8);

    auto mlirString = mlir::StringRef(str, strlen(str) + 1);
    auto type = mlir::LLVM::LLVMArrayType::get(charType, mlirString.size());

    builder->create<mlir::LLVM::GlobalOp>(
        loc,
        type,
        true,
        mlir::LLVM::Linkage::Internal,
        name,
        builder->getStringAttr(mlirString),
        0
    );
}

void BackEnd::setupPrintFormats() {
    createGlobalString("%d", "fmt_int");
    createGlobalString("%f", "fmt_float");
    createGlobalString("%c", "fmt_char");
    createGlobalString("%s", "fmt_string");
}

void BackEnd::loadPrints(){
    visitor->prints->printf_func = module.lookupSymbol<mlir::LLVM::LLVMFuncOp>("printf");
    visitor->prints->fmt_int = module.lookupSymbol<mlir::LLVM::GlobalOp>("fmt_int");
    visitor->prints->fmt_float = module.lookupSymbol<mlir::LLVM::GlobalOp>("fmt_float");
    visitor->prints->fmt_char = module.lookupSymbol<mlir::LLVM::GlobalOp>("fmt_char");
    visitor->prints->fmt_string = module.lookupSymbol<mlir::LLVM::GlobalOp>("fmt_string");
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

void BackEnd::loadRuntimeFuncs() {

    auto &ctx = context;

    // ----------------------------
    // char* type
    // ----------------------------
    auto ptr = mlir::LLVM::LLVMPointerType::get(&ctx);

    // ----------------------------
    // rex_string_eq : i1 (i8*, i8*)
    // ----------------------------
    auto i1 = builder->getI1Type();

    auto stringEqType = mlir::LLVM::LLVMFunctionType::get(
        i1,
        {ptr, ptr},
        false
    );

    builder->create<mlir::LLVM::LLVMFuncOp>(
        loc,
        "rex_string_eq",
        stringEqType
    );
}
