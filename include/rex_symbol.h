#pragma once
#include <string>
#include <variant>
#include <memory>
#include <unordered_map>

#include "rex_ast_nodes.h"
#include "rex_types.h"

namespace rex {

enum class symbol_kind {
    variable,
    function,
    type
};

inline const char* to_string(symbol_kind k) {
    switch (k) {
        case symbol_kind::variable: return "var";
        case symbol_kind::function: return "fn";
        case symbol_kind::type:     return "type";
    }
    return "?";
}

// Forward-declare Scope
struct Scope;

/*
 * A Symbol represents:
 *   - let x : T
 *   - function f(a,b) : T
 *   - type T { ... }
 */
struct Symbol {
    symbol_kind kind;
    std::string name;

    // Holds the AST node that defined this symbol
    std::variant<
        std::shared_ptr<let_stmt>,
        std::shared_ptr<function_decl>,
        std::shared_ptr<type_decl>
    > node;

    // The declared/annotated type
    type annotated_type;

    // Where this symbol was declared
    Scope* defining_scope = nullptr;

    Symbol() = default;

    Symbol(symbol_kind k,
           const std::string& n,
           std::variant<
               std::shared_ptr<let_stmt>,
               std::shared_ptr<function_decl>,
               std::shared_ptr<type_decl>
           > dn)
        : kind(k),
          name(n),
          node(dn),
          annotated_type(type::Error()) {}

    const let_stmt* as_let() const {
        if (std::holds_alternative<std::shared_ptr<let_stmt>>(node))
            return std::get<std::shared_ptr<let_stmt>>(node).get();
        return nullptr;
    }

    const function_decl* as_func() const {
        if (std::holds_alternative<std::shared_ptr<function_decl>>(node))
            return std::get<std::shared_ptr<function_decl>>(node).get();
        return nullptr;
    }

    const type_decl* as_type() const {
        if (std::holds_alternative<std::shared_ptr<type_decl>>(node))
            return std::get<std::shared_ptr<type_decl>>(node).get();
        return nullptr;
    }
};

} // namespace rex