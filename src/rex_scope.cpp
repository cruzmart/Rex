#include "rex_scope.h"
#include <stdexcept>

namespace rex {

Scope::Scope(std::shared_ptr<Scope> p)
    : parent(std::move(p)) {}

void Scope::define(const std::shared_ptr<Symbol> sym) {
    const auto& name = sym->name;

    // Optional safety check:
    if (table.find(name) != table.end()) {
        throw std::runtime_error("Symbol '" + name + "' already defined in this scope.");
    }

    table[name] = sym;
}

std::shared_ptr<Symbol> Scope::resolve(const std::string& name) {
    auto it = table.find(name);
    if (it != table.end()) {
        return it->second;
    }
    if (parent) {
        return parent->resolve(name);
    }
    return nullptr;
}

std::shared_ptr<Scope> Scope::push() {
    // child scope whose parent is this scope
    return std::make_shared<Scope>(shared_from_this());
}

} // namespace rex