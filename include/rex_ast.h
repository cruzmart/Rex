#pragma once
#include <iostream>
#include <memory>

namespace rex {

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
    virtual ~AstNode() = default;
    virtual void dump(std::ostream& os, int indent = 0) const = 0;
};

} // namespace rex