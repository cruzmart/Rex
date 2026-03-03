#pragma once
#include <memory>
#include <string>

namespace rex {

struct Type;
struct Expr;

// ------------------ SYMBOL KIND -------------------

enum class SymbolType {
    variable,
    function,
    typealias
};

// ------------------ SYMBOL -----------------------

struct Symbol {
    SymbolType kind;
    std::string name;

    // type: shared_type information
    std::shared_ptr<Type> type;

    // optional expression (initializer or function body)
    std::shared_ptr<Expr> expr;

    Symbol(SymbolType k, std::string n)
        : kind(k), name(std::move(n)) {}
};

} // namespace rex