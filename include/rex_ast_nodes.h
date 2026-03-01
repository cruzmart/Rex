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

struct NamedType : Type {
    // type alias
    std::string alias;

    NamedType() : Type(TypeKind::Named) {}
    NamedType(std::string name) : Type(TypeKind::Named), alias(name){}
};

struct ArrayType : Type {

    // For Array / Slice
    std::shared_ptr<Type> array_type; 
    int size = -1;   

    ArrayType() : Type(TypeKind::Array), array_type(std::make_shared<Type>()){}       
    ArrayType(std::shared_ptr<Type> elem, int sz) : Type(TypeKind::Array), array_type(elem), size(sz) {}

};

struct SliceType : Type {
     // For Array / Slice
    std::shared_ptr<Type> slice_type;

    SliceType() : Type(TypeKind::Slice), slice_type(std::make_shared<Type>()) {}       
    SliceType(std::shared_ptr<Type> elem) : Type(TypeKind::Slice), slice_type(elem) {}
};

struct RangeType : Type {
    std::shared_ptr<Type> lower_type;
    std::shared_ptr<Type> upper_type;

    RangeType() : Type(TypeKind::Range), lower_type(std::make_shared<Type>()), upper_type(std::make_shared<Type>()) {}
    RangeType(std::shared_ptr<Type> l, std::shared_ptr<Type> u) : Type(TypeKind::Range), lower_type(l), upper_type(u) {}
};

struct PipeType : Type {
    std::shared_ptr<Type> func_left_type;
    std::shared_ptr<Type> func_right_type;

    PipeType() : Type(TypeKind::Pipe) {}
    PipeType(std::shared_ptr<Type> l, std::shared_ptr<Type> u) : Type(TypeKind::Pipe),func_left_type(l), func_right_type(u) {}
};

struct TupleType : Type {

     // For Tuple
    std::vector<std::shared_ptr<Type>> tuple_types;

    TupleType() : Type(TypeKind::Tuple) {}

};
struct PrimType  : Type {
    enum class Prims { 
    Int,
    Bool,
    Char,
    Real,
    String};

    Prims prim_type;

    PrimType(Prims d) : Type(TypeKind::Primitive), prim_type(d) {}
    PrimType() : Type(TypeKind::Primitive) {}

    std::string prim_to_string(){
        switch(prim_type){
            case Prims::Int: return "Int";
            case Prims::Bool: return "Bool";
            case Prims::Char: return "Char";
            case Prims::Real: return "Real";
            case Prims::String: return "String";
            default:
                return "<?>";
        }
    }
};

struct FunctionType : Type {
    std::vector<std::shared_ptr<Type>> params_type;
    std::shared_ptr<Type> return_type;

    FunctionType() : Type(TypeKind::Function) {}

    FunctionType(std::vector<std::shared_ptr<Type>> params, std::shared_ptr<Type> ret)
        : Type(TypeKind::Function), params_type(std::move(params)), return_type(ret) {}
};
 
struct TypeDecl :Stmt {
    std::string name;                    // name of alias
    std::shared_ptr<Type> type;             // type of the alias 
    std::shared_ptr<rex::Symbol> resolved = nullptr; // will have to be saved in the symbole table

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "new type:  " << name;
        if (type) {
            os << " -> " << type->to_string();
        } else {
            os << " -> <error>";
        }
        if (resolved) {
            os << " [resolved]";
        }
        os << "\n";
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
struct Parameter : AstNode {
    std::string para_name;
    std::shared_ptr<Type> para_type;

    // NEW:
    std::shared_ptr<rex::Symbol> resolved = nullptr;

    Parameter() :para_name("") , para_type(std::make_shared<Type>()) {}
    Parameter(std::string name, std::shared_ptr<Type> type) : para_name(name), para_type(type) {}

     void dump(std::ostream& os, int i) const override {
            indent(os, i);
    }
};

struct FunctionDecl : AstNode {
    std::string func_name;
    std::shared_ptr<FunctionType> func_type;
    std::vector<std::shared_ptr<Parameter>> parameters;
    std::shared_ptr<BlockExpr> body;          

    // NEW:
    std::shared_ptr<rex::Symbol> resolved = nullptr;

    void dump(std::ostream& os, int i) const override {}
};

// ---------------------- STATEMENTS --------------------
struct Pattern {
    virtual ~Pattern() = default; // makes it polymorphic, else I cannot dynamic cast 
};

struct PatternId : Pattern {
    std::string id;
    PatternId(std::string id) : id(id) {}
};

struct PatternIds : Pattern {
    std::vector<std::string> ids;
};

struct LetStmt : Stmt {

    std::shared_ptr<Pattern> id_pattern;
    std::shared_ptr<Type> type; 
    std::shared_ptr<Expr> exp; 

    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        os << "let:\n";

        // Dump pattern
        indent(os, i + 1);
        os << "pattern: ";
        if (auto p = std::dynamic_pointer_cast<PatternId>(id_pattern)) {
            os << p->id << "\n";
        } else if (auto p = std::dynamic_pointer_cast<PatternIds>(id_pattern)) {
            for (size_t idx = 0; idx < p->ids.size(); ++idx) {
                
                os << p->ids[idx];
                if (idx < p->ids.size() - 1) os << ", ";
            }
            os << "\n";
        } else {
            os << "<unknown pattern>\n";
        }

        // Dump type
        if (type) {
            indent(os, i + 1);
            os << "type: " << type->to_string() << "\n";
        }

        // Dump initializer
        if (exp) {
            indent(os, i + 1);
            os << "initializer:\n";
            exp->dump(os, i + 2);
        }
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
    std::shared_ptr<rex::Symbol> resolved = nullptr;

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
    std::shared_ptr<rex::Symbol> resolved = nullptr;

    IdExpr() {}
    IdExpr(std::string name) : name(name) {}
    void dump(std::ostream& os, int i) const override { indent(os, i); os << "id " << name << "\n"; }
};

struct LiteralExpr : Expr {
    std::string value;
    void dump(std::ostream& os, int i) const override {
        indent(os, i);
        
        if(auto prim = std::dynamic_pointer_cast<PrimType>(type)){
            os << "literal " << value << " -> " << prim->prim_to_string() <<" : " << type->to_string() << "\n";
        } else {
            os << "literal " << value << " -> <?> " << " : " << type->to_string() << "\n";
        }
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
        indent(os, i);
        os << "binary " << binop_name(operation) << " : ";
        if(type) os << type->to_string();
        else os << "<no type>";
        os << "\n";

        if(lhs) lhs->dump(os, i + 1);
        else indent(os, i + 1), os << "<null lhs>\n";

        if(rhs) rhs->dump(os, i + 1);
        else indent(os, i + 1), os << "<null rhs>\n";
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
