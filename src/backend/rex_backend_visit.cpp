
#include "backend/rex_backend_visit.h"
#include "rex_ast.h"
#include "rex_ast_nodes.h"
#include "rex_exps.h"
#include "rex_stmts.h"
#include "rex_types.h"
#include <memory>

namespace rex {
  IRGen::IRGen(std::shared_ptr<mlir::OpBuilder> b,
      mlir::ModuleOp &m,
      mlir::Location loc)
    : builder(std::move(b)), module(m), loc(loc) {}

    void IRGen::visit(std::shared_ptr<FileAst> file){
       for(auto item : file->items){
            if(item->ast_kind == AstNodeKind::Stmt){
               auto stmt = std::static_pointer_cast<Stmt>(item);
                visitStmt(stmt);
            }
        } 

    
    }
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
        
      return mlir::Value();
    }
    mlir::Value IRGen::visitLiteral(std::shared_ptr<LiteralExpr> l){
      return exps->createPrimitiveLiteral(l);
    }
    mlir::Value IRGen::visitBinary(std::shared_ptr<BinaryExpr> bi){

  
        BinaryOp op = bi->operation;

        if (op == BinaryOp::ADD && exps->isConstStringExpr(bi)) {
            std::string folded = exps->foldConstString(bi);
            return exps->createString(folded);
        }


        TypeKind exp_t = bi->type->kind;
        mlir::Value lhs = visitExp(bi->lhs);
        mlir::Value rhs = visitExp(bi->rhs);

         switch(exp_t){

            case TypeKind::Primitive:{
                auto primt_t = std::static_pointer_cast<PrimType>(bi->type);
                switch(primt_t->prim){
                    case PrimType::Prims::String:{
                        // do basic string concat here
                        llvm::report_fatal_error("Runtime string concatenation not implemented yet");
                        break;
                    }
                    default:{
                        // do basic op of the values ( uh shit)
                        return exps->createBinaryExp(lhs, rhs, primt_t->prim, op);
                        
                    }
                }
            }

            case TypeKind::Array:{
                auto array_t = std::static_pointer_cast<ArrayType>(bi->type);
                switch(array_t->elem->kind){
                    case TypeKind::Primitive:{
                        auto elem_t = std::static_pointer_cast<PrimType>(bi->type);
                        switch(elem_t->prim){
                            case PrimType::Prims::String:{
                                // we will combine two arrays of strings together
                            }
                            default:{
                                // we will add another prim type to a array of the same prim type
                            }
                        }
                    }
                    case TypeKind::Array: {
                        // double array case we will handle that in a seperate function
                        // THIS will def be done last
                    }
                    default:{break;}

                }

            }
            default:{break;}
        }

     return mlir::Value();
    }
    mlir::Value IRGen::visitArray(std::shared_ptr<ArrayExpr> arr) {
    auto arrTy = std::static_pointer_cast<ArrayType>(arr->type);
    auto prim = std::static_pointer_cast<PrimType>(arrTy->elem);

    std::vector<mlir::Value> elements;
    elements.reserve(arr->elements.size());

    bool allConst = true;

    for (auto &e : arr->elements) {
        mlir::Value v = visitExp(e);
        elements.push_back(v);

        if (!v.getDefiningOp<mlir::arith::ConstantOp>()) {
            allConst = false;
        }
    }

    // -----------------------------------
    // CASE 1: compile-time array
    // -----------------------------------
    if (allConst) {
        return exps->createConstArray(elements, prim->prim);
    }

    // -----------------------------------
    // CASE 2: runtime array
    // -----------------------------------
    return exps->createRuntimeArray(elements, prim->prim);
    }
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
    mlir::Value IRGen::visitIndex(std::shared_ptr<IndexExpr> i) {
        // 1. evaluate base (this already gives you the pointer)
        mlir::Value arr_p = visitExp(i->base);

        // 2. evaluate index
        mlir::Value index = visitExp(i->index);

        // 3. get ARRAY TYPE from AST (this is the key)
        auto arrTy = std::static_pointer_cast<ArrayType>(i->base->type);

        // 4. get element MLIR type, (yeah prints have this so I just go directly to that ig)
        mlir::Type elemTy = prints->types->getMLIRType(arrTy->elem);

        // 5. delegate to helper
        return exps->index(arr_p, index, elemTy);
    }
    mlir::Value IRGen::visitIndexTuple(std::shared_ptr<IndexTupleExpr> it){
        mlir::Value tup_ptr = visitExp(it->base);
        mlir::Value field_val = visitExp(it->field);
        std::vector<mlir::Type> typs;
        llvm::errs() << (it->base->exp_kind == ExprKind::Tuple);
        if(it->base->exp_kind == ExprKind::Tuple){
            for(auto type : std::static_pointer_cast<TupleType>(it->base->type)->elements){
                typs.push_back(prints->types->getMLIRType(type));
            }

            auto struc_t = mlir::LLVM::LLVMStructType::getLiteral(builder->getContext(), typs);
            return exps->index(tup_ptr, struc_t, typs[it->field_index], field_val);
        }
        
        return mlir::Value();
    }
    void IRGen::visitStmt(std::shared_ptr<Stmt> stmt){
      auto stmt_t = stmt->stmt_kind;
      if(stmt_t == StmtKind::Print)
        visitPrint(std::static_pointer_cast<PrintStmt>(stmt));
      if(stmt_t == StmtKind::If)
        visitIf(std::static_pointer_cast<IfStmt>(stmt));
      return;
    }
   

void IRGen::visitIf(std::shared_ptr<IfStmt> if_stmt) {
    auto *curBlock = builder->getInsertionBlock();
    auto *func = curBlock->getParentOp();
    auto *region = &func->getRegion(0);

    // save insertion point safety anchor
    auto *entryBlock = curBlock;

    auto *thenBlock = builder->createBlock(region);
    auto *elseBlock = builder->createBlock(region);
    auto *mergeBlock = builder->createBlock(region);

    // condition MUST be emitted in current valid block
    builder->setInsertionPointToEnd(entryBlock);
    mlir::Value cond = visitExp(if_stmt->condition);

    builder->create<mlir::LLVM::CondBrOp>(
        loc, cond, thenBlock, elseBlock
    );

    // THEN
    builder->setInsertionPointToStart(thenBlock);
    visitBlock(if_stmt->then_block);
    
        builder->create<mlir::LLVM::BrOp>(loc, mergeBlock);
    

    // ELSE
    builder->setInsertionPointToStart(elseBlock);
    if (if_stmt->else_block)
        visitBlock(if_stmt->else_block);

    builder->create<mlir::LLVM::BrOp>(loc, mergeBlock);
    

    // MERGE
    builder->setInsertionPointToStart(mergeBlock);
}

   void IRGen::visitBlock(std::shared_ptr<BlockExpr> block) {
    for (auto stmt : block->statements) {

        auto *b = builder->getBlock();

        if (!b->empty() &&
            b->back().hasTrait<mlir::OpTrait::IsTerminator>()) {
            return; // 🔥 HARD STOP
        }

        visitStmt(stmt);
    }
}
}