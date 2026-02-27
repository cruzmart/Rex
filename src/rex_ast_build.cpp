#include "rex_ast_build.h"
#include "RexParser.h"
#include "rex_ast.h"
#include "rex_ast_nodes.h"
#include "rex_types.h"
#include <any>
#include <cstddef>
#include <memory>

using namespace rex;

// ==================================================
// ------------------- HELPERS ---------------------
// ==================================================

SourceLocation rex_ast_build::loc(antlr4::ParserRuleContext* ctx) {
    return { ctx->getStart()->getLine(), ctx->getStart()->getCharPositionInLine() };
}

template <typename T>
std::shared_ptr<AstNode> try_cast_ast(const antlrcpp::Any& a) {
    if (auto p = std::any_cast<std::shared_ptr<T>>(&a)) {
        return std::dynamic_pointer_cast<AstNode>(*p);
    }
    return nullptr;
}

static std::shared_ptr<AstNode> as_ast_node(const antlrcpp::Any& a) {
    if (auto n = std::any_cast<std::shared_ptr<AstNode>>(&a)) return *n;
    if (auto r = try_cast_ast<Stmt>(a)) return r;
    if (auto r = try_cast_ast<TypeDecl>(a)) return r;
    if (auto r = try_cast_ast<FunctionDecl>(a)) return r;
    if (auto r = try_cast_ast<TypeNode>(a)) return r;
    throw std::runtime_error("Unknown item type in as_ast_node");
}

BinaryOp operationType (const size_t op) {
    switch(op) {
        case RexParser::PLUS: return BinaryOp::ADD;
        case RexParser::MINUS: return BinaryOp::SUB;
        case RexParser::STAR: return BinaryOp::MUL;
        case RexParser::DIV: return BinaryOp::DIV;
        case RexParser::MOD: return BinaryOp::MOD;
        case RexParser::EQ: return BinaryOp::EQ;
        case RexParser::NEQ: return BinaryOp::NEQ;
        case RexParser::LT: return BinaryOp::LT;
        case RexParser::GT: return BinaryOp::GT;
        case RexParser::LTE: return BinaryOp::LE;
        case RexParser::GTE: return BinaryOp::GE;
        case RexParser::AND: return BinaryOp::AND;
        case RexParser::OR: return BinaryOp::OR; 
        default: throw std::runtime_error("Unknown operation token type");
    }
}

template <typename Ctx>
antlrcpp::Any build_binary_expr(rex_ast_build* self, Ctx* ctx) {
    auto bin = std::make_shared<BinaryExpr>();
    size_t op = ctx->op->getType();
    bin->loc = self->loc(ctx);
    bin->operation = operationType(op);
    bin->type = rex::Type::Error();
    bin->lhs = std::any_cast<std::shared_ptr<Expr>>(self->visit(ctx->expr(0)));
    bin->rhs = std::any_cast<std::shared_ptr<Expr>>(self->visit(ctx->expr(1)));
    return std::dynamic_pointer_cast<Expr>(bin);
}

// ==================================================
// ------------------- TOP LEVEL -------------------
// ==================================================

antlrcpp::Any rex_ast_build::visitFile(RexParser::FileContext* ctx) {
    auto file = std::make_shared<FileAst>();
    file->loc = loc(ctx);
    for (auto item : ctx->item())
        file->items.push_back(as_ast_node(visit(item)));
    return file;
}

antlrcpp::Any rex_ast_build::visitItem(RexParser::ItemContext* ctx) {
    if (ctx->functionDef()) return visit(ctx->functionDef());
    if (ctx->typeDef()) return visit(ctx->typeDef());
    return visit(ctx->statement());
}

// ==================================================
// ---------------------- TYPES --------------------
// ==================================================

antlrcpp::Any rex_ast_build::visitTypeDef(RexParser::TypeDefContext* ctx) {
    auto td = std::make_shared<TypeDecl>();
    auto alias_name = ctx->ID()->getText();
    auto line_location = loc(ctx);

    td->loc = line_location;
    td->type = std::any_cast<std::shared_ptr<TypeNode>>(visit(ctx->type()))->type;
    td->type->alias = alias_name;

    return std::dynamic_pointer_cast<TypeNode>(td);
}

antlrcpp::Any rex_ast_build::visitPrimeType(RexParser::PrimeTypeContext* ctx) { 
    return visit(ctx->primitiveType());
}

