#include "rex_resolver.h"
#include <stdexcept>
#include <iostream>

using namespace rex;

void resolver::resolve(file_ast& file) {
    if (debug)
        std::cerr << "\n=== RESOLVER START ===\n";

    // Predeclare functions and types
    for (auto& item : file.items) {
        if (auto fn = dynamic_cast<function_decl*>(item.get())) {
            current->define({symbol_kind::function, fn->name, fn});
        }
        if (auto td = dynamic_cast<type_decl*>(item.get())) {
            current->define({symbol_kind::type, td->name, td});
        }
    }

    for (auto& item : file.items) {
        if (auto s = dynamic_cast<stmt*>(item.get())) {
            resolve_stmt(*s);
        }
        else if (auto fn = dynamic_cast<function_decl*>(item.get())) {
            enter_scope();
            for (auto& p : fn->params) {
                current->define({symbol_kind::variable, p.name, nullptr});
            }
            resolve_expr(*fn->body);
            leave_scope();
        }
    }

    if (debug)
        std::cerr << "=== RESOLVER END ===\n\n";
}

void resolver::resolve_stmt(stmt& s) {
    if (auto let = dynamic_cast<let_stmt*>(&s)) {
        resolve_expr(*let->init);
        current->define({symbol_kind::variable, let->name, let});
    }
    else if (auto asg = dynamic_cast<assign_stmt*>(&s)) {
        if (!dynamic_cast<id_expr*>(asg->target.get()) &&
            !dynamic_cast<index_expr*>(asg->target.get())) {
            throw std::runtime_error("Invalid assignment target");
        }
        resolve_expr(*asg->target);
        resolve_expr(*asg->value);
    }
    else if (auto ret = dynamic_cast<return_stmt*>(&s)) {
        if (ret->value)
            resolve_expr(*ret->value);
    }
    else if (auto w = dynamic_cast<while_stmt*>(&s)) {
        resolve_expr(*w->cond);
        enter_scope();
        resolve_expr(*w->body);
        leave_scope();
    }
    else if (auto f = dynamic_cast<for_stmt*>(&s)) {
        resolve_expr(*f->iterable);
        enter_scope();
        current->define({symbol_kind::variable, f->iter_var, &s});
        resolve_expr(*f->body);
        leave_scope();
    }
    else if (auto ifs = dynamic_cast<if_stmt*>(&s)) {
    // Resolve the main if condition
    resolve_expr(*ifs->condition);

    // Enter new scope for then block
    enter_scope();
    resolve_expr(*ifs->then_block);
    leave_scope();

    // Elifx blocks
    for (auto& [cond, block] : ifs->elifx_blocks) {
        resolve_expr(*cond);
        enter_scope();
        resolve_expr(*block);
        leave_scope();
    }

    // Else block
    if (ifs->else_block) {
        enter_scope();
        resolve_expr(*ifs->else_block);
        leave_scope();
    }
}
}

void resolver::resolve_expr(expr& e) {
    if (auto id = dynamic_cast<id_expr*>(&e)) {
        auto sym = current->lookup(id->name);
        if (!sym)
            throw std::runtime_error("Undefined identifier: " + id->name);
        id->sym = sym;
    }
    else if (auto bin = dynamic_cast<binary_expr*>(&e)) {
        resolve_expr(*bin->lhs);
        resolve_expr(*bin->rhs);
    }
    else if (auto un = dynamic_cast<unary_expr*>(&e)) {
        resolve_expr(*un->rhs);
    }
    else if (auto call = dynamic_cast<call_expr*>(&e)) {
        auto sym = current->lookup(call->callee);
        if (!sym || sym->kind != symbol_kind::function)
            throw std::runtime_error("Not a function: " + call->callee);
        for (auto& a : call->args)
            resolve_expr(*a);
    }
    else if (auto block = dynamic_cast<block_expr*>(&e)) {
        enter_scope();
        for (auto& s : block->statements)
            resolve_stmt(*s);
        if (block->result)
            resolve_expr(*block->result);
        leave_scope();
    }
}
