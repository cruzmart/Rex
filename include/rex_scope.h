#pragma once
#include <map>
#include <string>
#include <memory>
#include "rex_symbol.h"

namespace rex {

struct scope : std::enable_shared_from_this<scope> {
    std::shared_ptr<scope> parent;
    std::map<std::string, std::shared_ptr<symbol>> table;
    bool debug = false;

    explicit scope(std::shared_ptr<scope> p = nullptr);

    void define(std::shared_ptr<symbol> sym);
    std::shared_ptr<symbol> resolve(const std::string& n);

    std::shared_ptr<scope> push();
};

} // namespace rex