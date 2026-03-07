#pragma once
#include <memory>
#include "rex_ast_nodes.h"
#include "rex_funcs.h"
#include "rex_operator_type_system.h"
#include "rex_scope.h"
#include "rex_types.h"
#include "rex_exps.h"
#include "rex_symbol.h"
#include "rex_stmts.h"


namespace rex {

struct ExprPass {

    private: 
        OperatorTypeSystem ots;
        std::shared_ptr<Scope> current_scope;
        int scope_depth = 0;

    public:

        std::shared_ptr<Type> visitExpr(const std::shared_ptr<Expr> exp);
        void visitStmt(const std::shared_ptr<Stmt> stmt);
        void visitBlock(const std::shared_ptr<BlockExpr> block);

        void visitLetStmt(const std::shared_ptr<LetStmt> ls);
        void visitFunctionDecl(const std::shared_ptr<FunctionDecl> f);

        std::shared_ptr<Type> visitLiteral(const std::shared_ptr<LiteralExpr> literal);
        std::shared_ptr<Type> visitId(const std::shared_ptr<IdExpr> id);
        std::shared_ptr<Type> visitBinary(const std::shared_ptr<BinaryExpr> bexp);
        std::shared_ptr<Type> visitUnary (const std::shared_ptr<Type> uexp);        
        
        std::shared_ptr<Type> visitTuple(const std::shared_ptr<TupleExpr> texp);
        std::shared_ptr<Type> visitArray (const std::shared_ptr<ArrayExpr> aexp);
        std::shared_ptr<Type> visitIndex (const std::shared_ptr<IndexExpr> iexp);

    
        std::shared_ptr<Type> visitRangeExpr (const std::shared_ptr<Type> rexp);
        std::shared_ptr<Type> visitCall (const std::shared_ptr<CallExpr> cexp);
        std::shared_ptr<Type> visitPipe ( const std::shared_ptr<PipeExpr> pexp);

        std::shared_ptr<Type> resolveExp(const std::shared_ptr<Expr> type);


};


}