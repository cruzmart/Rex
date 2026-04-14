#include "backend/rex_backend_exps.h"
#include "backend/rex_backend_prints.h"
#include "backend/rex_backend_types.h"
#include "rex_exps.h"
#include "rex_ops.h"
#include "rex_types.h"
#include <memory>
#include <mlir/Conversion/ArithToLLVM/ArithToLLVM.h>
#include <mlir/Dialect/Arith/IR/Arith.h>
#include <mlir/Dialect/LLVMIR/LLVMTypes.h>
#include <mlir/IR/BuiltinAttributes.h>
#include <mlir/IR/ValueRange.h>


 namespace rex {

    // Literal Creation 

    ExpressionsHelper::ExpressionsHelper(
                         std::shared_ptr<mlir::OpBuilder> b,
                         mlir::ModuleOp & m,
                         mlir::Location l,
                         std::shared_ptr<TypesHelper> t)
            
    : builder(b), module(m), loc(l), types(t) {}

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
    mlir::Value ExpressionsHelper::createInt(const std::string &text){
        llvm::APInt value(32, text, 10); // avoids std::stoi overflow
        return builder->create<mlir::arith::ConstantOp>(loc, builder->getIntegerAttr(types->i32, value));
    }
    mlir::Value ExpressionsHelper::createFloat(const std::string &text){
        llvm::APFloat value(0.0f);

        auto statusOrError = value.convertFromString(text, llvm::APFloat::rmNearestTiesToEven);
        if (!statusOrError) {
            throw std::runtime_error("Invalid float literal: " + text);
        }
        auto status = *statusOrError;
        if (status & llvm::APFloat::opInvalidOp) {
            throw std::runtime_error("Invalid float literal: " + text);
        }

        return builder->create<mlir::arith::ConstantOp>(
            loc,
            builder->getFloatAttr(types->f32, value)
        );
    }
    mlir::Value ExpressionsHelper::createBool(const std::string &text){
        if (text == "true") {
        return builder->create<mlir::arith::ConstantOp>(
            loc,
            builder->getIntegerAttr(types->b1, 1)
        );
        } else if (text == "false") {
            return builder->create<mlir::arith::ConstantOp>(
                loc,
                builder->getIntegerAttr(types->b1, 0)
            );
        }

        throw std::runtime_error("Invalid boolean literal: " + text);
    }
    mlir::Value ExpressionsHelper::createChar(const std::string &text) {
        if (text.empty())
            throw std::runtime_error("Empty char literal");

        char value;

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

    mlir::Value ExpressionsHelper::createString(const std::string &text) {

        // 🔥 1. reuse existing string literal
        auto it = stringPool.find(text);
        if (it != stringPool.end()) {
            return builder->create<mlir::LLVM::AddressOfOp>(loc, it->second);
        }

        // 🔥 2. insert global at module start
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

        // 🔥 3. return pointer to first element (IMPORTANT CHANGE)
        auto addr = builder->create<mlir::LLVM::AddressOfOp>(loc, global);

        // cast to i8*
        return builder->create<mlir::LLVM::BitcastOp>(
            loc,
            mlir::LLVM::LLVMPointerType::get(builder->getContext()),
            addr
        );
    }


    mlir::Value ExpressionsHelper::createBinaryExp(mlir::Value lhs, mlir::Value rhs, PrimType::Prims prim_t, BinaryOp op){

         mlir::Type res_t;

         // we got to check if they are pointer types (it would be string)
        //  if(lhs.isa<mlir::LLVM::LLVMPointerType>() )
 
        // string concat 
         if(lhs.getType().isa<mlir::LLVM::LLVMPointerType>() && rhs.getType().isa<mlir::LLVM::LLVMPointerType>() && op == BinaryOp::ADD)
            return concatString(lhs, rhs);



        switch(prim_t){
            case PrimType::Prims::Int:{
                res_t = types->i32;
                break;
            }
            case PrimType::Prims::Real:{
                res_t = types->f32;
                break;
            }
            case rex::PrimType::Prims::Bool:{
                res_t = types->b1;
                break;
            }
            case PrimType::Prims::Char:{
                res_t = types->c8;
                break;
            }
            default: {
                res_t = mlir::Type();
                break;
            }
        }
        
        

        // arth op
        switch(op){
            case BinaryOp::ADD: return add(lhs, rhs, res_t);
            case BinaryOp::SUB: return sub(lhs, rhs, res_t);
            case BinaryOp::MUL: return mul(lhs, rhs, res_t);
            case BinaryOp::DIV: return div(lhs, rhs, res_t);
            case BinaryOp::MOD: return mod(lhs, rhs, res_t);
            default:
                break;
        }

        
        // =========================
        // comparisons
        // =========================
        switch(op){
            case BinaryOp::EQ:  return eq(lhs, rhs);
            case BinaryOp::NEQ: return neq(lhs, rhs);
            case BinaryOp::LT:  return lt(lhs, rhs);
            case BinaryOp::GT:  return gt(lhs, rhs);
            case BinaryOp::LE:  return le(lhs, rhs);
            case BinaryOp::GE:  return ge(lhs, rhs);
            default:
                break;
        }

        // =========================
        // logic ops
        // =========================
        switch(op){
            case BinaryOp::AND: return and_(lhs, rhs);
            case BinaryOp::OR:  return or_(lhs, rhs);
            default:
                break;
        }
        
        return mlir::Value();

    }
   

    mlir::Value ExpressionsHelper::add( mlir::Value lhs, mlir::Value rhs,  mlir::Type resultType) {

        // 🔥 STEP 1: choose computation type
        mlir::Type computeType = getComputeType(lhs.getType(), rhs.getType());

        // 🔥 STEP 2: cast operands to compute type
        lhs = castTo(lhs, computeType);
        rhs = castTo(rhs, computeType);

        // 🔥 STEP 3: perform operation
        mlir::Value result;

        if (computeType.isF32()) {
            result = builder->create<mlir::arith::AddFOp>(loc, lhs, rhs);
        } else {
            result = builder->create<mlir::arith::AddIOp>(loc, lhs, rhs);
        }

    
        // 🔥 STEP 4: cast to final result type
        return castTo(result, resultType);
    }
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
    mlir::Value ExpressionsHelper::mul( mlir::Value lhs, mlir::Value rhs, mlir::Type resultType) {
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
    mlir::Value ExpressionsHelper::div( mlir::Value lhs, mlir::Value rhs, mlir::Type resultType) {
        mlir::Type computeType = getComputeType(lhs.getType(), rhs.getType());

        lhs = castTo(lhs, computeType);
        rhs = castTo(rhs, computeType);

        // 🔥 Constant zero check (compile-time only)
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
            // signed division
            result = builder->create<mlir::arith::DivSIOp>(loc, lhs, rhs);
        }

        return castTo(result, resultType);
    }
    mlir::Value ExpressionsHelper::mod( mlir::Value lhs, mlir::Value rhs, mlir::Type resultType) {
        mlir::Type computeType = getComputeType(lhs.getType(), rhs.getType());

        lhs = castTo(lhs, computeType);
        rhs = castTo(rhs, computeType);

        if (computeType.isF32()) {
            llvm::report_fatal_error("Modulo not supported for float");
        }

        // Optional: same zero check as div
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


    // =====================================
    // Comparison Helpers
    // =====================================
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

    // =====================================
    // Comparisons
    // =====================================
    mlir::Value ExpressionsHelper::eq(mlir::Value lhs, mlir::Value rhs) {
        auto lhsTy = lhs.getType();
        auto rhsTy = rhs.getType();

        bool lhsIsStr = lhsTy.isa<mlir::LLVM::LLVMPointerType>();
        bool rhsIsStr = rhsTy.isa<mlir::LLVM::LLVMPointerType>();

        // -------------------------
        // STRING == STRING
        // -------------------------
        if (lhsIsStr && rhsIsStr) {
            auto strcmpFunc = getOrInsertStrcmp();

            auto call = builder->create<mlir::LLVM::CallOp>(
                loc,
                strcmpFunc,
                mlir::ValueRange{lhs, rhs}
            );

            mlir::Value result = call.getResult();

            auto zero = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);

            return builder->create<mlir::arith::CmpIOp>(
                loc,
                mlir::arith::CmpIPredicate::eq,
                result,
                zero
            );
        }

        // -------------------------
        // NORMAL NUMERIC CASE
        // -------------------------
        auto computeType = getComputeType(lhsTy, rhsTy);
        lhs = castTo(lhs, computeType);
        rhs = castTo(rhs, computeType);

        if (computeType.isF32()) {
            return builder->create<mlir::arith::CmpFOp>(
                loc,
                mlir::arith::CmpFPredicate::OEQ,
                lhs,
                rhs
            );
        }

        return builder->create<mlir::arith::CmpIOp>(
            loc,
            mlir::arith::CmpIPredicate::eq,
            lhs,
            rhs
        );
    }

   mlir::Value ExpressionsHelper::neq(mlir::Value lhs, mlir::Value rhs) {
        auto lhsTy = lhs.getType();
        auto rhsTy = rhs.getType();

        bool lhsIsStr = lhsTy.isa<mlir::LLVM::LLVMPointerType>();
        bool rhsIsStr = rhsTy.isa<mlir::LLVM::LLVMPointerType>();

        // -------------------------
        // STRING != STRING
        // -------------------------
        if (lhsIsStr && rhsIsStr) {
            auto strcmpFunc = getOrInsertStrcmp();

            auto call = builder->create<mlir::LLVM::CallOp>(
                loc,
                strcmpFunc,
                mlir::ValueRange{lhs, rhs}
            );

            mlir::Value result = call.getResult();

            auto zero = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);

            return builder->create<mlir::arith::CmpIOp>(
                loc,
                mlir::arith::CmpIPredicate::ne,
                result,
                zero
            );
        }

        // -------------------------
        // NORMAL NUMERIC CASE
        // -------------------------
        auto computeType = getComputeType(lhsTy, rhsTy);
        lhs = castTo(lhs, computeType);
        rhs = castTo(rhs, computeType);

        if (computeType.isF32()) {
            return builder->create<mlir::arith::CmpFOp>(
                loc,
                mlir::arith::CmpFPredicate::ONE,
                lhs,
                rhs
            );
        }

        return builder->create<mlir::arith::CmpIOp>(
            loc,
            mlir::arith::CmpIPredicate::ne,
            lhs,
            rhs
        );
    }


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

    mlir::Value ExpressionsHelper::and_(mlir::Value lhs, mlir::Value rhs){
        lhs = castTo(lhs, types->b1);
        rhs = castTo(rhs, types->b1);

        return builder->create<mlir::arith::AndIOp>(loc, lhs, rhs);
    }

    mlir::Value ExpressionsHelper::or_(mlir::Value lhs, mlir::Value rhs){
        lhs = castTo(lhs, types->b1);
        rhs = castTo(rhs, types->b1);

        return builder->create<mlir::arith::OrIOp>(loc, lhs, rhs);
    }



    mlir::Value ExpressionsHelper::castTo(mlir::Value val, mlir::Type targetType) {
        mlir::Type srcType = val.getType();

        // 🚨 GUARD: never cast pointers
        if (srcType.isa<mlir::LLVM::LLVMPointerType>() ||
            targetType.isa<mlir::LLVM::LLVMPointerType>()) {
            llvm::report_fatal_error("Invalid cast involving pointer type");
        }

        // SAME TYPE → no-op
        if (srcType == targetType)
            return val;

        // =========================
        // INT → FLOAT
        // =========================
        if (srcType.isInteger(32) && targetType.isF32()) {
            return builder->create<mlir::arith::SIToFPOp>(loc, targetType, val);
        }

        // =========================
        // FLOAT → INT
        // =========================
        if (srcType.isF32() && targetType.isInteger(32)) {
            return builder->create<mlir::arith::FPToSIOp>(loc, targetType, val);
        }

        // =========================
        // BOOL (i1) → INT
        // =========================
        if (srcType.isInteger(1) && targetType.isInteger(32)) {
            return builder->create<mlir::arith::ExtUIOp>(loc, targetType, val);
        }

        // =========================
        // BOOL → FLOAT
        // =========================
        if (srcType.isInteger(1) && targetType.isF32()) {
            auto i32 = builder->create<mlir::arith::ExtUIOp>(loc, types->i32, val);
            return builder->create<mlir::arith::SIToFPOp>(loc, targetType, i32);
        }

        // =========================
        // BOOL → CHAR
        // =========================
        if (srcType.isInteger(1) && targetType.isInteger(8)) {
            return builder->create<mlir::arith::ExtUIOp>(loc, targetType, val);
        }

        // =========================
        // CHAR (i8) → INT
        // =========================
        if (srcType.isInteger(8) && targetType.isInteger(32)) {
            return builder->create<mlir::arith::ExtUIOp>(loc, targetType, val);
        }

        // =========================
        // CHAR → FLOAT
        // =========================
        if (srcType.isInteger(8) && targetType.isF32()) {
            auto i32 = builder->create<mlir::arith::ExtUIOp>(loc, types->i32, val);
            return builder->create<mlir::arith::SIToFPOp>(loc, targetType, i32);
        }

        // =========================
        // INT → CHAR
        // =========================
        if (srcType.isInteger(32) && targetType.isInteger(8)) {
            return builder->create<mlir::arith::TruncIOp>(loc, targetType, val);
        }

        // =========================
        // INT → BOOL
        // =========================
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

    mlir::Type ExpressionsHelper::getComputeType(mlir::Type lhs,  mlir::Type rhs) {
        // FLOAT dominates everything
        if (lhs.isF32() || rhs.isF32()) {
            return types->f32;
        }

        // otherwise use i32 as canonical integer compute type
        return types->i32;
    }
    mlir::Value ExpressionsHelper::concatString(
        mlir::Value lhs,
        mlir::Value rhs
    ) {
        // Normalize both sides
        lhs = toStringValue(lhs);
        rhs = toStringValue(rhs);

        // Extract globals
        auto lhsAddr = lhs.getDefiningOp<mlir::LLVM::AddressOfOp>();
        auto rhsAddr = rhs.getDefiningOp<mlir::LLVM::AddressOfOp>();

        if (!lhsAddr || !rhsAddr) {
            llvm::report_fatal_error("Only constant string concat supported");
        }

        auto lhsGlobal = module.lookupSymbol<mlir::LLVM::GlobalOp>(
            lhsAddr.getGlobalName()
        );

        auto rhsGlobal = module.lookupSymbol<mlir::LLVM::GlobalOp>(
            rhsAddr.getGlobalName()
        );

        auto lhsAttr = lhsGlobal.getValue()->dyn_cast<mlir::StringAttr>();
        auto rhsAttr = rhsGlobal.getValue()->dyn_cast<mlir::StringAttr>();

        std::string lhsStr = lhsAttr.getValue().str();
        std::string rhsStr = rhsAttr.getValue().str();

        // Remove duplicate null
        if (!lhsStr.empty())
            lhsStr.pop_back();

        std::string combined = lhsStr + rhsStr;

        return createString(combined);
    }

    mlir::Value ExpressionsHelper::toStringValue(mlir::Value v) {
        auto type = v.getType();

        // already string (i8*)
        if (type.isa<mlir::LLVM::LLVMPointerType>()) {
            return v;
        }

        // char → string (constant only)
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

    std::string ExpressionsHelper::foldConstString(std::shared_ptr<Expr> expr) {
    
        // =========================
        // CASE 1: Literal
        // =========================
        if(expr->exp_kind == ExprKind::Literal){
            auto lit = std::static_pointer_cast<LiteralExpr>(expr);
            auto prim = std::static_pointer_cast<PrimType>(lit->type);

            switch (prim->prim) {

                case PrimType::Prims::String: {
                    // Already a full string
                    return lit->value;
                }

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

        // =========================
        // CASE 2: BinaryExpr (+ only)
        // =========================
        if(expr->exp_kind == ExprKind::Binary){
            auto bin = std::static_pointer_cast<BinaryExpr>(expr);

            if (bin->operation != BinaryOp::ADD) {
                llvm::report_fatal_error("Only + supported in foldConstString");
            }

            // Recursively fold both sides
            std::string lhs = foldConstString(bin->lhs);
            std::string rhs = foldConstString(bin->rhs);

            return lhs + rhs;
        }

        // =========================
        // OTHERWISE
        // =========================
        llvm::report_fatal_error("Expression is not a constant string expression");
    }

    bool ExpressionsHelper::isStringValue(mlir::Value v){
        return v.getType().isa<mlir::LLVM::LLVMPointerType>();
    }

    mlir::Value ExpressionsHelper::eqStringsConst(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs) {
        std::string l = foldConstString(lhs);
        std::string r = foldConstString(rhs);

        bool result = (l == r);

        return builder->create<mlir::arith::ConstantOp>(
            loc,
            builder->getBoolAttr(result)
        );
    }

    mlir::LLVM::LLVMFuncOp ExpressionsHelper::getOrInsertStrcmp() {
        // Try to find existing declaration
        if (auto func = module.lookupSymbol<mlir::LLVM::LLVMFuncOp>("strcmp")) {
            return func;
        }

        // i8*
        auto i8Ptr = mlir::LLVM::LLVMPointerType::get(builder->getContext());

        // int strcmp(i8*, i8*)
        auto fnType = mlir::LLVM::LLVMFunctionType::get(
            builder->getI32Type(),
            {i8Ptr, i8Ptr},
            /*isVarArg=*/false
        );

        // IMPORTANT: insert at module level (not inside a function)
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
 
    mlir::Value ExpressionsHelper::createArray( const std::vector<mlir::Value>& elements, std::shared_ptr<Type> type) {
        auto arrType = std::static_pointer_cast<ArrayType>(type);
        auto elemType = arrType->elem;
        int size = elements.size();

        // -----------------------------------
        // Resolve MLIR element type
        // -----------------------------------
        mlir::Type mlirElemTy;

        if (elemType->kind == TypeKind::Primitive) {
            auto prim = std::static_pointer_cast<PrimType>(elemType);

            switch (prim->prim) {
                case PrimType::Prims::Int:    mlirElemTy = types->i32; break;
                case PrimType::Prims::Real:   mlirElemTy = types->f32; break;
                case PrimType::Prims::Bool:   mlirElemTy = types->b1;  break;
                case PrimType::Prims::Char:   mlirElemTy = types->c8;  break;
                case PrimType::Prims::String:
                    mlirElemTy = mlir::LLVM::LLVMPointerType::get(builder->getContext());
                    break;
                default:
                    llvm::report_fatal_error("Unsupported primitive in array");
            }
        } else {
            llvm::report_fatal_error("Nested arrays not supported yet");
        }

        // -----------------------------------
        // Create LLVM array type
        // -----------------------------------
        auto llvmArrayTy = mlir::LLVM::LLVMArrayType::get(mlirElemTy, size);

        // -----------------------------------
        // Allocate
        // -----------------------------------
        auto arrayPtr = builder->create<mlir::LLVM::AllocaOp>(
            loc,
            mlir::LLVM::LLVMPointerType::get(builder->getContext()),
            llvmArrayTy,
            builder->create<mlir::arith::ConstantIntOp>(loc, 1, 32)
        );

        // -----------------------------------
        // Store elements
        // -----------------------------------
        for (int i = 0; i < size; i++) {

            mlir::Value val = elements[i];

            // ⚠️ only cast non-pointers
            if (!val.getType().isa<mlir::LLVM::LLVMPointerType>()) {
                val = castTo(val, mlirElemTy);
            }

            auto zero = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);
            auto idx  = builder->create<mlir::arith::ConstantIntOp>(loc, i, 32);

            auto elemPtr = builder->create<mlir::LLVM::GEPOp>(
                loc,
                mlir::LLVM::LLVMPointerType::get(builder->getContext()),
                llvmArrayTy,
                arrayPtr,
                mlir::ValueRange{zero, idx}
            );

            builder->create<mlir::LLVM::StoreOp>(loc, val, elemPtr);
        }

        return arrayPtr;
    }
 }
