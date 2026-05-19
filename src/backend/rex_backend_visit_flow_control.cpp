
#include "backend/rex_backend_visit.h"
#include "rex_exps.h"
#include "rex_stmts.h"
#include "rex_types.h"
#include <memory>


#include <mlir/Dialect/LLVMIR/LLVMDialect.h>
#include <mlir/IR/ValueRange.h>


namespace rex {

    // =====================================================
    // CONTROL FLOW HELPERS
    // =====================================================

    bool IRGen::isTerminated(mlir::Block *block) {
        return block &&
            !block->empty() &&
            block->back().hasTrait<
                mlir::OpTrait::IsTerminator>();
    }

    bool IRGen::blockHasTerminator(mlir::Block *block) {
        return !block->empty() &&
            block->back().hasTrait<mlir::OpTrait::IsTerminator>();
    }


    void IRGen::branchIfNeeded(mlir::Block *target) {
        auto *block = builder->getInsertionBlock();

        if (block && !isTerminated(block)) {
            builder->create<mlir::LLVM::BrOp>(
                loc,
                target
            );
        }
    }

    mlir::Value IRGen::getIterableSize(
        std::shared_ptr<Expr> expr
    ) {

        if (expr->exp_kind ==
            ExprKind::Array) {

            
            auto arr =
                cast<ArrayType>(expr->type);

            
            auto [rows, cols] = arr->dimensions();
                // this is bugged because for matricies this will break.

            return i32(rows * cols);
        }

        if (expr->exp_kind ==
            ExprKind::Range) {

            auto range =
                cast<RangeExpr>(expr);

            auto diff =
                exps->sub(
                    visitExp(range->rhs),
                    visitExp(range->lhs),
                    types->i32
                );

            return exps->add(
                diff,
                i32(1),
                types->i32
            );
        }

        return {};
    }


    // =====================================================
    // CONTROL FLOW IMPLEMENTATIONS
    // =====================================================
    void IRGen::visitLoop(
        std::shared_ptr<LoopStmt> stmt
    ) {

        auto *entry =
            builder->getInsertionBlock();

        auto *func =
            entry->getParentOp();

        auto *region =
            &func->getRegion(0);

        auto *condBlock =
            builder->createBlock(region);

        auto *bodyBlock =
            builder->createBlock(region);

        auto *mergeBlock =
            builder->createBlock(region);

        builder->setInsertionPointToEnd(entry);

        builder->create<mlir::LLVM::BrOp>(
            loc,
            condBlock
        );

        builder->setInsertionPointToStart(
            condBlock
        );

        builder->create<mlir::LLVM::CondBrOp>(
            loc,
            exps->createBool("true"),
            bodyBlock,
            mergeBlock
        );

        builder->setInsertionPointToStart(
            bodyBlock
        );

        contStack.push_back(condBlock);
        breakStack.push_back(mergeBlock);

        visitBlock(stmt->body);

        breakStack.pop_back();
        contStack.pop_back();

        branchIfNeeded(condBlock);

        builder->setInsertionPointToStart(
            mergeBlock
        );
    }


    void IRGen::visitWhile(
        std::shared_ptr<WhileStmt> stmt
    ) {

        auto *entry =
            builder->getInsertionBlock();

        auto *func =
            entry->getParentOp();

        auto *region =
            &func->getRegion(0);

        auto *condBlock =
            builder->createBlock(region);

        auto *bodyBlock =
            builder->createBlock(region);

        auto *mergeBlock =
            builder->createBlock(region);

        builder->setInsertionPointToEnd(entry);

        builder->create<mlir::LLVM::BrOp>(
            loc,
            condBlock
        );

        builder->setInsertionPointToStart(
            condBlock
        );

        auto cond =
            visitExp(stmt->cond);

        builder->create<mlir::LLVM::CondBrOp>(
            loc,
            cond,
            bodyBlock,
            mergeBlock
        );

        builder->setInsertionPointToStart(
            bodyBlock
        );

        contStack.push_back(condBlock);
        breakStack.push_back(mergeBlock);

        visitBlock(stmt->body);

        breakStack.pop_back();
        contStack.pop_back();

        branchIfNeeded(condBlock);

        builder->setInsertionPointToStart(
            mergeBlock
        );
    }

