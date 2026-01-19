#pragma once
#include "rex_ast.h"
#include <iostream>
#include <memory>

namespace rex {

// ---------------------- ENUMS -----------------------
enum class binary_op {
    add, sub, mul, div, mod,
    eq, neq, lt, gt, le, ge,
    and_, or_, pipe, range_
};

enum class uni_op {
    pos, neg
};

 enum class literal_kind { 
    int_lit, real_lit, bool_lit,
    char_lit, string_lit, null_lit 
};

 enum class prim_type { 
    int_, real_, bool_, 
    char_, string_, void_ 
};

// ------------------- FORWARD DECLARATIONS -------------------
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
struct if_stmt;

struct expr;
struct block_expr;
struct id_expr;
struct literal_expr;
struct binary_expr;
struct unary_expr;
struct call_expr;
struct index_expr;
struct tuple_expr;

struct param;
struct function_decl;

// ---------------------- UTILITY FUNCTIONS -------------------
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

inline const char* uniop_name(uni_op o){
     switch (o) {
        case uni_op::pos:    return "+";
        case uni_op::neg:    return "-";
    }
    return "?";
}

inline literal_kind classify_literal(const std::string& txt) {
    if(txt.empty()) 
        return literal_kind::null_lit;
    if (txt.size() >= 2 && txt.front() == '"' && txt.back() == '"')
        return literal_kind::string_lit;
    if (txt.size() >= 3 && txt.front() == '\'' && txt.back() == '\'')
        return literal_kind::char_lit;
    if (txt == "true" || txt == "false")
        return literal_kind::bool_lit;
    if (txt.find('.') != std::string::npos)
        return literal_kind::real_lit;
    return literal_kind::int_lit;
}

// ---------------------- TOP LEVEL -----------------------
struct file_ast : ast_node {
    std::vector<std::shared_ptr<ast_node>> items;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "file\n";
        for (auto& it : items)
            it->dump(os, i + 1);
    }
};

// ---------------------- TYPES --------------------------
struct primitive_type : type_node {
    prim_type value;

    explicit primitive_type(prim_type k) : type_node(kind::primitive), value(k) {}

    static prim_type from_name(const std::string& n) {
        if (n == "Int") return prim_type::int_;
        if (n == "Real") return prim_type::real_;
        if (n == "Bool") return prim_type::bool_;
        if (n == "Char") return prim_type::char_;
        if (n == "String") return prim_type::string_;
        return prim_type::void_;
    }

    void dump(std::ostream& os, int) const override {
        switch (value) {
            case prim_type::int_:    os << "Int"; break;
            case prim_type::real_:   os << "Real"; break;
            case prim_type::bool_:   os << "Bool"; break;
            case prim_type::char_:   os << "Char"; break;
            case prim_type::string_: os << "String"; break;
            case prim_type::void_:   os << "Void"; break;
        }
    }
};

struct named_type : type_node {
    std::string name;
    explicit named_type(std::string n) : type_node(kind::named), name(std::move(n)) {}
    void dump(std::ostream& os, int) const override { os << "(" << name << ")\n"; }
};

struct array_type : type_node {
    std::shared_ptr<type_node> element;
    int size;
    array_type(std::shared_ptr<type_node> elem, int s) : type_node(kind::array), element(std::move(elem)), size(s) {}
    void dump(std::ostream& os, int) const override { element->dump(os, 0); os << "[" << size << "]\n"; }
};

struct slice_type : type_node {
    std::shared_ptr<type_node> element;
    explicit slice_type(std::shared_ptr<type_node> elem) : type_node(kind::slice), element(std::move(elem)) {}
    void dump(std::ostream& os, int) const override { element->dump(os, 0); os << "[]\n"; }
};

struct tuple_type : type_node {
    std::vector<std::shared_ptr<type_node>> elements;
    tuple_type() : type_node(kind::tuple) {}
    void dump(std::ostream& os, int) const override {
        os << "(";
        for (size_t i = 0; i < elements.size(); ++i) {
            elements[i]->dump(os, 0);
            if (i + 1 < elements.size()) os << ", ";
        }
        os << ")\n";
    }
};

struct type_decl : ast_node {
    std::string name;
    std::shared_ptr<type_node> aliased;
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "type " << name << " = ";
        aliased->dump(os, 0);
    }
};

