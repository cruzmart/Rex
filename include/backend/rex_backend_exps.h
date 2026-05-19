#pragma once

/// =============================================================
/// Standard Library
/// =============================================================

#include <memory>
#include <string_view>

/// =============================================================
/// LLVM
/// =============================================================

#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_os_ostream.h>

/// =============================================================
/// MLIR - Core IR
/// =============================================================

#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/TypeRange.h"
#include "mlir/IR/Types.h"
#include "mlir/IR/Value.h"
#include "mlir/IR/ValueRange.h"
#include "mlir/IR/Verifier.h"

/// =============================================================
/// MLIR - Dialects
/// =============================================================

#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/ControlFlow/IR/ControlFlow.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Dialect/LLVMIR/LLVMTypes.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Dialect/SCF/IR/SCF.h"

/// =============================================================
/// MLIR - Conversion Passes
/// =============================================================

#include "mlir/Conversion/ArithToLLVM/ArithToLLVM.h"
#include "mlir/Conversion/ControlFlowToLLVM/ControlFlowToLLVM.h"
#include "mlir/Conversion/FuncToLLVM/ConvertFuncToLLVM.h"
#include "mlir/Conversion/FuncToLLVM/ConvertFuncToLLVMPass.h"
#include "mlir/Conversion/MemRefToLLVM/MemRefToLLVM.h"
#include "mlir/Conversion/ReconcileUnrealizedCasts/ReconcileUnrealizedCasts.h"
#include "mlir/Conversion/SCFToControlFlow/SCFToControlFlow.h"

/// =============================================================
/// MLIR - Pass Infrastructure
/// =============================================================

#include "mlir/Pass/Pass.h"
#include "mlir/Pass/PassManager.h"

/// =============================================================
/// MLIR - LLVM Translation
/// =============================================================

#include "mlir/Target/LLVMIR/Dialect/Builtin/BuiltinToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Dialect/LLVMIR/LLVMToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Export.h"

/// =============================================================
/// Backend
/// =============================================================


#include "rex_backend_types.h"

/// =============================================================
/// Frontend / AST
/// =============================================================

#include "rex_errors.h"
#include "rex_exps.h"
#include "rex_types.h"

namespace rex {

/// =============================================================
/// Aliases
/// =============================================================

using lit_ptr = std::shared_ptr<LiteralExpr>;

enum class OpClass {
    Additive,
    Multiplicative,
    Comparison,
    Modulo,
    Logical
};


/// =============================================================
/// ExpressionsHelper
/// =============================================================
/// Responsible for:
///  - Literal lowering
///  - Binary operations
///  - Type casting
///  - String pooling + folding
///  - Tuple construction
///  - Runtime array creation
///  - Array copying/indexing
/// =============================================================

class ExpressionsHelper {

private:

    /// ---------------------------------------------------------
    /// Core IR State
    /// ---------------------------------------------------------

    std::shared_ptr<mlir::OpBuilder> builder;
    mlir::ModuleOp module;
    mlir::Location loc;

    std::shared_ptr<TypesHelper> types;
    Errors errors;

    /// ---------------------------------------------------------
    /// String Pooling
    /// ---------------------------------------------------------

    std::unordered_map<
        std::string,
        mlir::LLVM::GlobalOp
    > stringPool;

    int globalCounter = 0;

    /// ---------------------------------------------------------
    /// Array Pooling
    /// ---------------------------------------------------------

    std::unordered_map<
        std::string,
        mlir::LLVM::GlobalOp
    > arrayPool;

    int arrayCounter = 0;

    /// ---------------------------------------------------------
    /// Builder State
    /// ---------------------------------------------------------

    mlir::OpBuilder::InsertPoint old_insertion_point;
    OpClass opClass;

    mlir::Value i32(int value);
    mlir::LLVM::LLVMPointerType ptrty ();


public:

    /// =========================================================
    /// Construction
    /// =========================================================

    ExpressionsHelper(
        std::shared_ptr<mlir::OpBuilder> b,
        mlir::ModuleOp m,
        mlir::Location l,
        std::shared_ptr<TypesHelper> t
    );

    /// =========================================================
    /// Literal Creation
    /// =========================================================

    mlir::Value createPrimitiveLiteral(
        std::shared_ptr<LiteralExpr> literal_ptr
    );

    mlir::Value createInt(
        const std::string &text
    );

    mlir::Value createFloat(
        const std::string &text
    );

    mlir::Value createBool(
        const std::string &text
    );

    mlir::Value createChar(
        const std::string &text
    );

    /// =========================================================
    /// String Creation
    /// =========================================================

    mlir::Value createString(
        const std::string &text
    );

    /// =========================================================
    /// Tuple Creation
    /// =========================================================

    mlir::Value createTuple(
        const std::vector<mlir::Type> types,
        std::vector<mlir::Value> values
    );


