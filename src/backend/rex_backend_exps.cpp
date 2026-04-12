#include "backend/rex_backend_exps.h"
#include "backend/rex_backend_prints.h"
#include "backend/rex_backend_types.h"
#include "rex_exps.h"
#include "rex_ops.h"
#include "rex_types.h"
#include <memory>
#include <mlir/Conversion/ArithToLLVM/ArithToLLVM.h>
#include <mlir/Dialect/Arith/IR/Arith.h>
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
            // Must be at least: 'x'
            if (text.size() < 3 || text.front() != '\'' || text.back() != '\'') {
                throw std::runtime_error("Invalid char literal: " + text);
            }

            char value;

            // Handle escape sequences
            if (text[1] == '\\') {
                if (text.size() != 4) {
                    throw std::runtime_error("Invalid escaped char literal: " + text);
                }

                switch (text[2]) {
                    case 'n':  value = '\n'; break;
                    case 't':  value = '\t'; break;
                    case 'r':  value = '\r'; break;
                    case '\\': value = '\\'; break;
                    case '\'': value = '\''; break;
                    case '0':  value = '\0'; break;
                    default:
                        throw std::runtime_error("Unknown escape sequence: " + text);
                }
            } else {
                // Normal case: 'c'
                if (text.size() != 3) {
                    throw std::runtime_error("Invalid char literal: " + text);
                }
                value = text[1];
            }

            return builder->create<mlir::arith::ConstantOp>(
                loc,
                builder->getI8IntegerAttr(static_cast<int8_t>(value))
            );
    }    
    mlir::Value ExpressionsHelper::createString(const std::string &text) {
        // Save current insertion point
        auto oldInsertionPoint = builder->saveInsertionPoint();


        // Switch to module level to create global
        builder->setInsertionPointToStart(module.getBody());


        // Include null-terminator in the string
        std::string strWithNull = text + '\0';
        std::string name = "str_const_" + std::to_string(globalCounter++);

        // i8 type for characters
        auto i8Ty = builder->getIntegerType(8);

        // LLVM array type [N x i8]
        auto arrayTy = mlir::LLVM::LLVMArrayType::get(i8Ty, strWithNull.size());

        // Create the global constant at module level
        auto global = builder->create<mlir::LLVM::GlobalOp>(
            loc,
            arrayTy,                       // array type
            /*isConstant=*/true,            // constant
            mlir::LLVM::Linkage::Internal, // internal linkage
            name,                           // global name
            builder->getStringAttr(strWithNull) // initial value
        );

        // Restore previous insertion point
        builder->restoreInsertionPoint(oldInsertionPoint);


        return builder->create<mlir::LLVM::AddressOfOp>(loc, global);
    }


    mlir::Value ExpressionsHelper::createBinaryExp(mlir::Value lhs, mlir::Value rhs, PrimType::Prims prim_t, BinaryOp op){

         mlir::Type res_t;


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

        
        // logic op
         switch(op){
            case BinaryOp::EQ:{}
            case BinaryOp::NEQ:{}
            case BinaryOp::LT:{}
            case BinaryOp::GT:{}
            case BinaryOp::LE:{}
            case BinaryOp::GE:{}
            default:
                break;
        }


        // bitwise op (check if the lhs and rhs values are bool, both must be)
        switch(op){
            case BinaryOp::AND:{}
            case BinaryOp::OR:{}
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



    mlir::Value ExpressionsHelper::castTo(mlir::Value val, mlir::Type targetType) {
        mlir::Type srcType = val.getType();

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
    mlir::Value ExpressionsHelper::concatString(mlir::Value str_lhs, mlir::Value str_rhs){return mlir::Value();}

 }