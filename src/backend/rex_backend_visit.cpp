
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
/// -------------------------------------------------------------
/// Walks the AST and generates MLIR (LLVM dialect).
///
/// Responsibilities:
///  - Traverse AST nodes (expressions + statements)
///  - Manage scopes and symbol resolution
///  - Emit control flow (loops, ifs, etc.)
///  - Delegate expression lowering to ExpressionsHelper
/// =============================================================

IRGen::IRGen(std::shared_ptr<mlir::OpBuilder> b,
             mlir::ModuleOp &m,
             mlir::Location loc,
             std::shared_ptr<TypesHelper> t)
    : builder(std::move(b)), module(m), loc(loc), types(t) {}


/// =============================================================
/// ROOT VISIT
/// =============================================================

/// Entry point: visits entire file AST.
/// Initializes global scope and processes top-level statements.
void IRGen::visit(std::shared_ptr<FileAst> file){

    // Create global scope
    currentScope = std::make_shared<Scope>();

    for(auto item : file->items){

        auto *b = builder->getInsertionBlock();

        // Stop if current block is already terminated
        if (!b || blockHasTerminator(b)) {
            return;
        }

        // Only statements are handled at top level
        if(item->ast_kind == AstNodeKind::Stmt){
            auto stmt = std::static_pointer_cast<Stmt>(item);
            visitStmt(stmt);
        }
    } 
}


/// =============================================================
/// EXPRESSIONS
/// =============================================================

/// Dispatch expression visitor based on expression kind.
mlir::Value IRGen::visitExp(std::shared_ptr<Expr> expr){

    auto expr_t = expr->exp_kind;

    if(expr_t == ExprKind::Literal)
        return visitLiteral(std::static_pointer_cast<LiteralExpr>(expr));

    if(expr_t == ExprKind::Binary)
        return visitBinary(std::static_pointer_cast<BinaryExpr>(expr));

    if(expr_t == ExprKind::Array)
        return visitArray(std::static_pointer_cast<ArrayExpr>(expr));

    if(expr_t == ExprKind::Index)
        return visitIndex(std::static_pointer_cast<IndexExpr>(expr));

    if(expr_t == ExprKind::TupleIndex)
        return visitIndexTuple(std::static_pointer_cast<IndexTupleExpr>(expr));

    if(expr_t == ExprKind::Tuple)
        return visitTuple(std::static_pointer_cast<TupleExpr>(expr));

    if(expr_t == ExprKind::Id)
        return visitId(std::static_pointer_cast<IdExpr>(expr));
    
    return mlir::Value();
}

/// Literal → delegate to ExpressionsHelper
mlir::Value IRGen::visitLiteral(std::shared_ptr<LiteralExpr> l){
    return exps->createPrimitiveLiteral(l);
}

/// Binary expression handling.
/// Includes:
///  - constant string folding
///  - primitive arithmetic
///  - (future) array operations
mlir::Value IRGen::visitBinary(std::shared_ptr<BinaryExpr> bi){

    BinaryOp op = bi->operation;

    // -----------------------------------
    // CONSTANT STRING FOLDING
    // -----------------------------------
    if (op == BinaryOp::ADD && exps->isConstStringExpr(bi)) {
        std::string folded = exps->foldConstString(bi);
        return exps->createString(folded);
    }

    // Evaluate operands
    mlir::Value lhs = visitExp(bi->lhs);
    mlir::Value rhs = visitExp(bi->rhs);

    TypeKind exp_t = bi->type->kind;

    switch(exp_t){

        // =========================
        // PRIMITIVE TYPES
        // =========================
        case TypeKind::Primitive:{
            auto primt_t = std::static_pointer_cast<PrimType>(bi->type);

            switch(primt_t->prim){

                case PrimType::Prims::String:
                    // Runtime string concat not implemented
                    llvm::report_fatal_error("Runtime string concatenation not implemented yet");

                default:
                    // Delegate arithmetic/comparison
                    return exps->createBinaryExp(lhs, rhs, primt_t->prim, op);
            }
        }

        // =========================
        // ARRAY TYPES (NOT IMPLEMENTED)
        // =========================
        case TypeKind::Array:{
            // Placeholder for future array operations
            break;
        }

        default:
            break;
    }

    return mlir::Value();
}


