#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "rex_types.h"

namespace rex {

struct SourceLocation;
struct TypeNode;
struct AstNode;
struct Expr;
struct Stmt;

inline void indent(std::ostream& os, int n) {
    for (int i = 0; i < n; ++i) os << "  ";
}

struct SourceLocation {
    size_t line;
    size_t column;
};

struct AstNode {
    SourceLocation loc;
    virtual ~AstNode() = default;
    virtual void dump(std::ostream& os, int indent = 0) const = 0;
    
};

struct TypeNode: AstNode {

    std::shared_ptr<rex::Type> type;

    explicit TypeNode() {}
    virtual ~TypeNode() = default;

    virtual void dump(std::ostream& os, int i) const = 0;
};


struct Expr : AstNode {
    std::shared_ptr<rex::Type> type;
};

struct Stmt : AstNode {};


} // namespace rex
