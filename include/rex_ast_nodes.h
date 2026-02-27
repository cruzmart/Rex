#pragma once
#include "rex_ast.h"
#include <iostream>
#include <memory>

#include "rex_symbol.h"
#include "rex_types.h"

namespace rex {

// ---------------------- ENUMS -----------------------

enum class BinaryOp{
    ADD, SUB, MUL, DIV, MOD, EQ, NEQ, LT, GT, LE, GE, AND, OR, PIPE, RANGE
};

enum class UniOp{
    POS, NEG
};



// ------------------- FORWARD DECLARATIONS -------------------

struct FileAst;
struct TypeDelc;
struct TypeNode;
struct PrimType;
struct NamedType;
struct ArrayType;
struct SliceType;
struct TupleType;

struct Stmt;
struct LetStmt;
struct AssignStmt;
struct ReturnStmt;
struct ExprStmt;
struct WhileStmt;
struct ForStmt;
struct LoopStmt;
struct IfStmt;

struct Expr;
struct BlockExpr;
struct IdExpr;
struct LiteralExpr;
struct BinaryExpr;
struct UnaryExpr;
struct CallExpr;
struct IndexExpr;
struct TupleExpr;
struct RangeExpr;
struct PipeExpr;


struct Parameter;
struct FunctionDecl;

// ---------------------- UTILITY FUNCTIONS -------------------
inline const char* binop_name(BinaryOp o) {
    switch (o) {
        case BinaryOp::ADD:    return "+";
        case BinaryOp::SUB:    return "-";
        case BinaryOp::MUL:    return "*";
        case BinaryOp::DIV:    return "/";
        case BinaryOp::MOD:    return "%";
        case BinaryOp::EQ:     return "==";
        case BinaryOp::NEQ:    return "!=";
        case BinaryOp::LT:     return "<";
        case BinaryOp::GT:     return ">";
        case BinaryOp::LE:     return "<=";
        case BinaryOp::GE:     return ">=";
        case BinaryOp::AND:   return "and";
        case BinaryOp::OR:    return "or";
        case BinaryOp::PIPE:   return "|>";
        case BinaryOp::RANGE: return "..";
    }
    return "?";
}

inline const char* uniop_name(UniOp o){
     switch (o) {
        case UniOp::POS:    return "+";
        case UniOp::NEG:    return "-";
    }
    return "?";
}


// ---------------------- TOP LEVEL -----------------------
struct FileAst : AstNode {
    std::vector<std::shared_ptr<AstNode>> items;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "file\n";
        for (auto& it : items)
            it->dump(os, i + 1);
    }
};

// ---------------------- TYPES --------------------------

struct NamedType : TypeNode {
    void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "type aliased: " << type->alias << ")\n";
    }
};
struct ArrayType : TypeNode {

   void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "array type: " << type->to_string() << "\n";
    }


};
struct SliceType : TypeNode {

    void dump(std::ostream& os, int i) const override {
        indent(os,i);
        os << "slice type: " << type->to_string() << "\n";
    }

};
struct TupleType : TypeNode {

    void dump(std::ostream& os, int i) const override {
        indent(os,i);
        os << "tuple type: " << type->to_string() << "\n";
    }

};
struct PrimType  : TypeNode {

    void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "prim type: " << type->to_string()<< "\n";
    }

};
 

struct TypeDecl : TypeNode {
    symbol* resolved = nullptr; // will have to be saved in the symbole table
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "type decl " << type->alias << " -> " << type->to_string() << "\n";
    }
};


// ---------------------- BLOCKS ------------------------
struct BlockExpr : Expr {
    std::vector<std::shared_ptr<Stmt>> statements;
    std::shared_ptr<Expr> result;

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
struct Parameter : TypeNode {
    std::string name;

    // NEW:
    symbol* resolved = nullptr;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "param " << name << " -> " << type->to_string();

    }
    
};

struct FunctionDecl : AstNode {
    std::string name;
    std::shared_ptr<TypeNode> funcReturnType;
    std::vector<std::shared_ptr<Parameter>> params;
    std::shared_ptr<BlockExpr> body;

    // NEW:
    symbol* resolved = nullptr;

    void dump(std::ostream& os, int i) const override {
        
           indent(os, i);
        os << "func " << name << "\n";

        // Return type
        indent(os, i + 1);
        os << "return_type: \n";
        if (funcReturnType) 
            funcReturnType->dump(os, i + 2);
        else {
            indent(os, i + 2);
            os << "<void>\n";
        }

        // Parameters
        indent(os, i + 1);
        os << "params: \n";
        if (params.empty()) {
            indent(os, i + 2);
            os << "(none)\n";
        } else {
            for (auto &p : params)
                p->dump(os, i + 2);
        }

        // Body
        os << "\n";

        if (body)
            body->dump(os, i + 1);
        else {
            indent(os, i + 1);
            os << "(empty)\n";
        }
    }
};

