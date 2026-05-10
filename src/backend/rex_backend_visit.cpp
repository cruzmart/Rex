
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
/// IRGen
/// =============================================================

IRGen::IRGen(
    std::shared_ptr<mlir::OpBuilder> b,
    mlir::ModuleOp &m,
    mlir::Location l,
    std::shared_ptr<TypesHelper> t
)
    : builder(std::move(b)),
      module(m),
      loc(l),
      types(std::move(t)) {}

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

/// =============================================================
/// ROOT VISIT
/// =============================================================

/// Entry point: visits entire file AST.
/// Initializes global scope and processes top-level statements.
void IRGen::visit(std::shared_ptr<FileAst> file) {

    currentScope = std::make_shared<Scope>();

    for (auto &item : file->items) {

        auto *block = builder->getInsertionBlock();

        if (!block || isTerminated(block))
            return;

        if (item->ast_kind == AstNodeKind::Stmt) {
            visitStmt(cast<Stmt>(item));
        }
    }
}


/// =============================================================
/// EXPRESSIONS
/// =============================================================

/// =============================================================
/// Expressions
/// =============================================================

mlir::Value IRGen::visitExp(
    std::shared_ptr<Expr> expr
) {
    switch (expr->exp_kind) {

        case ExprKind::Literal:
            return visitLiteral(
                cast<LiteralExpr>(expr)
            );

        case ExprKind::Binary:
            return visitBinary(
                cast<BinaryExpr>(expr)
            );

        case ExprKind::Array:
            return visitArray(
                cast<ArrayExpr>(expr)
            );

        case ExprKind::Index:
            return visitIndex(
                cast<IndexExpr>(expr)
            );

        case ExprKind::TupleIndex:
            return visitIndexTuple(
                cast<IndexTupleExpr>(expr)
            );

        case ExprKind::Tuple:
            return visitTuple(
                cast<TupleExpr>(expr)
            );

        case ExprKind::Id:
            return visitId(
                cast<IdExpr>(expr)
            );

        default:
            return {};
    }
}

/// Literal → delegate to ExpressionsHelper
mlir::Value
IRGen::visitLiteral(
    std::shared_ptr<LiteralExpr> literal
) {
    return exps->createPrimitiveLiteral(literal);
}


/// Binary expression handling.
/// Includes:
///  - constant string folding
///  - primitive arithmetic
///  - automatic index loads
///  - (future) array operations
mlir::Value IRGen::visitBinary(
    std::shared_ptr<BinaryExpr> expr
) {

    auto op = expr->operation;

    // =========================================================
    // Constant string folding
    // =========================================================

    if (op == BinaryOp::ADD &&
        exps->isConstStringExpr(expr)) {

        return exps->createString(
            exps->foldConstString(expr)
        );
    }

    auto lhs = loadIfPointer(
        visitExp(expr->lhs),
        expr->lhs->type
    );

    auto rhs = loadIfPointer(
        visitExp(expr->rhs),
        expr->rhs->type
    );

    switch (expr->type->kind) {

        case TypeKind::Primitive: {

            auto prim =
                cast<PrimType>(expr->type);

            if (prim->prim ==
                PrimType::Prims::String) {

                llvm::report_fatal_error(
                    "Runtime string concat "
                    "not implemented"
                );
            }

            return exps->createBinaryExp(
                lhs,
                rhs,
                prim->prim,
                op
            );
        }

        case TypeKind::Array:
            return {};

        default:
            return {};
    }
}


/// =============================================================
/// ARRAY + TUPLE EXPRESSIONS
/// =============================================================

