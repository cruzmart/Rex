#pragma once
#include <memory>
#include "rex_ast_nodes.h"
#include "rex_funcs.h"
#include "rex_exps.h"
#include "rex_stmts.h"
#include "rex_ast_nodes.h"
#include "rex_errors.h"


namespace rex {

struct ReturnCheckPass {
    Errors err;
    std::shared_ptr<Type> current_return_type;

    void visit(const std::shared_ptr<FileAst>& file);

    void visitFunction(const std::shared_ptr<FunctionDecl>& fn);

    // returns true = this stmt ALWAYS returns
    bool visitStmt(const std::shared_ptr<Stmt>& stmt);

    bool visitBlock(const std::shared_ptr<BlockExpr>& block);

    bool visitIf(const std::shared_ptr<IfStmt>& stmt);
    bool visitWhile(const std::shared_ptr<WhileStmt>& stmt);
    bool visitFor(const std::shared_ptr<ForStmt>& stmt);

    bool visitReturn(const std::shared_ptr<ReturnStmt>& stmt);
    bool isVoid(const std::shared_ptr<Type>& t);

};

}