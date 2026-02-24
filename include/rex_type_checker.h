#pragma once
#include <memory>
#include <string>
#include <iostream>

#include "rex_types.h"
#include "rex_ast_nodes.h"

namespace rex {

struct type_checker {
    bool debug = false;

    // Entry point
    void check(file_ast& file);

private:
    // ---- Expressions ----
    type_ptr* check_expr(std::shared_ptr<expr> e);
    type_ptr* check_id(id_expr& id);
    type_ptr* check_binary(binary_expr& b);
    type_ptr* check_unary(unary_expr& u);
    type_ptr* check_call(call_expr& c);
    type_ptr* check_tuple(tuple_expr& t);
    type_ptr* check_index(index_expr& i);

    // ---- Statements ----
    void check_stmt(std::shared_ptr<stmt> s);
    void check_let(let_stmt& s);
    void check_assign(assign_stmt& s);
    void check_return(return_stmt& s);
    void check_if(if_stmt& s);
    void check_while(while_stmt& s);
    void check_loop(loop_stmt& s);
    void check_for(for_stmt& s);

    // Utility
    void expect(type_ptr* actual, type_ptr* expected, const std::string& msg);
};

} // namespace rex