#include "backend/rex_backend_exps.h"
#include <mlir/Conversion/ArithToLLVM/ArithToLLVM.h>
#include <mlir/Dialect/Arith/IR/Arith.h>
#include <mlir/Dialect/LLVMIR/LLVMDialect.h>
#include <mlir/Dialect/LLVMIR/LLVMTypes.h>
#include <mlir/IR/Builders.h>
#include <mlir/IR/BuiltinAttributes.h>
#include <mlir/IR/ValueRange.h>
#include "rex_ops.h"


namespace rex {

/// Expression Binary

/// =============================================================
/// Type Helpers
/// =============================================================

    inline bool isPointer(mlir::Type t) {
        return t.isa<mlir::LLVM::LLVMPointerType>();
    }

    inline bool isFloat(mlir::Type t) {
        return t.isF32();
    }
    mlir::Value ExpressionsHelper::dispatchArithmetic(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type resultTy,
        BinaryOp op
    ) {
        switch (op) {


            case BinaryOp::ADD: return add(lhs, rhs, resultTy);
            case BinaryOp::SUB: return sub(lhs, rhs, resultTy);
            case BinaryOp::MUL: return mul(lhs, rhs, resultTy);
            case BinaryOp::DIV: return div(lhs, rhs, resultTy);
            case BinaryOp::MOD: return mod(lhs, rhs, resultTy);

            default:
                return {};
        }
    }
    mlir::Value ExpressionsHelper::dispatchCompare(
        mlir::Value lhs,
        mlir::Value rhs,
        BinaryOp op
    ) {
        switch (op) {

            case BinaryOp::EQ:  return eq(lhs, rhs);
            case BinaryOp::NEQ: return neq(lhs, rhs);
            case BinaryOp::LT:  return lt(lhs, rhs);
            case BinaryOp::LE:  return le(lhs, rhs);
            case BinaryOp::GT:  return gt(lhs, rhs);
            case BinaryOp::GE:  return ge(lhs, rhs);

            default:
                return {};
        }
    }

    mlir::Value ExpressionsHelper::dispatchLogical(
        mlir::Value lhs,
        mlir::Value rhs,
        BinaryOp op
    ) {
        switch (op) {

            case BinaryOp::AND: return and_(lhs, rhs);
            case BinaryOp::OR:  return or_(lhs, rhs);

            default:
                return {};
        }
    }

    mlir::Type ExpressionsHelper::resolvePrimType(PrimType::Prims prim) {
        switch (prim) {

            case PrimType::Prims::Int:
                return types->i32_t();

            case PrimType::Prims::Real:
                return types->f32_t();

            case PrimType::Prims::Bool:
                return types->b1_t();

            case PrimType::Prims::Char:
                return types->c8_t();

            default:
                llvm::report_fatal_error("Unknown primitive type");
        }
    }


    /// =============================================================
    /// BINARY EXPRESSION CREATION
    /// =============================================================


    mlir::Value ExpressionsHelper::createBinaryExp(
        mlir::Value lhs,
        mlir::Value rhs,
        PrimType::Prims prim,
        BinaryOp op
    ) {

        // =====================================================
        // STRING CONCAT SPECIAL CASE
        // =====================================================

        if (isPointer(lhs.getType()) &&
            isPointer(rhs.getType()) &&
            op == BinaryOp::ADD) {

            return concatString(lhs, rhs);
        }

        // =====================================================
        // TYPE RESOLUTION
        // =====================================================

        mlir::Type resultTy = resolvePrimType(prim);

        // =====================================================
        // DISPATCH
        // =====================================================

        if (auto arith = dispatchArithmetic(lhs, rhs, resultTy, op); arith) {
            return arith;
        }

        if (auto cmp = dispatchCompare(lhs, rhs, op); cmp) {
            return cmp;
        }

        if (auto log = dispatchLogical(lhs, rhs, op); log) {
            return log;
        }

        llvm::report_fatal_error("Unsupported binary operator");
    }


    /// =============================================================
    /// Generic IR Emitters
    /// =============================================================

    template<typename IntOp, typename FloatOp>
    inline mlir::Value ExpressionsHelper::emitArithmeticOp(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type resultTy
    ) {
        auto computeTy = getComputeType(lhs.getType(), rhs.getType());

        lhs = castTo(lhs, computeTy);
        rhs = castTo(rhs, computeTy);

        mlir::Value result;

        if (isFloat(computeTy)) {
            result = builder->create<FloatOp>(loc, lhs, rhs);
        } else {
            result = builder->create<IntOp>(loc, lhs, rhs);
        }

        return castTo(result, resultTy);
    }

    template<typename IntCmp, typename FloatCmp>
    inline mlir::Value ExpressionsHelper::emitCompareOp(
        mlir::Value lhs,
        mlir::Value rhs,
        IntCmp iPred,
        FloatCmp fPred
    ) {
        auto computeTy = getComputeType(lhs.getType(), rhs.getType());

        lhs = castTo(lhs, computeTy);
        rhs = castTo(rhs, computeTy);

        if (isFloat(computeTy)) {
            return builder->create<mlir::arith::CmpFOp>(
                loc, fPred, lhs, rhs
            );
        }

        return builder->create<mlir::arith::CmpIOp>(
            loc, iPred, lhs, rhs
        );
    }

