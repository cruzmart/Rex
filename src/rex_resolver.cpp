#include "rex_resolver.h"
#include <iostream>

namespace rex {

resolver::resolver() {
    current = std::make_shared<scope>();
}

void resolver::resolve(file_ast& file) {
    if(debug)
        std::cout << "\n=== RESOLVER START ===\n";

    // Phase 1: predeclare types, functions, globals
    if(debug)
        std::cout << "[resolver] Predeclaring top-level symbols...\n";
    predeclare(file);

    if(debug)
        std::cout << "[resolver] Entering function body resolution...\n";

    // Phase 2: resolve bodies
    for (auto& item : file.items) {

        if (auto fn = std::dynamic_pointer_cast<function_decl>(item)) {
            if(debug)
                std::cout << "\n[resolver] Resolving function '" << fn->name << "'\n";

            // Function body gets its own scope
            auto saved = current;
            current = current->push();
            current->debug = debug;

            // Declare parameters inside this scope
            declare_params(*fn);

            // Resolve statements
            for (auto& st : fn->body->statements) {
                if(debug)
                    std::cout << "[resolver]   Resolving statement inside function '" << fn->name << "'\n";
                resolve_stmt(st);
            }

            if (fn->body->result)
                resolve_expr(fn->body->result);

            current = saved;
            if(debug)
                std::cout << "[resolver] Exited function '" << fn->name << "' scope\n";
        }
    }

    if(debug)
        std::cout << "=== RESOLVER END ===\n\n";
}

void resolver::predeclare(file_ast& file) {
    for (auto& item : file.items) {

        if (auto td = std::dynamic_pointer_cast<type_decl>(item)) {
            if(debug)
                std::cout << "[resolver] Predeclare TYPE '" << td->name << "'\n";

            auto sym = std::make_shared<symbol>(symbol_kind::type, td->name);
            sym->type = td->aliased;
            td->resolved = sym.get();
            current->define(sym);
        }

        else if (auto fn = std::dynamic_pointer_cast<function_decl>(item)) {
            if(debug)
                std::cout << "[resolver] Predeclare FUNCTION '" << fn->name << "'\n";

            auto sym = std::make_shared<symbol>(symbol_kind::function, fn->name);
            sym->func = fn.get();
            fn->resolved = sym.get();
            current->define(sym);
        }

        else if (auto let = std::dynamic_pointer_cast<let_stmt>(item)) {
            if(debug)
                std::cout << "[resolver] Predeclare GLOBAL LET '" << let->name << "'\n";

            auto sym = std::make_shared<symbol>(symbol_kind::variable, let->name);
            sym->type = let->explicit_type;
            sym->let = let.get();
            let->resolved = sym.get();
            current->define(sym);
        }
    }
}

void resolver::declare_params(function_decl& fn) {
    for (auto& p : fn.params) {
        if(debug)
            std::cout << "  [resolver] Declare PARAM '" << p.name << "'\n";

        auto sym = std::make_shared<symbol>(symbol_kind::variable, p.name);
        sym->type = p.type;
        sym->param_decl = &p;
        p.resolved = sym.get();
        current->define(sym);
    }
}

// ---------------------- STATEMENTS ------------------------

void resolver::resolve_stmt(std::shared_ptr<stmt> s) {

    if (auto let = std::dynamic_pointer_cast<let_stmt>(s)) {
        if(debug)
            std::cout << "[resolver]   LET stmt: '" << let->name << "'\n";

        auto sym = std::make_shared<symbol>(symbol_kind::variable, let->name);
        sym->type = let->explicit_type;
        sym->let = let.get();
        let->resolved = sym.get();
        current->define(sym);

        if (let->init) {
            std::cout << "[resolver]   Resolving LET initializer for '" << let->name << "'\n";
            resolve_expr(let->init);
        }
    }

    else if (auto as = std::dynamic_pointer_cast<assign_stmt>(s)) {
        if(debug)
            std::cout << "[resolver]   ASSIGN stmt\n";
        resolve_expr(as->target);
        resolve_expr(as->value);
    }

    else if (auto ret = std::dynamic_pointer_cast<return_stmt>(s)) {
        if(debug)
            std::cout << "[resolver]   RETURN stmt\n";
        if (ret->value)
            resolve_expr(ret->value);
    }

    else if (auto es = std::dynamic_pointer_cast<expr_stmt>(s)) {
        if(debug)
            std::cout << "[resolver]   EXPR stmt\n";
        resolve_expr(es->value);
    }

    else if (auto wh = std::dynamic_pointer_cast<while_stmt>(s)) {
        if(debug)
            std::cout << "[resolver]   WHILE stmt\n";
        resolve_expr(wh->cond);

        auto saved = current;
        current = current->push();
        current->debug = debug;

        for (auto& st : wh->body->statements)
            resolve_stmt(st);

        if (wh->body->result)
            resolve_expr(wh->body->result);

        current = saved;
    }

    else if (auto fs = std::dynamic_pointer_cast<for_stmt>(s)) {
        if(debug)
            std::cout << "[resolver]   FOR stmt, iter var '" << fs->iter_var << "'\n";

        resolve_expr(fs->iterable);

        auto saved = current;
        current = current->push();
        current->debug = debug;

        auto sym = std::make_shared<symbol>(symbol_kind::variable, fs->iter_var);
        fs->resolved = sym.get();
        current->define(sym);

        for (auto& st : fs->body->statements)
            resolve_stmt(st);

        current = saved;
    }

    else if (auto lp = std::dynamic_pointer_cast<loop_stmt>(s)) {
        if(debug)
            std::cout << "[resolver]   LOOP stmt\n";

        auto saved = current;
        current = current->push();
        current->debug = debug;

        for (auto& st : lp->body->statements)
            resolve_stmt(st);

        current = saved;
    }

    else if (auto is = std::dynamic_pointer_cast<if_stmt>(s)) {
        if(debug)
            std::cout << "[resolver]   IF stmt\n";

        resolve_expr(is->condition);
        auto saved = current;

        if(debug)
            std::cout << "[resolver]     -> THEN block\n";
        current = current->push();
        for (auto& st : is->then_block->statements)
            resolve_stmt(st);
        current = saved;

        for (auto& [c, b] : is->elifx_blocks) {
            if(debug)
                std::cout << "[resolver]     -> ELIF block\n";
            resolve_expr(c);

            current = current->push();
            for (auto& st : b->statements)
                resolve_stmt(st);
            current = saved;
        }

        if (is->else_block) {
            if(debug)
                std::cout << "[resolver]     -> ELSE block\n";
            current = current->push();
            for (auto& st : is->else_block->statements)
                resolve_stmt(st);
            current = saved;
        }
    }
}

// --------------------- EXPRESSIONS ------------------------

void resolver::resolve_expr(std::shared_ptr<expr> e) {

    if (auto id = std::dynamic_pointer_cast<id_expr>(e)) {
        if(debug)
            std::cout << "[resolver]     resolve IDENTIFIER '" << id->name << "'\n";

        auto sym = current->resolve(id->name);
        if(debug){

            if (!sym)
                std::cerr << "[resolver]     ❌ UNRESOLVED identifier: " << id->name << "\n";
            else
                std::cout << "[resolver]     ✔ resolved to symbol '" << sym->name << "'\n";
        }

        id->resolved = sym ? sym.get() : nullptr;
    }

    else if (auto be = std::dynamic_pointer_cast<binary_expr>(e)) {
        if(debug)
            std::cout << "[resolver]     binary expr\n";
        resolve_expr(be->lhs);
        resolve_expr(be->rhs);
    }

    else if (auto ue = std::dynamic_pointer_cast<unary_expr>(e)) {
        if(debug)
            std::cout << "[resolver]     unary expr\n";
        resolve_expr(ue->rhs);
    }

    else if (auto ce = std::dynamic_pointer_cast<call_expr>(e)) {
        if(debug)
            std::cout << "[resolver]     CALL expr: '" << ce->callee << "'\n";

        auto sym = current->resolve(ce->callee);
        if(debug){
            if (!sym)
                std::cerr << "[resolver]     ❌ UNRESOLVED function: " << ce->callee << "\n";
            else
               std::cout << "[resolver]     ✔ resolved function '" << sym->name << "'\n";
        }

        ce->resolved = sym ? sym.get() : nullptr;

        for (auto& a : ce->args)
            resolve_expr(a);
    }

    else if (auto ie = std::dynamic_pointer_cast<index_expr>(e)) {
        if(debug)
            std::cout << "[resolver]     index expr\n";
        resolve_expr(ie->base);
        resolve_expr(ie->index);
    }

    else if (auto te = std::dynamic_pointer_cast<tuple_expr>(e)) {
        if(debug)
            std::cout << "[resolver]     tuple expr\n";
        for (auto& el : te->elements)
            resolve_expr(el);
    }
}

} // namespace rex