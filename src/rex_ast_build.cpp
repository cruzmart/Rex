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
    bin->type = std::make_shared<Type>(rex::TypeKind::Error);
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
    td->name = ctx->ID()->getText();
    td->loc = loc(ctx);
    td->type = std::any_cast<std::shared_ptr<Type>>(visit(ctx->type()));

    return std::dynamic_pointer_cast<TypeNode>(td);
}

antlrcpp::Any rex_ast_build::visitPrimeType(RexParser::PrimeTypeContext* ctx) { 
    return visit(ctx->primitiveType());
}

antlrcpp::Any rex_ast_build::visitPrimitiveType(RexParser::PrimitiveTypeContext *ctx)  {

    std::shared_ptr<PrimType> pt;

    if(ctx->BOOL())
        pt  = std::make_shared<PrimType>(rex::PrimType::Prims::Bool);
    if(ctx->INT())
        pt  = std::make_shared<PrimType>(rex::PrimType::Prims::Int);
    if(ctx->REAL())
        pt  = std::make_shared<PrimType>(rex::PrimType::Prims::Real);
    if(ctx->CHAR())
        pt  = std::make_shared<PrimType>(rex::PrimType::Prims::Char);
    if(ctx->STRING())
        pt  = std::make_shared<PrimType>(rex::PrimType::Prims::String);

    return std::dynamic_pointer_cast<Type>(pt);

}

antlrcpp::Any rex_ast_build::visitNamedType(RexParser::NamedTypeContext* ctx) {
    std::shared_ptr<NamedType> nt = std::make_shared<NamedType>();

    nt->alias = ctx->ID()->getText();
    nt->fundamental_kind = rex::TypeKind::Named;

    return std::dynamic_pointer_cast<Type>(nt);
}

antlrcpp::Any rex_ast_build::visitArrayType(RexParser::ArrayTypeContext* ctx) {
    auto array_size = std::stoi(ctx->INT_LITERAL()->getText());
    auto array_type_prim = std::any_cast<std::shared_ptr<Type>>(visit(ctx->type()));

    std::shared_ptr<ArrayType> at = std::make_shared<ArrayType>(array_type_prim, array_size);
    return std::dynamic_pointer_cast<Type>(at);
}

antlrcpp::Any rex_ast_build::visitSliceType(RexParser::SliceTypeContext* ctx) { 
   
    auto slice_type_prim = std::any_cast<std::shared_ptr<Type>>(visit(ctx->type()));
    std::shared_ptr<SliceType> st = std::make_shared<SliceType>(slice_type_prim);
  
    return std::dynamic_pointer_cast<Type>(st);
}

antlrcpp::Any rex_ast_build::visitTupleType(RexParser::TupleTypeContext* ctx) { 

    std::shared_ptr<TupleType> tt = std::make_shared<TupleType>();
    for(auto type : ctx->type()){
        auto tp = std::any_cast<std::shared_ptr<Type>>(visit(type));
        tt->tuple_types.push_back(tp);

    }
    
    return std::dynamic_pointer_cast<Type>(tt);
}

// ==================================================
// -------------------- FUNCTIONS ------------------
// ==================================================

antlrcpp::Any rex_ast_build::visitFunctionDef(RexParser::FunctionDefContext* ctx) {
    auto fn = std::make_shared<FunctionDecl>();
    auto fn_type = std::make_shared<FunctionType>();
    fn->func_type = fn_type;

    fn->loc = loc(ctx);
    fn->func_name = ctx->ID()->getText();

    // if it has parameters, get the list of them.
    if (ctx->paramList()){
        fn->parameters = std::any_cast<std::vector<std::shared_ptr<Parameter>>>(visit(ctx->paramList()));
    }
   
    // if the function HAS a return type else it must be void type
    if(ctx->returnType()){
        fn->func_type->return_type = std::any_cast<std::shared_ptr<Type>>(visit(ctx->returnType()));
    } 

    fn->body = std::any_cast<std::shared_ptr<BlockExpr>>(visit(ctx->block()));
    return fn;
}

antlrcpp::Any rex_ast_build::visitParam(RexParser::ParamContext* ctx) {
    return std::make_shared<Parameter>(ctx->ID()->getText(), std::any_cast<std::shared_ptr<Type>>(visit(ctx->type())));
}

