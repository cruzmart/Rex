#include "backend/rex_backend_exps.h"
#include "backend/rex_backend_types.h"
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

    namespace {

        template<typename T>
        std::shared_ptr<T> cast(
            const std::shared_ptr<void> &ptr
        ) {
            return std::static_pointer_cast<T>(ptr);
        }

        inline bool isPointer(mlir::Type t) {
            return t.isa<mlir::LLVM::LLVMPointerType>();
        }

    } // namespace

  /// =============================================================
/// ExpressionsHelper
/// -------------------------------------------------------------
/// Responsible for lowering high-level expressions into MLIR.
/// Handles:
///  - Literal creation
///  - Binary expressions
///  - Type casting
///  - Tuple + array construction
///  - String handling (including pooling + folding)
/// =============================================================

ExpressionsHelper::ExpressionsHelper(
    std::shared_ptr<mlir::OpBuilder> b,
    mlir::ModuleOp m,
    mlir::Location l,
    std::shared_ptr<TypesHelper> t
)
    : builder(std::move(b)),
      module(m),
      loc(l),
      types(std::move(t)) {}

/// =============================================================
/// Literal Creation
/// =============================================================

mlir::Value ExpressionsHelper::createPrimitiveLiteral(
    std::shared_ptr<LiteralExpr> lit
) {

    assert(lit && "null literal");
    assert(lit->type && "literal missing type");

    auto prim =
        std::static_pointer_cast<PrimType>(
            lit->type
        );

    const auto &value = lit->value;

    switch (prim->prim) {

        case PrimType::Prims::Int:
            return createInt(value);

        case PrimType::Prims::Bool:
            return createBool(value);

        case PrimType::Prims::Char:
            return createChar(value);

        case PrimType::Prims::Real:
            return createFloat(value);

        case PrimType::Prims::String:
            return createString(value);

        default:
            llvm_unreachable(
                "Unhandled literal type"
            );
    }
}

/// Create 32-bit integer constant (safe parsing via APInt).
mlir::Value ExpressionsHelper::createInt(
    const std::string &text
) {

    llvm::APInt value(32, text, 10);

    return builder->create<mlir::arith::ConstantOp>(
        loc,
        builder->getIntegerAttr(
            types->i32,
            value
        )
    );
}


mlir::Value ExpressionsHelper::createFloat(
    const std::string &text
) {

    llvm::APFloat value(0.0f);

    auto status =
        value.convertFromString(
            text,
            llvm::APFloat::rmNearestTiesToEven
        );

    if (!status ||
        (*status & llvm::APFloat::opInvalidOp)) {

        throw std::runtime_error(
            "Invalid float literal: " + text
        );
    }

    return builder->create<mlir::arith::ConstantOp>(
        loc,
        builder->getFloatAttr(
            types->f32,
            value
        )
    );
}

mlir::Value ExpressionsHelper::createBool(
    const std::string &text
) {

    if (text == "true") {
        return builder->create<mlir::arith::ConstantOp>(
            loc,
            builder->getIntegerAttr(
                types->b1,
                1
            )
        );
    }

    if (text == "false") {
        return builder->create<mlir::arith::ConstantOp>(
            loc,
            builder->getIntegerAttr(
                types->b1,
                0
            )
        );
    }

    throw std::runtime_error(
        "Invalid boolean literal: " + text
    );
}

mlir::Value ExpressionsHelper::createChar(
    const std::string &text
) {

    if (text.empty()) {
        throw std::runtime_error(
            "Empty char literal"
        );
    }

    char value;

    if (text[0] == '\\') {

        if (text.size() != 2) {
            throw std::runtime_error(
                "Invalid escaped char"
            );
        }

        switch (text[1]) {

            case 'n':  value = '\n'; break;
            case 't':  value = '\t'; break;
            case 'r':  value = '\r'; break;
            case '\\': value = '\\'; break;
            case '\'': value = '\''; break;
            case '0':  value = '\0'; break;

            default:
                throw std::runtime_error(
                    "Unknown escape sequence"
                );
        }

    } else {

        if (text.size() != 1) {
            throw std::runtime_error(
                "Invalid char literal"
            );
        }

        value = text[0];
    }

    return builder->create<mlir::arith::ConstantOp>(
        loc,
        builder->getI8IntegerAttr(
            static_cast<int8_t>(value)
        )
    );
}

