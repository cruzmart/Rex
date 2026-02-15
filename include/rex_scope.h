#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include "rex_symbol.h"

namespace rex {

struct Scope {
    Scope* parent = nullptr;
    int depth = 0;

    std::unordered_map<std::string, Symbol> symbols;

    bool debug = false;

    explicit Scope(Scope* p = nullptr, bool dbg = false)
        : parent(p), depth(p ? p->depth + 1 : 0), debug(dbg) {}

    // Add symbol to this scope
    bool define(const Symbol& sym);

    // Lookup symbol
    Symbol* lookup(const std::string& name);

private:
    std::string indent() const {
        return std::string(depth * 2, ' ');
    }
};

} // namespace rex