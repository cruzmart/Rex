  #include "backend/rex_backend_types.h"


namespace rex {
 TypesHelper::TypesHelper(
                         mlir::OpBuilder &b,
                         mlir::Location l)
            
    : builder(&b), loc(l) {
          // initialize the types
        i32 = builder->getI32Type();
        b1 = builder->getI1Type();
        f32 = builder->getF32Type();  // For 32-bit float
        c8 = builder->getI8Type();
        ptr = mlir::LLVM::LLVMPointerType::get(builder->getContext());
    }

 }