#pragma once
#include <memory>
#include "rex_ast_nodes.h"
#include "rex_scope.h"
#include "rex_types.h"
#include "rex_exps.h"
#include "rex_symbol.h"
#include "rex_stmts.h"


namespace rex {

struct TypeDecl;

struct AliasPass {
    std::shared_ptr<Scope> current_scope;

    explicit AliasPass(std::shared_ptr<Scope> global) : current_scope(global) {}

    // Entry point for visiting the whole program
    void visit(const std::shared_ptr<FileAst> file);

private:
    void visitStmt(const std::shared_ptr<Stmt> stmt);
    void visitBlock(const std::shared_ptr<BlockExpr> block);
    void visitTypeDecl(const std::shared_ptr<TypeDecl> td);
    void visitLetStmt(const std::shared_ptr<LetStmt> ls);


    // Resolves a NamedType recursively
    std::shared_ptr<Type> resolveType(const std::shared_ptr<Type>& type);
};

} // namespace rex