/// =============================================================
/// ARRAY + TUPLE EXPRESSIONS
/// =============================================================

/// Handles array construction.
/// Distinguishes:
///  - compile-time constant arrays → global
///  - runtime arrays → stack allocation
mlir::Value IRGen::visitArray(std::shared_ptr<ArrayExpr> arr) {

    auto arrTy = std::static_pointer_cast<ArrayType>(arr->type);
    auto prim = std::static_pointer_cast<PrimType>(arrTy->elem);

    std::vector<mlir::Value> elements;
    elements.reserve(arr->elements.size());

    bool allConst = true;

    // Evaluate elements
    for (auto &e : arr->elements) {

        mlir::Value v = visitExp(e);
        elements.push_back(v);

        // Check if element is constant
        if (!v.getDefiningOp<mlir::arith::ConstantOp>()) {
            allConst = false;
        }
    }

    // Compile-time array
    if (allConst) {
        arrTy->arrayKind = ArrayStorageKind::GlobalConst;
        return exps->createConstArray(elements, prim->prim);
    }

    // Runtime array
    arrTy->arrayKind = ArrayStorageKind::RuntimeAlloc;
    return exps->createRuntimeArray(elements, prim->prim);
}

/// Tuple creation → delegates to ExpressionsHelper
mlir::Value IRGen::visitTuple(std::shared_ptr<TupleExpr> tup){

    std::vector<mlir::Type> types;
    for(auto type : std::static_pointer_cast<TupleType>(tup->type)->elements){
        types.push_back(prints->types->getMLIRType(type));
    }

    std::vector<mlir::Value> values;
    for(auto value : tup->elements){
        values.push_back(visitExp(value));
    }

    return exps->createTuple(types, values);
}

/// Array indexing:
///  - evaluate base pointer
///  - compute index
///  - load element
mlir::Value IRGen::visitIndex(std::shared_ptr<IndexExpr> i) {

    mlir::Value arr_p = visitExp(i->base);
    mlir::Value index = visitExp(i->index);

    auto arrTy = std::static_pointer_cast<ArrayType>(i->base->type);
    mlir::Type elemTy = prints->types->getMLIRType(arrTy->elem);

    return exps->index(arr_p, index, elemTy);
}

/// Variable access:
///  - arrays → return pointer
///  - scalars → load value
mlir::Value IRGen::visitId(std::shared_ptr<IdExpr> id) {

    auto sym = currentScope->resolve(id->name);

    if (sym->kind != SymbolType::Variable)
        llvm_unreachable("Not a variable");

    auto var = std::static_pointer_cast<VariableSymbol>(sym);

    if (!var->ptr)
        llvm_unreachable("No storage");

    auto ty = sym->type;

    // Arrays are already pointers
    if (ty->kind == TypeKind::Array) {
        return var->ptr;
    }

    // Scalars → load
    return builder->create<mlir::LLVM::LoadOp>(
        loc,
        types->getMLIRType(ty),
        var->ptr
    );
}

/// Tuple indexing:
/// Uses struct layout + GEP
mlir::Value IRGen::visitIndexTuple(std::shared_ptr<IndexTupleExpr> it){

    mlir::Value tup_ptr = visitExp(it->base);
    mlir::Value field_val = visitExp(it->field);

    std::vector<mlir::Type> typs;

    if(it->base->exp_kind == ExprKind::Tuple){

        for(auto type : std::static_pointer_cast<TupleType>(it->base->type)->elements){
            typs.push_back(prints->types->getMLIRType(type));
        }

        auto struc_t = mlir::LLVM::LLVMStructType::getLiteral(
            builder->getContext(), typs);

        return exps->index(tup_ptr, struc_t, typs[it->field_index], field_val);
    }
    
    return mlir::Value();
}


/// =============================================================
/// STATEMENTS
/// =============================================================

