#include "backend/rex_backend_exps.h"
#include "backend/rex_backend_prints.h"
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
    std::shared_ptr<TypesHelper> t)
    : builder(b), module(m), loc(l), types(t) {}


/// =============================================================
/// Literal Creation
/// =============================================================

/// Dispatch creation of primitive literals based on type.
mlir::Value ExpressionsHelper::createPrimitiveLiteral(std::shared_ptr<LiteralExpr> lit) {
    assert(lit && "null literal");
    assert(lit->type && "literal has no type");

    auto prim = std::static_pointer_cast<PrimType>(lit->type);
    const std::string &value = lit->value;

    switch (prim->prim) {
        case rex::PrimType::Prims::Int:    return createInt(value);
        case rex::PrimType::Prims::Bool:   return createBool(value);
        case rex::PrimType::Prims::Char:   return createChar(value);
        case rex::PrimType::Prims::Real:   return createFloat(value);
        case rex::PrimType::Prims::String: return createString(value);
        default:
            break;
    }

    llvm_unreachable("Unhandled literal type");
}

/// Create 32-bit integer constant (safe parsing via APInt).
mlir::Value ExpressionsHelper::createInt(const std::string &text){
    llvm::APInt value(32, text, 10);
    return builder->create<mlir::arith::ConstantOp>(
        loc, builder->getIntegerAttr(types->i32, value));
}

/// Create 32-bit float constant with validation.
mlir::Value ExpressionsHelper::createFloat(const std::string &text){
    llvm::APFloat value(0.0f);

    auto statusOrError = value.convertFromString(text, llvm::APFloat::rmNearestTiesToEven);
    if (!statusOrError || (*statusOrError & llvm::APFloat::opInvalidOp)) {
        throw std::runtime_error("Invalid float literal: " + text);
    }

    return builder->create<mlir::arith::ConstantOp>(
        loc,
        builder->getFloatAttr(types->f32, value)
    );
}

/// Create boolean constant (true/false → i1).
mlir::Value ExpressionsHelper::createBool(const std::string &text){
    if (text == "true") {
        return builder->create<mlir::arith::ConstantOp>(
            loc, builder->getIntegerAttr(types->b1, 1));
    } else if (text == "false") {
        return builder->create<mlir::arith::ConstantOp>(
            loc, builder->getIntegerAttr(types->b1, 0));
    }

    throw std::runtime_error("Invalid boolean literal: " + text);
}

/// Create character constant (handles escape sequences).
mlir::Value ExpressionsHelper::createChar(const std::string &text) {
    if (text.empty())
        throw std::runtime_error("Empty char literal");

    char value;

    // Handle escape sequences like '\n', '\t', etc.
    if (text[0] == '\\') {
        if (text.size() != 2)
            throw std::runtime_error("Invalid escaped char");

        switch (text[1]) {
            case 'n':  value = '\n'; break;
            case 't':  value = '\t'; break;
            case 'r':  value = '\r'; break;
            case '\\': value = '\\'; break;
            case '\'': value = '\''; break;
            case '0':  value = '\0'; break;
            default:
                throw std::runtime_error("Unknown escape sequence");
        }
    } else {
        if (text.size() != 1)
            throw std::runtime_error("Invalid char literal");

        value = text[0];
    }

    return builder->create<mlir::arith::ConstantOp>(
        loc,
        builder->getI8IntegerAttr(static_cast<int8_t>(value))
    );
}

