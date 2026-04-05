#include "backend/rex_backend.h"
#include "backend/rex_print.h"
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


    printer = std::make_shared<PrintHelper>(context, *builder, loc);


    // initialize the types
    types.i32 = builder->getI32Type();
    types.b1 = builder->getI1Type();
    types.f32 = builder->getF32Type();  // For 32-bit float
    types.c8 = builder->getI8Type();
    types.ptr = mlir::LLVM::LLVMPointerType::get(&context);

    setupPrintf();
    setupPrintFormats();
    loadPrints();

    
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


int BackEnd::emitMain(){
    auto funcType = builder->getFunctionType({}, {types.i32});

    auto func = builder->create<mlir::func::FuncOp>(loc, "main", funcType);

    auto &entryBlock = *func.addEntryBlock();
    builder->setInsertionPointToStart(&entryBlock);

    example();

    auto zero = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);

    builder->create<mlir::func::ReturnOp>(loc, mlir::ValueRange{zero});

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
    createGlobalString("%d\n", "fmt_int");
    createGlobalString("%f\n", "fmt_float");
    createGlobalString("%c\n", "fmt_char");
    createGlobalString("%s\n", "fmt_string");
}

void BackEnd::loadPrints(){
    printer->printf_func = module.lookupSymbol<mlir::LLVM::LLVMFuncOp>("printf");
    printer->fmt_int = module.lookupSymbol<mlir::LLVM::GlobalOp>("fmt_int");
    printer->fmt_float = module.lookupSymbol<mlir::LLVM::GlobalOp>("fmt_float");
    printer->fmt_char = module.lookupSymbol<mlir::LLVM::GlobalOp>("fmt_char");
    printer->fmt_string = module.lookupSymbol<mlir::LLVM::GlobalOp>("fmt_string");
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

void BackEnd::example() {
    // Example constants
    auto intVal   = builder->create<mlir::arith::ConstantIntOp>(loc, 42, 32);
    auto floatVal = builder->create<mlir::arith::ConstantFloatOp>(loc, llvm::APFloat(3.14f), types.f32.cast<mlir::FloatType>());
    auto charVal  = builder->create<mlir::arith::ConstantIntOp>(loc, 'A', 8);
    auto boolVal  = builder->create<mlir::arith::ConstantIntOp>(loc, 1, 1); // true

    // Print them
    printer->print(intVal);
    printer->print(floatVal);
    printer->print(charVal);
    printer->print(boolVal);
    //printer->printValue(strVal);


    printer->print(intVal);
}