/// Create (or reuse) global string literal.
/// Implements string pooling + LLVM global emission.
mlir::Value ExpressionsHelper::createString(
    const std::string &text
) {

    // =====================================================
    // STRING POOL REUSE
    // =====================================================

    auto it = stringPool.find(text);

    if (it != stringPool.end()) {

        return builder->create<
            mlir::LLVM::AddressOfOp
        >(
            loc,
            it->second
        );
    }

    // =====================================================
    // CREATE GLOBAL
    // =====================================================

    auto oldIP =
        builder->saveInsertionPoint();

    builder->setInsertionPointToStart(
        module.getBody()
    );

    auto str =
        text + '\0';

    auto name =
        "str_const_" +
        std::to_string(globalCounter++);

    auto i8Ty =
        builder->getIntegerType(8);

    auto arrayTy =
        mlir::LLVM::LLVMArrayType::get(
            i8Ty,
            str.size()
        );

    auto global =
        builder->create<mlir::LLVM::GlobalOp>(
            loc,
            arrayTy,
            true,
            mlir::LLVM::Linkage::Internal,
            name,
            builder->getStringAttr(str)
        );

    builder->restoreInsertionPoint(
        oldIP
    );

    stringPool[text] = global;

    // =====================================================
    // ADDRESS
    // =====================================================

    auto addr =
        builder->create<
            mlir::LLVM::AddressOfOp
        >(
            loc,
            global
        );

    return builder->create<
        mlir::LLVM::BitcastOp
    >(
        loc,
        mlir::LLVM::LLVMPointerType::get(
            builder->getContext()
        ),
        addr
    );
}

/// =============================================================
/// Tuple Creation
/// =============================================================

/// Creates an LLVM struct on the stack and initializes fields.
mlir::Value ExpressionsHelper::createTuple(
    const std::vector<mlir::Type> types,
    std::vector<mlir::Value> values
) {

    assert(!values.empty());
    assert(values.size() == types.size());

    auto *ctx =
        builder->getContext();

    auto structTy =
        mlir::LLVM::LLVMStructType::getLiteral(
            ctx,
            types
        );

    auto ptrTy =
        mlir::LLVM::LLVMPointerType::get(
            ctx
        );

    auto one =
        builder->create<
            mlir::arith::ConstantIntOp
        >(
            loc,
            1,
            32
        );

    auto storage =
        builder->create<mlir::LLVM::AllocaOp>(
            loc,
            ptrTy,
            structTy,
            one
        );

    for (size_t i = 0;
         i < values.size();
         ++i) {

        auto fieldPtr =
            builder->create<
                mlir::LLVM::GEPOp
            >(
                loc,
                ptrTy,
                structTy,
                storage,
                mlir::ArrayRef<
                    mlir::LLVM::GEPArg
                >{
                    0,
                    static_cast<int64_t>(i)
                }
            );

        builder->create<mlir::LLVM::StoreOp>(
            loc,
            values[i],
            fieldPtr
        );
    }

    return storage;
}



/// =============================================================
/// Binary Expressions
/// =============================================================

mlir::Value ExpressionsHelper::createBinaryExp(
    mlir::Value lhs,
    mlir::Value rhs,
    PrimType::Prims prim,
    BinaryOp op
) {

    // =====================================================
    // STRING CONCAT
    // =====================================================

    if (isPointer(lhs.getType()) &&
        isPointer(rhs.getType()) &&
        op == BinaryOp::ADD) {

        return concatString(lhs, rhs);
    }

    mlir::Type resultTy;

    switch (prim) {

        case PrimType::Prims::Int:
            resultTy = types->i32;
            break;

        case PrimType::Prims::Real:
            resultTy = types->f32;
            break;

        case PrimType::Prims::Bool:
            resultTy = types->b1;
            break;

        case PrimType::Prims::Char:
            resultTy = types->c8;
            break;

        default:
            resultTy = {};
            break;
    }

    // =====================================================
    // ARITHMETIC
    // =====================================================

    switch (op) {

        case BinaryOp::ADD:
            return add(lhs, rhs, resultTy);

        case BinaryOp::SUB:
            return sub(lhs, rhs, resultTy);

        case BinaryOp::MUL:
            return mul(lhs, rhs, resultTy);

        case BinaryOp::DIV:
            return div(lhs, rhs, resultTy);

        case BinaryOp::MOD:
            return mod(lhs, rhs, resultTy);

        default:
            break;
    }

    // =====================================================
    // COMPARISONS
    // =====================================================

    switch (op) {

        case BinaryOp::EQ:
            return eq(lhs, rhs);

        case BinaryOp::NEQ:
            return neq(lhs, rhs);

        case BinaryOp::LT:
            return lt(lhs, rhs);

        case BinaryOp::LE:
            return le(lhs, rhs);

        case BinaryOp::GT:
            return gt(lhs, rhs);

        case BinaryOp::GE:
            return ge(lhs, rhs);

        default:
            break;
    }

    // =====================================================
    // LOGICAL
    // =====================================================

    switch (op) {

        case BinaryOp::AND:
            return and_(lhs, rhs);

        case BinaryOp::OR:
            return or_(lhs, rhs);

        default:
            return {};
    }
}


