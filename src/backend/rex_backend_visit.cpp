
#include "backend/rex_backend_visit.h"
#include "rex_ast.h"
#include "rex_ast_nodes.h"
#include "rex_exps.h"
#include "rex_stmts.h"
#include "rex_types.h"
#include <memory>

namespace rex {
  CodegenVisitor::CodegenVisitor( std::shared_ptr<mlir::OpBuilder> b,
                    mlir::ModuleOp & m,
                    mlir::Location l
                  ) : builder(b), module(m), loc(l) {}

      
    mlir::Value CodegenVisitor::visitExp(std::shared_ptr<Expr> expr){
      auto expr_t = expr->exp_kind;
      if(expr_t == ExprKind::Literal)
        return visitLiteral(std::static_pointer_cast<LiteralExpr>(expr));
      if(expr_t == ExprKind::Binary)
        return visitBinary(std::static_pointer_cast<BinaryExpr>(expr));
      if(expr_t == ExprKind::Array)
        return visitArray(std::static_pointer_cast<ArrayExpr>(expr));
      if(expr_t == ExprKind::Index)
        return visitIndex(std::static_pointer_cast<IndexExpr>(expr));
        
      return mlir::Value();
    }

    mlir::Value CodegenVisitor::visitLiteral(std::shared_ptr<LiteralExpr> l){
      return exps->createPrimitiveLiteral(l);
    }
    mlir::Value CodegenVisitor::visitBinary(std::shared_ptr<BinaryExpr> bi){

  
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

   mlir::Value CodegenVisitor::visitArray(std::shared_ptr<ArrayExpr> arr) {
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
    
    void CodegenVisitor::visitPrint(std::shared_ptr<PrintStmt> p) {
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

            default:
                llvm::report_fatal_error("Unsupported type in print");
        }
    }
    mlir::Value CodegenVisitor::visitIndex(std::shared_ptr<IndexExpr> i) {
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
    void CodegenVisitor::visitStmt(std::shared_ptr<Stmt> stmt){
      auto stmt_t = stmt->stmt_kind;
      if(stmt_t == StmtKind::Print)
        visitPrint(std::static_pointer_cast<PrintStmt>(stmt));
      return;
    }
    void CodegenVisitor::visit(std::shared_ptr<FileAst> file){
       for(auto item : file->items){
            if(item->ast_kind == AstNodeKind::Stmt){
               auto stmt = std::static_pointer_cast<Stmt>(item);
                visitStmt(stmt);
            }
        } 
    }

}