/// Dispatch statement visitor
void IRGen::visitStmt(std::shared_ptr<Stmt> stmt){

    auto stmt_t = stmt->stmt_kind;

    if(stmt_t == StmtKind::Print)
        visitPrint(std::static_pointer_cast<PrintStmt>(stmt));

    if(stmt_t == StmtKind::If)
        visitIf(std::static_pointer_cast<IfStmt>(stmt));

    if(stmt_t == StmtKind::While)
        visitWhile(std::static_pointer_cast<WhileStmt>(stmt));

    if(stmt_t == StmtKind::Loop)
        visitLoop(std::static_pointer_cast<LoopStmt>(stmt));

    if(stmt_t == StmtKind::For)
        visitFor(std::static_pointer_cast<ForStmt>(stmt));

    if(stmt_t == StmtKind::Break)
        visitBreak(std::static_pointer_cast<BreakStmt>(stmt));

    if(stmt_t == StmtKind::LetDecl)
        visitDelc(std::static_pointer_cast<LetStmt>(stmt));

    if(stmt_t == StmtKind::Assign)
        visitAssign(std::static_pointer_cast<AssignStmt>(stmt));
}


/// =============================================================
/// VARIABLE STORAGE
/// =============================================================

/// Allocates stack storage for a variable (LLVM alloca)
mlir::Value IRGen::allocateStorage(std::shared_ptr<Type> type,
                                   mlir::Type ptr_t,
                                   mlir::Value one) {

    auto mlirTy = types->getMLIRType(type);

    return builder->create<mlir::LLVM::AllocaOp>(
        loc,
        ptr_t,
        mlirTy,
        one
    );
}

