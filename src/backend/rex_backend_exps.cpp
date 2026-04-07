#include "backend/rex_backend_exps.h"
#include "backend/rex_backend_prints.h"
#include "backend/rex_backend_types.h"
#include "rex_exps.h"
#include "rex_types.h"
#include <memory>
#include <mlir/IR/BuiltinAttributes.h>
#include <mlir/IR/ValueRange.h>


 namespace rex {

    // Literal Creation 

    ExpressionsHelper::ExpressionsHelper(
                         mlir::OpBuilder &b,
                         mlir::ModuleOp &m,
                         mlir::Location l,
                         std::shared_ptr<TypesHelper> t,
                         std::shared_ptr<PrintHelper> p)
            
    : builder(&b), module(&m), loc(l), types(t), prints(p) {}

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
    
mlir::LLVM::GlobalOp ExpressionsHelper::createString(const std::string &text) {
    // Save current insertion point
    auto oldInsertionPoint = builder->saveInsertionPoint();

    // Switch to module level to create global
    builder->setInsertionPointToStart(module->getBody());

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

    return global;
}
    
 }