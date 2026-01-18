#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>

namespace rex {

inline void indent(std::ostream& os, int n) {
    for (int i = 0; i < n; ++i) os << "  ";
}

struct source_location {
    size_t line;
    size_t column;
};

struct ast_node {
    source_location loc;
    virtual ~ast_node() = default;
    virtual void dump(std::ostream& os, int indent = 0) const = 0;
    
};

struct type_node : ast_node {
    enum class kind { primitive, named, array, slice, tuple };
    kind node_kind;

    explicit type_node(kind k) : node_kind(k) {}
    virtual ~type_node() = default;

    virtual void dump(std::ostream& os, int i) const = 0;
};



struct expr : ast_node {
    // Type type;  // later
};

struct stmt : ast_node {};


} // namespace rex
