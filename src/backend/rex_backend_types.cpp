  #include "backend/rex_backend_types.h"


namespace rex {
 TypesHelper::TypesHelper(
                         std::shared_ptr<mlir::OpBuilder> b,
                         mlir::Location l)
            
    : builder(b), loc(l) {}

    mlir::Type TypesHelper::i32_t(){
      return builder->getI32Type();
    }
    mlir::Type TypesHelper::c8_t(){
      return builder->getI8Type();
    }
    mlir::Type TypesHelper::f32_t(){
      return builder->getF32Type();
    }
    mlir::Type TypesHelper::b1_t(){
      return builder->getI1Type();
    }
    mlir::Type TypesHelper::ptrty(){
      return mlir::LLVM::LLVMPointerType::get(builder->getContext());
    }

    mlir::Type TypesHelper::getMLIRType(std::shared_ptr<Type> t) {

        // =====================================================
        // ARRAY
        // =====================================================

        if (t->kind == TypeKind::Array) {

            auto arrTy =
                std::static_pointer_cast<ArrayType>(t);

            auto elemTy =
                getMLIRType(arrTy->elem);

            auto [rows, cols] =
                arrTy->dimensions();

            // matrix -> flattened array
            if (arrTy->isMatrix()) {
                auto elemTy =
                getMLIRType(std::static_pointer_cast<ArrayType>(arrTy->elem)->elem);
                return mlir::LLVM::LLVMArrayType::get(
                    elemTy,
                    rows * cols
                );
            }

            // normal array
            return mlir::LLVM::LLVMArrayType::get(
                elemTy,
                cols
            );
        }

        // =====================================================
        // RANGE
        // =====================================================

        if (t->kind == TypeKind::Range) {
            return i32_t();
        }

        // =====================================================
        // PRIMITIVE
        // =====================================================

        auto prim =
            std::static_pointer_cast<PrimType>(t)->prim;

        switch (prim) {

            case PrimType::Prims::Int:
                return i32_t();

            case PrimType::Prims::Bool:
                return b1_t();

            case PrimType::Prims::Char:
                return c8_t();

            case PrimType::Prims::String:
                return ptrty();

            case PrimType::Prims::Real:
                return f32_t();

            default:
                llvm::report_fatal_error(
                    "Unsupported type in MLIR"
                );
        }
    }

    mlir::LLVM::LLVMStructType TypesHelper::createStruct(std::vector<std::shared_ptr<Type>> types){
        std::vector<mlir::Type> m_t;
        for(auto type : types){
            m_t.push_back(getMLIRType(type));
        }
        return mlir::LLVM::LLVMStructType::getLiteral(builder->getContext(), m_t);
    }

 }