#pragma once
#include "rex_ast.h"
#include <iostream>
#include <memory>

namespace rex {

// ------------- Class Helpers -------------------
enum class binary_op {
    add, sub, mul, div, mod,
    eq, neq, lt, gt, le, ge,
    and_, or_, pipe, range_
};

// ---------- Forward declarations ----------
struct file_ast;
struct type_decl;
struct type_node;
struct primitive_type;
struct named_type;
struct array_type;
struct slice_type;
struct tuple_type;
struct pattern_node;

struct stmt;
struct let_stmt;
struct assign_stmt;
struct return_stmt;
struct expr_stmt;
struct while_stmt;
struct for_stmt;
struct loop_stmt;

struct expr;
struct block_expr;
struct id_expr;
struct literal_expr;
struct binary_expr;
const char* binop_name(binary_op);


struct call_expr;
struct index_expr;
struct tuple_expr;

struct param;
struct function_decl;

// ---------- Function Helpers -----------

inline const char* binop_name(binary_op o) {
    switch (o) {
        case binary_op::add:    return "+";
        case binary_op::sub:    return "-";
        case binary_op::mul:    return "*";
        case binary_op::div:    return "/";
        case binary_op::mod:    return "%";
        case binary_op::eq:     return "==";
        case binary_op::neq:    return "!=";
        case binary_op::lt:     return "<";
        case binary_op::gt:     return ">";
        case binary_op::le:     return "<=";
        case binary_op::ge:     return ">=";
        case binary_op::and_:   return "and";
        case binary_op::or_:    return "or";
        case binary_op::pipe:   return "|>";
        case binary_op::range_: return "..";
    }
    return "?";
}

// ---------- Top level ----------

struct file_ast : ast_node {
    std::vector<std::shared_ptr<ast_node>> items;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "file\n";
        for (auto& it : items)
            it->dump(os, i + 1);
    }
};

// ----------- Decleration ----------------

// type decleration
struct type_decl : ast_node {
    std::string name;                     // alias name
    std::shared_ptr<type_node> aliased;   // the real type. Type expression

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "type " << name << " = ";
        aliased->dump(os, 0);
    }
};

// ---------------------- Types -------------------

// --- Primitive Type ---
struct primitive_type : type_node {
    enum class prim_kind { int_, real_, bool_, char_, string_, void_ };
    prim_kind value;

    explicit primitive_type(prim_kind k)
        : type_node(kind::primitive), value(k) {}

    static prim_kind from_name(const std::string& n) {
        if (n == "Int") return prim_kind::int_;
        if (n == "Real") return prim_kind::real_;
        if (n == "Bool") return prim_kind::bool_;
        if (n == "Char") return prim_kind::char_;
        if (n == "String") return prim_kind::string_;
        return prim_kind::void_;
    }

    void dump(std::ostream& os, int) const override {
        switch (value) {
            case prim_kind::int_:    os << "Int"; break;
            case prim_kind::real_:   os << "Real"; break;
            case prim_kind::bool_:   os << "Bool"; break;
            case prim_kind::char_:   os << "Char"; break;
            case prim_kind::string_: os << "String"; break;
            case prim_kind::void_:   os << "Void"; break;
        }
       
    }
};


// --- Named Type ---
struct named_type : type_node {
    std::string name;

    explicit named_type(std::string n)
        : type_node(kind::named), name(std::move(n)) {}

    void dump(std::ostream& os, int) const override {
        os << "(" << name << ")";
         os << "\n";
    }
};



// --- Array Type ---
struct array_type : type_node {
    std::shared_ptr<type_node> element;
    int size;

    array_type(std::shared_ptr<type_node> elem, int s)
        : type_node(kind::array), element(std::move(elem)), size(s) {}

    void dump(std::ostream& os, int) const override {
        element->dump(os, 0);
        os << "[" << size << "]";
         os << "\n";
    }
};

// --- Slice Type ---
struct slice_type : type_node {
    std::shared_ptr<type_node> element;

    explicit slice_type(std::shared_ptr<type_node> elem)
        : type_node(kind::slice), element(std::move(elem)) {}

    void dump(std::ostream& os, int) const override {
        element->dump(os, 0);
        os << "[]";
        os << "\n";
    }
};
// --- Tuple Type ---
struct tuple_type : type_node {
    std::vector<std::shared_ptr<type_node>> elements;

    tuple_type() : type_node(kind::tuple) {}

    void dump(std::ostream& os, int) const override {
        os << "(";
        for (size_t i = 0; i < elements.size(); ++i) {
            elements[i]->dump(os, 0);
            if (i + 1 < elements.size()) os << ", ";
        }
        os << ")";
         os << "\n";
    }
};

struct pattern_node : ast_node {
    std::string name; // valid if leaf
    std::vector<std::shared_ptr<pattern_node>> elements; // valid if tuple

    bool is_tuple() const { return !elements.empty(); } // To check if the pattern is a ID else a tuple of ID's

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        if (is_tuple()) {
            os << "pattern tuple\n";
            for (auto& e : elements)
                e->dump(os, i + 1);
        } else {
            os << "pattern " << name << "\n";
        }
    }
};



// ---------- Expressions ----------

