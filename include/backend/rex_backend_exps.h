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


#include "backend/rex_backend_prints.h"
#include "rex_backend_exps.h"
#include "rex_backend_types.h"
#include "rex_types.h"
#include "rex_exps.h"
#include "rex_errors.h"

namespace rex {

    // -------------------------------------------------
// Shared pointer aliases
// -------------------------------------------------

using lit_ptr = std::shared_ptr<LiteralExpr>;

class ExpressionsHelper {

    private:
        
        std::shared_ptr<mlir::OpBuilder> builder;
        mlir::ModuleOp & module;
        mlir::Location loc;
        std::shared_ptr<TypesHelper> types;
        Errors errors;
        std::unordered_map<std::string, mlir::LLVM::GlobalOp> stringPool;

        int globalCounter = 0; // start at 0

        mlir::OpBuilder::InsertPoint old_insertion_point;


    // we are going to make literals and return it
    // than we do operations and return the result

    public:

    ExpressionsHelper(  std::shared_ptr<mlir::OpBuilder> b,
                        mlir::ModuleOp  & m,
                        mlir::Location l,
                        std::shared_ptr<TypesHelper> t);

    // Literal Creation 
    mlir::Value createPrimitiveLiteral(std::shared_ptr<LiteralExpr> literal_ptr);
    mlir::Value createInt(const std::string &text);
    mlir::Value createFloat(const std::string &text);
    mlir::Value createBool(const std::string &text);
    mlir::Value createChar(const std::string &text);

    // String Creation
    mlir::Value createString(const std::string &text);


    // we got to impliment these, I wonder what I should pass..

    // Expressions
    mlir::Value createBinaryExp(mlir::Value lhs, mlir::Value rhs, PrimType::Prims prim_t, BinaryOp op);

    mlir::Value add(mlir::Value lhs, mlir::Value rhs, mlir::Type typ);
    mlir::Value sub(mlir::Value lhs, mlir::Value rhs, mlir::Type typ);
    mlir::Value div(mlir::Value lhs, mlir::Value rhs, mlir::Type typ);
    mlir::Value mod(mlir::Value lhs, mlir::Value rhs, mlir::Type typ);
    mlir::Value mul(mlir::Value lhs, mlir::Value rhs, mlir::Type typ);


    mlir::Value cmp(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::arith::CmpIPredicate iPred,
        mlir::arith::CmpFPredicate fPred
    );
    mlir::Value eq(mlir::Value lhs, mlir::Value rhs);
    mlir::Value neq(mlir::Value lhs, mlir::Value rhs); 
    mlir::Value lt(mlir::Value lhs, mlir::Value rhs); 
    mlir::Value le(mlir::Value lhs, mlir::Value rhs);
    mlir::Value ge(mlir::Value lhs, mlir::Value rhs);
    mlir::Value gt(mlir::Value lhs, mlir::Value rhs);

    mlir::Value and_(mlir::Value lhs, mlir::Value rhs);
    mlir::Value or_(mlir::Value lhs, mlir::Value rhs);

    mlir::Value castTo(mlir::Value val, mlir::Type targetType);
    mlir::Type  getComputeType(mlir::Type lhs, mlir::Type rhs);  


    mlir::Value concatString(mlir::Value str_lhs, mlir::Value str_rhs);
    mlir::Value toStringValue(mlir::Value v);
    bool isConstStringExpr(std::shared_ptr<Expr> expr);
    std::string foldConstString(std::shared_ptr<Expr> expr);
    bool isStringValue(mlir::Value v);
    std::string getStringFromValue(mlir::Value v);
};
}