/// Create (or reuse) global string literal.
/// Implements string pooling + LLVM global emission.
mlir::Value ExpressionsHelper::createString(const std::string &text) {

    // 1. Reuse existing string if already created
    auto it = stringPool.find(text);
    if (it != stringPool.end()) {
        return builder->create<mlir::LLVM::AddressOfOp>(loc, it->second);
    }

    // 2. Insert global string at module start
    auto oldIP = builder->saveInsertionPoint();
    builder->setInsertionPointToStart(module.getBody());

    std::string strWithNull = text + '\0';
    std::string name = "str_const_" + std::to_string(globalCounter++);

    auto i8Ty = builder->getIntegerType(8);
    auto arrayTy = mlir::LLVM::LLVMArrayType::get(i8Ty, strWithNull.size());

    auto global = builder->create<mlir::LLVM::GlobalOp>(
        loc,
        arrayTy,
        true,
        mlir::LLVM::Linkage::Internal,
        name,
        builder->getStringAttr(strWithNull)
    );

    builder->restoreInsertionPoint(oldIP);
    stringPool[text] = global;

    // 3. Return i8* pointer to string
    auto addr = builder->create<mlir::LLVM::AddressOfOp>(loc, global);

    return builder->create<mlir::LLVM::BitcastOp>(
        loc,
        mlir::LLVM::LLVMPointerType::get(builder->getContext()),
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

    auto ctx = builder->getContext();

    // 1. Build struct type
    auto structTy = mlir::LLVM::LLVMStructType::getLiteral(ctx, types);
    auto ptrTy = mlir::LLVM::LLVMPointerType::get(ctx);

    // 2. Allocate stack memory
    auto one = builder->create<mlir::arith::ConstantIntOp>(loc, 1, 32);
    auto alloca = builder->create<mlir::LLVM::AllocaOp>(loc, ptrTy, structTy, one);

    // 3. Store each field
    for (size_t i = 0; i < values.size(); i++) {
        auto fieldPtr = builder->create<mlir::LLVM::GEPOp>(
            loc,
            ptrTy,
            structTy,
            alloca,
            mlir::ArrayRef<mlir::LLVM::GEPArg>{
                mlir::LLVM::GEPArg(0),
                mlir::LLVM::GEPArg(static_cast<int64_t>(i))
            }
        );

        builder->create<mlir::LLVM::StoreOp>(loc, values[i], fieldPtr);
    }

    // 4. Return pointer to struct
    return alloca;
}


/// =============================================================
/// Binary Expressions
/// =============================================================

/// Entry point for binary operations.
/// Handles:
///  - arithmetic
///  - comparisons
///  - logical ops
///  - string concatenation
mlir::Value ExpressionsHelper::createBinaryExp(
    mlir::Value lhs,
    mlir::Value rhs,
    PrimType::Prims prim_t,
    BinaryOp op
){
    mlir::Type res_t;

    // Special case: string concatenation
    if(lhs.getType().isa<mlir::LLVM::LLVMPointerType>() &&
       rhs.getType().isa<mlir::LLVM::LLVMPointerType>() &&
       op == BinaryOp::ADD)
        return concatString(lhs, rhs);

    // Determine result type
    switch(prim_t){
        case PrimType::Prims::Int:  res_t = types->i32; break;
        case PrimType::Prims::Real: res_t = types->f32; break;
        case PrimType::Prims::Bool: res_t = types->b1; break;
        case PrimType::Prims::Char: res_t = types->c8; break;
        default: res_t = mlir::Type(); break;
    }

    // Arithmetic
    switch(op){
        case BinaryOp::ADD: return add(lhs, rhs, res_t);
        case BinaryOp::SUB: return sub(lhs, rhs, res_t);
        case BinaryOp::MUL: return mul(lhs, rhs, res_t);
        case BinaryOp::DIV: return div(lhs, rhs, res_t);
        case BinaryOp::MOD: return mod(lhs, rhs, res_t);
        default: break;
    }

    // Comparisons
    switch(op){
        case BinaryOp::EQ:  return eq(lhs, rhs);
        case BinaryOp::NEQ: return neq(lhs, rhs);
        case BinaryOp::LT:  return lt(lhs, rhs);
        case BinaryOp::GT:  return gt(lhs, rhs);
        case BinaryOp::LE:  return le(lhs, rhs);
        case BinaryOp::GE:  return ge(lhs, rhs);
        default: break;
    }

    // Logical
    switch(op){
        case BinaryOp::AND: return and_(lhs, rhs);
        case BinaryOp::OR:  return or_(lhs, rhs);
        default: break;
    }

    return mlir::Value();
}


/// =============================================================
/// Arithmetic Helpers
/// =============================================================

/// Generic addition with type promotion.
/// Steps:
///  1. Determine common compute type
///  2. Cast operands
///  3. Perform op (int or float)
///  4. Cast back to requested result type
mlir::Value ExpressionsHelper::add(mlir::Value lhs, mlir::Value rhs, mlir::Type resultType) {

    mlir::Type computeType = getComputeType(lhs.getType(), rhs.getType());

    lhs = castTo(lhs, computeType);
    rhs = castTo(rhs, computeType);

    mlir::Value result;

    if (computeType.isF32()) {
        result = builder->create<mlir::arith::AddFOp>(loc, lhs, rhs);
    } else {
        result = builder->create<mlir::arith::AddIOp>(loc, lhs, rhs);
    }

    return castTo(result, resultType);
}

/// Subtraction (same structure as add).
mlir::Value ExpressionsHelper::sub(mlir::Value lhs, mlir::Value rhs, mlir::Type resultType) {

    mlir::Type computeType = getComputeType(lhs.getType(), rhs.getType());

    lhs = castTo(lhs, computeType);
    rhs = castTo(rhs, computeType);

    mlir::Value result;

    if (computeType.isF32()) {
        result = builder->create<mlir::arith::SubFOp>(loc, lhs, rhs);
    } else {
        result = builder->create<mlir::arith::SubIOp>(loc, lhs, rhs);
    }

    return castTo(result, resultType);
}

/// Multiplication with type promotion.
mlir::Value ExpressionsHelper::mul(mlir::Value lhs, mlir::Value rhs, mlir::Type resultType) {

    mlir::Type computeType = getComputeType(lhs.getType(), rhs.getType());

    lhs = castTo(lhs, computeType);
    rhs = castTo(rhs, computeType);

    mlir::Value result;

    if (computeType.isF32()) {
        result = builder->create<mlir::arith::MulFOp>(loc, lhs, rhs);
    } else {
        result = builder->create<mlir::arith::MulIOp>(loc, lhs, rhs);
    }

    return castTo(result, resultType);
}

/// Division with safety checks.
/// - Performs compile-time divide-by-zero detection for constants
/// - Uses signed division for integers
mlir::Value ExpressionsHelper::div(mlir::Value lhs, mlir::Value rhs, mlir::Type resultType) {

    mlir::Type computeType = getComputeType(lhs.getType(), rhs.getType());

    lhs = castTo(lhs, computeType);
    rhs = castTo(rhs, computeType);

    // Compile-time zero check
    if (auto cst = rhs.getDefiningOp<mlir::arith::ConstantOp>()) {
        if (auto intAttr = cst.getValue().dyn_cast<mlir::IntegerAttr>()) {
            if (intAttr.getValue().isZero()) {
                llvm::report_fatal_error("Division by zero (constant)");
            }
        }
        if (auto floatAttr = cst.getValue().dyn_cast<mlir::FloatAttr>()) {
            if (floatAttr.getValue().isZero()) {
                llvm::report_fatal_error("Division by zero (constant)");
            }
        }
    }

    mlir::Value result;

    if (computeType.isF32()) {
        result = builder->create<mlir::arith::DivFOp>(loc, lhs, rhs);
    } else {
        result = builder->create<mlir::arith::DivSIOp>(loc, lhs, rhs);
    }

    return castTo(result, resultType);
}

/// Modulo (integer only).
/// - Rejects float usage
/// - Includes optional divide-by-zero check
mlir::Value ExpressionsHelper::mod(mlir::Value lhs, mlir::Value rhs, mlir::Type resultType) {

    mlir::Type computeType = getComputeType(lhs.getType(), rhs.getType());

    lhs = castTo(lhs, computeType);
    rhs = castTo(rhs, computeType);

    if (computeType.isF32()) {
        llvm::report_fatal_error("Modulo not supported for float");
    }

    // Compile-time zero check
    if (auto cst = rhs.getDefiningOp<mlir::arith::ConstantOp>()) {
        if (auto intAttr = cst.getValue().dyn_cast<mlir::IntegerAttr>()) {
            if (intAttr.getValue().isZero()) {
                llvm::report_fatal_error("Modulo by zero");
            }
        }
    }

    auto result = builder->create<mlir::arith::RemSIOp>(loc, lhs, rhs);
    return castTo(result, resultType);
}



/// =============================================================
/// Comparison Helpers
/// =============================================================

/// Generic comparison dispatcher.
/// Chooses integer or floating-point comparison based on compute type.
mlir::Value ExpressionsHelper::cmp(
    mlir::Value lhs,
    mlir::Value rhs,
    mlir::arith::CmpIPredicate iPred,
    mlir::arith::CmpFPredicate fPred
) {
    auto computeType = getComputeType(lhs.getType(), rhs.getType());

    lhs = castTo(lhs, computeType);
    rhs = castTo(rhs, computeType);

    if (computeType.isF32()) {
        return builder->create<mlir::arith::CmpFOp>(loc, fPred, lhs, rhs);
    } else {
        return builder->create<mlir::arith::CmpIOp>(loc, iPred, lhs, rhs);
    }
}


/// =============================================================
/// Equality / Inequality
/// =============================================================

/// Equality:
/// - Special handling for strings via strcmp
/// - Otherwise numeric comparison
mlir::Value ExpressionsHelper::eq(mlir::Value lhs, mlir::Value rhs) {

    bool lhsIsStr = lhs.getType().isa<mlir::LLVM::LLVMPointerType>();
    bool rhsIsStr = rhs.getType().isa<mlir::LLVM::LLVMPointerType>();

    // String comparison using strcmp
    if (lhsIsStr && rhsIsStr) {
        auto strcmpFunc = getOrInsertStrcmp();

        auto call = builder->create<mlir::LLVM::CallOp>(
            loc,
            strcmpFunc,
            mlir::ValueRange{lhs, rhs}
        );

        auto zero = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);

        return builder->create<mlir::arith::CmpIOp>(
            loc,
            mlir::arith::CmpIPredicate::eq,
            call.getResult(),
            zero
        );
    }

    return cmp(lhs, rhs,
        mlir::arith::CmpIPredicate::eq,
        mlir::arith::CmpFPredicate::OEQ
    );
}

