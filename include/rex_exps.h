#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "rex_ast.h"

namespace rex {

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
struct Stmt;
struct Type;
struct Symbol;
struct PrimType;
enum class UniOp;
enum class BinaryOp;


enum class ExprKind {
    Literal,
    Id,
    Unary,
    Binary,
    Tuple,
    Array,
    Index,
    Call,
    Range,
    Pipe,
    Block,
    Null
};

struct Expr : AstNode {

    ExprKind exp_kind;
    std::shared_ptr<Type> type;
    Expr() : exp_kind(ExprKind::Null) {}
    Expr(ExprKind kind) : exp_kind(kind) {}

    virtual ~Expr() = default;
    virtual void dump(std::ostream& os, int i) const override = 0;
    std::string expr_string() {
           switch(exp_kind)
        {
            case ExprKind::Literal:
                return "Literal";
            case ExprKind::Id:
                return "Id";
            case ExprKind::Binary:
                return "Binary";
            case ExprKind::Tuple:
                return "Tuple";
            case ExprKind::Array:
                return "Array";
            case ExprKind::Index:
                return "Index";
            case ExprKind::Call:
                return "Call";
            case ExprKind::Pipe:
                return "Pipe";
            default:
                return "<?>";
        }
    }
};

struct BlockExpr : Expr {
    std::vector<std::shared_ptr<Stmt>> statements;
    std::shared_ptr<Expr> result;
    BlockExpr() : Expr(ExprKind::Block) {}
    void dump(std::ostream& os, int i) const override;
};

// Other expression declarations...
struct IdExpr : Expr { 
    std::string name; std::shared_ptr<Symbol> resolved; 
    IdExpr() : Expr(ExprKind::Id) {}
    void dump(std::ostream& os, int i) const override; 
};
struct LiteralExpr : Expr {
     std::string value; 
     LiteralExpr() : Expr(ExprKind::Literal) {}
     void dump(std::ostream& os, int i) const override;
};
struct UnaryExpr : Expr { 
    UniOp operation;
    std::shared_ptr<Expr> rhs;
    UnaryExpr() : Expr(ExprKind::Unary) {}
    void dump(std::ostream& os, int i) const override;
};
struct BinaryExpr : Expr { 
    BinaryOp operation; 
    std::shared_ptr<Expr> lhs, rhs; 
    BinaryExpr() : Expr(ExprKind::Binary) {}
    BinaryExpr(ExprKind kind) : Expr(kind) {}
    void dump(std::ostream& os, int i) const override; 
};
struct RangeExpr : BinaryExpr { 
    RangeExpr() : BinaryExpr(ExprKind::Range) {}
    void dump(std::ostream& os, int i) const override; 
};
struct PipeExpr : BinaryExpr { 
    PipeExpr() : BinaryExpr(ExprKind::Pipe) {}
    void dump(std::ostream& os, int i) const override; 
};
struct CallExpr : Expr {
     std::string callee; 
     std::vector<std::shared_ptr<Expr>> args; 
     CallExpr() : Expr(ExprKind::Call) {}
     void dump(std::ostream& os, int i) const override; 
};
struct IndexExpr : Expr { 
    std::shared_ptr<Expr> base, index; 
    IndexExpr() : Expr(ExprKind::Index) {}
    void dump(std::ostream& os, int i) const override; 
};
struct TupleExpr : Expr { 
    std::vector<std::shared_ptr<Expr>> elements; 
    TupleExpr() : Expr(ExprKind::Tuple) {}
    void dump(std::ostream& os, int i) const override; 
};
struct ArrayExpr : Expr { 
    std::vector<std::shared_ptr<Expr>> elements; 
    ArrayExpr() : Expr(ExprKind::Array) {}
    void dump(std::ostream& os, int i) const override; 
};

} // namespace rex