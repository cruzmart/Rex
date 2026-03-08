  
#include "passes/rex_exp_pass.h"
#include "rex_ast_nodes.h"
#include "rex_exps.h"
#include "rex_funcs.h"
#include "rex_ops.h"
#include "rex_stmts.h"
#include "rex_types.h"
#include <memory>
#include <stdexcept>
#include <string>
#include "rex_operator_type_system.h"

  namespace rex {

    // Helpers //////

    bool ExprPass::is_tuple_type(const std::shared_ptr<Type> T){
        return std::dynamic_pointer_cast<TupleType>(T) != nullptr;
    }

    bool ExprPass::is_tuple_exp(const std::shared_ptr<Expr> T){
        return std::dynamic_pointer_cast<TupleExpr>(T) != nullptr;
    }





    void ExprPass::visit(const std::shared_ptr<FileAst> file){
        for(auto item : file->items){
            if(auto stmt = std::dynamic_pointer_cast<Stmt>(item)){
                visitStmt(stmt);
            }
            if(auto func = std::dynamic_pointer_cast<FunctionDecl>(item)){
                visitFunctionDecl(func);
            }

            if(auto as_stmt = std::dynamic_pointer_cast<AssignStmt>(item)){
                visitAsgStmt(as_stmt);
            }
        }
    
    }


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

        if(auto let = std::dynamic_pointer_cast<LetStmt>(stmt)){
            visitLetStmt(let);
        }

        if(auto as = std::dynamic_pointer_cast<AssignStmt>(stmt)){

        }
        if(auto es = std::dynamic_pointer_cast<ExprStmt>(stmt)){

        }
        
        if(auto ws = std::dynamic_pointer_cast<WhileStmt>(stmt)){

        }
        if(auto fs = std::dynamic_pointer_cast<ForStmt>(stmt)){

        }
        if(auto is = std::dynamic_pointer_cast<IfStmt>(stmt)){

        }

    }

    void ExprPass ::visitBlock(const std::shared_ptr<BlockExpr> block){

    }

    void ExprPass ::visitLetStmt(const std::shared_ptr<LetStmt> ls){

        // check what type of pattern is the id
        if(auto id = std::dynamic_pointer_cast<PatternId>(ls->id_pattern)){
            auto sym = std::make_shared<Symbol>(SymbolType::variable, id->id);
            sym->type = ls->type;
            ls->exp->type = visitExpr(ls->exp);
            sym->expr = ls->exp;

            current_scope->define(sym);

            return;
        } 

        else if (auto ids = std::dynamic_pointer_cast<PatternIds>(ls->id_pattern))

        {
            auto types = std::dynamic_pointer_cast<TupleType>(ls->type);
            auto exprs = std::dynamic_pointer_cast<TupleExpr>(ls->exp);

            for(size_t i = 0; i < ids->ids.size(); i++){
                auto sym = std::make_shared<Symbol>(SymbolType::variable, ids->ids[i]);
                sym->type = types->tuple_types[i];
                sym->expr = exprs->elements[i];

                current_scope->define(sym);
            }
        }

    }

    void ExprPass ::visitFunctionDecl(const std::shared_ptr<FunctionDecl> f){

          auto sym = std::make_shared<Symbol>(SymbolType::function, f->func_name);
           sym->type = f->func_type;

           // in the body solve every expression in it
           for(auto stmt : f->body->statements){
                visitStmt(stmt);
           }


            sym->expr = f->body;
            current_scope->define(sym);

    }

    void ExprPass::visitAsgStmt(const std::shared_ptr<AssignStmt> as){}

    void ExprPass::visitExprStmt(const std::shared_ptr<ExprStmt> es){}
    void ExprPass::visitWhileStmt(const std::shared_ptr<WhileStmt> ws){}
    void ExprPass::visitForStmt(const std::shared_ptr<ForStmt> fs){}
    void ExprPass::visitIfStmt(const std::shared_ptr<IfStmt> is){}
   

    std::shared_ptr<Type> ExprPass::visitLiteral(const std::shared_ptr<LiteralExpr> literal){
        return literal->type;
    }
    std::shared_ptr<Type> ExprPass::visitId(const std::shared_ptr<IdExpr> id){

        if(!id->resolved){
            id->resolved = current_scope->resolve(id->name);
            id->type = id->resolved->type;
        } 

        id->type = id->resolved->type;

        return id->resolved->type;
    }
    std::shared_ptr<Type> ExprPass::visitBinary(const std::shared_ptr<BinaryExpr> bexp){
        bexp->type = ots.check_binary(bexp->operation, visitExpr(bexp->lhs), visitExpr(bexp->rhs));
        return bexp->type;
    }
    
    std::shared_ptr<Type> ExprPass::visitUnary (const std::shared_ptr<Type> uexp){}  
    
        
    std::shared_ptr<Type> ExprPass::visitTuple(const std::shared_ptr<TupleExpr> texp){
        std::vector<std::shared_ptr<Type>> types;
        for(auto e : texp->elements)
            types.push_back(visitExpr(e));
        auto t = std::make_shared<TupleType>(types);
        return t;

    }
    
    std::shared_ptr<Type> ExprPass::visitArray (const std::shared_ptr<ArrayExpr> aexp){

        // we should check that ALL expressions within this array are ALL of the same type
        // because now in expressions, we are giving it actual information

        int size = aexp->elements.size(); // get the size of all of the elements in it

        if(size){
            for(auto exp : aexp->elements)
                exp->type = visitExpr(exp);
            aexp -> type = std::make_shared<ArrayType>(aexp->elements[0]->type, size);
        }
        else
            aexp -> type = std::make_shared<ArrayType>(std::make_shared<PrimType>(PrimType::Prims::Null));

        
        return aexp->type;

    }

    std::shared_ptr<Type> ExprPass::visitIndex (const std::shared_ptr<IndexExpr> iexp){
        auto base_type = visitExpr(iexp->base);
        auto index_type = visitExpr(iexp->index);

        if(!ots.is_array(base_type))
            throw std::runtime_error("Indexing Non-Array");
        if(!ots.is_integer(index_type))
            throw std::runtime_error("Index value is not of type INT");
    
        iexp->type = std::dynamic_pointer_cast<ArrayType>(base_type)->array_type;

        return iexp->type;

    }

    
    std::shared_ptr<Type> ExprPass::visitRangeExpr (const std::shared_ptr<Type> rexp){}
    std::shared_ptr<Type> ExprPass::visitCall (const std::shared_ptr<CallExpr> cexp){}
    std::shared_ptr<Type> ExprPass::visitPipe ( const std::shared_ptr<PipeExpr> pexp){}

    std::shared_ptr<Type> ExprPass::resolveExp(const std::shared_ptr<Expr> type){}
  }