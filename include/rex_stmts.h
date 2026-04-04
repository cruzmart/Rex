#pragma once
#include <iostream>
#include <memory>
#include <ostream>
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

enum class StmtKind {
    LetDecl,
    TypeDecl,
    Assign,
    Expr,
    While,
    For,
    Loop,
    If,
    Print,
    Return_Normal,
    Return_Expr,
    Break
};

enum class PatternType {
    Multiple,
    Single
};


// ---------------------- STATEMENTS BASE --------------------
struct Stmt : AstNode {
    StmtKind stmt_kind;
    Stmt() : AstNode(AstNodeKind::Stmt){}
    Stmt(StmtKind stmt) : AstNode(AstNodeKind::Stmt), stmt_kind(stmt) {}
    virtual ~Stmt() = default;
    void dump(std::ostream& os, int i) const override = 0;
    std::string stmt_string(){
        switch(stmt_kind){
            case StmtKind::LetDecl:
                return "Let";
            case StmtKind::TypeDecl:
                return "Type";
            case StmtKind::Assign:
                return "Assign";
            case StmtKind::Expr:
                return "Expr";
            case StmtKind::While:
                return "While";
            case StmtKind::For:
                return "For";
            case StmtKind::Loop:
                return "Loop";
            case StmtKind::If:
                return "If";
            case StmtKind::Return_Normal:
                return "Return_Normal";
            case StmtKind::Return_Expr:
                return "Return_Expr";
            case StmtKind::Break:
                return "Break";
            default:
                    return "<?>";
        }
    }
};

// ---------------------- PATTERNS ---------------------------
struct Pattern {
    PatternType pat_type;
    Pattern(PatternType pat) : pat_type(pat) {}
    virtual ~Pattern() = default;
    virtual std::string to_string() const {
        return "<id(s)>";
    }
};

struct PatternId : Pattern {
    std::string id;
    PatternId() : Pattern(PatternType::Single) {}
    PatternId(std::string id) : Pattern(PatternType::Single), id(std::move(id)) {}
    std::string to_string() const override {
        return id;
    }
};

struct PatternIds : Pattern {
    std::vector<std::string> ids;
    PatternIds() : Pattern(PatternType::Multiple) {}
    PatternIds(std::vector<std::string> ids) : Pattern(PatternType::Multiple), ids(ids) {}

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
    LetStmt() : Stmt(StmtKind::LetDecl) {}
    void dump(std::ostream& os, int i) const override;
};

struct TypeDecl : Stmt {
    std::string name;
    std::shared_ptr<Type> type;
    std::shared_ptr<Symbol> resolved;
    TypeDecl() : Stmt(StmtKind::TypeDecl) {}
    void dump(std::ostream& os, int i) const override;
};

struct AssignStmt : Stmt {
    std::shared_ptr<Expr> target;
    std::shared_ptr<Expr> value;
    AssignStmt() : Stmt(StmtKind::Assign) {}
    void dump(std::ostream& os, int i) const override;
};

struct ReturnStmt : Stmt {
    std::shared_ptr<Expr> value;
    ReturnStmt() : Stmt(StmtKind::Return_Normal) {}
    void dump(std::ostream& os, int i) const override;
};

struct ExprStmt : Stmt {
    std::shared_ptr<Expr> value;
    ExprStmt() : Stmt(StmtKind::Return_Expr) {}
    void dump(std::ostream& os, int i) const override;
};

struct WhileStmt : Stmt {
    std::shared_ptr<Expr> cond;
    std::shared_ptr<BlockExpr> body;
    WhileStmt() : Stmt(StmtKind::While) {}
    void dump(std::ostream& os, int i) const override;
};

struct ForStmt : Stmt {
    std::shared_ptr<Expr> iter_var;
    std::shared_ptr<Expr> iterable;
    std::shared_ptr<BlockExpr> body;
    std::shared_ptr<Symbol> resolved;
    ForStmt() : Stmt(StmtKind::For) {}
    void dump(std::ostream& os, int i) const override;
};

struct LoopStmt : Stmt {
    std::shared_ptr<BlockExpr> body;
    LoopStmt() : Stmt(StmtKind::Loop) {}
    void dump(std::ostream& os, int i) const override;
};

struct IfStmt : Stmt {
    std::shared_ptr<Expr> condition;
    std::shared_ptr<BlockExpr> then_block;
    std::vector<std::pair<std::shared_ptr<Expr>, std::shared_ptr<BlockExpr>>> elifx_blocks;
    std::shared_ptr<BlockExpr> else_block;
    IfStmt() : Stmt(StmtKind::If) {}
    void dump(std::ostream& os, int i) const override;
};

struct PrintStmt : Stmt {
    std::shared_ptr<Expr> argument;
    PrintStmt() : Stmt(StmtKind::Print) {}
    void dump(std::ostream& os, int i) const override;
};

struct BreakStmt : Stmt {
    BreakStmt() : Stmt(StmtKind::Break) {}
    void dump(std::ostream& os, int i) const override;
};

} // namespace rex