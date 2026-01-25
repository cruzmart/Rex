#include "rex_ast_build.h"
#include "RexParser.h"
#include "rex_ast.h"
#include "rex_ast_nodes.h"

#include <cstddef>
#include <memory>

using namespace rex;

// ==================================================
// ------------------- HELPERS ---------------------
// ==================================================

source_location rex_ast_build::loc(antlr4::ParserRuleContext* ctx) {
    return { ctx->getStart()->getLine(), ctx->getStart()->getCharPositionInLine() };
}

template <typename T>
std::shared_ptr<ast_node> try_cast_ast(const antlrcpp::Any& a) {
    if (auto p = std::any_cast<std::shared_ptr<T>>(&a)) {
        return std::dynamic_pointer_cast<ast_node>(*p);
    }
    return nullptr;
}

static std::shared_ptr<ast_node> as_ast_node(const antlrcpp::Any& a) {
    if (auto n = std::any_cast<std::shared_ptr<ast_node>>(&a)) return *n;
    if (auto r = try_cast_ast<stmt>(a)) return r;
    if (auto r = try_cast_ast<type_decl>(a)) return r;
    if (auto r = try_cast_ast<function_decl>(a)) return r;
    if (auto r = try_cast_ast<type_node>(a)) return r;
    throw std::runtime_error("Unknown item type in as_ast_node");
}

binary_op operation_type (const size_t op_token_type) {
    switch(op_token_type) {
        case RexParser::PLUS: return binary_op::add;
        case RexParser::MINUS: return binary_op::sub;
        case RexParser::STAR: return binary_op::mul;
        case RexParser::DIV: return binary_op::div;
        case RexParser::MOD: return binary_op::mod;
        case RexParser::EQ: return binary_op::eq;
        case RexParser::NEQ: return binary_op::neq;
        case RexParser::LT: return binary_op::lt;
        case RexParser::GT: return binary_op::gt;
        case RexParser::LTE: return binary_op::le;
        case RexParser::GTE: return binary_op::ge;
        case RexParser::AND: return binary_op::and_;
        case RexParser::OR: return binary_op::or_; 
        default: throw std::runtime_error("Unknown operation token type");
    }
}

template <typename Ctx>
antlrcpp::Any build_binary_expr(rex_ast_build* self, Ctx* ctx) {
    auto bin = std::make_shared<binary_expr>();
    bin->loc = self->loc(ctx);
    bin->operation = operation_type(ctx->op->getType());
    bin->lhs = std::any_cast<std::shared_ptr<expr>>(self->visit(ctx->expr(0)));
    bin->rhs = std::any_cast<std::shared_ptr<expr>>(self->visit(ctx->expr(1)));
    return std::dynamic_pointer_cast<expr>(bin);
}

// ==================================================
// ------------------- TOP LEVEL -------------------
// ==================================================

antlrcpp::Any rex_ast_build::visitFile(RexParser::FileContext* ctx) {
    auto file = std::make_shared<file_ast>();
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
    auto td = std::make_shared<type_decl>();
    td->loc = loc(ctx);
    td->name = ctx->ID()->getText();
    td->aliased = std::any_cast<std::shared_ptr<type_node>>(visit(ctx->type()));
    return td;
}

antlrcpp::Any rex_ast_build::visitPrimeType(RexParser::PrimeTypeContext* ctx) { 
    return visit(ctx->primitiveType());
}

antlrcpp::Any rex_ast_build::visitPrimitiveType(RexParser::PrimitiveTypeContext *ctx)  {
    auto k = primitive_type::from_name(ctx->getText());
    auto t = std::make_shared<primitive_type>(k);
    t->loc = loc(ctx);
    return std::shared_ptr<type_node>(t);
}

antlrcpp::Any rex_ast_build::visitNamedType(RexParser::NamedTypeContext* ctx) {
    auto t = std::make_shared<named_type>(ctx->ID()->getText());
    t->loc = loc(ctx);
    return std::shared_ptr<type_node>(t);
}

