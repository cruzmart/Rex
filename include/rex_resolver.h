#pragma once
#include "rex_scope.h"
#include "rex_ast_nodes.h"

namespace rex {

struct resolver {
    std::shared_ptr<Scope> current;
    bool debug = false;

    resolver();

    void resolve(FileAst& file);

    // ---- helpers ----
    void predeclare(FileAst& file);
    void declare_function(FunctionDecl& fn);
    void declare_params(FunctionDecl& fn);

    // ---- visitors ----
    void resolve_stmt(std::shared_ptr<Stmt> s);
    void resolve_expr(std::shared_ptr<Expr> e);
};

} // namespace rex