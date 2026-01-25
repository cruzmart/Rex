#pragma once
#include "rex_ast_nodes.h"
#include "rex_symbols.h"

namespace rex {

struct resolver {
    scope* current = nullptr;
    bool debug = false;

    explicit resolver(bool dbg = false)
        : debug(dbg) {
        current = new scope(nullptr, debug);
        current->dump_enter();
    }

    ~resolver() {
        while (current) {
            scope* p = current->parent;
            delete current;
            current = p;
        }
    }

    void enter_scope() {
        current = new scope(current, debug);
        current->dump_enter();
    }

    void leave_scope() {
        current->dump_exit();
        scope* old = current;
        current = current->parent;
        delete old;
    }

    void resolve(file_ast& file);
    void resolve_stmt(stmt& s);
    void resolve_expr(expr& e);
};

} // namespace rex
