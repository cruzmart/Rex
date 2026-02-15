#pragma once
#include <string>
#include <memory>

namespace rex {

struct type_node;
struct function_decl;
struct let_stmt;
struct param;

// ------------------ SYMBOL KIND -------------------

enum class symbol_kind {
    variable,
    function,
    type
};

// ------------------ SYMBOL -----------------------

struct symbol {
    symbol_kind kind;
    std::string name;

    // Bound AST node
    std::shared_ptr<type_node> type;      // for variables & params
    function_decl* func = nullptr;        // for functions
    let_stmt* let = nullptr;              // for variable decls
    param* param_decl = nullptr;          // for parameters

    explicit symbol(symbol_kind k, std::string n)
        : kind(k), name(std::move(n)) {}
};

} // namespace rex