/// Inequality (mirrors eq).
mlir::Value ExpressionsHelper::neq(mlir::Value lhs, mlir::Value rhs) {

    bool lhsIsStr = lhs.getType().isa<mlir::LLVM::LLVMPointerType>();
    bool rhsIsStr = rhs.getType().isa<mlir::LLVM::LLVMPointerType>();

    if (lhsIsStr && rhsIsStr) {
        auto strcmpFunc = getOrInsertStrcmp();

        auto call = builder->create<mlir::LLVM::CallOp>(
            loc,
            strcmpFunc,
            mlir::ValueRange{lhs, rhs}
        );

        auto zero = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);

        return builder->create<mlir::arith::CmpIOp>(
            loc,
            mlir::arith::CmpIPredicate::ne,
            call.getResult(),
            zero
        );
    }

    return cmp(lhs, rhs,
        mlir::arith::CmpIPredicate::ne,
        mlir::arith::CmpFPredicate::ONE
    );
}


/// =============================================================
/// Relational Comparisons
/// =============================================================

mlir::Value ExpressionsHelper::lt(mlir::Value lhs, mlir::Value rhs){
    return cmp(lhs, rhs,
        mlir::arith::CmpIPredicate::slt,
        mlir::arith::CmpFPredicate::OLT
    );
}

