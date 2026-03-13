#pragma once
#include <memory>
#include <string>
#include <iostream>

#include "rex_types.h"
#include "rex_scope.h"
#include "rex_ast_nodes.h"

#include "rex_exps.h"
#include "rex_stmts.h"
#include "rex_funcs.h"
#include "rex_ops.h"


namespace rex {

struct TypeChecker {
    bool debug = false;

    std::shared_ptr<Scope> global;
    std::shared_ptr<Scope> current_scope;

    void check(std::shared_ptr<FileAst> file);

    TypeChecker(std::shared_ptr<Scope> g);

private:

    // ---- Expressions ----
    type_ptr check_expr(std::shared_ptr<Expr> e);
    type_ptr check_literal(std::shared_ptr<LiteralExpr> e);
    type_ptr check_id(std::shared_ptr<IdExpr> id);
    type_ptr check_binary(std::shared_ptr<BinaryExpr> b);
    type_ptr check_unary(std::shared_ptr<UnaryExpr> u);
    type_ptr check_call(std::shared_ptr<CallExpr> c);
    type_ptr check_tuple(std::shared_ptr<TupleExpr> t);
    type_ptr check_index(std::shared_ptr<IndexExpr> i);
    type_ptr check_array(std::shared_ptr<ArrayExpr> a);
    type_ptr check_function(std::shared_ptr<FunctionDecl> f);

    type_ptr resolve_id(const std::string name);

    // ---- Statements ----
    void check_stmt(std::shared_ptr<Stmt> s);
    void check_let(std::shared_ptr<LetStmt> s);
    void check_assign(std::shared_ptr<AssignStmt> s);
    void check_return(std::shared_ptr<ReturnStmt> s);
    void check_if(std::shared_ptr<IfStmt> s);
    void check_while(std::shared_ptr<WhileStmt> s);
    void check_loop(std::shared_ptr<LoopStmt> s);
    void check_for(std::shared_ptr<ForStmt> s);
    void check_type_decl(std::shared_ptr<TypeDecl> t);
  


    // Utility
    void expect(type_ptr actual, type_ptr expected, const std::string& msg);
    void error(const std::string msg, const size_t l);
    void push_scope(); 
    void pop_scope();
};

} // namespace rex