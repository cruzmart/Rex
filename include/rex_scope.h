#pragma once
#include <map>
#include <string>
#include <memory>
#include "rex_symbol.h"

namespace rex {

struct scope : std::enable_shared_from_this<scope> {
    std::shared_ptr<scope> parent;
    std::map<std::string, std::shared_ptr<symbol>> table;

    explicit scope(std::shared_ptr<scope> p = nullptr)
        : parent(std::move(p)) {}

    void define(std::shared_ptr<symbol> sym) {
        table[sym->name] = sym;
    }

    std::shared_ptr<symbol> resolve(const std::string& n) {
        auto it = table.find(n);
        if (it != table.end())
            return it->second;
        return parent ? parent->resolve(n) : nullptr;
    }

    std::shared_ptr<scope> push() {
        return std::make_shared<scope>(shared_from_this());
    }
};

} // namespace rex