mlir::Value ExpressionsHelper::le(mlir::Value lhs, mlir::Value rhs){
    return cmp(lhs, rhs,
        mlir::arith::CmpIPredicate::sle,
        mlir::arith::CmpFPredicate::OLE
    );
}

mlir::Value ExpressionsHelper::gt(mlir::Value lhs, mlir::Value rhs){
    return cmp(lhs, rhs,
        mlir::arith::CmpIPredicate::sgt,
        mlir::arith::CmpFPredicate::OGT
    );
}

mlir::Value ExpressionsHelper::ge(mlir::Value lhs, mlir::Value rhs){
    return cmp(lhs, rhs,
        mlir::arith::CmpIPredicate::sge,
        mlir::arith::CmpFPredicate::OGE
    );
}


/// =============================================================
/// Logical Operations
/// =============================================================

/// Logical AND (forces operands to boolean).
mlir::Value ExpressionsHelper::and_(mlir::Value lhs, mlir::Value rhs){
    lhs = castTo(lhs, types->b1);
    rhs = castTo(rhs, types->b1);

    return builder->create<mlir::arith::AndIOp>(loc, lhs, rhs);
}

/// Logical OR (forces operands to boolean).
mlir::Value ExpressionsHelper::or_(mlir::Value lhs, mlir::Value rhs){
    lhs = castTo(lhs, types->b1);
    rhs = castTo(rhs, types->b1);

    return builder->create<mlir::arith::OrIOp>(loc, lhs, rhs);
}