    /// =============================================================
    /// Validation Helpers
    /// =============================================================

    void ExpressionsHelper::checkConstantZero(
        mlir::Value value,
        llvm::StringRef message
    ) {

        auto cst =
            value.getDefiningOp<
                mlir::arith::ConstantOp
            >();

        if (!cst)
            return;

        if (auto intAttr =
                cst.getValue().dyn_cast<mlir::IntegerAttr>()) {

            if (intAttr.getValue().isZero()) {
                llvm::report_fatal_error(message);
            }
        }

        if (auto floatAttr =
                cst.getValue().dyn_cast<mlir::FloatAttr>()) {

            if (floatAttr.getValue().isZero()) {
                llvm::report_fatal_error(message);
            }
        }
    }

    /// =============================================================
    /// Arithmetic
    /// =============================================================

    mlir::Value ExpressionsHelper::add(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type resultTy
    ) {
        return emitArithmeticOp<
            mlir::arith::AddIOp,
            mlir::arith::AddFOp
        >(lhs, rhs, resultTy);
    }

    mlir::Value ExpressionsHelper::sub(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type resultTy
    ) {
        return emitArithmeticOp<
            mlir::arith::SubIOp,
            mlir::arith::SubFOp
        >(lhs, rhs, resultTy);
    }

    mlir::Value ExpressionsHelper::mul(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type resultTy
    ) {
        return emitArithmeticOp<
            mlir::arith::MulIOp,
            mlir::arith::MulFOp
        >(lhs, rhs, resultTy);
    }

    mlir::Value ExpressionsHelper::div(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type resultTy
    ) {

        checkConstantZero(
            rhs,
            "Division by zero"
        );

        return emitArithmeticOp<
            mlir::arith::DivSIOp,
            mlir::arith::DivFOp
        >(lhs, rhs, resultTy);
    }

    

    /// =============================================================
    /// Modulo
    /// =============================================================

    mlir::Value ExpressionsHelper::mod(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type resultTy
    ) {

        auto computeTy =
            getComputeType(
                lhs.getType(),
                rhs.getType()
            );

        if (computeTy.isF32()) {
            llvm::report_fatal_error(
                "Modulo not supported for float"
            );
        }

        lhs = castTo(lhs, computeTy);
        rhs = castTo(rhs, computeTy);

        checkConstantZero(
            rhs,
            "Modulo by zero"
        );

        auto result =
            builder->create<
                mlir::arith::RemSIOp
            >(
                loc,
                lhs,
                rhs
            );

        return castTo(result, resultTy);
    }

    /// =============================================================
    /// Comparisons
    /// =============================================================

    mlir::Value ExpressionsHelper::lt(
        mlir::Value lhs,
        mlir::Value rhs
    ) {
        return emitCompareOp(
            lhs,
            rhs,
            mlir::arith::CmpIPredicate::slt,
            mlir::arith::CmpFPredicate::OLT
        );
    }

    mlir::Value ExpressionsHelper::gt(
        mlir::Value lhs,
        mlir::Value rhs
    ) {
        return emitCompareOp(
            lhs,
            rhs,
            mlir::arith::CmpIPredicate::sgt,
            mlir::arith::CmpFPredicate::OGT
        );
    }
    mlir::Value ExpressionsHelper::eq(
        mlir::Value lhs, 
        mlir::Value rhs
    ) {
        return emitCompareOp(
            lhs,
            rhs,
            mlir::arith::CmpIPredicate::eq,
            mlir::arith::CmpFPredicate::OEQ
        );
    }

    mlir::Value ExpressionsHelper::neq(
        mlir::Value lhs, 
        mlir::Value rhs
    ) {
        return emitCompareOp(
            lhs,
            rhs,
            mlir::arith::CmpIPredicate::ne,
            mlir::arith::CmpFPredicate::ONE
        );
    }

    mlir::Value ExpressionsHelper::le(
        mlir::Value lhs, 
        mlir::Value rhs
    ) {
        return emitCompareOp(
            lhs,
            rhs,
            mlir::arith::CmpIPredicate::sle,
            mlir::arith::CmpFPredicate::OLE
        );
    }

    mlir::Value ExpressionsHelper::ge(
        mlir::Value lhs, 
        mlir::Value rhs
    ) {
        return emitCompareOp(
            lhs,
            rhs,
            mlir::arith::CmpIPredicate::sge,
            mlir::arith::CmpFPredicate::OGE
        );
    }

    /// =============================================================
    /// Logicals
    /// =============================================================

    mlir::Value ExpressionsHelper::and_(
        mlir::Value lhs,
        mlir::Value rhs
    ) {
        lhs = castTo(lhs, types->b1_t());
        rhs = castTo(rhs, types->b1_t());

        return builder->create<mlir::arith::AndIOp>(loc, lhs, rhs);
    }

    mlir::Value ExpressionsHelper::or_(
        mlir::Value lhs, 
        mlir::Value rhs
    ) {
        lhs = castTo(lhs, types->b1_t());
        rhs = castTo(rhs, types->b1_t());

        return builder->create<mlir::arith::OrIOp>(loc, lhs, rhs);
    }
}