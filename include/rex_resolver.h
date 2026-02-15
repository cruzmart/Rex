#pragma once
#include "rex_scope.h"
#include "rex_ast_nodes.h"

namespace rex {

struct resolver {
    std::shared_ptr<scope> current;
    bool debug = false;

    resolver();

    void resolve(file_ast& file);

    // ---- helpers ----
    void predeclare(file_ast& file);
    void declare_function(function_decl& fn);
    void declare_params(function_decl& fn);

    // ---- visitors ----
    void resolve_stmt(std::shared_ptr<stmt> s);
    void resolve_expr(std::shared_ptr<expr> e);
};

} // namespace rex