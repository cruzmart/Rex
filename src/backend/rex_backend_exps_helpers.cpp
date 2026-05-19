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

/// Casts a value into another supported primitive type.
///
/// Supported conversions:
///  - int   <-> float
///  - bool  -> int/float/char
///  - char  -> int/float
///  - int   -> bool/char
///
/// Notes:
///  - Pointer casts are intentionally rejected
///  - No-op if source and target types already match
mlir::Value ExpressionsHelper::castTo(
    mlir::Value val,
    mlir::Type targetType
) {
    mlir::Type srcType = val.getType();


    // =========================================================
    // Reject pointer casts
    // =========================================================
    if (srcType.isa<mlir::LLVM::LLVMPointerType>() ||
        targetType.isa<mlir::LLVM::LLVMPointerType>()) {
        llvm::report_fatal_error(
            "Invalid cast involving pointer type"
        );
    }

    // =========================================================
    // No-op cast
    // =========================================================
    if (srcType == targetType) {
        return val;
    }

    // =========================================================
    // INT -> FLOAT
    // =========================================================
    if (srcType.isInteger(32) && targetType.isF32()) {
        return builder->create<mlir::arith::SIToFPOp>(
            loc,
            targetType,
            val
        );
    }

    // =========================================================
    // FLOAT -> INT
    // =========================================================
    if (srcType.isF32() && targetType.isInteger(32)) {
        return builder->create<mlir::arith::FPToSIOp>(
            loc,
            targetType,
            val
        );
    }

    // =========================================================
    // BOOL -> INT
    // =========================================================
    if (srcType.isInteger(1) && targetType.isInteger(32)) {
        return builder->create<mlir::arith::ExtUIOp>(
            loc,
            targetType,
            val
        );
    }

    // =========================================================
    // BOOL -> FLOAT
    // =========================================================
    if (srcType.isInteger(1) && targetType.isF32()) {

        auto i32 =
            builder->create<mlir::arith::ExtUIOp>(
                loc,
                types->i32,
                val
            );

        return builder->create<mlir::arith::SIToFPOp>(
            loc,
            targetType,
            i32
        );
    }

    // =========================================================
    // BOOL -> CHAR
    // =========================================================
    if (srcType.isInteger(1) && targetType.isInteger(8)) {
        return builder->create<mlir::arith::ExtUIOp>(
            loc,
            targetType,
            val
        );
    }

    // =========================================================
    // CHAR -> INT
    // =========================================================
    if (srcType.isInteger(8) && targetType.isInteger(32)) {
        return builder->create<mlir::arith::ExtUIOp>(
            loc,
            targetType,
            val
        );
    }

    // =========================================================
    // CHAR -> FLOAT
    // =========================================================
    if (srcType.isInteger(8) && targetType.isF32()) {

        auto i32 =
            builder->create<mlir::arith::ExtUIOp>(
                loc,
                types->i32,
                val
            );

        return builder->create<mlir::arith::SIToFPOp>(
            loc,
            targetType,
            i32
        );
    }

    // =========================================================
    // INT -> CHAR
    // =========================================================
    if (srcType.isInteger(32) && targetType.isInteger(8)) {
        return builder->create<mlir::arith::TruncIOp>(
            loc,
            targetType,
            val
        );
    }

    // =========================================================
    // INT -> BOOL
    // =========================================================
    if (srcType.isInteger(32) && targetType.isInteger(1)) {

        auto zero =
            builder->create<mlir::arith::ConstantIntOp>(
                loc,
                0,
                32
            );

        return builder->create<mlir::arith::CmpIOp>(
            loc,
            mlir::arith::CmpIPredicate::ne,
            val,
            zero
        );
    }

    llvm_unreachable("Unsupported cast");
}

/// Determines the common computation type used for arithmetic.
///
/// Promotion rules:
///  - If either operand is float -> use f32
///  - Otherwise -> use i32
mlir::Type ExpressionsHelper::getComputeType(
    mlir::Type lhs,
    mlir::Type rhs
) {
    if (lhs.isF32() || rhs.isF32()) {
        return types->f32;
    }

    return types->i32;
}

/// =============================================================
/// String Handling
/// =============================================================

/// Concatenate two *constant* strings at compile time.
/// Requirements:
///  - Both operands must be backed by LLVM globals
///  - No runtime string concat supported here
mlir::Value ExpressionsHelper::concatString(mlir::Value lhs, mlir::Value rhs) {

    // Normalize inputs to string values (i8*)
    lhs = toStringValue(lhs);
    rhs = toStringValue(rhs);

    // Extract AddressOf ops (must be globals)
    auto lhsAddr = lhs.getDefiningOp<mlir::LLVM::AddressOfOp>();
    auto rhsAddr = rhs.getDefiningOp<mlir::LLVM::AddressOfOp>();

    if (!lhsAddr || !rhsAddr) {
        llvm::report_fatal_error("Only constant string concat supported");
    }

    // Lookup global string definitions
    auto lhsGlobal = module.lookupSymbol<mlir::LLVM::GlobalOp>(
        lhsAddr.getGlobalName()
    );

    auto rhsGlobal = module.lookupSymbol<mlir::LLVM::GlobalOp>(
        rhsAddr.getGlobalName()
    );

    // Extract string contents
    auto lhsAttr = lhsGlobal.getValue()->dyn_cast<mlir::StringAttr>();
    auto rhsAttr = rhsGlobal.getValue()->dyn_cast<mlir::StringAttr>();

    std::string lhsStr = lhsAttr.getValue().str();
    std::string rhsStr = rhsAttr.getValue().str();

    // Remove null terminator from LHS to avoid duplication
    if (!lhsStr.empty())
        lhsStr.pop_back();

    std::string combined = lhsStr + rhsStr;

    // Recreate pooled string
    return createString(combined);
}

