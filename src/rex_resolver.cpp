#include "rex_resolver.h"
#include <iostream>

namespace rex {

resolver::resolver() {
    current = std::make_shared<scope>();
}

void resolver::resolve(file_ast& file) {

    // Phase 1: predeclare types, functions, globals
    predeclare(file);

    // Phase 2: resolve bodies
    for (auto& item : file.items) {

        if (auto fn = std::dynamic_pointer_cast<function_decl>(item)) {
            // Function body gets its own scope
            auto saved = current;
            current = current->push();

            // Declare parameters inside this scopex
            declare_params(*fn);

            // Resolve statements
            for (auto& st : fn->body->statements)
                resolve_stmt(st);

            if (fn->body->result)
                resolve_expr(fn->body->result);

            current = saved;
        }
    }
}

void resolver::predeclare(file_ast& file) {
    for (auto& item : file.items) {

        if (auto td = std::dynamic_pointer_cast<type_decl>(item)) {
            auto sym = std::make_shared<symbol>(symbol_kind::type, td->name);
            sym->type = td->aliased;
            td->resolved = sym.get();
            current->define(sym);
        }

        else if (auto fn = std::dynamic_pointer_cast<function_decl>(item)) {
            auto sym = std::make_shared<symbol>(symbol_kind::function, fn->name);
            sym->func = fn.get();
            fn->resolved = sym.get();
            current->define(sym);
        }

        else if (auto let = std::dynamic_pointer_cast<let_stmt>(item)) {
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
        // already predeclared in enclosing scope? no → local let
        auto sym = std::make_shared<symbol>(symbol_kind::variable, let->name);
        sym->type = let->explicit_type;
        sym->let = let.get();
        let->resolved = sym.get();
        current->define(sym);

        if (let->init)
            resolve_expr(let->init);
    }

    else if (auto as = std::dynamic_pointer_cast<assign_stmt>(s)) {
        resolve_expr(as->target);
        resolve_expr(as->value);
    }

    else if (auto ret = std::dynamic_pointer_cast<return_stmt>(s)) {
        if (ret->value)
            resolve_expr(ret->value);
    }

    else if (auto es = std::dynamic_pointer_cast<expr_stmt>(s)) {
        resolve_expr(es->value);
    }

    else if (auto wh = std::dynamic_pointer_cast<while_stmt>(s)) {
        resolve_expr(wh->cond);
        auto saved = current;
        current = current->push();
        for (auto& st : wh->body->statements)
            resolve_stmt(st);
        if (wh->body->result)
            resolve_expr(wh->body->result);
        current = saved;
    }

    else if (auto fs = std::dynamic_pointer_cast<for_stmt>(s)) {
        resolve_expr(fs->iterable);
        auto saved = current;
        current = current->push();
        auto sym = std::make_shared<symbol>(symbol_kind::variable, fs->iter_var);
        fs->resolved = sym.get();
        current->define(sym);
        for (auto& st : fs->body->statements)
            resolve_stmt(st);
        current = saved;
    }

    else if (auto lp = std::dynamic_pointer_cast<loop_stmt>(s)) {
        auto saved = current;
        current = current->push();
        for (auto& st : lp->body->statements)
            resolve_stmt(st);
        current = saved;
    }

    else if (auto is = std::dynamic_pointer_cast<if_stmt>(s)) {
        resolve_expr(is->condition);
        auto saved = current;

        current = current->push();
        for (auto& st : is->then_block->statements)
            resolve_stmt(st);
        current = saved;

        for (auto& [c, b] : is->elifx_blocks) {
            resolve_expr(c);
            current = current->push();
            for (auto& st : b->statements)
                resolve_stmt(st);
            current = saved;
        }

        if (is->else_block) {
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
        auto sym = current->resolve(id->name);
        if (!sym)
            std::cerr << "Unresolved identifier: " << id->name << "\n";
        id->resolved = sym ? sym.get() : nullptr;
    }

    else if (auto be = std::dynamic_pointer_cast<binary_expr>(e)) {
        resolve_expr(be->lhs);
        resolve_expr(be->rhs);
    }

    else if (auto ue = std::dynamic_pointer_cast<unary_expr>(e)) {
        resolve_expr(ue->rhs);
    }

    else if (auto ce = std::dynamic_pointer_cast<call_expr>(e)) {
        auto sym = current->resolve(ce->callee);
        if (!sym)
            std::cerr << "Unresolved function: " << ce->callee << "\n";
        ce->resolved = sym ? sym.get() : nullptr;
        for (auto& a : ce->args)
            resolve_expr(a);
    }

    else if (auto ie = std::dynamic_pointer_cast<index_expr>(e)) {
        resolve_expr(ie->base);
        resolve_expr(ie->index);
    }

    else if (auto te = std::dynamic_pointer_cast<tuple_expr>(e)) {
        for (auto& el : te->elements)
            resolve_expr(el);
    }
}

} // namespace rex