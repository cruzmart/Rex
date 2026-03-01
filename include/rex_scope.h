#pragma once
#include <map>
#include <string>
#include <memory>
#include "rex_symbol.h"

namespace rex {

struct Scope : std::enable_shared_from_this<Scope> {
    std::shared_ptr<Scope> parent;
    std::map<std::string, std::shared_ptr<Symbol>> table;
    bool debug = false;

    explicit Scope(std::shared_ptr<Scope> p = nullptr);

    void define(std::shared_ptr<Symbol> sym);
    std::shared_ptr<Symbol> resolve(const std::string& n);

    std::shared_ptr<Scope> push();
};

} // namespace rex