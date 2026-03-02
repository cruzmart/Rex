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

struct Expr : AstNode {
    std::shared_ptr<Type> type;
    virtual ~Expr() = default;
    virtual void dump(std::ostream& os, int i) const override = 0;
};

struct BlockExpr : Expr {
    std::vector<std::shared_ptr<Stmt>> statements;
    std::shared_ptr<Expr> result;
    void dump(std::ostream& os, int i) const override;
};

// Other expression declarations...
struct IdExpr : Expr { std::string name; std::shared_ptr<Symbol> resolved; void dump(std::ostream& os, int i) const override; };
struct LiteralExpr : Expr { std::string value; void dump(std::ostream& os, int i) const override; };
struct UnaryExpr : Expr { UniOp operation; std::shared_ptr<Expr> rhs; void dump(std::ostream& os, int i) const override; };
struct BinaryExpr : Expr { BinaryOp operation; std::shared_ptr<Expr> lhs, rhs; void dump(std::ostream& os, int i) const override; };
struct RangeExpr : BinaryExpr { void dump(std::ostream& os, int i) const override; };
struct PipeExpr : BinaryExpr { void dump(std::ostream& os, int i) const override; };
struct CallExpr : Expr { std::string callee; std::vector<std::shared_ptr<Expr>> args; void dump(std::ostream& os, int i) const override; };
struct IndexExpr : Expr { std::shared_ptr<Expr> base, index; void dump(std::ostream& os, int i) const override; };
struct TupleExpr : Expr { std::vector<std::shared_ptr<Expr>> elements; void dump(std::ostream& os, int i) const override; };
struct ArrayExpr : Expr { std::vector<std::shared_ptr<Expr>> elements; void dump(std::ostream& os, int i) const override; };

} // namespace rex