struct block_expr : expr {
    std::vector<std::shared_ptr<stmt>> statements;
    std::shared_ptr<expr> result;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "block\n";
        for (auto& s : statements)
            s->dump(os, i + 1);
        if (result) {
            indent(os, i + 1);
            os << "result\n";
            result->dump(os, i + 2);
        }
    }
};

// ----------- Function Structs ----------------------------- 
struct param {
    std::string name;
    std::shared_ptr<type_node> type; // <- use type_node, not primitive_type
};


struct function_decl : ast_node {
    std::string name;
    std::shared_ptr<type_node> func_return_type; // <- type_node
    std::vector<param> params;
    std::shared_ptr<block_expr> body;

void dump(std::ostream& os, int i) const override {
    indent(os, i);
    os << "function " << name << "(";

    for (size_t p = 0; p < params.size(); ++p) {
        os << params[p].name << " : ";
        params[p].type->dump(os, 0);
        if (p + 1 < params.size()) os << ", ";
    }

    os << ") -> ";
    if (func_return_type)
        func_return_type->dump(os, 0);
    else
        os << "Void";

    os << "\n";
    body->dump(os, i + 1);
}
};

// ---------- Statements ----------

struct let_stmt : stmt {
    std::string name;
    std::shared_ptr<type_node> explicit_type; // may be null
    std::shared_ptr<expr> init;

 void dump(std::ostream& os, int i) const override {
    indent(os, i);
    os << "let " << name;

    if (explicit_type) {
        os << " : ";
        explicit_type->dump(os, 0);
    }

    os << "\n";
    init->dump(os, i + 1);
}
};

struct assign_stmt : stmt {
    std::shared_ptr<expr> target;
    std::shared_ptr<expr> value;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "assign\n";
        target->dump(os, i + 1);
        value->dump(os, i + 1);
    }
};

struct return_stmt : stmt {
    std::shared_ptr<expr> value;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "return\n";
        if (value) value->dump(os, i + 1);
    }
};

struct expr_stmt : stmt {
    std::shared_ptr<expr> value;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "expr_stmt\n";
        value->dump(os, i + 1);
    }
};

struct while_stmt : stmt {
    std::shared_ptr<expr> cond;
    std::shared_ptr<block_expr> body;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "while\n";
        cond->dump(os, i + 1);
        body->dump(os, i + 1);
    }
};

struct for_stmt : stmt {
    std::string iter_var;
    std::shared_ptr<expr> iterable;
    std::shared_ptr<block_expr> body;

    void dump(std::ostream& os, int i) const override {
       indent(os, i);
        os << "for " << iter_var << " in\n";
        iterable->dump(os, i + 1);    // print the iterable expression
        body->dump(os, i + 1);        // print the loop body
    }
};

struct loop_stmt : stmt {   // infinite loop
    std::shared_ptr<block_expr> body;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "loop\n";
        body->dump(os, i + 1);   // print the block body
    }
};



// ---------- Expressions ----------

struct id_expr : expr {
    std::string name;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "id " << name << "\n";
    }
};

struct literal_expr : expr {
    enum class kind { int_lit, real_lit, bool_lit, char_lit, string_lit, null_lit };
    kind value_kind;
    std::string text;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "literal ";
        switch (value_kind) {
            case kind::int_lit:    os << "Int "; break;
            case kind::real_lit:   os << "Real "; break;
            case kind::bool_lit:   os << "Bool "; break;
            case kind::char_lit:   os << "Char "; break;
            case kind::string_lit: os << "String "; break;
            case kind::null_lit:   os << "Null "; break;
        }
        os << text << "\n";
    }
};


struct binary_expr : expr {

    binary_op operation;
    std::shared_ptr<expr> lhs;
    std::shared_ptr<expr> rhs;
    
    
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "binary " << binop_name(operation) << "\n";
        lhs->dump(os, i + 1);
        rhs->dump(os, i + 1);
    }
};


struct call_expr : expr {
    std::string callee; // This is the function name
    std::vector<std::shared_ptr<expr>> args; // This is the argument passed inside the function call

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "call " << callee << "\n";
        for (auto& a : args)
            a->dump(os, i + 1);
    }
};

struct index_expr : expr {
    std::shared_ptr<expr> base;
    std::shared_ptr<expr> index;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "index\n";
        base->dump(os, i + 1);
        index->dump(os, i + 1);
    }
};

struct tuple_expr : expr {
    std::vector<std::shared_ptr<expr>> elements;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "tuple\n";
        for (auto& e : elements)
            e->dump(os, i + 1);
    }
};



// Utilies for AST nodes

inline literal_expr::kind classify_literal(const std::string& txt) {

    if(txt.empty()) 
        return literal_expr::kind::null_lit;
    if (txt.size() >= 2 && txt.front() == '"' && txt.back() == '"')
        return literal_expr::kind::string_lit;

    if (txt.size() >= 3 && txt.front() == '\'' && txt.back() == '\'')
        return literal_expr::kind::char_lit;

    if (txt == "true" || txt == "false")
        return literal_expr::kind::bool_lit;

    if (txt.find('.') != std::string::npos)
        return literal_expr::kind::real_lit;

    return literal_expr::kind::int_lit;
}


} // namespace rex