antlrcpp::Any rex_ast_build::visitArrayType(RexParser::ArrayTypeContext* ctx) {
    auto elem = std::any_cast<std::shared_ptr<type_node>>(visit(ctx->type()));
    int size = std::stoi(ctx->INT_LITERAL()->getText());
    auto t = std::make_shared<array_type>(elem, size);
    t->loc = loc(ctx);
    return std::shared_ptr<type_node>(t);
}

antlrcpp::Any rex_ast_build::visitSliceType(RexParser::SliceTypeContext* ctx) { 
    auto elem = std::any_cast<std::shared_ptr<type_node>>(visit(ctx->type()));
    auto t = std::make_shared<slice_type>(elem);
    t->loc = loc(ctx);
    return std::shared_ptr<type_node>(t);
}

antlrcpp::Any rex_ast_build::visitTupleType(RexParser::TupleTypeContext* ctx) { 
    auto tup = std::make_shared<tuple_type>();
    tup->loc = loc(ctx);
    for (auto ty : ctx->type())
        tup->elements.push_back(std::any_cast<std::shared_ptr<type_node>>(visit(ty)));
    return std::shared_ptr<type_node>(tup);
}

// ==================================================
// -------------------- FUNCTIONS ------------------
// ==================================================

antlrcpp::Any rex_ast_build::visitFunctionDef(RexParser::FunctionDefContext* ctx) {
    auto fn = std::make_shared<function_decl>();
    fn->loc = loc(ctx);
    fn->name = ctx->ID()->getText();

    if (ctx->paramList())
        fn->params = std::any_cast<std::vector<param>>(visit(ctx->paramList()));

    if (ctx->returnType()) {
        fn->func_return_type = std::any_cast<std::shared_ptr<type_node>>(visit(ctx->returnType()->type()));
        fn->func_return_type->loc = loc(ctx->returnType());
    } else {
        fn->func_return_type = std::make_shared<primitive_type>(prim_type::void_);
        fn->func_return_type->loc = loc(ctx);
    }

    fn->body = std::any_cast<std::shared_ptr<block_expr>>(visit(ctx->block()));
    return fn;
}

antlrcpp::Any rex_ast_build::visitParam(RexParser::ParamContext* ctx) {
    param p;
    p.name = ctx->ID()->getText();
    p.type = ctx->type() ? std::any_cast<std::shared_ptr<type_node>>(visit(ctx->type()))
                         : std::make_shared<primitive_type>(prim_type::void_);
    return p;
}

antlrcpp::Any rex_ast_build::visitParamList(RexParser::ParamListContext* ctx) {
    std::vector<param> params;
    for (auto p : ctx->param())
        params.push_back(std::any_cast<param>(visit(p)));
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
    auto let = std::make_shared<let_stmt>();
    let->loc = loc(ctx);
    let->name = ctx->pattern()->getText();
    let->init = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr()));
    let->explicit_type = ctx->type() ? std::any_cast<std::shared_ptr<type_node>>(visit(ctx->type()))
                                     : nullptr;
    return std::dynamic_pointer_cast<stmt>(let);
}

antlrcpp::Any rex_ast_build::visitReturnStmt(RexParser::ReturnStmtContext* ctx) {
    auto ret = std::make_shared<return_stmt>();
    ret->loc = loc(ctx);
    if (ctx->expr()) ret->value = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr()));
    return std::dynamic_pointer_cast<stmt>(ret);
}

antlrcpp::Any rex_ast_build::visitExprStmt(RexParser::ExprStmtContext* ctx) {
    auto es = std::make_shared<expr_stmt>();
    es->loc = loc(ctx);
    es->value = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr()));
    return es;
}