    /// =========================================================
    /// Binary Expressions
    /// =========================================================

    mlir::Value createBinaryExp(
        mlir::Value lhs,
        mlir::Value rhs,
        PrimType::Prims prim_t,
        BinaryOp op
    );

    /// =========================================================
    /// Array Operation TO Scalar/Vector
    /// =========================================================
    mlir::Value getOpDefault(BinaryOp op);
    mlir::Value createVectorVectorOp(
        mlir::Value lhs, 
        std::shared_ptr<ArrayType> lhs_t, 
        mlir::Value rhs, 
        std::shared_ptr<ArrayType> rhs_t, 
        BinaryOp op, 
        std::shared_ptr<ArrayType> res_t
    );
    
    mlir::Value createVectorScalarOp(
        mlir::Value lhs, 
        std::shared_ptr<Type> lhs_t, 
        mlir::Value rhs, 
        std::shared_ptr<Type> rhs_t, 
        BinaryOp op, 
        bool vectorIsLHS, 
        std::shared_ptr<ArrayType> res_t
    );
    
    mlir::Value loadArrayElem(
        mlir::Value array,
        mlir::Type elemTy,
        mlir::Value index
    );
    void storeArrayElem(
        mlir::Value array,
        mlir::Type elemTy,
        mlir::Value index,
        mlir::Value value
    );

    /// =========================================================
    /// Arithmetic
    /// =========================================================

    mlir::Value add(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type typ
    );

    mlir::Value sub(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type typ
    );

    mlir::Value mul(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type typ
    );

    mlir::Value div(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type typ
    );

    mlir::Value mod(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type typ
    );

    /// =========================================================
    /// Comparisons
    /// =========================================================

    mlir::Value cmp(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::arith::CmpIPredicate iPred,
        mlir::arith::CmpFPredicate fPred
    );

    mlir::Value eq(
        mlir::Value lhs,
        mlir::Value rhs
    );

    mlir::Value neq(
        mlir::Value lhs,
        mlir::Value rhs
    );

    mlir::Value lt(
        mlir::Value lhs,
        mlir::Value rhs
    );

    mlir::Value le(
        mlir::Value lhs,
        mlir::Value rhs
    );

    mlir::Value gt(
        mlir::Value lhs,
        mlir::Value rhs
    );

    mlir::Value ge(
        mlir::Value lhs,
        mlir::Value rhs
    );

    /// =========================================================
    /// Logical Operations
    /// =========================================================

    mlir::Value and_(
        mlir::Value lhs,
        mlir::Value rhs
    );

    mlir::Value or_(
        mlir::Value lhs,
        mlir::Value rhs
    );

    /// =========================================================
    /// Indexing
    /// =========================================================

    mlir::Value index(
        mlir::Value arr_p,
        mlir::Value i,
        mlir::Type type
    );

    mlir::Value index(
        mlir::Value tuple_d,
        mlir::LLVM::LLVMStructType stc_t,
        mlir::Type tar_ty,
        mlir::Value i
    );

    /// =========================================================
    /// Casting + Type Promotion
    /// =========================================================

    mlir::Value castTo(
        mlir::Value val,
        mlir::Type targetType
    );

    mlir::Type getComputeType(
        mlir::Type lhs,
        mlir::Type rhs
    );

    /// =========================================================
    /// String Helpers
    /// =========================================================

    mlir::Value concatString(
        mlir::Value str_lhs,
        mlir::Value str_rhs
    );

    mlir::Value toStringValue(
        mlir::Value v
    );

    bool isConstStringExpr(
        std::shared_ptr<Expr> expr
    );

    std::string foldConstString(
        std::shared_ptr<Expr> expr
    );

    bool isStringValue(
        mlir::Value v
    );

    mlir::Value eqStringsConst(
        std::shared_ptr<Expr> lhs,
        std::shared_ptr<Expr> rhs
    );

    mlir::LLVM::LLVMFuncOp getOrInsertStrcmp();

    /// =========================================================
    /// Array Helpers
    /// =========================================================

    bool isConstArrayExpr(
        std::shared_ptr<ArrayExpr> arr
    );

    bool isCompileTimeValue(
        mlir::Value v
    );

    void copyArray(
        mlir::Value dstPtr,
        mlir::Value srcPtr,
        std::shared_ptr<ArrayType> arrTy
    );

    mlir::Value matrixRowPtr(
        mlir::Value arrPtr,
        mlir::Value index,
        std::shared_ptr<ArrayType> arrTy
    );

    mlir::Value arrayElementPtr(
        mlir::Value arrPtr,
        mlir::Value index,
        std::shared_ptr<ArrayType> arrTy
    );

    /// =========================================================
    /// Runtime Array Creation
    /// =========================================================

    mlir::Value createRuntimeArray(
        const std::vector<mlir::Value> &elements,
        std::shared_ptr<PrimType> kind
    );
};

} // namespace rex