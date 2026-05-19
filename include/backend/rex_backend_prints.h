#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "backend/rex_backend_types.h"
#include "rex_types.h"

// =============================================================
// MLIR IR
// =============================================================

#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/TypeRange.h"
#include "mlir/IR/Types.h"
#include "mlir/IR/Value.h"
#include "mlir/IR/ValueRange.h"
#include "mlir/IR/Verifier.h"

// =============================================================
// Dialects
// =============================================================

#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/ControlFlow/IR/ControlFlow.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Dialect/LLVMIR/LLVMTypes.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Dialect/SCF/IR/SCF.h"

// =============================================================
// Conversion Passes
// =============================================================

#include "mlir/Conversion/ArithToLLVM/ArithToLLVM.h"
#include "mlir/Conversion/ControlFlowToLLVM/ControlFlowToLLVM.h"
#include "mlir/Conversion/FuncToLLVM/ConvertFuncToLLVM.h"
#include "mlir/Conversion/FuncToLLVM/ConvertFuncToLLVMPass.h"
#include "mlir/Conversion/MemRefToLLVM/MemRefToLLVM.h"
#include "mlir/Conversion/ReconcileUnrealizedCasts/ReconcileUnrealizedCasts.h"
#include "mlir/Conversion/SCFToControlFlow/SCFToControlFlow.h"

// =============================================================
// Pass Manager
// =============================================================

#include "mlir/Pass/Pass.h"
#include "mlir/Pass/PassManager.h"

// =============================================================
// LLVM Translation
// =============================================================

#include "llvm/Support/raw_os_ostream.h"

#include "mlir/Target/LLVMIR/Dialect/Builtin/BuiltinToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Dialect/LLVMIR/LLVMToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Export.h"

namespace rex {

struct PrintHelper {

    // =========================================================
    // Runtime Symbols
    // =========================================================

    mlir::LLVM::LLVMFuncOp printf_func;

    mlir::LLVM::GlobalOp fmt_int;
    mlir::LLVM::GlobalOp fmt_float;
    mlir::LLVM::GlobalOp fmt_char;
    mlir::LLVM::GlobalOp fmt_string;

    // =========================================================
    // Shared State
    // =========================================================

    std::shared_ptr<mlir::OpBuilder> builder;
    mlir::Location loc;
    std::shared_ptr<TypesHelper> types;

public:

    PrintHelper(
        std::shared_ptr<mlir::OpBuilder> b,
        mlir::Location l,
        std::shared_ptr<TypesHelper> t
    );

    // =========================================================
    // Low-Level Helpers
    // =========================================================

    mlir::LLVM::AddressOfOp getFmtAddress(mlir::LLVM::GlobalOp fmt);

    mlir::Value i32(int value);
    mlir::Value i8(char value);

    // =========================================================
    // LLVM abstraction layer (IMPORTANT)
    // =========================================================

    mlir::Type ptrTy();

    mlir::Value gep(
        mlir::Type elemTy,
        mlir::Value basePtr,
        mlir::ValueRange indices
    );

    mlir::Value load(
        mlir::Type type,
        mlir::Value ptr
    );

    mlir::Value loadArrayElem(
        mlir::Value arrayPtr,
        mlir::Type elemTy,
        mlir::Value index
    );

    // =========================================================
    // Printing backend
    // =========================================================

    void emitPrintf(mlir::Value fmt, mlir::Value value);
    void emitChar(char c);
    void emitSeparator();

    // =========================================================
    // Control flow helpers
    // =========================================================

    template<typename Fn>
    void forLoop(mlir::Value upperBound, Fn&& body);

    template<typename Fn>
    void emitIfNotLast(
        mlir::Value index,
        mlir::Value size,
        Fn&& body
    );

    // =========================================================
    // Core printing
    // =========================================================

    void printInline(mlir::Value value);

    void printValue(
        mlir::Value value,
        std::shared_ptr<Type> type
    );

    // =========================================================
    // Arrays
    // =========================================================

    void printArray(
        mlir::Value arrayPtr,
        std::shared_ptr<ArrayType> arrType
    );

    void printFlatArray(
        mlir::Value arrayPtr,
        std::shared_ptr<ArrayType> arrType
    );

    void printMatrix(
        mlir::Value arrayPtr,
        std::shared_ptr<ArrayType> arrType
    );

    // =========================================================
    // Indexed
    // =========================================================

    void printIndexed(
        mlir::Value value,
        std::shared_ptr<IndexExpr> idx
    );

    // =========================================================
    // Tuples
    // =========================================================

    void printTuple(
        mlir::Value tupPtr,
        mlir::LLVM::LLVMStructType structTy,
        const std::vector<std::shared_ptr<Type>>& fieldTypes
    );
};

} // namespace rex