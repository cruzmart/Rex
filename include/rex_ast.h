#pragma once
//#include "rex_funcs.h"
#include <iostream>
#include <memory>

namespace rex {

enum class AstNodeKind {
    Stmt,
    FunctionDecl,
    Expr,
    Parameter,
    Type,
    Null
};

// struct Type;
struct AstNode;



inline void indent(std::ostream& os, int n) {
    for (int i = 0; i < n; ++i) os << "  ";
}

struct SourceLocation {
    size_t line;
    size_t column;
};

struct AstNode {
    SourceLocation loc;
    AstNodeKind ast_kind;
    AstNode(AstNodeKind node_kind) : ast_kind(node_kind) {}
    AstNode() : ast_kind(AstNodeKind::Null) {}
    virtual ~AstNode() = default;
    virtual void dump(std::ostream& os, int indent = 0) const = 0;
};

} // namespace rex