antlrcpp::Any rex_ast_build::visitAssignStmt(RexParser::AssignStmtContext *ctx) {
    auto asg = std::make_shared<assign_stmt>();
    asg->target = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr(0)));
    asg->value = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr(1)));
    return std::dynamic_pointer_cast<stmt>(asg);
}

antlrcpp::Any rex_ast_build::visitPattern(RexParser::PatternContext* ctx) {
    if (ctx->ID()) {
        auto p = std::make_shared<pattern_node>();
        p->name = ctx->ID()->getText();
        return p;
    } else {
        auto p = std::make_shared<pattern_node>();
        for (auto sub : ctx->pattern())
            p->elements.push_back(std::any_cast<std::shared_ptr<pattern_node>>(visit(sub)));
        return p;
    }
}

antlrcpp::Any rex_ast_build::visitLoopStmt(RexParser::LoopStmtContext* ctx) {
    if (ctx->WHILE()) {
        auto w = std::make_shared<while_stmt>();
        w->loc = loc(ctx);
        w->cond = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr()));
        w->body = std::any_cast<std::shared_ptr<block_expr>>(visit(ctx->block()));
        return std::dynamic_pointer_cast<stmt>(w);
    }
    if (ctx->FOR()) {
        auto f = std::make_shared<for_stmt>();
        f->loc = loc(ctx);
        f->iter_var = ctx->ID()->getText();
        f->iterable = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr()));
        f->body = std::any_cast<std::shared_ptr<block_expr>>(visit(ctx->block()));
        return std::dynamic_pointer_cast<stmt>(f);
    }
    if (ctx->LOOP()) {
        auto l = std::make_shared<loop_stmt>();
        l->loc = loc(ctx);
        l->body = std::any_cast<std::shared_ptr<block_expr>>(visit(ctx->block()));
        return std::dynamic_pointer_cast<stmt>(l);
    }
    throw std::runtime_error("Unknown loop kind");
}

antlrcpp::Any rex_ast_build::visitIfStmt(RexParser::IfStmtContext *ctx){
    auto if_ = std::make_shared<if_stmt>();
    if_->condition = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr()));
    if_->then_block = std::any_cast<std::shared_ptr<block_expr>>(visit(ctx->block()));

    if(ctx->elifxChain()){
        size_t elif_size = ctx->elifxChain()->expr().size();
        auto elifs_conds = ctx->elifxChain()->expr();
        auto elifs_blocks = ctx->elifxChain()->block();
        for(size_t i = 0; i < elif_size; i++){
            if_->elifx_blocks.emplace_back(
                std::any_cast<std::shared_ptr<expr>>(visit(elifs_conds[i])),
                std::any_cast<std::shared_ptr<block_expr>>(visit(elifs_blocks[i]))
            );
        }
    }

    if(ctx->elseBlock()){
        if_->else_block = std::any_cast<std::shared_ptr<block_expr>>(visit(ctx->elseBlock()->block()));
    }

    return std::dynamic_pointer_cast<stmt>(if_);
}

// ==================================================
// ---------------------- BLOCKS -------------------
// ==================================================

antlrcpp::Any rex_ast_build::visitBlock(RexParser::BlockContext* ctx) {
    auto block = std::make_shared<block_expr>();
    block->loc = loc(ctx);

    for (auto state : ctx->statement())
        block->statements.push_back(std::any_cast<std::shared_ptr<stmt>>(visit(state)));

    if (ctx->expr())
        block->result = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr()));

    return block;
}

// ==================================================
// ------------------- EXPRESSIONS -----------------
// ==================================================

antlrcpp::Any rex_ast_build::visitIdExpr(RexParser::IdExprContext* ctx) {
    auto id = std::make_shared<id_expr>();
    id->loc = loc(ctx);
    id->name = ctx->ID()->getText();
    return std::dynamic_pointer_cast<expr>(id);
}

antlrcpp::Any rex_ast_build::visitLiteralExpr(RexParser::LiteralExprContext* ctx) {
    return visit(ctx->literal());
}