/// =============================================================
/// Casting + Type Promotion
/// =============================================================

/// Cast value between supported primitive types.
/// Disallows pointer casts (handled elsewhere).
mlir::Value ExpressionsHelper::castTo(mlir::Value val, mlir::Type targetType) {

    mlir::Type srcType = val.getType();

    // Prevent unsafe pointer casting
    if (srcType.isa<mlir::LLVM::LLVMPointerType>() ||
        targetType.isa<mlir::LLVM::LLVMPointerType>()) {
        llvm::report_fatal_error("Invalid cast involving pointer type");
    }

    // No-op if already correct type
    if (srcType == targetType)
        return val;

    // INT → FLOAT
    if (srcType.isInteger(32) && targetType.isF32()) {
        return builder->create<mlir::arith::SIToFPOp>(loc, targetType, val);
    }

    // FLOAT → INT
    if (srcType.isF32() && targetType.isInteger(32)) {
        return builder->create<mlir::arith::FPToSIOp>(loc, targetType, val);
    }

    // BOOL → INT
    if (srcType.isInteger(1) && targetType.isInteger(32)) {
        return builder->create<mlir::arith::ExtUIOp>(loc, targetType, val);
    }

    // BOOL → FLOAT
    if (srcType.isInteger(1) && targetType.isF32()) {
        auto i32 = builder->create<mlir::arith::ExtUIOp>(loc, types->i32, val);
        return builder->create<mlir::arith::SIToFPOp>(loc, targetType, i32);
    }

    // BOOL → CHAR
    if (srcType.isInteger(1) && targetType.isInteger(8)) {
        return builder->create<mlir::arith::ExtUIOp>(loc, targetType, val);
    }

    // CHAR → INT
    if (srcType.isInteger(8) && targetType.isInteger(32)) {
        return builder->create<mlir::arith::ExtUIOp>(loc, targetType, val);
    }

    // CHAR → FLOAT
    if (srcType.isInteger(8) && targetType.isF32()) {
        auto i32 = builder->create<mlir::arith::ExtUIOp>(loc, types->i32, val);
        return builder->create<mlir::arith::SIToFPOp>(loc, targetType, i32);
    }

    // INT → CHAR (truncate)
    if (srcType.isInteger(32) && targetType.isInteger(8)) {
        return builder->create<mlir::arith::TruncIOp>(loc, targetType, val);
    }

    // INT → BOOL
    if (srcType.isInteger(32) && targetType.isInteger(1)) {
        auto zero = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);
        return builder->create<mlir::arith::CmpIOp>(
            loc,
            mlir::arith::CmpIPredicate::ne,
            val,
            zero
        );
    }

    llvm_unreachable("Unsupported cast");
}

