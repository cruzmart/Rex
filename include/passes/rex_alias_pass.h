#pragma once
#include <memory>
#include "rex_ast_nodes.h"
#include "rex_funcs.h"
#include "rex_scope.h"
#include "rex_types.h"
#include "rex_exps.h"
#include "rex_symbol.h"
#include "rex_stmts.h"

namespace rex {

struct AliasPass {
    std::shared_ptr<Scope> current_scope;
    bool debug = false;
    int scope_depth = 0;

    explicit AliasPass(std::shared_ptr<Scope> global, bool debug_mode = false)
        : current_scope(global), debug(debug_mode) {}

    void visit(const std::shared_ptr<FileAst>& file);

private:
    void visitStmt(const std::shared_ptr<Stmt>& stmt);
    void visitBlock(const std::shared_ptr<BlockExpr>& block);

    void visitLetStmt(const std::shared_ptr<LetStmt>& ls);
    void visitTypeDecl(const std::shared_ptr<TypeDecl>& td);
    void visitFunctionDecl(const std::shared_ptr<FunctionDecl>& fn);

    std::shared_ptr<Type> resolveType(const std::shared_ptr<Type>& type);

    void print(const std::string& msg) const;
};

} // namespace rex