/// Handles array construction.
/// Distinguishes:
///  - compile-time constant arrays → global
///  - runtime arrays → stack allocation
mlir::Value IRGen::visitArray(
    std::shared_ptr<ArrayExpr> arr
) {

    auto arrTy = cast<ArrayType>(arr->type);

    std::vector<mlir::Value> values;

    auto flatten =
        [&](std::shared_ptr<ArrayExpr> a) {

        for (auto &elem : a->elements) {

            if (arrTy->isMatrix()) {

                auto row =
                    cast<ArrayExpr>(elem);

                for (auto &v : row->elements) {
                    values.push_back(
                        visitExp(v)
                    );
                }
            }
            else {
                values.push_back(
                    visitExp(elem)
                );
            }
        }
    };

    flatten(arr);

    arrTy->arrayKind =
        ArrayStorageKind::RuntimeAlloc;

    return exps->createRuntimeArray(
        values,
        arrTy->isMatrix()
            ? arrTy->matrixType()
            : arrTy->arrayType()
    );
}

/// Tuple creation → delegates to ExpressionsHelper
mlir::Value IRGen::visitTuple(
    std::shared_ptr<TupleExpr> tup
) {

    auto tupleTy =
        cast<TupleType>(tup->type);

    std::vector<mlir::Type> mlirTypes;
    std::vector<mlir::Value> values;

    for (auto &type : tupleTy->elements) {
        mlirTypes.push_back(
            types->getMLIRType(type)
        );
    }

    for (auto &elem : tup->elements) {
        values.push_back(
            visitExp(elem)
        );
    }

    return exps->createTuple(
        mlirTypes,
        values
    );
}

/// Array indexing:
///  - evaluate base pointer
///  - compute index
///  - either:
///      * load scalar element
///      * return row pointer for matrix row access
mlir::Value IRGen::visitIndex(std::shared_ptr<IndexExpr> i) {

    mlir::Value arrPtr = visitExp(i->base);
    mlir::Value index  = visitExp(i->index);
    auto arrTy = std::static_pointer_cast<ArrayType>(i->base->type);
    
    // =====================================================
    // MATRIX CASE
    // =====================================================
    //
    // matrix[row]
    //
    // We DO NOT load.
    // We return pointer to start of row.
    //
    // Since matrix is flattened:
    //
    // linear_index = row * cols
    //
    // Then GEP to that offset.
    //
    // Example:
    //
    // [[1,2,3],
    //  [4,5,6]]
    //
    // stored as:
    //
    // [1,2,3,4,5,6]
    //
    // matrix[1]
    // -> pointer to element 3
    // -> [4,5,6]
    //
    // =====================================================

    if (arrTy->isMatrix())
        return exps->matrixRowPtr(arrPtr, index, arrTy);

    // =====================================================
    // NORMAL ARRAY CASE
    // =====================================================
  
    return exps->arrayElementPtr(arrPtr, index, arrTy);

}

/// Tuple indexing:
/// Uses struct layout + GEP
mlir::Value IRGen::visitIndexTuple(
    std::shared_ptr<IndexTupleExpr> idx
) {

    auto tupPtr = visitExp(idx->base);
    auto field  = visitExp(idx->field);

    if (idx->base->exp_kind !=
        ExprKind::Tuple) {
        return {};
    }

    auto tupleTy =
        cast<TupleType>(idx->base->type);

    std::vector<mlir::Type> fields;

    for (auto &type : tupleTy->elements) {
        fields.push_back(
            types->getMLIRType(type)
        );
    }

    auto structTy =
        mlir::LLVM::LLVMStructType::getLiteral(
            builder->getContext(),
            fields
        );

    return exps->index(
        tupPtr,
        structTy,
        fields[idx->field_index],
        field
    );
}



/// Variable access:
///  - arrays → return pointer
///  - scalars → load value
mlir::Value IRGen::visitId(
    std::shared_ptr<IdExpr> id
) {

    auto sym =
        currentScope->resolve(id->name);

    if (sym->kind != SymbolType::Variable)
        llvm_unreachable("Not variable");

    auto var =
        cast<VariableSymbol>(sym);

    if (!var->ptr)
        llvm_unreachable("Missing storage");

    if (sym->type->kind ==
        TypeKind::Array) {
        return var->ptr;
    }

    return builder->create<mlir::LLVM::LoadOp>(
        loc,
        types->getMLIRType(sym->type),
        var->ptr
    );
}