// ---------------------- PATTERNS ----------------------
struct pattern_node : ast_node {
    std::string name;
    std::vector<std::shared_ptr<pattern_node>> elements;
    bool is_tuple() const { return !elements.empty(); }
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        if (is_tuple()) {
            os << "pattern tuple\n";
            for (auto& e : elements) e->dump(os, i + 1);
        } else os << "pattern " << name << "\n";
    }
};

// ---------------------- BLOCKS ------------------------
struct block_expr : expr {
    std::vector<std::shared_ptr<stmt>> statements;
    std::shared_ptr<expr> result;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "block\n";
        for (auto& s : statements) s->dump(os, i + 1);
        if (result) {
            indent(os, i + 1);
            os << "result\n";
            result->dump(os, i + 2);
        }
    }
};

// ---------------------- FUNCTIONS ---------------------
struct param {
    std::string name;
    std::shared_ptr<type_node> type;
};

struct function_decl : ast_node {
    std::string name;
    std::shared_ptr<type_node> func_return_type;
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
        if (func_return_type) func_return_type->dump(os, 0);
        else os << "Void";
        os << "\n";
        body->dump(os, i + 1);
    }
};

// ---------------------- STATEMENTS --------------------
struct let_stmt : stmt {
    std::string name;
    std::shared_ptr<type_node> explicit_type;
    std::shared_ptr<expr> init;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "let " << name;
        if (explicit_type) { os << " : "; explicit_type->dump(os, 0); }
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
        iterable->dump(os, i + 1);
        body->dump(os, i + 1);
    }
};

struct loop_stmt : stmt {
    std::shared_ptr<block_expr> body;
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "loop\n";
        body->dump(os, i + 1);
    }
};

struct if_stmt : stmt {
    std::shared_ptr<expr> condition;
    std::shared_ptr<block_expr> then_block;
    std::vector<std::pair<std::shared_ptr<expr>, std::shared_ptr<block_expr>>> elifx_blocks;
    std::shared_ptr<block_expr> else_block;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "if\n";
        indent(os, i + 1); os << "condition\n"; condition->dump(os, i + 2);
        indent(os, i + 1); os << "then\n"; then_block->dump(os, i + 2);

        for (const auto& [elifCond, elifBlock] : elifx_blocks) {
            indent(os, i + 1); os << "elifx\n";
            indent(os, i + 2); os << "condition\n"; elifCond->dump(os, i + 3);
            indent(os, i + 2); os << "then\n"; elifBlock->dump(os, i + 3);
        }

        if (else_block) { indent(os, i + 1); os << "else\n"; else_block->dump(os, i + 2); }
    }
};

// ---------------------- EXPRESSIONS --------------------
struct id_expr : expr {
    std::string name;
    void dump(std::ostream& os, int i) const override { indent(os, i); os << "id " << name << "\n"; }
};

struct literal_expr : expr {
    enum class kind { int_lit, real_lit, bool_lit, char_lit, string_lit, null_lit };
    literal_kind value_kind;
    std::string text;
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "literal ";
        switch (value_kind) {
            case literal_kind::int_lit:    os << "Int "; break;
            case literal_kind::real_lit:   os << "Real "; break;
            case literal_kind::bool_lit:   os << "Bool "; break;
            case literal_kind::char_lit:   os << "Char "; break;
            case literal_kind::string_lit: os << "String "; break;
            case literal_kind::null_lit:   os << "Null "; break;
        }
        os << text << "\n";
    }
};

struct unary_expr : expr {
    uni_op operation;
    std::shared_ptr<expr> rhs;
    void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "unary " << uniop_name(operation) << "\n";
        rhs->dump(os, i + 1);
    }
};

struct binary_expr : expr {
    binary_op operation;
    std::shared_ptr<expr> lhs;
    std::shared_ptr<expr> rhs;
    void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "binary " << binop_name(operation) << "\n";
        lhs->dump(os, i + 1);
        rhs->dump(os, i + 1);
    }
};

struct call_expr : expr {
    std::string callee;
    std::vector<std::shared_ptr<expr>> args;
    void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "call " << callee << "\n";
        for (auto& a : args) a->dump(os, i + 1);
    }
};

struct index_expr : expr {
    std::shared_ptr<expr> base;
    std::shared_ptr<expr> index;
    void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "index\n";
        base->dump(os, i + 1);
        index->dump(os, i + 1);
    }
};

struct tuple_expr : expr {
    std::vector<std::shared_ptr<expr>> elements;
    void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "tuple\n";
        for (auto& e : elements) e->dump(os, i + 1);
    }
};

} // namespace rex
