
#include "backend/rex_backend_visit.h"
#include "rex_exps.h"
#include "rex_stmts.h"
#include <memory>

namespace rex {
  CodegenVisitor::CodegenVisitor( std::shared_ptr<mlir::OpBuilder> b,
                    mlir::ModuleOp m,
                    mlir::Location l
                  ) : builder(b), module(m), loc(l) {}
    mlir::Value CodegenVisitor::visitExpr(std::shared_ptr<Expr> expr){
      auto expr_t = expr->exp_kind;
      if(expr_t == ExprKind::Literal)
        visitLiteral(std::static_pointer_cast<LiteralExpr>(expr));
    }
    mlir::Value CodegenVisitor::visitLiteral(std::shared_ptr<LiteralExpr> l){
      return exps->createPrimitiveLiteral(l);
    }
    void CodegenVisitor::visitPrint(std::shared_ptr<PrintStmt> p){
      auto print_t = p->argument->exp_kind;
      mlir::Value expr_arg = visitExpr(p->argument);
      if(print_t == ExprKind::Literal)
        prints->printPrimtive(expr_arg);
      return;
    }
    void CodegenVisitor::visitStmt(std::shared_ptr<Stmt> stmt){
      auto stmt_t = stmt->stmt_kind;
      if(stmt_t == StmtKind::Print)
        visitPrint(std::static_pointer_cast<PrintStmt>(stmt));
      return;
    }

}