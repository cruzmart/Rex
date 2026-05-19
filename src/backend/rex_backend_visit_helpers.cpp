
#include "backend/rex_backend_visit.h"
#include "rex_ast.h"
#include "rex_ast_nodes.h"
#include "rex_exps.h"
#include "rex_stmts.h"
#include "rex_types.h"
#include <memory>


#include <mlir/Dialect/LLVMIR/LLVMDialect.h>
#include <mlir/IR/ValueRange.h>

namespace rex {

/// =============================================================
/// Helpers
/// =============================================================

namespace {

template<typename T, typename U>
std::shared_ptr<T> cast(U &&value) {
    return std::static_pointer_cast<T>(std::forward<U>(value));
}

} // namespace

mlir::Value IRGen::i32(int value) {
    return builder->create<mlir::arith::ConstantIntOp>(
        loc,
        value,
        32
    );
}

mlir::Value IRGen::loadIfPointer(
    mlir::Value value,
    std::shared_ptr<Type> type
) {
    if (!value.getType().isa<mlir::LLVM::LLVMPointerType>())
        return value;

    if (type->kind == TypeKind::Array)
        return value;

    return builder->create<mlir::LLVM::LoadOp>(
        loc,
        types->getMLIRType(type),
        value
    );
}

    
}