// ---------------------- STATEMENTS --------------------
struct LetStmt {
    std::string variable_name;
    std::shared_ptr<Type> type_exp;
    std::shared_ptr<Expr> init_exp;

    // NEW:
    symbol* resolved = nullptr;

    void dump(std::ostream& os, int i) const {
        indent(os, i);
        os << "let\n";

        indent(os, i + 1);
        os << "name " << variable_name << "\n";

        indent(os, i + 1);
        if (type_exp)
            os << "explicit_type " << type_exp->to_string() << "\n";
        else
            os << "explicit_type <none>\n";

        indent(os, i + 1);
        os << "init\n";

        if (init_exp)
            init_exp->dump(os, i + 2);
        else {
            indent(os, i + 2);
            os << "<no init>\n";
        }
    }
};

struct LetStmts : Stmt {
    std::vector<std::shared_ptr<LetStmt>> letStmts;
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "let_stmt_group\n";

        for (auto& s : letStmts)
            s->dump(os, i + 1);
    }
};

struct AssignStmt : Stmt {
    std::shared_ptr<Expr> target;
    std::shared_ptr<Expr> value;
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "assign\n";
        target->dump(os, i + 1);
        value->dump(os, i + 1);
    }
};

struct ReturnStmt : Stmt {
    std::shared_ptr<Expr> value;
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "return\n";
        if (value) value->dump(os, i + 1);
    }
};

struct ExprStmt : Stmt {
    std::shared_ptr<Expr> value;
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "expr_stmt\n";
        value->dump(os, i + 1);
    }
};

struct WhileStmt : Stmt {
    std::shared_ptr<Expr> cond;
    std::shared_ptr<BlockExpr> body;
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "while\n";
        cond->dump(os, i + 1);
        body->dump(os, i + 1);
    }
};

struct ForStmt : Stmt {
    std::string iter_var;
    std::shared_ptr<Expr> iterable;
    std::shared_ptr<BlockExpr> body;


    // NEW for the iterable variable:
    symbol* resolved = nullptr;

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "for " << iter_var << " in\n";
        iterable->dump(os, i + 1);
        body->dump(os, i + 1);
    }
};

struct LoopStmt : Stmt {
    std::shared_ptr<BlockExpr> body;
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "loop\n";
        body->dump(os, i + 1);
    }
};

struct IfStmt : Stmt {
    std::shared_ptr<Expr> condition;
    std::shared_ptr<BlockExpr> then_block;
    std::vector<std::pair<std::shared_ptr<Expr>, std::shared_ptr<BlockExpr>>> elifx_blocks;
    std::shared_ptr<BlockExpr> else_block;

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
struct IdExpr : Expr {
    std::string name;
    // NEW:
    symbol* resolved = nullptr;
    void dump(std::ostream& os, int i) const override { indent(os, i); os << "id " << name << "\n"; }
};

struct LiteralExpr : Expr {
    std::string value;
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "literal_value " << value << " : " << type->to_string() << "\n";
    }
};

struct UnaryExpr : Expr {
    UniOp operation;
    std::shared_ptr<Expr> rhs;
    void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "unary " << uniop_name(operation) << "\n";
        rhs->dump(os, i + 1);
    }
};

struct BinaryExpr : Expr {
    
    BinaryOp operation;
    std::shared_ptr<Expr> lhs;
    std::shared_ptr<Expr> rhs;
    void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "binary " << binop_name(operation) << " " << type->to_string() << "\n";
        lhs->dump(os, i + 1);
        rhs->dump(os, i + 1);
    }
};

struct RangeExpr: BinaryExpr{

     void dump(std::ostream& os, int i) const override {
        indent(os, i); 
        os << "ranges:" << "\n";
        lhs->dump(os, i + 1);
        rhs->dump(os, i + 1);
    }
};

struct PipeExpr: BinaryExpr{

     void dump(std::ostream& os, int i) const override {
        indent(os, i); 
        os << "pipe:" << "\n";
        lhs->dump(os, i + 1);
        rhs->dump(os, i + 1);
    }
};
struct CallExpr : Expr {
    std::string callee;
    std::vector<std::shared_ptr<Expr>> args;
    // NEW:
    symbol* resolved = nullptr;
    void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "call " << callee << "\n";
        for (auto& a : args) a->dump(os, i + 1);
    }
};

struct IndexExpr : Expr {
    std::shared_ptr<Expr> base;
    std::shared_ptr<Expr> index;
    void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "index\n";
        base->dump(os, i + 1);
        index->dump(os, i + 1);
    }
};

struct TupleExpr : Expr {
    std::vector<std::shared_ptr<Expr>> elements;
    void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "tuple\n";
        for (auto& e : elements) e->dump(os, i + 1);
    }
};

struct ArrayExpr : Expr {
    std::vector<std::shared_ptr<Expr>> elements;
    void dump(std::ostream& os, int i) const override {
        indent(os, i); os << "array:\n";
        for (auto& e : elements) e->dump(os, i + 1);
    }
};

} // namespace rex
