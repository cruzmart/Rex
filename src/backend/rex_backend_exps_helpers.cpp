#include "backend/rex_backend_exps.h"
#include "rex_exps.h"
#include "rex_ops.h"
#include "rex_types.h"
#include <memory>
#include <mlir/Conversion/ArithToLLVM/ArithToLLVM.h>
#include <mlir/Dialect/Arith/IR/Arith.h>
#include <mlir/Dialect/LLVMIR/LLVMDialect.h>
#include <mlir/Dialect/LLVMIR/LLVMTypes.h>
#include <mlir/IR/Builders.h>
#include <mlir/IR/BuiltinAttributes.h>
#include <mlir/IR/ValueRange.h>

namespace rex {
                        /// =============================================================
                        ///                         Helpers
                        /// =============================================================


/// =============================================================
/// Casting + Type Promotion
/// =============================================================

/// Casts a primitive value into another primitive type.
///
/// Supported:
///  - int   <-> float
///  - bool  -> int/float/char
///  - char  -> int/float
///  - int   -> bool/char
///
/// Notes:
///  - Pointer casts are rejected
///  - Same-type casts are no-ops
mlir::Value ExpressionsHelper::castTo(
    mlir::Value val,
    mlir::Type targetTy
) {

    auto srcTy =
        val.getType();

    // =====================================================
    // INVALID POINTER CASTS
    // =====================================================

    if (srcTy.isa<mlir::LLVM::LLVMPointerType>() ||
        targetTy.isa<mlir::LLVM::LLVMPointerType>()) {

        llvm::report_fatal_error(
            "Invalid cast involving pointer type"
        );
    }

    // =====================================================
    // NO-OP
    // =====================================================

    if (srcTy == targetTy)
        return val;

    // =====================================================
    // INT -> FLOAT
    // =====================================================

    if (srcTy.isInteger(32) && targetTy.isF32()) {
        return builder->create<mlir::arith::SIToFPOp>(
            loc,
            targetTy,
            val
        );
    }

    // =====================================================
    // FLOAT -> INT
    // =====================================================

    if (srcTy.isF32() && targetTy.isInteger(32)) {
        return builder->create<mlir::arith::FPToSIOp>(
            loc,
            targetTy,
            val
        );
    }

    // =====================================================
    // BOOL -> INT/CHAR
    // =====================================================

    if (srcTy.isInteger(1) &&
       (targetTy.isInteger(32) || targetTy.isInteger(8))) {

        return builder->create<mlir::arith::ExtUIOp>(
            loc,
            targetTy,
            val
        );
    }

    // =====================================================
    // BOOL -> FLOAT
    // =====================================================

    if (srcTy.isInteger(1) && targetTy.isF32()) {

        auto i32Val =
            builder->create<mlir::arith::ExtUIOp>(
                loc,
                types->i32_t(),
                val
            );

        return builder->create<mlir::arith::SIToFPOp>(
            loc,
            targetTy,
            i32Val
        );
    }

    // =====================================================
    // CHAR -> INT
    // =====================================================

    if (srcTy.isInteger(8) && targetTy.isInteger(32)) {
        return builder->create<mlir::arith::ExtUIOp>(
            loc,
            targetTy,
            val
        );
    }

    // =====================================================
    // CHAR -> FLOAT
    // =====================================================

    if (srcTy.isInteger(8) && targetTy.isF32()) {

        auto i32Val =
            builder->create<mlir::arith::ExtUIOp>(
                loc,
                types->i32_t(),
                val
            );

        return builder->create<mlir::arith::SIToFPOp>(
            loc,
            targetTy,
            i32Val
        );
    }

    // =====================================================
    // INT -> CHAR
    // =====================================================

    if (srcTy.isInteger(32) && targetTy.isInteger(8)) {
        return builder->create<mlir::arith::TruncIOp>(
            loc,
            targetTy,
            val
        );
    }

    // =====================================================
    // INT -> BOOL
    // =====================================================

    if (srcTy.isInteger(32) && targetTy.isInteger(1)) {

        return builder->create<mlir::arith::CmpIOp>(
            loc,
            mlir::arith::CmpIPredicate::ne,
            val,
            i32(0)
        );
    }

    llvm_unreachable("Unsupported cast");
}

/// Determines the common arithmetic compute type.
///
/// Rules:
///  - float present -> f32
///  - otherwise -> i32
mlir::Type ExpressionsHelper::getComputeType(
    mlir::Type lhs,
    mlir::Type rhs
) {

    if (lhs.isF32() || rhs.isF32())
        return types->f32_t();

    return types->i32_t();
}

/// =============================================================
/// String Handling
/// =============================================================

/// Concatenates two constant strings.
///
/// Requirements:
///  - both operands must be backed by LLVM globals
///  - runtime concat is unsupported
mlir::Value ExpressionsHelper::concatString(
    mlir::Value lhs,
    mlir::Value rhs
) {

    lhs = toStringValue(lhs);
    rhs = toStringValue(rhs);

    auto lhsAddr =
        lhs.getDefiningOp<mlir::LLVM::AddressOfOp>();

    auto rhsAddr =
        rhs.getDefiningOp<mlir::LLVM::AddressOfOp>();

    if (!lhsAddr || !rhsAddr) {
        llvm::report_fatal_error(
            "Only constant string concat supported"
        );
    }

    auto lhsGlobal =
        module.lookupSymbol<mlir::LLVM::GlobalOp>(
            lhsAddr.getGlobalName()
        );

    auto rhsGlobal =
        module.lookupSymbol<mlir::LLVM::GlobalOp>(
            rhsAddr.getGlobalName()
        );

    auto lhsAttr =
        lhsGlobal.getValue()->dyn_cast<mlir::StringAttr>();

    auto rhsAttr =
        rhsGlobal.getValue()->dyn_cast<mlir::StringAttr>();

    std::string lhsStr =
        lhsAttr.getValue().str();

    std::string rhsStr =
        rhsAttr.getValue().str();

    // remove duplicated null terminator
    if (!lhsStr.empty())
        lhsStr.pop_back();

    return createString(lhsStr + rhsStr);
}

/// Converts a value into a string-compatible value.
///
/// Supported:
///  - string pointer (no-op)
///  - constant char -> string
mlir::Value ExpressionsHelper::toStringValue(
    mlir::Value v
) {

    auto ty =
        v.getType();

    // =====================================================
    // ALREADY STRING
    // =====================================================

    if (ty.isa<mlir::LLVM::LLVMPointerType>())
        return v;

    // =====================================================
    // CHAR -> STRING
    // =====================================================

    if (ty.isInteger(8)) {

        auto cst =
            v.getDefiningOp<mlir::arith::ConstantOp>();

        if (!cst) {
            llvm::report_fatal_error(
                "Runtime char->string not supported yet"
            );
        }

        auto attr =
            cst.getValue().dyn_cast<mlir::IntegerAttr>();

        char c =
            static_cast<char>(attr.getInt());

        return createString(
            std::string(1, c)
        );
    }

    llvm::report_fatal_error(
        "Cannot convert value to string"
    );
}

/// =============================================================
/// Constant String Folding
/// =============================================================

/// Returns true if an expression is a compile-time string.
bool ExpressionsHelper::isConstStringExpr(
    std::shared_ptr<Expr> expr
) {

    // =====================================================
    // LITERAL
    // =====================================================

    if (expr->exp_kind == ExprKind::Literal) {

        auto lit =
            std::static_pointer_cast<LiteralExpr>(expr);

        auto prim =
            std::static_pointer_cast<PrimType>(lit->type);

        return
            prim->prim == PrimType::Prims::String ||
            prim->prim == PrimType::Prims::Char;
    }

    // =====================================================
    // BINARY CONCAT
    // =====================================================

    if (expr->exp_kind == ExprKind::Binary) {

        auto bin =
            std::static_pointer_cast<BinaryExpr>(expr);

        if (bin->operation != BinaryOp::ADD)
            return false;

        return
            isConstStringExpr(bin->lhs) &&
            isConstStringExpr(bin->rhs);
    }

    return false;
}

/// Folds a constant string expression into std::string.
std::string ExpressionsHelper::foldConstString(
    std::shared_ptr<Expr> expr
) {

    // =====================================================
    // LITERAL
    // =====================================================

    if (expr->exp_kind == ExprKind::Literal) {

        auto lit =
            std::static_pointer_cast<LiteralExpr>(expr);

        auto prim =
            std::static_pointer_cast<PrimType>(lit->type);

        switch (prim->prim) {

            case PrimType::Prims::String:
                return lit->value;

            case PrimType::Prims::Char: {

                auto text =
                    lit->value;

                if (text.empty()) {
                    llvm::report_fatal_error(
                        "Empty char in foldConstString"
                    );
                }

                // escaped char
                if (text[0] == '\\') {

                    if (text.size() != 2) {
                        llvm::report_fatal_error(
                            "Invalid escaped char"
                        );
                    }

                    switch (text[1]) {

                        case 'n':  return "\n";
                        case 't':  return "\t";
                        case 'r':  return "\r";
                        case '\\': return "\\";
                        case '\'': return "'";
                        case '0':  return "\0";

                        default:
                            llvm::report_fatal_error(
                                "Unknown escape sequence"
                            );
                    }
                }

                // regular char
                if (text.size() != 1) {
                    llvm::report_fatal_error(
                        "Invalid char"
                    );
                }

                return text;
            }

            default:
                llvm::report_fatal_error(
                    "Non-string literal in foldConstString"
                );
        }
    }

    // =====================================================
    // CONCAT
    // =====================================================

    if (expr->exp_kind == ExprKind::Binary) {

        auto bin =
            std::static_pointer_cast<BinaryExpr>(expr);

        if (bin->operation != BinaryOp::ADD) {
            llvm::report_fatal_error(
                "Only + supported in foldConstString"
            );
        }

        return
            foldConstString(bin->lhs) +
            foldConstString(bin->rhs);
    }

    llvm::report_fatal_error(
        "Expression is not a constant string expression"
    );
}

/// Returns true if a value is a string pointer.
bool ExpressionsHelper::isStringValue(
    mlir::Value v
) {
    return
        v.getType().isa<mlir::LLVM::LLVMPointerType>();
}

/// Performs constant string equality at compile time.
mlir::Value ExpressionsHelper::eqStringsConst(
    std::shared_ptr<Expr> lhs,
    std::shared_ptr<Expr> rhs
) {

    bool result =
        foldConstString(lhs) ==
        foldConstString(rhs);

    return builder->create<mlir::arith::ConstantOp>(
        loc,
        builder->getBoolAttr(result)
    );
}

/// =============================================================
/// External Function Handling
/// =============================================================

/// Ensures strcmp exists in the module.
mlir::LLVM::LLVMFuncOp ExpressionsHelper::getOrInsertStrcmp() {

    if (auto func =
        module.lookupSymbol<mlir::LLVM::LLVMFuncOp>(
            "strcmp"
        )) {
        return func;
    }

    auto i8Ptr =
        mlir::LLVM::LLVMPointerType::get(
            builder->getContext()
        );

    auto fnTy =
        mlir::LLVM::LLVMFunctionType::get(
            builder->getI32Type(),
            {i8Ptr, i8Ptr},
            false
        );

    auto oldIP =
        builder->saveInsertionPoint();

    builder->setInsertionPointToStart(
        module.getBody()
    );

    auto func =
        builder->create<mlir::LLVM::LLVMFuncOp>(
            loc,
            "strcmp",
            fnTy
        );

    builder->restoreInsertionPoint(
        oldIP
    );

    return func;
}


}