/// Determines common compute type.
/// Rule:
///  - float dominates
///  - otherwise use i32
mlir::Type ExpressionsHelper::getComputeType(mlir::Type lhs, mlir::Type rhs) {
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

/// Creates a global constant array.
/// Emits LLVM global and returns pointer.
mlir::Value ExpressionsHelper::createConstArray(
    const std::vector<mlir::Value>& elements,
    PrimType::Prims kind
) {
    assert(!elements.empty());

    mlir::Type elemTy;

    switch (kind) {
        case PrimType::Prims::Int:   elemTy = builder->getIntegerType(32); break;
        case PrimType::Prims::Real:  elemTy = builder->getF32Type(); break;
        case PrimType::Prims::Bool:  elemTy = builder->getI1Type(); break;
        case PrimType::Prims::Char:  elemTy = builder->getIntegerType(8); break;
        default:
            elemTy = mlir::LLVM::LLVMPointerType::get(builder->getContext());
    }

    // Extract constant attributes
    std::vector<mlir::Attribute> attrs;
    for (auto v : elements) {
        auto cst = v.getDefiningOp<mlir::arith::ConstantOp>();
        if (!cst)
            llvm::report_fatal_error("Non-constant array element");

        attrs.push_back(cst.getValue());
    }

    // LLVM array type
    auto arrayTy = mlir::LLVM::LLVMArrayType::get(elemTy, elements.size());

    auto denseAttr = mlir::DenseElementsAttr::get(
        mlir::RankedTensorType::get({(int64_t)elements.size()}, elemTy),
        attrs
    );

    // Create global
    auto oldIP = builder->saveInsertionPoint();
    builder->setInsertionPointToStart(module.getBody());

    std::string name = "arr_const_" + std::to_string(globalCounter++);

    auto global = builder->create<mlir::LLVM::GlobalOp>(
        loc,
        arrayTy,
        true,
        mlir::LLVM::Linkage::Internal,
        name,
        denseAttr
    );

    builder->restoreInsertionPoint(oldIP);

    return builder->create<mlir::LLVM::AddressOfOp>(loc, global);
}

/// Creates runtime array on stack using alloca.
/// Stores elements sequentially.
mlir::Value ExpressionsHelper::createRuntimeArray(
    const std::vector<mlir::Value>& elements,
    PrimType::Prims kind
) {
    mlir::Type elemTy;

    switch (kind) {
        case PrimType::Prims::Int:   elemTy = types->i32; break;
        case PrimType::Prims::Real:  elemTy = types->f32; break;
        case PrimType::Prims::Bool:  elemTy = types->b1; break;
        case PrimType::Prims::Char:  elemTy = types->c8; break;
        default:
            elemTy = mlir::LLVM::LLVMPointerType::get(builder->getContext());
            break;
    }

    auto ptrTy = mlir::LLVM::LLVMPointerType::get(builder->getContext());

    // Allocate N elements
    auto size = builder->create<mlir::arith::ConstantIntOp>(
        loc, elements.size(), 32);

    auto alloca = builder->create<mlir::LLVM::AllocaOp>(
        loc, ptrTy, elemTy, size);

    // Store each element
    for (size_t i = 0; i < elements.size(); i++) {

        auto idx = builder->create<mlir::arith::ConstantIntOp>(loc, i, 32);

        auto gep = builder->create<mlir::LLVM::GEPOp>(
            loc,
            ptrTy,
            elemTy,
            alloca,
            mlir::ValueRange{idx}
        );

        builder->create<mlir::LLVM::StoreOp>(loc, elements[i], gep);
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