#pragma once
#include "rex_ast.h"

namespace rex {

// ---------- Forward declarations ----------
struct file_ast;
struct type_decl;
struct function_decl;

struct stmt;
struct expr;

struct block_expr;
struct id_expr;
struct literal_expr;
struct binary_expr;
struct call_expr;
struct index_expr;
struct tuple_expr;

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

struct type_decl : ast_node {
    std::string name;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "type_decl " << name << "\n";
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

struct function_decl : ast_node {
    std::string name;
    std::vector<std::string> params;
    std::shared_ptr<block_expr> body;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "function " << name << "(";
        for (size_t p = 0; p < params.size(); ++p) {
            os << params[p];
            if (p + 1 < params.size()) os << ", ";
        }
        os << ")\n";
        body->dump(os, i + 1);
    }
};

// ---------- Statements ----------

struct let_stmt : stmt {
    std::string name;
    std::shared_ptr<expr> init;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "let " << name << "\n";
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
            case kind::int_lit:    os << "int "; break;
            case kind::real_lit:   os << "real "; break;
            case kind::bool_lit:   os << "bool "; break;
            case kind::char_lit:   os << "char "; break;
            case kind::string_lit: os << "string "; break;
            case kind::null_lit:   os << "null "; break;
        }
        os << text << "\n";
    }
};

struct binary_expr : expr {
    enum class op {
        add, sub, mul, div, mod,
        eq, neq, lt, gt, le, ge,
        and_, or_, pipe
    };

    op operation;
    std::shared_ptr<expr> lhs;
    std::shared_ptr<expr> rhs;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "binary ";
        os << static_cast<int>(operation) << "\n";
        lhs->dump(os, i + 1);
        rhs->dump(os, i + 1);
    }
};

struct call_expr : expr {
    std::string callee;
    std::vector<std::shared_ptr<expr>> args;

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