antlrcpp::Any rex_ast_build::visitPrimitiveType(RexParser::PrimitiveTypeContext *ctx)  {
    auto pt = std::make_shared<PrimType>();
    auto type = pt->type;
    if(ctx->BOOL())
        pt->type = rex::Type::Bool();
    if(ctx->INT())
        pt->type = rex::Type::Int();
    if(ctx->REAL())
        pt->type = rex::Type::Real();
    if(ctx->CHAR())
        pt->type = rex::Type::Char();
    if(ctx->STRING())
        pt->type = rex::Type::String();

    pt->loc = loc(ctx);
    return std::dynamic_pointer_cast<TypeNode>(pt);

}

antlrcpp::Any rex_ast_build::visitNamedType(RexParser::NamedTypeContext* ctx) {
    std::shared_ptr<rex::NamedType> nameType = std::make_shared<NamedType>();
    auto name = ctx->ID()->getText();

    auto type = rex::Type::Named(name);

    nameType->type = type;
    nameType->loc = loc(ctx);

    // we would have to find the id of this type in the symbol table to find the actual Type information, than copy that data and put it on this i think..
    // because this only even exists if we do type def, and based on that we can put the proper information. If it is empty than it is a error..

    
    return std::dynamic_pointer_cast<TypeNode>(nameType);
}

antlrcpp::Any rex_ast_build::visitArrayType(RexParser::ArrayTypeContext* ctx) {
    int size = std::stoi(ctx->INT_LITERAL()->getText());
    // get the base of the array which IS a type, but first we must make a array
    // check if the base is a ID type first though, else we have to do something a bit differently. 
    // we will fix this in another pass through the AST by using a symbol table.
    auto base = std::any_cast<std::shared_ptr<TypeNode>>(visit(ctx->type())); // if it is a NamedType, than it will be empty, which is okay
    auto arrayType = std::make_shared<ArrayType>();
    arrayType->type = rex::Type::Array(base->type, size);
    
    return std::dynamic_pointer_cast<TypeNode>(arrayType);
}

antlrcpp::Any rex_ast_build::visitSliceType(RexParser::SliceTypeContext* ctx) { 
    auto elem = std::any_cast<std::shared_ptr<TypeNode>>(visit(ctx->type()));
    auto slice = std::make_shared<SliceType>();
    auto sliceType = rex::Type::Slice(elem->type);
    slice->type = sliceType;
    slice->loc = loc(ctx);
    return std::dynamic_pointer_cast<TypeNode>(slice);
}

antlrcpp::Any rex_ast_build::visitTupleType(RexParser::TupleTypeContext* ctx) { 
    std::vector<std::shared_ptr<Type>> elemTypes;
    for (auto ty : ctx->type()){
        auto tp = std::any_cast<std::shared_ptr<TypeNode>>(visit(ty));
        elemTypes.push_back(tp->type);
    }

    auto tupleType = std::make_shared<rex::TupleType>();
    tupleType->type = rex::Type::Tuple(elemTypes);
    tupleType->loc = loc(ctx);
    
    return std::dynamic_pointer_cast<TypeNode>(tupleType);
}

// ==================================================
// -------------------- FUNCTIONS ------------------
// ==================================================

antlrcpp::Any rex_ast_build::visitFunctionDef(RexParser::FunctionDefContext* ctx) {
    auto fn = std::make_shared<FunctionDecl>();
    fn->loc = loc(ctx);
    fn->name = ctx->ID()->getText();

    // get the list of parameters what they are
    if (ctx->paramList())
        fn->params = std::any_cast<std::vector<std::shared_ptr<Parameter>>>(visit(ctx->paramList()));
    
    // if the  function HAS a return type else it must be void type
    if(ctx->returnType() && ctx->returnType()->type()){
        fn->funcReturnType = std::any_cast<std::shared_ptr<TypeNode>>(visit(ctx->returnType()->type()));
    }

    fn->body = std::any_cast<std::shared_ptr<BlockExpr>>(visit(ctx->block()));
    return fn;
}

antlrcpp::Any rex_ast_build::visitParam(RexParser::ParamContext* ctx) {
    std::shared_ptr<rex::Parameter> param = std::make_shared<rex::Parameter>();


    auto paramName = ctx->ID()->getText();
    param->name = paramName;


    auto contextType = ctx->type();

    if(contextType){
        param->type = std::any_cast<std::shared_ptr<TypeNode>>(visit(contextType))->type;
    } else {
        param->type = rex::Type::Void();
    }

    return param;
}

