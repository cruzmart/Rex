#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

#include "rex_ast.h"

namespace rex {

enum class symbol_kind {
    variable,
    function,
    type
};

inline const char* to_string(symbol_kind k) {
    switch (k) {
        case symbol_kind::variable: return "var";
        case symbol_kind::function: return "fn";
        case symbol_kind::type:     return "type";
    }
    return "?";
}

struct symbol {
    symbol_kind kind;
    std::string name;
    ast_node* decl; // points into AST
};

struct scope {
    scope* parent = nullptr;
    std::unordered_map<std::string, symbol> symbols;

    bool debug;
    int depth;

    explicit scope(scope* p = nullptr, bool dbg = false)
        : parent(p),
          debug(dbg),
          depth(p ? p->depth + 1 : 0) {}

    bool define(const symbol& sym) {
        if (debug) {
            std::cerr
                << indent()
                << "[declare] "
                << to_string(sym.kind)
                << " "
                << sym.name
                << " (depth " << depth << ")\n";
        }
        return symbols.emplace(sym.name, sym).second;
    }

    symbol* lookup(const std::string& name) {
        for (scope* s = this; s; s = s->parent) {
            auto it = s->symbols.find(name);
            if (it != s->symbols.end()) {
                if (debug) {
                    std::cerr
                        << indent()
                        << "[lookup] "
                        << name
                        << " -> depth " << s->depth
                        << " (" << to_string(it->second.kind) << ")\n";
                }
                return &it->second;
            }
        }

        if (debug) {
            std::cerr
                << indent()
                << "[lookup] "
                << name
                << " -> NOT FOUND\n";
        }

        return nullptr;
    }

    void dump_enter() const {
        if (debug)
            std::cerr << indent() << ">>> ENTER scope depth " << depth << "\n";
    }

    void dump_exit() const {
        if (debug)
            std::cerr << indent() << "<<< EXIT  scope depth " << depth << "\n";
    }

private:
    std::string indent() const {
        return std::string(depth * 2, ' ');
    }
};

} // namespace rex
