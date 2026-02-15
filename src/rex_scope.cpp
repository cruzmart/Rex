#include "rex_scope.h"
#include <iostream>

namespace rex {

scope::scope(std::shared_ptr<scope> p)
    : parent(std::move(p)) {}

void scope::define(std::shared_ptr<symbol> sym) {
    if (debug) {
        std::cout << "[scope] define '" << sym->name << "'\n";
    }
    table[sym->name] = sym;
}

std::shared_ptr<symbol> scope::resolve(const std::string& n) {
    if (debug) {
        std::cout << "[scope] resolve '" << n << "' in scope " << this << "\n";
    }

    auto it = table.find(n);
    if (it != table.end()) {
        if (debug) {
            std::cout << "[scope]   found in this scope\n";
        }
        return it->second;
    }

    if (parent) {
        if (debug) {
            std::cout << "[scope]   not found, checking parent\n";
        }
        return parent->resolve(n);
    }

    if (debug) {
        std::cout << "[scope]   not found in any scope\n";
    }
    return nullptr;
}

std::shared_ptr<scope> scope::push() {
    if (debug) {
        std::cout << "[scope] pushing new child scope\n";
    }

    auto child = std::make_shared<scope>(shared_from_this());
    child->debug = debug; // inherit debug flag
    return child;
}

} // namespace rex