#pragma once
#include <memory>
#include "rex_ast.h"
#include "rex_ast_nodes.h"
#include "rex_errors.h"
#include "rex_stmts.h"
#include "rex_errors.h"
#include "rex_funcs.h"
#include "rex_exps.h"
#include "rex_ast_nodes.h"
#include "rex_errors.h"

namespace rex {

struct BreakCheckPass {
    Errors err;

    int loop_depth = 0;

    void visit(const std::shared_ptr<FileAst>& file);

private:
    void visitStmt(const std::shared_ptr<Stmt>& stmt);
    void visitBlock(const std::shared_ptr<BlockExpr>& block);
    void visitWhile(const std::shared_ptr<WhileStmt>& stmt);
    void visitFor(const std::shared_ptr<ForStmt>& stmt);
    void visitLoop(const std::shared_ptr<LoopStmt>& stmt);
    void visitIf(const std::shared_ptr<IfStmt>& stmt);
    void visitBreak(const std::shared_ptr<Stmt>& stmt);
};

}