/// =============================================================
/// Arithmetic Helpers
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


/// =============================================================
/// Array Handling
/// =============================================================

/// Checks if array expression is fully constant.
/// Allows:
///  - literals
///  - nested arrays
///  - constant string expressions
bool ExpressionsHelper::isConstArrayExpr(std::shared_ptr<ArrayExpr> arr) {

    for (auto &elem : arr->elements) {

        if (elem->exp_kind == ExprKind::Literal)
            continue;

        if (elem->exp_kind == ExprKind::Array) {
            if (!isConstArrayExpr(std::static_pointer_cast<ArrayExpr>(elem)))
                return false;
            continue;
        }

        if (elem->exp_kind == ExprKind::Binary) {
            if (!isConstStringExpr(elem))
                return false;
            continue;
        }

        return false;
    }

    return true;
}

bool ExpressionsHelper::isCompileTimeValue(mlir::Value v) {
    if (v.getDefiningOp<mlir::arith::ConstantOp>())
        return true;

    if (v.getDefiningOp<mlir::LLVM::AddressOfOp>())
        return true;

    if (v.getDefiningOp<mlir::arith::ConstantIntOp>())
        return true;

    if (v.getDefiningOp<mlir::arith::ConstantFloatOp>())
        return true;

    return false;
}

void ExpressionsHelper::copyArray(
    mlir::Value dstPtr,
    mlir::Value srcPtr,
    std::shared_ptr<ArrayType> arrTy
) {

    auto ctx =
        builder->getContext();

    auto ptrTy =
        mlir::LLVM::LLVMPointerType::get(ctx);

    auto elemTy =
        types->getMLIRType(arrTy);
    elemTy = mlir::cast<mlir::LLVM::LLVMArrayType>(elemTy).getElementType();

    auto [rows, cols] =
        arrTy->dimensions();

    int total =
        rows * cols;

    auto zero =
        builder->create<mlir::arith::ConstantIntOp>(
            loc,
            0,
            32
        );

    auto one =
        builder->create<mlir::arith::ConstantIntOp>(
            loc,
            1,
            32
        );

    auto size =
        builder->create<mlir::arith::ConstantIntOp>(
            loc,
            total,
            32
        );

    auto loop =
        builder->create<mlir::scf::ForOp>(
            loc,
            zero,
            size,
            one
        );

    builder->setInsertionPointToStart(
        loop.getBody()
    );

    auto i =
        loop.getInductionVar();

    // =========================================
    // SOURCE ELEMENT
    // =========================================

    auto srcElemPtr =
        builder->create<mlir::LLVM::GEPOp>(
            loc,
            ptrTy,
            elemTy,
            srcPtr,
            mlir::ValueRange{i}
        );

    auto srcElem =
        builder->create<mlir::LLVM::LoadOp>(
            loc,
            elemTy,
            srcElemPtr
        );

    // =========================================
    // DEST ELEMENT
    // =========================================

    auto dstElemPtr =
        builder->create<mlir::LLVM::GEPOp>(
            loc,
            ptrTy,
            elemTy,
            dstPtr,
            mlir::ValueRange{i}
        );

    builder->create<mlir::LLVM::StoreOp>(
        loc,
        srcElem,
        dstElemPtr
    );

    builder->setInsertionPointAfter(loop);
}