antlrcpp::Any rex_ast_build::visitParamList(RexParser::ParamListContext* ctx) {
   std::vector<std::shared_ptr<Parameter>> params;
    params.reserve(ctx->param().size());

    for (auto p : ctx->param()) {
        params.push_back(std::any_cast<std::shared_ptr<Parameter>>(visit(p)));
    }

    return params;
}

antlrcpp::Any rex_ast_build::visitReturnType(RexParser::ReturnTypeContext* ctx) { 
    return visit(ctx->type());
}

// ==================================================
// -------------------- STATEMENTS -----------------
// ==================================================

antlrcpp::Any rex_ast_build::visitStatement(RexParser::StatementContext* ctx) {
    if (ctx->letStmt()) return visit(ctx->letStmt());
    if (ctx->returnStmt()) return visit(ctx->returnStmt());
    if (ctx->exprStmt()) return visit(ctx->exprStmt());
    if (ctx->loopStmt()) return visit(ctx->loopStmt());
    if (ctx->ifStmt()) return visit(ctx->ifStmt());
    if (ctx->assignStmt()) return visit(ctx->assignStmt());
    throw std::runtime_error("Unknown statement type");
}

antlrcpp::Any rex_ast_build::visitLetStmt(RexParser::LetStmtContext* ctx) {


    std::shared_ptr<LetStmts> l = std::make_shared<LetStmts>();

    auto ids = ctx->pattern();
    size_t size = ctx->pattern()->ID().size();
    l->letStmts.reserve(size);

    for(size_t s = 0; s < size; s++){
        l->letStmts.push_back(std::make_shared<LetStmt>());   
    }

    for(size_t i = 0; i < size; i++){
        auto id = ids->ID()[i]->getText();
        l->letStmts[i]->variable_name = id;
    }

    // check if it is a tuple of id's but no type was given, than its fine

    if(ctx->type()){
       auto typ = std::any_cast<std::shared_ptr<TypeNode>>(visit(ctx->type()));

        if(auto tup = std::dynamic_pointer_cast<TupleType>(typ)){
            for(size_t s  = 0; s < size; s ++){
                l->letStmts[s]->type_exp= tup->type->elements[s];
            }
        }  else {
            l->letStmts[0]->type_exp = typ->type;
        }
    } else {

         for(auto letCell : l->letStmts){
            letCell->type_exp = rex::Type::Error();
        }
    }


    auto exprs = ctx->expr();
    if(exprs){

        // if size is equal to 1, and it is a tuple or anything else we directly add it as is
        // if size of ids is >1, and it is a tuple we got to iterate it
        // the rest will go through type checking of the many combos, we will have "checkLetStmt" function in a pass to see if it is properly done

        auto typ_exp = std::any_cast<std::shared_ptr<Expr>>(visit(exprs));
       
        // if tuple
       if(auto tup_expr = std::dynamic_pointer_cast<TupleExpr>(typ_exp)){

            if(size == 1){
                l->letStmts[0]->init_exp = typ_exp;
            } else {
                size_t tup_exp_size = tup_expr->elements.size();
                    for(size_t t = 0; t < tup_exp_size; t++){
                        l->letStmts[t]->init_exp = tup_expr->elements[t];
                    }
            }
          
       } else {
            l->letStmts[0]->init_exp = typ_exp;
       }
    }
 
    return std::dynamic_pointer_cast<Stmt>(l);
}

antlrcpp::Any rex_ast_build::visitReturnStmt(RexParser::ReturnStmtContext* ctx) {
    auto ret = std::make_shared<ReturnStmt>();
    ret->loc = loc(ctx);
    if (ctx->expr()) ret->value = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr()));
    return std::dynamic_pointer_cast<Stmt>(ret);
}

antlrcpp::Any rex_ast_build::visitExprStmt(RexParser::ExprStmtContext* ctx) {
    auto es = std::make_shared<ExprStmt>();
    es->loc = loc(ctx);
    es->value = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr()));
    return es;
}

