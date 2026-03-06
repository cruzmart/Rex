#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "rex_ast.h"
#include "rex_types.h" 


namespace rex {

struct Expr;
struct BlockExpr;
struct Stmt;
struct Pattern;
struct PatternId;
struct PatternIds;
struct Symbol;
struct Type;

// ---------------------- STATEMENTS BASE --------------------
struct Stmt : AstNode {
    virtual ~Stmt() = default;
    virtual void dump(std::ostream& os, int i) const override = 0;
};

// ---------------------- PATTERNS ---------------------------
struct Pattern {
    virtual ~Pattern() = default;
    virtual std::string to_string() const {
        return "<id(s)>";
    }
};

struct PatternId : Pattern {
    std::string id;
    explicit PatternId(std::string id);
    std::string to_string() const override {
        return id;
    }
};

struct PatternIds : Pattern {
    std::vector<std::string> ids;

    std::string to_string() const override {

        std::string result = "(";
        for(size_t i = 0; i < ids.size(); ++i){
            result += ids[i];
            if(i + 1 < ids.size()){
                result += ", ";
            }
        }

        result += ")";

       

        return result;
    }
};

// ---------------------- STATEMENTS ------------------------
struct LetStmt : Stmt {
    std::shared_ptr<Pattern> id_pattern;
    std::shared_ptr<Type> type;
    std::shared_ptr<Expr> exp;
    void dump(std::ostream& os, int i) const override;
};

struct TypeDecl : Stmt {
    std::string name;
    std::shared_ptr<Type> type;
    std::shared_ptr<Symbol> resolved;
    void dump(std::ostream& os, int i) const override;
};

struct AssignStmt : Stmt {
    std::shared_ptr<Expr> target;
    std::shared_ptr<Expr> value;
    void dump(std::ostream& os, int i) const override;
};

struct ReturnStmt : Stmt {
    std::shared_ptr<Expr> value;
    void dump(std::ostream& os, int i) const override;
};

struct ExprStmt : Stmt {
    std::shared_ptr<Expr> value;
    void dump(std::ostream& os, int i) const override;
};

struct WhileStmt : Stmt {
    std::shared_ptr<Expr> cond;
    std::shared_ptr<BlockExpr> body;
    void dump(std::ostream& os, int i) const override;
};

struct ForStmt : Stmt {
    std::string iter_var;
    std::shared_ptr<Expr> iterable;
    std::shared_ptr<BlockExpr> body;
    std::shared_ptr<Symbol> resolved;
    void dump(std::ostream& os, int i) const override;
};

struct LoopStmt : Stmt {
    std::shared_ptr<BlockExpr> body;
    void dump(std::ostream& os, int i) const override;
};

struct IfStmt : Stmt {
    std::shared_ptr<Expr> condition;
    std::shared_ptr<BlockExpr> then_block;
    std::vector<std::pair<std::shared_ptr<Expr>, std::shared_ptr<BlockExpr>>> elifx_blocks;
    std::shared_ptr<BlockExpr> else_block;
    void dump(std::ostream& os, int i) const override;
};

} // namespace rex