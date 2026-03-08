#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "rex_ast.h"


namespace rex {

struct Type;
struct FunctionType;
struct BlockExpr;
struct Symbol;
struct Parameter;
struct FunctionDecl;

// ---------------------- FUNCTIONS ---------------------
struct Parameter : AstNode {
    std::string para_name;
    std::shared_ptr<Type> para_type;
    std::shared_ptr<Symbol> resolved;
    Parameter();
    Parameter(std::string name, std::shared_ptr<Type> type);
    void dump(std::ostream& os, int i) const override;
};

struct FunctionDecl : AstNode {
    std::string func_name;
    std::shared_ptr<FunctionType> func_type; // Could be FunctionType if defined

    std::shared_ptr<BlockExpr> body;
    std::shared_ptr<Symbol> resolved;
    void dump(std::ostream& os, int i) const override;
};

} // namespace rex