antlrcpp::Any rex_ast_build::visitAssignStmt(RexParser::AssignStmtContext *ctx) {
    auto asg = std::make_shared<AssignStmt>();
    asg->target = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr(0)));
    asg->value = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr(1)));
    return std::dynamic_pointer_cast<Stmt>(asg);
}

antlrcpp::Any rex_ast_build::visitLoopStmt(RexParser::LoopStmtContext* ctx) {
    if (ctx->WHILE()) {
        auto w = std::make_shared<WhileStmt>();
        w->loc = loc(ctx);
        w->cond = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr()));
        w->body = std::any_cast<std::shared_ptr<BlockExpr>>(visit(ctx->block()));
        return std::dynamic_pointer_cast<Stmt>(w);
    }
    if (ctx->FOR()) {
        auto f = std::make_shared<ForStmt>();
        f->loc = loc(ctx);
        f->iter_var = ctx->ID()->getText();
        f->iterable = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr()));
        f->body = std::any_cast<std::shared_ptr<BlockExpr>>(visit(ctx->block()));
        return std::dynamic_pointer_cast<Stmt>(f);
    }
    if (ctx->LOOP()) {
        auto l = std::make_shared<LoopStmt>();
        l->loc = loc(ctx);
        l->body = std::any_cast<std::shared_ptr<BlockExpr>>(visit(ctx->block()));
        return std::dynamic_pointer_cast<Stmt>(l);
    }
    throw std::runtime_error("Unknown loop kind");
}

antlrcpp::Any rex_ast_build::visitIfStmt(RexParser::IfStmtContext *ctx){
    auto if_ = std::make_shared<IfStmt>();
    if_->condition = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr()));
    if_->then_block = std::any_cast<std::shared_ptr<BlockExpr>>(visit(ctx->block()));

    if(ctx->elifxChain()){
        size_t elif_size = ctx->elifxChain()->expr().size();
        auto elifs_conds = ctx->elifxChain()->expr();
        auto elifs_blocks = ctx->elifxChain()->block();
        for(size_t i = 0; i < elif_size; i++){
            if_->elifx_blocks.emplace_back(
                std::any_cast<std::shared_ptr<Expr>>(visit(elifs_conds[i])),
                std::any_cast<std::shared_ptr<BlockExpr>>(visit(elifs_blocks[i]))
            );
        }
    }

    if(ctx->elseBlock()){
        if_->else_block = std::any_cast<std::shared_ptr<BlockExpr>>(visit(ctx->elseBlock()->block()));
    }

    return std::dynamic_pointer_cast<Stmt>(if_);
}

// ==================================================
// ---------------------- BLOCKS -------------------
// ==================================================

antlrcpp::Any rex_ast_build::visitBlock(RexParser::BlockContext* ctx) {
    auto block = std::make_shared<BlockExpr>();
    block->loc = loc(ctx);

    for (auto state : ctx->statement())
        block->statements.push_back(std::any_cast<std::shared_ptr<Stmt>>(visit(state)));

    if (ctx->expr())
        block->result = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr()));

    return block;
}

// ==================================================
// ------------------- EXPRESSIONS -----------------
// ==================================================

antlrcpp::Any rex_ast_build::visitIdExpr(RexParser::IdExprContext* ctx) {

    auto id = std::make_shared<IdExpr>();
    id->loc = loc(ctx);
    id->name = ctx->ID()->getText();
    id->type = rex::Type::Error();

    // Note when we do type checking, we will use the resolver in order to populate the ID Expr.
    return std::dynamic_pointer_cast<Expr>(id);
}

antlrcpp::Any rex_ast_build::visitLiteralExpr(RexParser::LiteralExprContext* ctx) {
    return visit(ctx->literal());
}

antlrcpp::Any rex_ast_build::visitLiteral(RexParser::LiteralContext* ctx) {
    auto lit = std::make_shared<LiteralExpr>();
    auto textValue = ctx->getText();
    std::shared_ptr<Type> type;
   
    if(ctx->INT_LITERAL())
        type = rex::Type::Int();
    if(ctx->REAL_LITERAL())
        type = rex::Type::Real();
    if(ctx->STRING_LITERAL())
        type = rex::Type::String();
    if(ctx->CHAR_LITERAL())
        type = rex::Type::Char();
    if(ctx->TRUE() || ctx->FALSE())
        type = rex::Type::Bool();

    lit->loc = loc(ctx);
    lit->value = textValue;
    lit->type = type;
    
    return std::dynamic_pointer_cast<Expr>(lit);
}

