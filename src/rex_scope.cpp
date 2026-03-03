#include "rex_scope.h"
#include "rex_symbol.h"
#include <stdexcept>

namespace rex {

Scope::Scope(std::shared_ptr<Scope> p)
    : parent(std::move(p)) {}

void Scope::define(const std::shared_ptr<Symbol>& sym) {
    const std::string& name = sym->name;

    if (symbols.count(name)) {
        throw std::runtime_error(
            "Redefinition of symbol '" + name + "' in this scope");
    }

    symbols[name] = sym;
}

std::shared_ptr<Symbol> Scope::resolve(const std::string& name) {
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        return it->second;
    }
    if (parent) {
        return parent->resolve(name);
    }
    return nullptr;
}

std::shared_ptr<Scope> Scope::push() {
    return std::make_shared<Scope>(shared_from_this());
}

} // namespace rex