antlrcpp::Any rex_ast_build::visitLiteral(RexParser::LiteralContext* ctx) {
    auto lit = std::make_shared<literal_expr>();
    lit->loc = loc(ctx);
    lit->text = ctx->getText();
    lit->value_kind = classify_literal(lit->text);
    return std::dynamic_pointer_cast<expr>(lit);
}

// Binary expressions
antlrcpp::Any rex_ast_build::visitAddExpr(RexParser::AddExprContext* ctx) {return build_binary_expr(this, ctx);}
antlrcpp::Any rex_ast_build::visitMulExpr(RexParser::MulExprContext* ctx) {return build_binary_expr(this, ctx);}
antlrcpp::Any rex_ast_build::visitAndExpr(RexParser::AndExprContext* ctx) { return build_binary_expr(this, ctx); }
antlrcpp::Any rex_ast_build::visitOrExpr(RexParser::OrExprContext* ctx) { return build_binary_expr(this, ctx); }
antlrcpp::Any rex_ast_build::visitCompareExpr(RexParser::CompareExprContext *ctx) {return build_binary_expr(this, ctx);}
antlrcpp::Any rex_ast_build::visitEqualityExpr(RexParser::EqualityExprContext* ctx) { return build_binary_expr(this, ctx); }    

antlrcpp::Any rex_ast_build::visitPipeExpr(RexParser::PipeExprContext* ctx) { 
    auto pip = std::make_shared<binary_expr>();
    pip->loc = loc(ctx);
    pip->operation = binary_op::pipe;
    pip->lhs = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr(0)));
    pip->rhs = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr(1)));
    return std::dynamic_pointer_cast<expr>(pip);
}

antlrcpp::Any rex_ast_build::visitRangeExpr(RexParser::RangeExprContext* ctx) { 
    auto rng = std::make_shared<binary_expr>();
    rng->loc = loc(ctx);
    rng->operation = binary_op::range_;
    rng->lhs = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr(0)));
    rng->rhs = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr(1)));
    return std::dynamic_pointer_cast<expr>(rng);
}

// Other expressions
antlrcpp::Any rex_ast_build::visitIndexExpr(RexParser::IndexExprContext* ctx) {  
    auto idx = std::make_shared<index_expr>();
    idx->loc = loc(ctx);
    idx->base = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr(0)));
    idx->index = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr(1)));
    return std::dynamic_pointer_cast<expr>(idx);
}

antlrcpp::Any rex_ast_build::visitTupleExpr(RexParser::TupleExprContext* ctx) {
    auto t = std::make_shared<tuple_expr>();
    t->loc = loc(ctx);
    for (auto e : ctx->expr())
        t->elements.push_back(std::any_cast<std::shared_ptr<expr>>(visit(e)));
    return std::dynamic_pointer_cast<expr>(t);
}

antlrcpp::Any rex_ast_build::visitCallExpr(RexParser::CallExprContext* ctx) {
    auto call = std::make_shared<call_expr>();
    call->loc = loc(ctx);
    call->callee = ctx->ID()->getText();
    if (ctx->argList()) {
        for (auto e : ctx->argList()->expr())
            call->args.push_back(std::any_cast<std::shared_ptr<expr>>(visit(e)));
    }
    return std::dynamic_pointer_cast<expr>(call);
}

antlrcpp::Any rex_ast_build::visitParenExpr(RexParser::ParenExprContext* ctx) {
    return visit(ctx->expr());
}

antlrcpp::Any rex_ast_build::visitUnaryExpr(RexParser::UnaryExprContext *ctx) {
    auto unary_exp = std::make_shared<unary_expr>();
    unary_exp->operation = ctx->op->getText() == "-" ? uni_op::neg : uni_op::pos;
    unary_exp->rhs = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr()));
    return std::dynamic_pointer_cast<expr>(unary_exp);
}
