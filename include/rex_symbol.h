#pragma once
#include <string>
#include <memory>
#include "rex_ast.h"
#include "rex_types.h"


namespace rex {

// ------------------ SYMBOL KIND -------------------

enum class SymbolType{
    variable,
    function,
    typealias
};

// ------------------ SYMBOL -----------------------

struct Symbol {
    SymbolType symbol_type;          // store wether it is a variable (let stmt), function, or type alias 
    std::string name;                // Store the name of the variable, type alias, or 
    std::shared_ptr<rex::Type> type; // Will have all of the type information
    // to preserve the exp value (could be a binary, array, tuple, index, and etc expression. We would have to dynamic cast). 
    // I will prob not use this because the AST will hold i
    std::shared_ptr<rex::Expr> exp;  



    explicit Symbol(SymbolType k, std::string n)
        : symbol_type(k), name(std::move(n)) {}
};

} // namespace rex