    void IRGen::visitFor(
        std::shared_ptr<ForStmt> stmt
    ) {

        auto *entry =
            builder->getInsertionBlock();

        auto *func =
            entry->getParentOp();

        auto *region =
            &func->getRegion(0);

        auto ptrTy =
            mlir::LLVM::LLVMPointerType::get(
                builder->getContext()
            );

        auto one  = i32(1);
        auto zero = i32(0);

        // =====================================================
        // BLOCKS
        // =====================================================

        auto *condBlock =
            builder->createBlock(region);

        auto *bodyBlock =
            builder->createBlock(region);

        auto *mergeBlock =
            builder->createBlock(region);

        builder->setInsertionPointToEnd(entry);

        // =====================================================
        // LOOP SCOPE
        // =====================================================

        auto oldScope = currentScope;
        currentScope  = currentScope->push();

        // =====================================================
        // ITERABLE
        // =====================================================

        auto iterable =
            visitExp(stmt->iterable);

        auto iterableTy =
            std::static_pointer_cast<ArrayType>(
                stmt->iterable->type
            );

        // =====================================================
        // ITERATION COUNT
        // =====================================================
        //
        // arrays  -> element count
        // matrices -> row count
        //
        // =====================================================

        int iterationCount;

        if (iterableTy->isMatrix()) {

            iterationCount =
                iterableTy->dimensions().first;

        } else {

            iterationCount =
                iterableTy->dimensions().second;
        }

        auto size =
            i32(iterationCount);

        // =====================================================
        // INDEX STORAGE
        // =====================================================

        auto indexPtr =
            allocateStorage(
                std::make_shared<PrimType>(
                    PrimType::Prims::Int
                ),
                ptrTy,
                one
            );

        builder->create<mlir::LLVM::StoreOp>(
            loc,
            zero,
            indexPtr
        );

        // =====================================================
        // LOOP VARIABLE STORAGE
        // =====================================================

        auto iterSym =
            cast<VariableSymbol>(
                cast<IdExpr>(
                    stmt->iter_var
                )->resolved
            );

        auto valuePtr =
            allocateStorage(
                iterSym->type,
                ptrTy,
                one
            );

        iterSym->ptr = valuePtr;

        currentScope->define(iterSym);

        // =====================================================
        // ENTRY → CONDITION
        // =====================================================

        builder->create<mlir::LLVM::BrOp>(
            loc,
            condBlock
        );

        // =====================================================
        // CONDITION
        // =====================================================

        builder->setInsertionPointToStart(
            condBlock
        );

        auto index =
            builder->create<mlir::LLVM::LoadOp>(
                loc,
                types->i32,
                indexPtr
            );

        auto cond =
            builder->create<mlir::arith::CmpIOp>(
                loc,
                mlir::arith::CmpIPredicate::slt,
                index,
                size
            );

        builder->create<mlir::LLVM::CondBrOp>(
            loc,
            cond,
            bodyBlock,
            mergeBlock
        );

        // =====================================================
        // BODY
        // =====================================================

        builder->setInsertionPointToStart(
            bodyBlock
        );

        contStack.push_back(condBlock);
        breakStack.push_back(mergeBlock);

        // =====================================================
        // LOAD ITERATION VALUE
        // =====================================================

        if (iterableTy->isMatrix()) {

            // =========================================
            // MATRIX ITERATION
            // =========================================
            //
            // for row in matrix
            //
            // row is an array
            //
            // matrix is flattened:
            //
            // [1,2,3,4,5,6]
            //
            // rowStart = rowIndex * cols
            //
            // =========================================

            auto cols =
                iterableTy->dimensions().second;

            auto rowTy =
                std::static_pointer_cast<ArrayType>(
                    iterSym->type
                );

            auto elemTy =
                types->getMLIRType(
                    rowTy->elem
                );

            auto rowStart =
                builder->create<mlir::arith::MulIOp>(
                    loc,
                    index,
                    i32(cols)
                );

            auto rowPtr =
                builder->create<mlir::LLVM::GEPOp>(
                    loc,
                    ptrTy,
                    elemTy,
                    iterable,
                    mlir::ValueRange{rowStart}
                );

            exps->copyArray(
                valuePtr,
                rowPtr,
                rowTy
            );

        } else {

            // =========================================
            // NORMAL ARRAY ITERATION
            // =========================================

            auto valueTy =
                types->getMLIRType(
                    iterSym->type
                );

            auto elemPtr =
                builder->create<mlir::LLVM::GEPOp>(
                    loc,
                    ptrTy,
                    valueTy,
                    iterable,
                    mlir::ValueRange{index}
                );

            auto loaded =
                builder->create<mlir::LLVM::LoadOp>(
                    loc,
                    valueTy,
                    elemPtr
                );

            builder->create<mlir::LLVM::StoreOp>(
                loc,
                loaded,
                valuePtr
            );
        }

        // =====================================================
        // INCREMENT INDEX FOR NEXT ITERATION
        // =====================================================

        auto current =
            builder->create<mlir::LLVM::LoadOp>(
                loc,
                types->i32,
                indexPtr
            );

        auto next =
            builder->create<mlir::arith::AddIOp>(
                loc,
                current,
                one
            );

        builder->create<mlir::LLVM::StoreOp>(
            loc,
            next,
            indexPtr
        );

        // =====================================================
        // LOOP BODY
        // =====================================================

        visitBlock(stmt->body);


        breakStack.pop_back();
        contStack.pop_back();

        branchIfNeeded(condBlock);

        // =====================================================
        // MERGE
        // =====================================================

        builder->setInsertionPointToStart(
            mergeBlock
        );

        currentScope = oldScope;
    }

