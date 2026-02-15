#include "rex_scope.h"

namespace rex {

// --------------------------------------------------------
// define
// --------------------------------------------------------
bool Scope::define(const Symbol& sym) {
    if (debug) {
        std::cerr
            << indent()
            << "[declare] " << to_string(sym.kind)
            << " " << sym.name
            << " (depth " << depth << ")\n";
    }

    auto res = symbols.emplace(sym.name, sym);
    if (res.second)
        res.first->second.defining_scope = this;

    return res.second;
}

// --------------------------------------------------------
// lookup
// --------------------------------------------------------
Symbol* Scope::lookup(const std::string& name) {
    for (Scope* s = this; s; s = s->parent) {
        auto it = s->symbols.find(name);
        if (it != s->symbols.end()) {
            if (debug) {
                std::cerr
                    << indent()
                    << "[lookup] " << name
                    << " -> depth " << s->depth
                    << " (" << to_string(it->second.kind) << ")\n";
            }
            return &it->second;
        }
    }

    if (debug) {
        std::cerr
            << indent()
            << "[lookup] " << name
            << " -> NOT FOUND\n";
    }

    return nullptr;
}

} // namespace rex