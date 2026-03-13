#pragma once

#include <string>
#include <memory>
#include <unordered_map>

namespace rex {

struct Symbol;

struct Scope : std::enable_shared_from_this<Scope> {
    std::shared_ptr<Scope> parent;
    std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols;

    explicit Scope(std::shared_ptr<Scope> parent = nullptr);

    void define(const std::shared_ptr<Symbol> sym);
    std::shared_ptr<Symbol> resolve(const std::string name);

    std::shared_ptr<Scope> push();
};

} // namespace rex