/// =============================================================
/// STATEMENTS
/// =============================================================

void IRGen::visitStmt(
    std::shared_ptr<Stmt> stmt
) {

    switch (stmt->stmt_kind) {

        case StmtKind::Print:
            return visitPrint(
                cast<PrintStmt>(stmt)
            );

        case StmtKind::If:
            return visitIf(
                cast<IfStmt>(stmt)
            );

        case StmtKind::While:
            return visitWhile(
                cast<WhileStmt>(stmt)
            );

        case StmtKind::Loop:
            return visitLoop(
                cast<LoopStmt>(stmt)
            );

        case StmtKind::For:
            return visitFor(
                cast<ForStmt>(stmt)
            );

        case StmtKind::Break:
            return visitBreak(
                cast<BreakStmt>(stmt)
            );

        case StmtKind::LetDecl:
            return visitDelc(
                cast<LetStmt>(stmt)
            );

        case StmtKind::Assign:
            return visitAssign(
                cast<AssignStmt>(stmt)
            );

        default:
            return;
    }
}


/// =============================================================
/// VARIABLE STORAGE
/// =============================================================
// Have to visit this back lowkey.
/// Allocates stack storage for a variable (LLVM alloca)
mlir::Value IRGen::allocateStorage(
    std::shared_ptr<Type> type,
    mlir::Type ptrTy,
    mlir::Value size
) {
    return builder->create<mlir::LLVM::AllocaOp>(
        loc,
        ptrTy,
        types->getMLIRType(type),
        size
    );
}


/// Initializes storage:
///  - arrays → deep copy
///  - scalars → direct store
void IRGen::initializeStorage(
    mlir::Value dst,
    mlir::Value src,
    std::shared_ptr<Type> type
) {

    if (type->kind != TypeKind::Array) {

        builder->create<mlir::LLVM::StoreOp>(
            loc,
            src,
            dst
        );

        return;
    }

    auto arr = cast<ArrayType>(type);

    exps->copyArray(
        dst,
        src,
        arr
    );
}
// =====================================================
// VARIABLE DECLERATION 
// =====================================================
// This needs to be fixed and updated.

void IRGen::visitDelc(
    std::shared_ptr<LetStmt> stmt
) {

    auto ptrTy =
        mlir::LLVM::LLVMPointerType::get(
            builder->getContext()
        );

    auto one = i32(1);

    // =========================================================
    // Single binding
    // =========================================================

    if (stmt->id_pattern->pat_type ==
        PatternType::Single) {

        auto id =
            cast<PatternId>(
                stmt->id_pattern
            );

        auto value =
            visitExp(stmt->exp);

        auto storage =
            allocateStorage(
                stmt->type,
                ptrTy,
                one
            );

        initializeStorage(
            storage,
            value,
            stmt->type
        );

        currentScope->define(
            std::make_shared<VariableSymbol>(
                id->id,
                stmt->type,
                storage
            )
        );

        return;
    }

    // =========================================================
    // Tuple destructuring
    // =========================================================

    auto ids =
        cast<PatternIds>(
            stmt->id_pattern
        )->ids;

    auto tuple =
        cast<TupleExpr>(stmt->exp);

    auto tupleTy =
        cast<TupleType>(tuple->type);

    std::vector<mlir::Value> values;

    for (auto &elem : tuple->elements) {
        values.push_back(
            visitExp(elem)
        );
    }

    for (size_t i = 0; i < ids.size(); ++i) {

        auto storage =
            allocateStorage(
                tupleTy->elements[i],
                ptrTy,
                one
            );

        initializeStorage(
            storage,
            values[i],
            tupleTy->elements[i]
        );

        currentScope->define(
            std::make_shared<VariableSymbol>(
                ids[i],
                tupleTy->elements[i],
                storage
            )
        );
    }
}