mlir::Value 
ExpressionsHelper::matrixRowPtr(mlir::Value arrPtr, mlir::Value index, std::shared_ptr<ArrayType> arrTy)
{
        auto [rows, cols] = arrTy->dimensions();
        auto ptrTy = mlir::LLVM::LLVMPointerType::get( builder->getContext());

        auto colsVal = builder->create<mlir::arith::ConstantIntOp>(
            loc,
            cols,
            32
        );

        // rowStart = index * cols
        auto rowStart = builder->create<mlir::arith::MulIOp>(
            loc,
            index,
            colsVal
        );

        mlir::Type elemTy = types->getMLIRType(arrTy->elem);

        auto rowPtr = builder->create<mlir::LLVM::GEPOp>(
            loc,
            ptrTy,
            elemTy,
            arrPtr,
            mlir::ValueRange{
                builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32),
                rowStart
            }
        );

        return rowPtr;
}

 mlir::Value 
 ExpressionsHelper::arrayElementPtr(mlir::Value arrPtr, mlir::Value index, std::shared_ptr<ArrayType> arrTy){
  
    auto ptrTy = mlir::LLVM::LLVMPointerType::get( builder->getContext());
    mlir::Type elemTy = types->getMLIRType(arrTy);


    auto elemPtr = builder->create<mlir::LLVM::GEPOp>(
        loc,
        ptrTy,
        elemTy,
        arrPtr,
        mlir::ValueRange{
            builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32),
            index
        }
    );

    

    return elemPtr;
 }

mlir::Value ExpressionsHelper::createRuntimeArray(
    const std::vector<mlir::Value>& elements,
    PrimType::Prims kind
) {
    mlir::Type elemTy;

    switch (kind) {
        case PrimType::Prims::Int:
            elemTy = types->i32;
            break;
        case PrimType::Prims::Real:
            elemTy = types->f32;
            break;
        case PrimType::Prims::Bool:
            elemTy = types->b1;
            break;
        case PrimType::Prims::Char:
            elemTy = types->c8;
            break;
        case PrimType::Prims::String:
            elemTy = mlir::LLVM::LLVMPointerType::get(builder->getContext());
            break;
        default:
            elemTy = mlir::LLVM::LLVMPointerType::get(builder->getContext());
            break;
    }


    auto ctx = builder->getContext();

    auto ptrTy = mlir::LLVM::LLVMPointerType::get(ctx);

    // =====================================================
    // ✅ REAL LLVM ARRAY TYPE
    // =====================================================
    auto arrayTy = mlir::LLVM::LLVMArrayType::get(elemTy, elements.size());

    // =====================================================
    // ✅ ALLOCATE ONE ARRAY OBJECT
    // =====================================================
    auto one = builder->create<mlir::arith::ConstantIntOp>(loc, 1, 32);

    auto alloca = builder->create<mlir::LLVM::AllocaOp>(
        loc,
        ptrTy,
        arrayTy,
        one   // ✅ NOT "size", ALWAYS 1 for array object
    );

    // =====================================================
    // ZERO INDEX FOR GEP ROOT
    // =====================================================
    auto zero = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);

    // =====================================================
    // STORE ELEMENTS
    // =====================================================
    for (size_t i = 0; i < elements.size(); i++) {

        auto idx = builder->create<mlir::arith::ConstantIntOp>(loc, i, 32);

        // =================================================
        // ✅ CORRECT GEP: (0, i)
        // =================================================
        auto gep = builder->create<mlir::LLVM::GEPOp>(
            loc,
            ptrTy,
            arrayTy,
            alloca,
            mlir::ValueRange{zero, idx}
        );

        builder->create<mlir::LLVM::StoreOp>(
            loc,
            elements[i],
            gep
        );
    }

    return alloca;
}

/// =============================================================
/// Indexing
/// =============================================================

/// Array indexing:
///  - GEP to element pointer
///  - Load element
mlir::Value ExpressionsHelper::index(
    mlir::Value arr_p,
    mlir::Value idx,
    mlir::Type elemTy
) {
    auto ptrTy = mlir::LLVM::LLVMPointerType::get(builder->getContext());

    auto gep = builder->create<mlir::LLVM::GEPOp>(
        loc,
        ptrTy,
        elemTy,
        arr_p,
        mlir::ValueRange{idx}
    );

    return builder->create<mlir::LLVM::LoadOp>(
        loc,
        elemTy,
        gep
    );
}

/// Tuple indexing:
///  - GEP using struct layout
///  - Load field value
mlir::Value ExpressionsHelper::index(
    mlir::Value tuple_ptr,
    mlir::LLVM::LLVMStructType struct_ty,
    mlir::Type tar_ty,
    mlir::Value i
){
    auto ptrTy = mlir::LLVM::LLVMPointerType::get(builder->getContext());

    auto index = mlir::ValueRange{
        builder->create<mlir::arith::ConstantOp>(
            loc, builder->getI32IntegerAttr(0)),
        i
    };

    auto fieldPtr = builder->create<mlir::LLVM::GEPOp>(
        loc,
        ptrTy,
        struct_ty,
        tuple_ptr,
        index
    );

    return builder->create<mlir::LLVM::LoadOp>(
        loc,
        tar_ty,
        fieldPtr
    );
}
}