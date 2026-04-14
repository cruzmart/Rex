
#include "backend/rex_backend_visit.h"
#include "rex_ast.h"
#include "rex_ast_nodes.h"
#include "rex_exps.h"
#include "rex_stmts.h"
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

      // -----------------------------------
      // 1. Evaluate all elements FIRST
      // -----------------------------------
      std::vector<mlir::Value> values;
      values.reserve(arr->elements.size());

      for (auto &el : arr->elements) {
          values.push_back(visitExp(el));
      }

      // -----------------------------------
      // 2. Delegate creation to helper
      // -----------------------------------
      return exps->createArray(values, arr->type);
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