antlrcpp::Any rex_ast_build::visitParamList(RexParser::ParamListContext* ctx) {
   std::vector<std::shared_ptr<Parameter>> params;

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

antlrcpp::Any rex_ast_build::visitPattern(RexParser::PatternContext* ctx) {
    if(ctx->ID().size() > 1){
        auto pattern_ids = std::make_shared<PatternIds>();
        for(auto id : ctx->ID()){
            pattern_ids->ids.push_back(id->getText());
        }
        return std::dynamic_pointer_cast<Pattern>(pattern_ids);
    } else {
        auto pattern_id = std::make_shared<PatternId>(ctx->ID()[0]->getText());
        return std::dynamic_pointer_cast<Pattern>(pattern_id);
    }

}

antlrcpp::Any rex_ast_build::visitLetStmt(RexParser::LetStmtContext* ctx) {

    auto l = std::make_shared<LetStmt>();

    l->id_pattern = std::any_cast<std::shared_ptr<Pattern>>(visit(ctx->pattern()));
    if(ctx->expr())
        l->type = std::any_cast<std::shared_ptr<Type>>(visit(ctx->type()));
    if(ctx->expr())
        l->exp = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr()));

    l->loc = loc(ctx);
 
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

    auto id = std::make_shared<IdExpr>(ctx->ID()->getText());
    id->loc = loc(ctx);
    id->type = std::make_shared<Type>(rex::TypeKind::Error);

    // Note when we do type checking, we will use the resolver in order to populate the ID Expr.
    return std::dynamic_pointer_cast<Expr>(id);
}

antlrcpp::Any rex_ast_build::visitLiteralExpr(RexParser::LiteralExprContext* ctx) {
    return visit(ctx->literal());
}

antlrcpp::Any rex_ast_build::visitLiteral(RexParser::LiteralContext* ctx) {
    auto lit = std::make_shared<LiteralExpr>();
    lit->value = ctx->getText();

    if(ctx->INT_LITERAL())
        lit->type = std::make_shared<PrimType>(rex::PrimType::Prims::Int);
    if(ctx->REAL_LITERAL())
       lit->type = std::make_shared<PrimType>(rex::PrimType::Prims::Real);
    if(ctx->STRING_LITERAL())
        lit->type = std::make_shared<PrimType>(rex::PrimType::Prims::String);
    if(ctx->CHAR_LITERAL())
        lit->type = std::make_shared<PrimType>(rex::PrimType::Prims::Char);
    if(ctx->TRUE() || ctx->FALSE())
        std::make_shared<PrimType>(rex::PrimType::Prims::Bool);

    lit->loc = loc(ctx);
    
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
    pip->type = std::make_shared<PipeType>(pip->lhs->type, pip->rhs->type);
    return std::dynamic_pointer_cast<Expr>(pip);
}

antlrcpp::Any rex_ast_build::visitRangeExpr(RexParser::RangeExprContext* ctx) { 
    auto rng = std::make_shared<RangeExpr>();
    rng->loc = loc(ctx);
    rng->operation = BinaryOp::RANGE;
    rng->lhs = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr(0)));
    rng->rhs = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr(1)));
    rng->type =  std::make_shared<RangeType>(rng->lhs->type, rng->rhs->type);
    return std::dynamic_pointer_cast<Expr>(rng);
}

// Other expressions
antlrcpp::Any rex_ast_build::visitIndexExpr(RexParser::IndexExprContext* ctx) {  
    auto idx = std::make_shared<IndexExpr>();
    idx->loc = loc(ctx);
    idx->base = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr(0)));
    idx->index = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr(1)));
    idx->type = std::make_shared<Type>(rex::TypeKind::Error);
    return std::dynamic_pointer_cast<Expr>(idx);
}

antlrcpp::Any rex_ast_build::visitTupleExpr(RexParser::TupleExprContext* ctx) {
    auto t = std::make_shared<TupleExpr>();
    t->loc = loc(ctx);
    for (auto e : ctx->expr())
        t->elements.push_back(std::any_cast<std::shared_ptr<Expr>>(visit(e)));
    t->type = std::make_shared<Type>(rex::TypeKind::Error);
    return std::dynamic_pointer_cast<Expr>(t);
}

antlrcpp::Any rex_ast_build::visitArrayExpr(RexParser::ArrayExprContext *ctx) {
    auto a = std::make_shared<ArrayExpr>();
    a->loc = loc(ctx);
    for (auto l : ctx->expr())
        a->elements.push_back(std::any_cast<std::shared_ptr<Expr>>(visit(l)));
    a->type = std::make_shared<Type>(rex::TypeKind::Error);
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
    call->type = std::make_shared<Type>(rex::TypeKind::Error);
    return std::dynamic_pointer_cast<Expr>(call);
}

antlrcpp::Any rex_ast_build::visitParenExpr(RexParser::ParenExprContext* ctx) {
    return visit(ctx->expr());
}

antlrcpp::Any rex_ast_build::visitUnaryExpr(RexParser::UnaryExprContext *ctx) {
    auto unary_exp = std::make_shared<UnaryExpr>();
    unary_exp->operation = ctx->op->getText() == "-" ? UniOp::NEG : UniOp::POS;
    unary_exp->rhs = std::any_cast<std::shared_ptr<Expr>>(visit(ctx->expr()));
    unary_exp->type = std::make_shared<Type>(rex::TypeKind::Error);
    return std::dynamic_pointer_cast<Expr>(unary_exp);
}