// Binary expressions
antlrcpp::Any rex_ast_build::visitAddExpr(RexParser::AddExprContext* ctx) {return build_binary_expr(this, ctx);}
antlrcpp::Any rex_ast_build::visitMulExpr(RexParser::MulExprContext* ctx) {return build_binary_expr(this, ctx);}
antlrcpp::Any rex_ast_build::visitAndExpr(RexParser::AndExprContext* ctx) { return build_binary_expr(this, ctx); }
antlrcpp::Any rex_ast_build::visitOrExpr(RexParser::OrExprContext* ctx) { return build_binary_expr(this, ctx); }
antlrcpp::Any rex_ast_build::visitCompareExpr(RexParser::CompareExprContext *ctx) {return build_binary_expr(this, ctx);}
antlrcpp::Any rex_ast_build::visitEqualityExpr(RexParser::EqualityExprContext* ctx) { return build_binary_expr(this, ctx); }    

antlrcpp::Any rex_ast_build::visitPipeExpr(RexParser::PipeExprContext* ctx) { 
    auto pip = std::make_shared<PipeExpr>();
    pip->loc = loc(ctx);
    pip->operation = BinaryOp::PIPE;
    pip->lhs = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr(0)));
    pip->rhs = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr(1)));
    pip->type = rex::Type::Error();
    return std::dynamic_pointer_cast<Expr>(pip);
}

antlrcpp::Any rex_ast_build::visitRangeExpr(RexParser::RangeExprContext* ctx) { 
    auto rng = std::make_shared<RangeExpr>();
    rng->loc = loc(ctx);
    rng->operation = BinaryOp::RANGE;
    rng->lhs = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr(0)));
    rng->rhs = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr(1)));
    rng->type = rex::Type::Error();
    return std::dynamic_pointer_cast<Expr>(rng);
}

// Other expressions
antlrcpp::Any rex_ast_build::visitIndexExpr(RexParser::IndexExprContext* ctx) {  
    auto idx = std::make_shared<IndexExpr>();
    idx->loc = loc(ctx);
    idx->base = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr(0)));
    idx->index = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr(1)));
    idx->type = rex::Type::Error();
    return std::dynamic_pointer_cast<Expr>(idx);
}

antlrcpp::Any rex_ast_build::visitTupleExpr(RexParser::TupleExprContext* ctx) {
    auto t = std::make_shared<TupleExpr>();
    t->loc = loc(ctx);
    for (auto e : ctx->expr())
        t->elements.push_back(std::any_cast<std::shared_ptr<Expr>>(visit(e)));
    t->type = rex::Type::Error();
    return std::dynamic_pointer_cast<Expr>(t);
}

antlrcpp::Any rex_ast_build::visitArrayExpr(RexParser::ArrayExprContext *ctx) {
    auto a = std::make_shared<ArrayExpr>();
    a->loc = loc(ctx);
    for (auto l : ctx->expr())
        a->elements.push_back(std::any_cast<std::shared_ptr<Expr>>(visit(l)));
    a->type = rex::Type::Error();
    return std::dynamic_pointer_cast<Expr>(a);
}

antlrcpp::Any rex_ast_build::visitCallExpr(RexParser::CallExprContext* ctx) {
    auto call = std::make_shared<CallExpr>();
    call->loc = loc(ctx);
    call->callee = ctx->ID()->getText();
    if (ctx->argList()) {
        for (auto e : ctx->argList()->expr())
            call->args.push_back(std::any_cast<std::shared_ptr<Expr>>(visit(e)));
    }
    call->type = rex::Type::Error();
    return std::dynamic_pointer_cast<Expr>(call);
}

antlrcpp::Any rex_ast_build::visitParenExpr(RexParser::ParenExprContext* ctx) {
    return visit(ctx->expr());
}

antlrcpp::Any rex_ast_build::visitUnaryExpr(RexParser::UnaryExprContext *ctx) {
    auto unary_exp = std::make_shared<UnaryExpr>();
    unary_exp->operation = ctx->op->getText() == "-" ? UniOp::NEG : UniOp::POS;
    unary_exp->rhs = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr()));
    unary_exp->type = rex::Type::Error();
    return std::dynamic_pointer_cast<Expr>(unary_exp);
}
