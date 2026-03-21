  
#include "passes/rex_exp_pass.h"
#include "rex_ast.h"
#include "rex_ast_nodes.h"
#include "rex_exps.h"
#include "rex_funcs.h"
#include "rex_ops.h"
#include "rex_stmts.h"
#include "rex_symbol.h"
#include "rex_types.h"
#include <memory>
#include <stdexcept>
#include <string>
#include "rex_binary_op.h"

  namespace rex {



    // Helpers //////

    void ExprPass::print(const std::string& msg) const {
    if (debug)
        std::cout << "[ExprPass] " << msg << "\n";
    }

    // Good
    void ExprPass::visit(const std::shared_ptr<FileAst> file){
        for(auto item : file->items){
            if(item->ast_kind == AstNodeKind::Stmt){
               auto stmt = std::static_pointer_cast<Stmt>(item);
                visitStmt(stmt);
            }
            if(item->ast_kind == AstNodeKind::FunctionDecl){
                auto func = std::static_pointer_cast<FunctionDecl>(item);
                visitFunctionDecl(func);
            }
        } 
    }
    //Good

    std::shared_ptr<Type> ExprPass::visitExpr(const std::shared_ptr<Expr> exp){
    
      switch(exp->exp_kind)
    {
        case ExprKind::Literal:
            return visitLiteral(std::static_pointer_cast<LiteralExpr>(exp));

        case ExprKind::Id:
            return visitId(std::static_pointer_cast<IdExpr>(exp));

        case ExprKind::Binary:
            return visitBinary(std::static_pointer_cast<BinaryExpr>(exp));

        case ExprKind::Tuple:
            return visitTuple(std::static_pointer_cast<TupleExpr>(exp));

        case ExprKind::Array:
            return visitArray(std::static_pointer_cast<ArrayExpr>(exp));

        case ExprKind::Index:
            return visitIndex(std::static_pointer_cast<IndexExpr>(exp));

        case ExprKind::Call:
            return visitCall(std::static_pointer_cast<CallExpr>(exp));

        case ExprKind::Pipe:
            return visitPipe(std::static_pointer_cast<PipeExpr>(exp));

        case ExprKind::Unary:
            return visitUnary(std::static_pointer_cast<UnaryExpr>(exp));
            
        case ExprKind::Range:
            return visitRangeExpr(std::static_pointer_cast<RangeExpr>(exp));
        default:
            std::cout << exp->expr_string() << std::endl;
            throw std::runtime_error("Unknown expression type");
    }
    }
    //Good
    void ExprPass ::visitStmt(const std::shared_ptr<Stmt> stmt){

        switch(stmt->stmt_kind)
        {
            case StmtKind::LetDecl:
                visitLetStmt(std::static_pointer_cast<LetStmt>(stmt));
                break;

            case StmtKind::Assign:
                visitAsgStmt(std::static_pointer_cast<AssignStmt>(stmt));
                break;

            case StmtKind::Expr:
                visitExprStmt(std::static_pointer_cast<ExprStmt>(stmt));
                break;

            case StmtKind::While:
                visitWhileStmt(std::static_pointer_cast<WhileStmt>(stmt));
                break;

            case StmtKind::For:
                visitForStmt(std::static_pointer_cast<ForStmt>(stmt));
                break;

            case StmtKind::If:
                visitIfStmt(std::static_pointer_cast<IfStmt>(stmt));
                break;

            case StmtKind::Return:
                visitReturnStmt(std::static_pointer_cast<ReturnStmt>(stmt));
                break;
            


            default:
                break;
        }
    }

    void ExprPass::visitBlock(const std::shared_ptr<BlockExpr> block){
        if (!block) return;

        print("Entering Scope Depth: " + std::to_string(scope_depth) + "\n");
        auto prev = current_scope;
        current_scope = current_scope->push();
        scope_depth += 1;

        for (auto& stmt : block->statements)
            visitStmt(stmt);

        current_scope = prev; // pop
        print("Leaving Scope Depth: " + std::to_string(scope_depth) + "\n");
        scope_depth -= 1;

    }
    // Good?
    void ExprPass ::visitLetStmt(const std::shared_ptr<LetStmt> ls){
        switch(ls->id_pattern->pat_type) {
            case PatternType::Single: {

                auto pid = std::static_pointer_cast<PatternId>(ls->id_pattern);
                 if (current_scope->symbols.contains(pid->id))
                    err.error(ls,"Variable/Function '" + pid->id + "' is already defined");
                auto sym = std::make_shared<Symbol>(SymbolType::Variable, pid->id);

                auto expr_t = visitExpr(ls->exp);

                sym->type = ls->type ? ls->type : expr_t;
                ls->type = sym->type;
                sym->expr = ls->exp;
                current_scope->define(sym);
                break;
            }
            case PatternType::Multiple: {
                auto pids = std::static_pointer_cast<PatternIds>(ls->id_pattern);
                auto texp = std::static_pointer_cast<TupleExpr>(ls->exp);
                auto ttype = std::static_pointer_cast<TupleType>(ls->type);

                bool no_init_type = ttype->elements.empty() ? true : false;

                for(size_t i = 0; i < pids->ids.size(); ++i) {
                    if (current_scope->symbols.contains(pids->ids[i]))
                        err.error(ls,"Variable/Function '" + pids->ids[i]+ "' is already defined");
                    auto sym = std::make_shared<Symbol>(SymbolType::Variable, pids->ids[i]);

                    if(no_init_type){
                        sym->expr = texp->elements[i];
                        sym->type = visitExpr(sym->expr);
                        ttype->elements.push_back(sym->type);
                        current_scope->define(sym);
                    } else {

                        sym->type = ttype->elements[i];
                        sym->expr = texp->elements[i];
                        visitExpr(sym->expr);
                        current_scope->define(sym);
                    }
                }
                break;
             }
             default:
                err.error(ls,"Pattern for Let Decleration does not exist");

        }

    }

   void ExprPass::visitFunctionDecl(const std::shared_ptr<FunctionDecl> f){

        // 1. define function
        if (current_scope->symbols.contains(f->func_name))
            err.error(f, "Function '" + f->func_name + "' already defined");

        auto sym = std::make_shared<Symbol>(SymbolType::Function, f->func_name);
        sym->type = f->func_type;
        current_scope->define(sym);

        // 2. new scope
        auto prev = current_scope;
        current_scope = current_scope->push();

        // 3. define parameters
        for (auto param : f->func_type->params) {
            auto psym = std::make_shared<Symbol>(SymbolType::Variable, param->para_name);
            psym->type = param->para_type;
            current_scope->define(psym);
        }

        // 4. visit body
        visitBlock(f->body);

        // 5. restore scope
        current_scope = prev;
    }
    void ExprPass::visitIfStmt(const std::shared_ptr<IfStmt> is){
        // check condition of if statement
        visitExpr(is->condition);
        // check the then statement
        visitBlock(is->then_block);

        if(!is->elifx_blocks.empty()){
            // if there are any "else if's"
            for(auto [expr, block] : is->elifx_blocks){
                visitExpr(expr);
                visitBlock(block);
            }
        }
        // if there is a else block
        if(is->else_block){
            visitBlock(is->else_block);
        }
    }
   
    void ExprPass::visitWhileStmt(const std::shared_ptr<WhileStmt> ws){
        visitExpr(ws->cond);
        visitBlock(ws->body);
    }


    void ExprPass::visitForStmt(const std::shared_ptr<ForStmt> fs){
        // got to check if the 
        std:: cout << "inside for loop" << std::endl;

        print("Entering Scope Depth: " + std::to_string(scope_depth) + "\n");
        auto prev = current_scope;
        current_scope = current_scope->push();
        scope_depth += 1;

        auto itr_var = std::static_pointer_cast<IdExpr>(fs->iter_var);
        

        // check what type is the iterable (it can only be range type or array type)
        auto itera_type = visitExpr(fs->iterable); 

        switch(fs->iterable->exp_kind){
            case ExprKind::Array:
                itr_var->type = std::static_pointer_cast<ArrayType>(itera_type)->elem;
                break;
            case ExprKind::Range:
                itr_var->type = std::make_shared<PrimType>(PrimType::Prims::Int);
                break;
            case ExprKind::Id:
                itr_var->type = itera_type;
                break;
            default:
                err.error(fs->iterable,"Iterable value MUST be either of type 'Array' or 'Range'");
        }

        auto sym = std::make_shared<Symbol>(SymbolType::Variable, itr_var->name);
        sym->type = itr_var->type;
        itr_var->resolved = sym;

        current_scope->define(sym);

        // visit the block, thats it. Also it is a completely new scope we enter the for loop, so we must go to the next scope. inside visitBlock it will go on another scope which is fine.
        visitBlock(fs->body);


        current_scope = prev; // pop
        print("Leaving Scope Depth: " + std::to_string(scope_depth) + "\n");
        scope_depth -= 1;

    }


    std::shared_ptr<Type> ExprPass::visitLiteral(const std::shared_ptr<LiteralExpr> literal){
        return literal->type;
    }

    std::shared_ptr<Type> ExprPass::visitId(const std::shared_ptr<IdExpr> id){

    if(!id->resolved){
            auto value = current_scope->resolve(id->name);

            if(!value){
                err.error(id, "Undefined identifier: '" + id->name + "'");
            }

            if(value->kind == SymbolType::Function){
                err.error(id, "'" + id->name + "' is a function that requires arguments");
            }

            id->resolved = value;
            id->type = value->type;
        }

        return id->type;
    }

    std::shared_ptr<Type> ExprPass::visitCall (const std::shared_ptr<CallExpr> cexp){

      

        auto sym = current_scope->resolve(cexp->callee);

        if(!sym)
            err.error(cexp, "Undefined function: " + cexp->callee);

        if(sym->kind != SymbolType::Function)
            err.error(cexp, "'" + cexp->callee + "' is not a function");


        auto fn = std::static_pointer_cast<FunctionType>(sym->type);


        if(fn->params.size() != cexp->args.size())
            err.error(cexp, "Argument count mismatch");



        for(size_t i=0;i<cexp->args.size();i++)
            visitExpr(cexp->args[i]);

        cexp->type = fn->ret;

        return cexp->type;
    }
    std::shared_ptr<Type> ExprPass::visitBinary(const std::shared_ptr<BinaryExpr> bexp){

        bexp->type = ots.check_binary(bexp->operation, visitExpr(bexp->lhs), visitExpr(bexp->rhs));
        return bexp->type;
    }    
        
    std::shared_ptr<Type> ExprPass::visitTuple(const std::shared_ptr<TupleExpr> texp){
        std::vector<std::shared_ptr<Type>> types;
        for(auto e : texp->elements)
            types.push_back(visitExpr(e));
        auto t = std::make_shared<TupleType>(types);
        texp->type = t;
        return t;

    }
    
    std::shared_ptr<Type> ExprPass::visitArray (const std::shared_ptr<ArrayExpr> aexp){

        // we should check that ALL expressions within this array are ALL of the same type
        // because now in expressions, we are giving it actual information

        size_t size = aexp->elements.size();

        // Empty array case
        if (size == 0) {
            aexp->type = std::make_shared<ArrayType>(
                std::make_shared<PrimType>(PrimType::Prims::Null),
                0
            );
            return aexp->type;
        }

        // Resolve first element type
        auto first_type = visitExpr(aexp->elements[0]);

        // Check all other elements
        for (size_t i = 1; i < size; i++) {

            auto t = visitExpr(aexp->elements[i]);

            if (!t->equals(first_type)) {
                err.error(aexp,
                    "Array elements must all have the same type. Found '" +
                    first_type->to_string() + "' and '" +
                    t->to_string() + "'"
                );
            }
        }

        aexp->type = std::make_shared<ArrayType>(first_type, size);

        return aexp->type;

    }
    std::shared_ptr<Type> ExprPass::visitUnary (const std::shared_ptr<UnaryExpr> uexp){
        return uexp->rhs->type;
    }  

    std::shared_ptr<Type> ExprPass::visitIndex (const std::shared_ptr<IndexExpr> iexp){
        auto base_type = visitExpr(iexp->base);
        auto index_type = visitExpr(iexp->index);

        if(!(base_type->kind == TypeKind::Array))
            err.error(iexp->base, "Indexing Non-Array");

        if(!(index_type->kind == TypeKind::Primitive))
            err.error(iexp->index, "Index value is not of type INT");
    
        auto arr = std::static_pointer_cast<ArrayType>(base_type);
        iexp->type = arr->elem;

        return iexp->type;

    }
    void ExprPass::visitReturnStmt(const std::shared_ptr<ReturnStmt> rs){
        rs->value->type = visitExpr(rs->value);
    }

    void ExprPass::visitAsgStmt(const std::shared_ptr<AssignStmt> as){
        // do not we have to make the logic to make sure it is compatible re assignment
        visitExpr(as->value);
        visitExpr(as->target);
    }

    std::shared_ptr<Type> ExprPass::visitRangeExpr (const std::shared_ptr<RangeExpr> rexp){
        
        auto lhs_type = visitExpr(rexp->lhs);
        auto rhs_type = visitExpr(rexp->rhs);

        if(lhs_type->kind == TypeKind::Primitive)
            if(std::static_pointer_cast<PrimType>(lhs_type)->prim != PrimType::Prims::Int)
                err.error(rexp->lhs,"lh range size is not INT");
   
 
        if(rhs_type->kind == TypeKind::Primitive)
             if(std::static_pointer_cast<PrimType>(rhs_type)->prim != PrimType::Prims::Int)
                err.error(rexp->rhs, "rh range size is not INT");

        return rexp->type;
    }



    std::shared_ptr<Type> ExprPass::visitPipe ( const std::shared_ptr<PipeExpr> pexp){

       // Resolve LHS
        auto lhs_type = visitExpr(pexp->lhs);

        if (pexp->rhs->exp_kind != ExprKind::Call)
            err.error(pexp->rhs, "Pipe RHS must be a function call");

        auto call = std::static_pointer_cast<CallExpr>(pexp->rhs);
        call->args.insert(call->args.begin(), pexp->lhs);
        
        auto sym = current_scope->resolve(call->callee);

        if(!sym)
              err.error(call,"Undefined function: " + call->callee);

        if(sym->kind != SymbolType::Function)
            throw std::runtime_error("'" + call->callee + "' is not a function");

        auto fn = std::static_pointer_cast<FunctionType>(sym->type);

        // pipe adds one argument
        size_t expected = fn->params.size();
        size_t provided = call->args.size();

        if(expected != provided){
        err.error(pexp,
                "Pipe argument mismatch: function '" + call->callee +
                "' expects " + std::to_string(expected) +
                " arguments but got " + std::to_string(provided)
            );
        }

        // type check pipe argument
        if(!lhs_type->equals(fn->params[0]->para_type))
            err.error(pexp->lhs,
                "Pipe type mismatch: expected '" +
                fn->params[0]->para_type->to_string() +
                "' but got '" +
                lhs_type->to_string() + "'"
            );

        // type check remaining args
        for(size_t i = 0; i < call->args.size(); i++)
        {
            auto arg_t = visitExpr(call->args[i]);

            if(!arg_t->equals(fn->params[i]->para_type))
                err.error(call->args[i],
                    "Argument " + std::to_string(i+1) +
                    " type mismatch in call to '" + call->callee + "'"
                );
        }

        pexp->rhs->type = fn->ret;
        pexp->type = fn->ret;

        return pexp->type;

    }
    std::shared_ptr<Type> ExprPass::resolveExp(const std::shared_ptr<Expr> type){}
    void ExprPass::visitExprStmt(const std::shared_ptr<ExprStmt> es){
        std::cout << "hello world" << std::endl;
        exit(1);
    }
    
  }