/// Initializes storage:
///  - arrays → deep copy
///  - scalars → direct store
void IRGen::initializeStorage(mlir::Value dst,
                              mlir::Value src,
                              std::shared_ptr<Type> type) {

    auto ptr_t = mlir::LLVM::LLVMPointerType::get(builder->getContext());

    // ARRAY COPY (element-wise)
    if (type->kind == TypeKind::Array) {

        auto arr = std::static_pointer_cast<ArrayType>(type);

        auto elemTy = types->getMLIRType(arr->elem);
        auto arrayTy = mlir::LLVM::LLVMArrayType::get(elemTy, arr->size);

        auto zero = builder->create<mlir::arith::ConstantOp>(
            loc, builder->getI32IntegerAttr(0));

        for (int i = 0; i < arr->size; i++) {

            auto idx = builder->create<mlir::arith::ConstantOp>(
                loc, builder->getI32IntegerAttr(i));

            // GEP src/dst
            auto srcPtr = builder->create<mlir::LLVM::GEPOp>(
                loc, ptr_t, arrayTy, src,
                mlir::ValueRange{zero, idx});

            auto dstPtr = builder->create<mlir::LLVM::GEPOp>(
                loc, ptr_t, arrayTy, dst,
                mlir::ValueRange{zero, idx});

            // Load + store
            auto val = builder->create<mlir::LLVM::LoadOp>(
                loc, elemTy, srcPtr);

            builder->create<mlir::LLVM::StoreOp>(loc, val, dstPtr);
        }

        return;
    }

    // Scalar store
    builder->create<mlir::LLVM::StoreOp>(loc, src, dst);
}
// =====================================================
// VARIABLE DECLERATION 
// =====================================================
void IRGen::visitDelc(std::shared_ptr<LetStmt> var) {
    auto ctx = builder->getContext();

    auto ptr_t = mlir::LLVM::LLVMPointerType::get(ctx);

    auto one = builder->create<mlir::arith::ConstantOp>(
        loc, builder->getI32IntegerAttr(1));

    // =====================================================
    // SINGLE BINDING
    // =====================================================
    if (var->id_pattern->pat_type == PatternType::Single) {

        auto id = std::static_pointer_cast<PatternId>(var->id_pattern);
        auto value = visitExp(var->exp);

        auto storage = allocateStorage(var->type, ptr_t, one);

        initializeStorage(storage, value, var->type);

        currentScope->define(
            std::make_shared<VariableSymbol>(id->id, var->type, storage)
        );
    }

    // =====================================================
    // MULTIPLE (TUPLES)
    // =====================================================
    else if (var->id_pattern->pat_type == PatternType::Multiple) {

        auto ids = std::static_pointer_cast<PatternIds>(var->id_pattern)->ids;

        auto tuple = std::static_pointer_cast<TupleExpr>(var->exp);
        auto tupleTy = std::static_pointer_cast<TupleType>(tuple->type);

        std::vector<mlir::Value> values;
        for (auto &e : tuple->elements)
            values.push_back(visitExp(e));

        for (size_t i = 0; i < ids.size(); i++) {

            auto storage = allocateStorage(
                tupleTy->elements[i],
                ptr_t,
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
}
// =====================================================
// VARIABLE REASSINGMENT
// =====================================================
void IRGen:: visitAssign(std::shared_ptr<AssignStmt> var){
auto ptr_t = mlir::LLVM::LLVMPointerType::get(builder->getContext());
   auto zero = builder->create<mlir::arith::ConstantOp>(
        loc, builder->getI32IntegerAttr(0));

    // if ID exp
    if(var->target->exp_kind == ExprKind::Id){
        auto id = std::static_pointer_cast<IdExpr>(var->target);
        auto symbol = currentScope->resolve(id->name);
        if(!(symbol->kind == SymbolType::Variable))
             llvm_unreachable("Variable ID is undefined or unknown");
        auto symbol_variable = std::static_pointer_cast<VariableSymbol>(symbol);
        builder->create<mlir::LLVM::StoreOp>(loc, visitExp(var->value) , symbol_variable->ptr);

    }

    // if it is a index base[index]

    if(var->target->exp_kind == ExprKind::Index){
        // array[value] = exp
        auto index = std::static_pointer_cast<IndexExpr>(var->target);

        // if the base is a id (i.e the base is a array)
        if(index->base->exp_kind == ExprKind::Id){
            
            auto arr_id = std::static_pointer_cast<IdExpr>(index->base);
        

            if(arr_id->type->kind == TypeKind::Array){

                auto arr_t = std::static_pointer_cast<ArrayType>(arr_id->type);
                auto arr_elem = types->getMLIRType(arr_t->elem);
                auto base = visitExp(index->base);
                auto value = visitExp(var->value);

                auto idx = visitExp(index->index);

                auto arrayTy = mlir::LLVM::LLVMArrayType::get(arr_elem, arr_t->size);


                auto elemPtr = builder->create<mlir::LLVM::GEPOp>(
                    loc,
                    ptr_t,
                    arrayTy,
                    base,
                    mlir::ValueRange{zero, idx}   // ✅ FIX
                );


                 builder->create<mlir::LLVM::StoreOp>(loc, value, elemPtr);

            }
        }
        
    }

    // the rest will be impliemented later
    return;
}

// =====================================================
// CONTROL FLOW HELPERS
// =====================================================

bool IRGen::blockHasTerminator(mlir::Block *block) {
    return !block->empty() &&
           block->back().hasTrait<mlir::OpTrait::IsTerminator>();
}

mlir::Value IRGen::getIterableSize(std::shared_ptr<Expr> exp){
    // if exp is a array 
    mlir::Value size;

    if(exp->exp_kind == ExprKind::Array){
        auto arr_t = std::static_pointer_cast<ArrayType>(exp->type);
        size = builder->create<mlir::arith::ConstantOp>(loc, builder->getI32IntegerAttr(arr_t->size));
        return size;
    }

    // if exp is a range
    if(exp->exp_kind == ExprKind::Range){
         auto rng_exp = std::static_pointer_cast<RangeExpr>(exp);
         // size = (b - a) + 1;
         auto size_sub = exps->sub(visitExp(rng_exp->rhs), visitExp(rng_exp->lhs), types->i32);
         size = exps->add(size_sub, builder->create<mlir::arith::ConstantOp>(loc, builder->getI32IntegerAttr(1)), types->i32);
         return size;

    }

    return mlir::Value();

}

// =====================================================
// CONTROL FLOW IMPLEMENTATIONS
// =====================================================
void IRGen::visitLoop(std::shared_ptr<LoopStmt> lop_stmt){
    auto *curBlock = builder->getInsertionBlock();
    auto *func = curBlock->getParentOp();
    auto *region = &func->getRegion(0);

    auto *condBlock  = builder->createBlock(region);
    auto *bodyBlock  = builder->createBlock(region);
    auto *mergeBlock = builder->createBlock(region);

      // jump to condition
    builder->setInsertionPointToEnd(curBlock);
    builder->create<mlir::LLVM::BrOp>(loc, condBlock);

    // CONDITION
    builder->setInsertionPointToStart(condBlock);
    
    builder->create<mlir::LLVM::CondBrOp>(
        loc, exps->createBool("true"), bodyBlock, mergeBlock
    );


    // BODY
    builder->setInsertionPointToStart(bodyBlock);

    // 🔥 continue target
    contStack.push_back(condBlock);

    // 🔥 break target
    breakStack.push_back(mergeBlock);

    visitBlock(lop_stmt->body);

    breakStack.pop_back();
    contStack.pop_back();


    auto *b = builder->getInsertionBlock();
    if (b && !blockHasTerminator(b)) {
        builder->create<mlir::LLVM::BrOp>(loc, condBlock);
    }

    // EXIT
    builder->setInsertionPointToStart(mergeBlock);
  
}
void IRGen::visitWhile(std::shared_ptr<WhileStmt> whle_stmt){
    auto *curBlock = builder->getInsertionBlock();
    auto *func = curBlock->getParentOp();
    auto *region = &func->getRegion(0);

    auto *condBlock  = builder->createBlock(region);
    auto *bodyBlock  = builder->createBlock(region);
    auto *mergeBlock = builder->createBlock(region);

    // jump to condition
    builder->setInsertionPointToEnd(curBlock);
    builder->create<mlir::LLVM::BrOp>(loc, condBlock);

    // CONDITION
    builder->setInsertionPointToStart(condBlock);
    auto cond = visitExp(whle_stmt->cond);

    builder->create<mlir::LLVM::CondBrOp>(
        loc, cond, bodyBlock, mergeBlock
    );

    // BODY
    builder->setInsertionPointToStart(bodyBlock);

    // 🔥 continue target
    contStack.push_back(condBlock);

    // 🔥 break target
    breakStack.push_back(mergeBlock);

    visitBlock(whle_stmt->body);

    breakStack.pop_back();
    contStack.pop_back();

    auto *b = builder->getInsertionBlock();
    if (b && !blockHasTerminator(b)) {
        builder->create<mlir::LLVM::BrOp>(loc, condBlock);
    }

    // EXIT
    builder->setInsertionPointToStart(mergeBlock);

}
void IRGen::visitFor(std::shared_ptr<ForStmt> for_stmt) {

    // -------------------------
    // GET CURRENT CONTEXT
    // -------------------------
    auto *entryBlock = builder->getInsertionBlock();
    auto *func = entryBlock->getParentOp();
    auto *region = &func->getRegion(0);

    auto ptr_t = mlir::LLVM::LLVMPointerType::get(builder->getContext());

    // constants
    auto one = builder->create<mlir::arith::ConstantOp>(
        loc, builder->getI32IntegerAttr(1));

    auto zero = builder->create<mlir::arith::ConstantOp>(
        loc, builder->getI32IntegerAttr(0));

    // -------------------------
    // CREATE BLOCKS
    // -------------------------
    auto *condBlock  = builder->createBlock(region);
    auto *bodyBlock  = builder->createBlock(region);
    //auto *incBlock   = builder->createBlock(region);   // 🔥 REQUIRED
    auto *mergeBlock = builder->createBlock(region);

    builder->setInsertionPointToEnd(entryBlock);

    // -------------------------
    // NEW SCOPE
    // -------------------------
    auto oldScope = currentScope;
    currentScope = currentScope->push();

    // -------------------------
    // EVALUATE ITERABLE (ONCE)
    // -------------------------
    auto iterable_ptr = visitExp(for_stmt->iterable);
    auto size = getIterableSize(for_stmt->iterable);

    // -------------------------
    // INDEX (i)
    // -------------------------
    auto index_ptr = builder->create<mlir::LLVM::AllocaOp>(
        loc, ptr_t, builder->getI32Type(), one);

    builder->create<mlir::LLVM::StoreOp>(loc, zero, index_ptr);

    // -------------------------
    // LOOP VARIABLE (user variable)
    // -------------------------
    auto iterSym =
        std::static_pointer_cast<IdExpr>(for_stmt->iter_var)->resolved;
    auto iterSymVar = std::static_pointer_cast<VariableSymbol>(iterSym);

    mlir::Type val_t = types->getMLIRType(iterSym->type);

    auto value_ptr = builder->create<mlir::LLVM::AllocaOp>(
        loc, ptr_t, val_t, one);

    iterSymVar->ptr = value_ptr;
    currentScope->define(iterSym);

    // -------------------------
    // JUMP → CONDITION
    // -------------------------
    builder->create<mlir::LLVM::BrOp>(loc, condBlock);

    // =====================================================
    // CONDITION BLOCK
    // =====================================================
    builder->setInsertionPointToStart(condBlock);

    auto index = builder->create<mlir::LLVM::LoadOp>(
        loc, types->i32, index_ptr);

    auto cond = builder->create<mlir::arith::CmpIOp>(
        loc,
        mlir::arith::CmpIPredicate::slt,
        index,
        size
    );

    builder->create<mlir::LLVM::CondBrOp>(
        loc, cond, bodyBlock, mergeBlock
    );

    // =====================================================
    // BODY BLOCK
    // =====================================================
    builder->setInsertionPointToStart(bodyBlock);

    // 🔥 IMPORTANT: continue → increment block
    contStack.push_back(condBlock);
    breakStack.push_back(mergeBlock);

    // -------------------------
    // LOAD ELEMENT → loop variable
    // -------------------------
    switch (for_stmt->iterable->exp_kind) {
        case ExprKind::Array: {
            auto gep = builder->create<mlir::LLVM::GEPOp>(
                loc,
                ptr_t,
                val_t,
                iterable_ptr,
                mlir::ValueRange{index} // ✅ correct for arrays
            );

            auto loaded = builder->create<mlir::LLVM::LoadOp>(
                loc, val_t, gep);

            builder->create<mlir::LLVM::StoreOp>(
                loc, loaded, value_ptr);
            break;
        }

        // Need to impliment case that it is a ID and Range

        default:
            llvm_unreachable("Unsupported iterable in for loop");
            break;
    }

    auto curIdx = builder->create<mlir::LLVM::LoadOp>(
        loc, types->i32, index_ptr);

    auto nextIdx = builder->create<mlir::arith::AddIOp>(
        loc, curIdx, one);

    builder->create<mlir::LLVM::StoreOp>(
        loc, nextIdx, index_ptr);

    // -------------------------
    // LOOP BODY
    // -------------------------
    visitBlock(for_stmt->body);

    breakStack.pop_back();
    contStack.pop_back();

    auto *b = builder->getInsertionBlock();
    if (b && !blockHasTerminator(b)) {
        builder->create<mlir::LLVM::BrOp>(loc, condBlock);
    }

    // =====================================================
    // MERGE BLOCK
    // =====================================================
    builder->setInsertionPointToStart(mergeBlock);

    // -------------------------
    // RESTORE SCOPE
    // -------------------------
    currentScope = oldScope;
}
void IRGen::visitIf(std::shared_ptr<IfStmt> if_stmt) {
    auto *curBlock = builder->getInsertionBlock();
    auto *func = curBlock->getParentOp();
    auto *region = &func->getRegion(0);

    auto *thenBlock  = builder->createBlock(region);
    auto *elseBlock  = builder->createBlock(region);
    auto *mergeBlock = builder->createBlock(region);

    struct ElseIfPair {
        mlir::Block *condBlock;
        mlir::Block *bodyBlock;
    };

    std::vector<ElseIfPair> chain;
    for (size_t i = 0; i < if_stmt->elifx_blocks.size(); i++) {
        chain.push_back({
            builder->createBlock(region),
            builder->createBlock(region)
        });
    }

    // -------------------------
    // CONDITION
    // -------------------------
    builder->setInsertionPointToEnd(curBlock);
    auto cond = visitExp(if_stmt->condition);

    mlir::Block *firstFalse =
        chain.empty() ? elseBlock : chain[0].condBlock;

    builder->create<mlir::LLVM::CondBrOp>(
        loc, cond, thenBlock, firstFalse
    );

    // -------------------------
    // THEN
    // -------------------------
    builder->setInsertionPointToStart(thenBlock);
    contStack.push_back(mergeBlock);
    visitBlock(if_stmt->then_block);
    contStack.pop_back();

    {
        auto *b = builder->getInsertionBlock();
        if (b && !blockHasTerminator(b)) {
            builder->create<mlir::LLVM::BrOp>(loc, mergeBlock);
        }
    }

    // -------------------------
    // ELSE-IF chain
    // -------------------------
    auto getFalseTarget = [&](size_t i) -> mlir::Block* {
        if (i + 1 < chain.size())
            return chain[i + 1].condBlock;

        return if_stmt->else_block ? elseBlock : mergeBlock;
    };

    for (size_t i = 0; i < chain.size(); i++) {
        auto condBlock = chain[i].condBlock;
        auto bodyBlock = chain[i].bodyBlock;

        // condition
        builder->setInsertionPointToStart(condBlock);
        auto cond = visitExp(if_stmt->elifx_blocks[i].first);

        builder->create<mlir::LLVM::CondBrOp>(
            loc, cond, bodyBlock, getFalseTarget(i)
        );

        // body
        builder->setInsertionPointToStart(bodyBlock);
        contStack.push_back(mergeBlock);
        visitBlock(if_stmt->elifx_blocks[i].second);
        contStack.pop_back();

        auto *b = builder->getInsertionBlock();
        if (b && !blockHasTerminator(b)) {
            builder->create<mlir::LLVM::BrOp>(loc, mergeBlock);
        }
    }

    // -------------------------
    // ELSE
    // -------------------------
    builder->setInsertionPointToStart(elseBlock);
    if (if_stmt->else_block) {
        contStack.push_back(mergeBlock);
        visitBlock(if_stmt->else_block);
        contStack.pop_back();

        auto *b = builder->getInsertionBlock();
        if (b && !blockHasTerminator(b)) {
            builder->create<mlir::LLVM::BrOp>(loc, mergeBlock);
        }
    } else {
        builder->create<mlir::LLVM::BrOp>(loc, mergeBlock);
    }

    // -------------------------
    // MERGE (NO TERMINATOR HERE)
    // -------------------------
    builder->setInsertionPointToStart(mergeBlock);
    // If nothing else gets inserted later, this block must still be valid

}
void IRGen::visitBlock(std::shared_ptr<BlockExpr> block) {
    auto oldScope = currentScope;
    currentScope = currentScope->push();
    for (auto stmt : block->statements) {

        auto *b = builder->getInsertionBlock();

        if (!b || blockHasTerminator(b)) {
            return; // 🔥 HARD STOP
        }

        visitStmt(stmt);
    }

    auto *b = builder->getInsertionBlock();

    if (b && !blockHasTerminator(b)) {
        builder->create<mlir::LLVM::BrOp>(loc, currentCont());
    }

    currentScope = oldScope;
}
void IRGen::visitBreak(std::shared_ptr<BreakStmt> brk) {
    auto *b = builder->getBlock();

    // If already terminated, do nothing
    if (blockHasTerminator(b))
        return;

    builder->create<mlir::LLVM::BrOp>(loc, currentBreak());
}
void IRGen::visitPrint(std::shared_ptr<PrintStmt> p) {
    mlir::Value val = visitExp(p->argument);

    auto type = p->argument->type;

    switch (type->kind) {

        case TypeKind::Primitive: {
            prints->printPrimtive(val);
            break;
        }

        case TypeKind::Array: {
            auto arrType = std::static_pointer_cast<ArrayType>(type);
            prints->printArray(val, arrType);
            break;
        }

        case TypeKind::Tuple: {
            auto tup_t = std::static_pointer_cast<TupleType>(type);
            auto tup_s = prints->types->createStruct(tup_t->elements);
            prints->printTuple(val, tup_s, tup_t->elements);
            break;
        }

        default:
            llvm::report_fatal_error("Unsupported type in print");
    }

    // -----------------------------------
    // print '\n'
    // -----------------------------------
    auto close = builder->create<mlir::arith::ConstantIntOp>(loc, '\n', 8);

    builder->create<mlir::LLVM::CallOp>(
        loc, prints->printf_func, mlir::ValueRange{prints->getFmtAddress(prints->fmt_char), close}
    );


}

}