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

    inline bool isPointer(mlir::Type t) {
        return t.isa<mlir::LLVM::LLVMPointerType>();
    }


                            /// =============================================================
                            ///                 ExpressionsHelper (Basic)
                            /// =============================================================


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
mlir::Value ExpressionsHelper::i32(int value){
    return builder->create<mlir::arith::ConstantIntOp>(
        loc,
        value,
        32
    );
}

 mlir::LLVM::LLVMPointerType ExpressionsHelper::ptrty(){
    return mlir::LLVM::LLVMPointerType::get(builder->getContext());
 }

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
            this->types->i32_t(),
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
            this->types->f32_t(),
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
                this->types->b1_t(),
                1
            )
        );
    }

    if (text == "false") {
        return builder->create<mlir::arith::ConstantOp>(
            loc,
            builder->getIntegerAttr(
                this->types->b1_t(),
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

    auto i8Ty = this->types->c8_t();
       

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
        this->types->ptrty(),
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
        this->types->ptrty();
    
    auto one = 
        i32(1);

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

}