  
  #include "passes/rex_exp_pass.h"
#include "rex_exps.h"
#include "rex_ops.h"
#include "rex_types.h"
#include <memory>
#include <stdexcept>

  namespace rex {

    
    std::shared_ptr<Type> ExprPass::visitExpr(const std::shared_ptr<Expr> exp){
        if(auto lit = std::dynamic_pointer_cast<LiteralExpr>(exp))
            return visitLiteral(lit);
        if(auto id = std::dynamic_pointer_cast<IdExpr>(exp))
            return visitId(id);
        if(auto bin = std::dynamic_pointer_cast<BinaryExpr>(exp))
            return visitBinary(bin);
        if(auto tup = std::dynamic_pointer_cast<TupleExpr>(exp))
            return visitTuple(tup);
        if(auto idx = std::dynamic_pointer_cast<IndexExpr>(exp))
            return visitIndex(idx);
        if(auto call = std::dynamic_pointer_cast<CallExpr>(exp))
            return visitCall(call);
        if (auto ar = std::dynamic_pointer_cast<ArrayExpr>(exp))
            return  visitArray(ar);
        if (auto pip = std::dynamic_pointer_cast<PipeExpr>(exp))
            return visitPipe(pip);

        throw std::runtime_error("unknown expression type");



    }
    void ExprPass ::visitStmt(const std::shared_ptr<Stmt> stmt){

    }
    void ExprPass ::visitBlock(const std::shared_ptr<BlockExpr> block){

    }

    void ExprPass ::visitLetStmt(const std::shared_ptr<LetStmt> ls){

    }

    void ExprPass ::visitFunctionDecl(const std::shared_ptr<FunctionDecl> f){

    }

    std::shared_ptr<Type> ExprPass::visitLiteral(const std::shared_ptr<LiteralExpr> literal){
        return literal->type;
    }
    std::shared_ptr<Type> ExprPass::visitId(const std::shared_ptr<IdExpr> id){
        if(!id->resolved){
            throw std::runtime_error("Unresolved Identifier: " + id->name);
        }

        id->type = id->resolved->type;
        return id->resolved->type;
    }
    std::shared_ptr<Type> ExprPass::visitBinary(const std::shared_ptr<BinaryExpr> bexp){
        auto left = visitExpr(bexp->lhs);
        auto right = visitExpr(bexp->rhs);

        if (!left->equals(right))
            throw std::runtime_error("Type mismatch in binary op");

    }
    
    std::shared_ptr<Type> ExprPass::visitUnary (const std::shared_ptr<Type> uexp){}  
    
    

        
    std::shared_ptr<Type> ExprPass::visitTuple(const std::shared_ptr<TupleExpr> texp){
        std::vector<std::shared_ptr<Type>> types;
        for(auto e : texp->elements)
            types.push_back(visitExpr(e));
        auto t = std::make_shared<TupleType>();
        t->tuple_types = types;
        return t;

    }
    std::shared_ptr<Type> ExprPass::visitArray (const std::shared_ptr<ArrayExpr> aexp){}
    std::shared_ptr<Type> ExprPass::visitIndex (const std::shared_ptr<IndexExpr> iexp){
        auto base_type = visitExpr(iexp->base);
        visitExpr(iexp->index);

        auto arr = std::dynamic_pointer_cast<ArrayType>(base_type);
        if(!arr)
            throw std::runtime_error("Indexing Non-Array");
    
        iexp->type = arr->array_type;

        return iexp->type;

    }

    
    std::shared_ptr<Type> ExprPass::visitRangeExpr (const std::shared_ptr<Type> rexp){}
    std::shared_ptr<Type> ExprPass::visitCall (const std::shared_ptr<CallExpr> cexp){}
    std::shared_ptr<Type> ExprPass::visitPipe ( const std::shared_ptr<PipeExpr> pexp){}

    std::shared_ptr<Type> ExprPass::resolveExp(const std::shared_ptr<Expr> type){}
  }