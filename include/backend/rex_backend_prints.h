#pragma once

// Pass manager
#include <string_view>

#include "backend/rex_backend_types.h"
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





#include "mlir/IR/Builders.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/BuiltinOps.h"


#include "rex_types.h"

namespace rex {


struct PrintHelper {

    mlir::LLVM::LLVMFuncOp printf_func;
    mlir::LLVM::GlobalOp fmt_int;
    mlir::LLVM::GlobalOp fmt_float;
    mlir::LLVM::GlobalOp fmt_char;
    mlir::LLVM::GlobalOp fmt_string;

    
    std::shared_ptr<mlir::OpBuilder> builder;
    mlir::Location loc;
    std::shared_ptr<TypesHelper> types;

    public:


        PrintHelper (        
                        std::shared_ptr<mlir::OpBuilder> b,
                        mlir::Location l,
                        std::shared_ptr<TypesHelper> types
                    );

        mlir::LLVM::AddressOfOp getFmtAddress(mlir::LLVM::GlobalOp fmt);

        void printPrimtive(mlir::Value value);
        void printInline(mlir::Value val);




        void printString(mlir::LLVM::GlobalOp val);
        void printArray( mlir::Value arrayPtr, std::shared_ptr<ArrayType> arrType);


};

}