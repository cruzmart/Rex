#pragma once

#include "rex_ast.h"
#include "rex_ast_nodes.h"
#include "rex_errors.h"
#include "rex_stmts.h"
#include "rex_exps.h"
#include "rex_types.h"
#include "rex_errors.h"

#include <memory>

namespace rex {

struct AssignmentCheckPass {

    Errors err;

    // Entry
    void visit(const std::shared_ptr<FileAst>& file);

private:
    // Traversal
    void visitStmt(const std::shared_ptr<Stmt>& stmt);
    void visitBlock(const std::shared_ptr<BlockExpr>& block);

    // Statements
    void visitAssign(const std::shared_ptr<AssignStmt>& stmt);
    void visitIf(const std::shared_ptr<IfStmt>& stmt);
    void visitWhile(const std::shared_ptr<WhileStmt>& stmt);
    void visitFor(const std::shared_ptr<ForStmt>& stmt);

    // Helpers
    bool isAssignable(const std::shared_ptr<Expr>& expr);
};

}