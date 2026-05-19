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

    inline bool isPointer(mlir::Type t) {
        return t.isa<mlir::LLVM::LLVMPointerType>();
    }

                                /// =============================================================
                                ///                     Arithmetic Helpers
                                /// =============================================================

    mlir::Value ExpressionsHelper::add(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type resultTy
    ) {

        auto computeTy =
            getComputeType(
                lhs.getType(),
                rhs.getType()
            );

        lhs = castTo(lhs, computeTy);
        rhs = castTo(rhs, computeTy);

        mlir::Value result;

        if (computeTy.isF32()) {

            result =
                builder->create<
                    mlir::arith::AddFOp
                >(
                    loc,
                    lhs,
                    rhs
                );

        } else {

            result =
                builder->create<
                    mlir::arith::AddIOp
                >(
                    loc,
                    lhs,
                    rhs
                );
        }

        return castTo(result, resultTy);
    }


    mlir::Value ExpressionsHelper::sub(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type resultTy
    ) {

        auto computeTy =
            getComputeType(
                lhs.getType(),
                rhs.getType()
            );

        lhs = castTo(lhs, computeTy);
        rhs = castTo(rhs, computeTy);

        mlir::Value result;

        if (computeTy.isF32()) {

            result =
                builder->create<
                    mlir::arith::SubFOp
                >(
                    loc,
                    lhs,
                    rhs
                );

        } else {

            result =
                builder->create<
                    mlir::arith::SubIOp
                >(
                    loc,
                    lhs,
                    rhs
                );
        }

        return castTo(result, resultTy);
    }

    /// Division operation with implicit type promotion.
    ///
    /// Steps:
    ///  1. Determine common compute type
    ///  2. Cast operands to compute type
    ///  3. Perform compile-time divide-by-zero validation
    ///  4. Emit integer or floating-point division
    ///  5. Cast result back to requested type
    ///
    /// Notes:
    ///  - Integer division uses signed semantics
    ///  - Float division uses IEEE floating-point division
    mlir::Value ExpressionsHelper::div(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type resultType
    ) {
        // =========================================================
        // Determine compute type
        // =========================================================
        mlir::Type computeType =
            getComputeType(lhs.getType(), rhs.getType());

        lhs = castTo(lhs, computeType);
        rhs = castTo(rhs, computeType);

        // =========================================================
        // Compile-time divide-by-zero check
        // =========================================================
        if (auto cst = rhs.getDefiningOp<mlir::arith::ConstantOp>()) {

            if (auto intAttr =
                    cst.getValue().dyn_cast<mlir::IntegerAttr>()) {

                if (intAttr.getValue().isZero()) {
                    llvm::report_fatal_error(
                        "Division by zero (constant)"
                    );
                }
            }

            if (auto floatAttr =
                    cst.getValue().dyn_cast<mlir::FloatAttr>()) {

                if (floatAttr.getValue().isZero()) {
                    llvm::report_fatal_error(
                        "Division by zero (constant)"
                    );
                }
            }
        }

        // =========================================================
        // Emit division op
        // =========================================================
        mlir::Value result;

        if (computeType.isF32()) {

            result =
                builder->create<mlir::arith::DivFOp>(
                    loc,
                    lhs,
                    rhs
                );

        } else {

            result =
                builder->create<mlir::arith::DivSIOp>(
                    loc,
                    lhs,
                    rhs
                );
        }

        // =========================================================
        // Cast result to requested type
        // =========================================================
        return castTo(result, resultType);
    }

    /// Modulo operation.
    ///
    /// Steps:
    ///  1. Determine compute type
    ///  2. Cast operands
    ///  3. Reject floating-point modulo
    ///  4. Validate divisor is non-zero (if constant)
    ///  5. Emit signed remainder op
    ///
    /// Notes:
    ///  - Only integer modulo is supported
    ///  - Uses signed remainder semantics
    mlir::Value ExpressionsHelper::mod(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type resultType
    ) {
        // =========================================================
        // Determine compute type
        // =========================================================
        mlir::Type computeType =
            getComputeType(lhs.getType(), rhs.getType());

        lhs = castTo(lhs, computeType);
        rhs = castTo(rhs, computeType);

        // =========================================================
        // Float modulo is unsupported
        // =========================================================
        if (computeType.isF32()) {
            llvm::report_fatal_error(
                "Modulo not supported for float"
            );
        }

        // =========================================================
        // Compile-time modulo-by-zero check
        // =========================================================
        if (auto cst = rhs.getDefiningOp<mlir::arith::ConstantOp>()) {

            if (auto intAttr =
                    cst.getValue().dyn_cast<mlir::IntegerAttr>()) {

                if (intAttr.getValue().isZero()) {
                    llvm::report_fatal_error(
                        "Modulo by zero"
                    );
                }
            }
        }

        // =========================================================
        // Emit signed remainder operation
        // =========================================================
        auto result =
            builder->create<mlir::arith::RemSIOp>(
                loc,
                lhs,
                rhs
            );

        // =========================================================
        // Cast result to requested type
        // =========================================================
        return castTo(result, resultType);
    }

    /// Multiplication with implicit type promotion.
    ///
    /// Steps:
    ///  1. Determine common compute type
    ///  2. Cast operands to compute type
    ///  3. Emit integer or floating-point multiplication
    ///  4. Cast result back to requested type
    ///
    /// Notes:
    ///  - Integer multiplication uses MulIOp
    ///  - Floating-point multiplication uses MulFOp
    mlir::Value ExpressionsHelper::mul(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::Type resultType
    ) {
        // =========================================================
        // Determine compute type
        // =========================================================
        mlir::Type computeType =
            getComputeType(lhs.getType(), rhs.getType());

        lhs = castTo(lhs, computeType);
        rhs = castTo(rhs, computeType);

        // =========================================================
        // Emit multiplication op
        // =========================================================
        mlir::Value result;

        if (computeType.isF32()) {

            result =
                builder->create<mlir::arith::MulFOp>(
                    loc,
                    lhs,
                    rhs
                );

        } else {

            result =
                builder->create<mlir::arith::MulIOp>(
                    loc,
                    lhs,
                    rhs
                );
        }

        // =========================================================
        // Cast result to requested type
        // =========================================================
        return castTo(result, resultType);
    }

    /// =============================================================
    /// Comparison Helpers
    /// =============================================================

    mlir::Value ExpressionsHelper::cmp(
        mlir::Value lhs,
        mlir::Value rhs,
        mlir::arith::CmpIPredicate iPred,
        mlir::arith::CmpFPredicate fPred
    ) {

        auto computeTy =
            getComputeType(
                lhs.getType(),
                rhs.getType()
            );

        lhs = castTo(lhs, computeTy);
        rhs = castTo(rhs, computeTy);

        if (computeTy.isF32()) {

            return builder->create<
                mlir::arith::CmpFOp
            >(
                loc,
                fPred,
                lhs,
                rhs
            );
        }

        return builder->create<
            mlir::arith::CmpIOp
        >(
            loc,
            iPred,
            lhs,
            rhs
        );
    }


    /// =============================================================
    /// Equality
    /// =============================================================

    mlir::Value ExpressionsHelper::eq(
        mlir::Value lhs,
        mlir::Value rhs
    ) {

        bool lhsStr =
            isPointer(lhs.getType());

        bool rhsStr =
            isPointer(rhs.getType());

        if (lhsStr && rhsStr) {

            auto strcmpFn =
                getOrInsertStrcmp();

            auto call =
                builder->create<
                    mlir::LLVM::CallOp
                >(
                    loc,
                    strcmpFn,
                    mlir::ValueRange{
                        lhs,
                        rhs
                    }
                );

            auto zero =
                builder->create<
                    mlir::arith::ConstantIntOp
                >(
                    loc,
                    0,
                    32
                );

            return builder->create<
                mlir::arith::CmpIOp
            >(
                loc,
                mlir::arith::CmpIPredicate::eq,
                call.getResult(),
                zero
            );
        }

        return cmp(
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

        bool lhsStr =
            isPointer(lhs.getType());

        bool rhsStr =
            isPointer(rhs.getType());

        if (lhsStr && rhsStr) {

            auto strcmpFn =
                getOrInsertStrcmp();

            auto call =
                builder->create<
                    mlir::LLVM::CallOp
                >(
                    loc,
                    strcmpFn,
                    mlir::ValueRange{
                        lhs,
                        rhs
                    }
                );

            auto zero =
                builder->create<
                    mlir::arith::ConstantIntOp
                >(
                    loc,
                    0,
                    32
                );

            return builder->create<
                mlir::arith::CmpIOp
            >(
                loc,
                mlir::arith::CmpIPredicate::ne,
                call.getResult(),
                zero
            );
        }

        return cmp(
            lhs,
            rhs,
            mlir::arith::CmpIPredicate::ne,
            mlir::arith::CmpFPredicate::ONE
        );
    }


    /// =============================================================
    /// Relational Comparisons
    /// =============================================================

    /// Less-than comparison.
    /// Uses signed integer comparison or ordered float comparison.
    mlir::Value ExpressionsHelper::lt(
        mlir::Value lhs,
        mlir::Value rhs
    ) {
        return cmp(
            lhs,
            rhs,
            mlir::arith::CmpIPredicate::slt,
            mlir::arith::CmpFPredicate::OLT
        );
    }

    /// Less-than-or-equal comparison.
    /// Uses signed integer comparison or ordered float comparison.
    mlir::Value ExpressionsHelper::le(
        mlir::Value lhs,
        mlir::Value rhs
    ) {
        return cmp(
            lhs,
            rhs,
            mlir::arith::CmpIPredicate::sle,
            mlir::arith::CmpFPredicate::OLE
        );
    }

    /// Greater-than comparison.
    /// Uses signed integer comparison or ordered float comparison.
    mlir::Value ExpressionsHelper::gt(
        mlir::Value lhs,
        mlir::Value rhs
    ) {
        return cmp(
            lhs,
            rhs,
            mlir::arith::CmpIPredicate::sgt,
            mlir::arith::CmpFPredicate::OGT
        );
    }

    /// Greater-than-or-equal comparison.
    /// Uses signed integer comparison or ordered float comparison.
    mlir::Value ExpressionsHelper::ge(
        mlir::Value lhs,
        mlir::Value rhs
    ) {
        return cmp(
            lhs,
            rhs,
            mlir::arith::CmpIPredicate::sge,
            mlir::arith::CmpFPredicate::OGE
        );
    }


    /// =============================================================
    /// Logical Operations
    /// =============================================================

    /// Logical AND.
    /// Steps:
    ///  1. Cast both operands to boolean (i1)
    ///  2. Emit bitwise AND operation
    mlir::Value ExpressionsHelper::and_(
        mlir::Value lhs,
        mlir::Value rhs
    ) {
        lhs = castTo(lhs, types->b1);
        rhs = castTo(rhs, types->b1);

        return builder->create<mlir::arith::AndIOp>(
            loc,
            lhs,
            rhs
        );
    }

    /// Logical OR.
    /// Steps:
    ///  1. Cast both operands to boolean (i1)
    ///  2. Emit bitwise OR operation
    mlir::Value ExpressionsHelper::or_(
        mlir::Value lhs,
        mlir::Value rhs
    ) {
        lhs = castTo(lhs, types->b1);
        rhs = castTo(rhs, types->b1);

        return builder->create<mlir::arith::OrIOp>(
            loc,
            lhs,
            rhs
        );
    }


}