// =====================================================
// VARIABLE REASSIGNMENT
// =====================================================

void IRGen::visitAssign(
    std::shared_ptr<AssignStmt> stmt
) {

    auto value =
        visitExp(stmt->value);

    // =========================================================
    // Variable assignment
    // =========================================================

    if (stmt->target->exp_kind ==
        ExprKind::Id) {

        auto id =
            cast<IdExpr>(stmt->target);

        auto sym =
            currentScope->resolve(
                id->name
            );

        if (!sym ||
            sym->kind !=
            SymbolType::Variable) {

            llvm_unreachable(
                "Invalid assignment target"
            );
        }

        auto var =
            cast<VariableSymbol>(sym);

        if (sym->type->kind ==
            TypeKind::Array) {

            exps->copyArray(
                var->ptr,
                value,
                cast<ArrayType>(sym->type)
            );

            return;
        }

        builder->create<mlir::LLVM::StoreOp>(
            loc,
            value,
            var->ptr
        );

        return;
    }

    // =========================================================
    // Indexed assignment
    // =========================================================

    if (stmt->target->exp_kind ==
        ExprKind::Index) {

        auto idx =
            cast<IndexExpr>(
                stmt->target
            );

        auto ptr =
            visitIndex(idx);

        if (idx->type->kind ==
            TypeKind::Array) {

            exps->copyArray(
                ptr,
                value,
                cast<ArrayType>(idx->type)
            );

            return;
        }

        builder->create<mlir::LLVM::StoreOp>(
            loc,
            value,
            ptr
        );

        return;
    }

    llvm_unreachable(
        "Unsupported assignment target"
    );
}

// =====================================================
// CONTROL FLOW HELPERS
// =====================================================


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

    auto size =
        getIterableSize(stmt->iterable);

    // =====================================================
    // INDEX STORAGE
    // =====================================================

    auto indexPtr =
        allocateStorage(
            stmt->iter_var->type,
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

    switch (stmt->iterable->exp_kind) {

        case ExprKind::Array: {

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

            break;
        }

        default:
            llvm_unreachable(
                "Unsupported iterable in for loop"
            );
    }

    // =====================================================
    // INCREMENT INDEX
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


// =====================================================
// PRINT
// =====================================================

void IRGen::visitPrint(
    std::shared_ptr<PrintStmt> stmt
) {

    // =====================================================
    // SPECIAL CASE:
    // matrix[row]
    // =====================================================

    if (stmt->argument->exp_kind ==
        ExprKind::Index) {

        auto idx =
            cast<IndexExpr>(
                stmt->argument
            );

        prints->printIndexed(
            visitExp(stmt->argument),
            idx
        );

        return;
    }

    // =====================================================
    // EVALUATE
    // =====================================================

    auto value =
        visitExp(stmt->argument);

    auto type =
        stmt->argument->type;

    // =====================================================
    // PRINT BY TYPE
    // =====================================================

    switch (type->kind) {

        case TypeKind::Primitive:

            prints->printInline(value);
            break;

        case TypeKind::Array:

            prints->printArray(
                value,
                cast<ArrayType>(type)
            );

            break;

        case TypeKind::Tuple: {

            auto tupleTy =
                cast<TupleType>(type);

            prints->printTuple(
                value,
                types->createStruct(
                    tupleTy->elements
                ),
                tupleTy->elements
            );

            break;
        }

        default:
            llvm::report_fatal_error(
                "Unsupported type in print"
            );
    }

    // =====================================================
    // NEWLINE
    // =====================================================

    auto nl =
        builder->create<
            mlir::arith::ConstantIntOp
        >(
            loc,
            '\n',
            8
        );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        prints->printf_func,
        mlir::ValueRange{
            prints->getFmtAddress(
                prints->fmt_char
            ),
            nl
        }
    );
}

}