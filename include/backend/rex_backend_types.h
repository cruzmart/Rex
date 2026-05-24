#pragma once
#include <cstddef>
#include <string>
#pragma once

// Pass manager
#include <memory>
#include <string_view>


#include "mlir/Conversion/ArithToLLVM/ArithToLLVM.h"
#include "mlir/Conversion/ControlFlowToLLVM/ControlFlowToLLVM.h"
#include "mlir/Conversion/FuncToLLVM/ConvertFuncToLLVM.h"
#include "mlir/Conversion/FuncToLLVM/ConvertFuncToLLVMPass.h"
#include "mlir/Conversion/MemRefToLLVM/MemRefToLLVM.h"
#include "mlir/Conversion/ReconcileUnrealizedCasts/ReconcileUnrealizedCasts.h"
#include "mlir/Conversion/SCFToControlFlow/SCFToControlFlow.h"
#include "mlir/IR/Types.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Pass/PassManager.h"

// Translation
#include "llvm/Support/raw_os_ostream.h"
#include "mlir/Target/LLVMIR/Dialect/Builtin/BuiltinToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Dialect/LLVMIR/LLVMToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Export.h"

// MLIR IR
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/TypeRange.h"
#include "mlir/IR/Value.h"
#include "mlir/IR/ValueRange.h"
#include "mlir/IR/Verifier.h"

// Dialects
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/ControlFlow/IR/ControlFlow.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Dialect/SCF/IR/SCF.h"


// Errors
#include "llvm/Support/Error.h"


#include "mlir/IR/Builders.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/BuiltinOps.h"

#include "rex_exps.h"
#include "rex_types.h"


namespace rex {

struct TypesHelper {

    private:
        std::shared_ptr<mlir::OpBuilder> builder;
        mlir::Location loc;
    public:
        TypesHelper( std::shared_ptr<mlir::OpBuilder> b, mlir::Location l);


    mlir::Type getMLIRType(std::shared_ptr<Type> t);

    mlir::Type i32_t();
    mlir::Type c8_t();
    mlir::Type f32_t();
    mlir::Type b1_t();
    mlir::Type ptrty();

    mlir::LLVM::LLVMStructType createStruct(std::vector<std::shared_ptr<Type>> types);
};

} // namespace rex