/// Normalize a value into a string (i8*).
/// Supports:
///  - string values (no-op)
///  - constant char → string
mlir::Value ExpressionsHelper::toStringValue(mlir::Value v) {

    auto type = v.getType();

    // Already a string pointer
    if (type.isa<mlir::LLVM::LLVMPointerType>()) {
        return v;
    }

    // Char → string (constant only)
    if (type.isInteger(8)) {
        if (auto cst = v.getDefiningOp<mlir::arith::ConstantOp>()) {
            auto attr = cst.getValue().dyn_cast<mlir::IntegerAttr>();
            char c = static_cast<char>(attr.getInt());

            return createString(std::string(1, c));
        }

        llvm::report_fatal_error("Runtime char→string not supported yet");
    }

    llvm::report_fatal_error("Cannot convert value to string");
}


/// =============================================================
/// Constant String Folding (AST-level)
/// =============================================================

/// Checks if an expression can be folded into a constant string.
/// Supports:
///  - string literals
///  - char literals
///  - nested string concatenations
bool ExpressionsHelper::isConstStringExpr(std::shared_ptr<Expr> expr) {

    if(expr->exp_kind == ExprKind::Literal){
        auto lit = std::static_pointer_cast<LiteralExpr>(expr);
        auto prim = std::static_pointer_cast<PrimType>(lit->type);

        return prim->prim == PrimType::Prims::String ||
               prim->prim == PrimType::Prims::Char;
    }

    if(expr->exp_kind == ExprKind::Binary){
        auto bin = std::static_pointer_cast<BinaryExpr>(expr);

        if (bin->operation != BinaryOp::ADD)
            return false;

        return isConstStringExpr(bin->lhs) &&
               isConstStringExpr(bin->rhs);
    }

    return false;   
}

/// Recursively folds a constant string expression into a std::string.
/// Handles:
///  - string literals
///  - char literals (with escape parsing)
///  - nested concatenation
std::string ExpressionsHelper::foldConstString(std::shared_ptr<Expr> expr) {

    // ---------- Literal ----------
    if(expr->exp_kind == ExprKind::Literal){
        auto lit = std::static_pointer_cast<LiteralExpr>(expr);
        auto prim = std::static_pointer_cast<PrimType>(lit->type);

        switch (prim->prim) {

            case PrimType::Prims::String:
                return lit->value;

            case PrimType::Prims::Char: {
                const std::string &text = lit->value;

                if (text.empty())
                    llvm::report_fatal_error("Empty char in foldConstString");

                char value;

                if (text[0] == '\\') {
                    if (text.size() != 2)
                        llvm::report_fatal_error("Invalid escaped char");

                    switch (text[1]) {
                        case 'n':  value = '\n'; break;
                        case 't':  value = '\t'; break;
                        case 'r':  value = '\r'; break;
                        case '\\': value = '\\'; break;
                        case '\'': value = '\''; break;
                        case '0':  value = '\0'; break;
                        default:
                            llvm::report_fatal_error("Unknown escape sequence");
                    }
                } else {
                    if (text.size() != 1)
                        llvm::report_fatal_error("Invalid char");

                    value = text[0];
                }

                return std::string(1, value);
            }

            default:
                llvm::report_fatal_error("Non-string literal in foldConstString");
        }
    }

    // ---------- Binary (+ only) ----------
    if(expr->exp_kind == ExprKind::Binary){
        auto bin = std::static_pointer_cast<BinaryExpr>(expr);

        if (bin->operation != BinaryOp::ADD) {
            llvm::report_fatal_error("Only + supported in foldConstString");
        }

        std::string lhs = foldConstString(bin->lhs);
        std::string rhs = foldConstString(bin->rhs);

        return lhs + rhs;
    }

    llvm::report_fatal_error("Expression is not a constant string expression");
}

/// Checks if an MLIR value is a string (i8*).
bool ExpressionsHelper::isStringValue(mlir::Value v){
    return v.getType().isa<mlir::LLVM::LLVMPointerType>();
}

/// Performs constant string equality at compile time.
mlir::Value ExpressionsHelper::eqStringsConst(
    std::shared_ptr<Expr> lhs,
    std::shared_ptr<Expr> rhs
) {
    std::string l = foldConstString(lhs);
    std::string r = foldConstString(rhs);

    bool result = (l == r);

    return builder->create<mlir::arith::ConstantOp>(
        loc,
        builder->getBoolAttr(result)
    );
}


/// =============================================================
/// External Function Handling
/// =============================================================

/// Ensures strcmp is declared in the module.
/// Returns existing declaration or inserts a new one.
mlir::LLVM::LLVMFuncOp ExpressionsHelper::getOrInsertStrcmp() {

    if (auto func = module.lookupSymbol<mlir::LLVM::LLVMFuncOp>("strcmp")) {
        return func;
    }

    auto i8Ptr = mlir::LLVM::LLVMPointerType::get(builder->getContext());

    // int strcmp(i8*, i8*)
    auto fnType = mlir::LLVM::LLVMFunctionType::get(
        builder->getI32Type(),
        {i8Ptr, i8Ptr},
        false
    );

    // Must be inserted at module scope
    auto oldIP = builder->saveInsertionPoint();
    builder->setInsertionPointToStart(module.getBody());

    auto func = builder->create<mlir::LLVM::LLVMFuncOp>(
        loc,
        "strcmp",
        fnType
    );

    builder->restoreInsertionPoint(oldIP);

    return func;
}


}