    void IRGen::visitIf(
        std::shared_ptr<IfStmt> stmt
    ) {

        auto *entry =
            builder->getInsertionBlock();

        auto *func =
            entry->getParentOp();

        auto *region =
            &func->getRegion(0);

        auto *thenBlock =
            builder->createBlock(region);

        auto *elseBlock =
            builder->createBlock(region);

        auto *mergeBlock =
            builder->createBlock(region);

        struct ElseIf {
            mlir::Block *cond;
            mlir::Block *body;
        };

        std::vector<ElseIf> elifs;

        for (size_t i = 0;
            i < stmt->elifx_blocks.size();
            ++i) {

            elifs.push_back({
                builder->createBlock(region),
                builder->createBlock(region)
            });
        }

        // =====================================================
        // INITIAL CONDITION
        // =====================================================

        builder->setInsertionPointToEnd(
            entry
        );

        auto cond =
            visitExp(stmt->condition);

        auto *falseTarget =
            elifs.empty()
                ? elseBlock
                : elifs.front().cond;

        builder->create<mlir::LLVM::CondBrOp>(
            loc,
            cond,
            thenBlock,
            falseTarget
        );

        // =====================================================
        // THEN
        // =====================================================

        builder->setInsertionPointToStart(
            thenBlock
        );

        contStack.push_back(mergeBlock);

        visitBlock(stmt->then_block);

        contStack.pop_back();

        branchIfNeeded(mergeBlock);

        // =====================================================
        // ELSE-IF CHAIN
        // =====================================================

        auto nextFalse =
            [&](size_t i) -> mlir::Block * {

            if (i + 1 < elifs.size())
                return elifs[i + 1].cond;

            return stmt->else_block
                ? elseBlock
                : mergeBlock;
        };

        for (size_t i = 0;
            i < elifs.size();
            ++i) {

            auto &elif = elifs[i];

            // -------------------------------------------------
            // CONDITION
            // -------------------------------------------------

            builder->setInsertionPointToStart(
                elif.cond
            );

            auto elifCond =
                visitExp(
                    stmt->elifx_blocks[i].first
                );

            builder->create<mlir::LLVM::CondBrOp>(
                loc,
                elifCond,
                elif.body,
                nextFalse(i)
            );

            // -------------------------------------------------
            // BODY
            // -------------------------------------------------

            builder->setInsertionPointToStart(
                elif.body
            );

            contStack.push_back(mergeBlock);

            visitBlock(
                stmt->elifx_blocks[i].second
            );

            contStack.pop_back();

            branchIfNeeded(mergeBlock);
        }

        // =====================================================
        // ELSE
        // =====================================================

        builder->setInsertionPointToStart(
            elseBlock
        );

        if (stmt->else_block) {

            contStack.push_back(mergeBlock);

            visitBlock(stmt->else_block);

            contStack.pop_back();
        }

        branchIfNeeded(mergeBlock);

        // =====================================================
        // MERGE
        // =====================================================

        builder->setInsertionPointToStart(
            mergeBlock
        );
    }


    // =====================================================
    // BLOCK
    // =====================================================

    void IRGen::visitBlock(
        std::shared_ptr<BlockExpr> block
    ) {

        auto oldScope = currentScope;

        currentScope =
            currentScope->push();

        for (auto &stmt : block->statements) {

            auto *cur =
                builder->getInsertionBlock();

            if (!cur ||
                blockHasTerminator(cur)) {
                return;
            }

            visitStmt(stmt);
        }

        branchIfNeeded(currentCont());

        currentScope = oldScope;
    }


    // =====================================================
    // BREAK
    // =====================================================

    void IRGen::visitBreak(
        std::shared_ptr<BreakStmt> stmt
    ) {

        auto *block =
            builder->getInsertionBlock();

        if (blockHasTerminator(block))
            return;

        builder->create<mlir::LLVM::BrOp>(
            loc,
            currentBreak()
        );
    }
}