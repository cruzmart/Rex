#include "backend/rex_backend_visit.h"
#include "rex_types.h"
#include "rex_symbol.h"

namespace rex {
void IRGen::visitFunctionDef(
    std::shared_ptr<FunctionDecl> funcDef
) {
    // =====================================================
    // FUNCTION TYPE
    // =====================================================


    auto funcTy = cast<FunctionType>(funcDef->func_type);

    std::vector<mlir::Type> paramTypes;
    for (auto &param : funcTy->params) {
        paramTypes.push_back(types->getMLIRType(param->para_type));
    }

    auto retTy = types->getMLIRType(funcTy->ret);

    auto llvmFuncTy =
        mlir::LLVM::LLVMFunctionType::get(retTy, paramTypes);

    // =====================================================
    // CREATE FUNCTION
    // =====================================================

    auto function =
        builder->create<mlir::LLVM::LLVMFuncOp>(
            loc,
            funcDef->func_name,
            llvmFuncTy
        );

    // =====================================================
    // ENTRY BLOCK
    // =====================================================
  
    auto savedPoint = builder->saveInsertionPoint();
    auto entry = function.addEntryBlock();
    auto exitBlock = builder->createBlock(&function.getBody());

    builder->setInsertionPointToStart(entry);

    contStack.push_back(exitBlock);
        
    // =====================================================
    // SCOPE
    // =====================================================

    auto oldScope = currentScope;
    currentScope = std::make_shared<Scope>(oldScope);

    // =====================================================
    // PARAMETERS
    // =====================================================

    for (size_t i = 0; i < funcTy->params.size(); ++i) {

        auto paramDecl = funcTy->params[i];
        auto arg = entry->getArgument(i);

        auto one = builder->create<mlir::arith::ConstantIntOp>(
            loc, 1, 32
        );

        auto ptr = builder->create<mlir::LLVM::AllocaOp>(
            loc,
            types->ptrty(),
            arg.getType(),
            one
        );

        builder->create<mlir::LLVM::StoreOp>(
            loc,
            arg,
            ptr
        );

        auto sym = std::make_shared<VariableSymbol>(
            paramDecl->para_name,
            paramDecl->para_type,
            ptr
        );

        currentScope->define(sym);
    }

    // =====================================================
    // BODY
    // =====================================================

    visitBlock(funcDef->body);

    // safety branch
    if (builder->getInsertionBlock() &&
        !blockHasTerminator(builder->getInsertionBlock())) {
        builder->create<mlir::LLVM::BrOp>(loc, exitBlock);
    }

    contStack.pop_back();

    // =====================================================
    // 4. exit block
    // =====================================================

    builder->setInsertionPointToStart(exitBlock);
    builder->create<mlir::LLVM::UnreachableOp>(loc);

    
    // =====================================================
    // RESTORE SCOPE
    // =====================================================

    currentScope = oldScope;

    // =====================================================
    // RESTORE OUTER INSERTION POINT (CRITICAL FIX)
    // =====================================================

    builder->restoreInsertionPoint(savedPoint);
}

void IRGen::visitReturn(std::shared_ptr<ReturnStmt> restmt){
        builder->create<mlir::LLVM::ReturnOp>(
        loc,
        visitExp(restmt->value)
    );
}
}