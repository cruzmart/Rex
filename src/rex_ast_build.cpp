
#include "rex_ast_build.h"
#include "RexParser.h"
#include "rex_ast.h"
#include "rex_ast_nodes.h"

#include <cstddef>
#include <iostream>
#include <memory>

using namespace rex;

// --------------------------------------------------
// Helpers
// --------------------------------------------------

source_location rex_ast_build::loc(antlr4::ParserRuleContext* ctx) {
    return {
        ctx->getStart()->getLine(),
        ctx->getStart()->getCharPositionInLine()
    };
}

// --------------------------------------------------
// Helper for converting Any -> ast_node safely in file visit
// ---------------------------------------------
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

    throw std::runtime_error("Unknown item type in as_ast_node");
}
// Building binary expression operation type from token type instead of using string comparisons 
binary_expr::op operation_type (const size_t op_token_type) {
    // Map token types to binary_expr::op enum values
    // 🔑 Make sure these token types match those defined in RexParser.
    // faster with switch-case than if-else chain and enums instead of strings
    switch(op_token_type) {
        case RexParser::PLUS:
            return binary_expr::op::add;
        case RexParser::MINUS:
            return binary_expr::op::sub;
        case RexParser::STAR:
            return binary_expr::op::mul;
        case RexParser::DIV:
            return binary_expr::op::div;
        case RexParser::MOD:
            return binary_expr::op::mod;
        case RexParser::EQ:
            return binary_expr::op::eq;
        case RexParser::NEQ:
            return binary_expr::op::neq;
        case RexParser::LT:    
            return binary_expr::op::lt;
        case RexParser::GT:
            return binary_expr::op::gt;
        case RexParser::LTE:
            return binary_expr::op::le;
        case RexParser::GTE:
            return binary_expr::op::ge;
        case RexParser::AND:
            return binary_expr::op::and_;
        case RexParser::OR:
            return binary_expr::op::or_;   
        default:
            throw std::runtime_error("Unknown operation token type");
    }
}

// Generic builder for binary expressions, to reduce code duplication on visit methods (AddExpr, MulExpr, etc.)
// Uses template to accept different context types
template <typename Ctx>
antlrcpp::Any build_binary_expr(
    rex_ast_build* self,
    Ctx* ctx
) {
    std::cout << "Building binary expression\n";
    auto bin = std::make_shared<binary_expr>();
    bin->loc = self->loc(ctx);
    bin->operation = operation_type(ctx->op->getType());
    bin->lhs = std::any_cast<std::shared_ptr<expr>>(self->visit(ctx->expr(0)));
    bin->rhs = std::any_cast<std::shared_ptr<expr>>(self->visit(ctx->expr(1)));
    return std::dynamic_pointer_cast<expr>(bin);
}


// --------------------------------------------------
// Top-level
// --------------------------------------------------

antlrcpp::Any rex_ast_build::visitFile(RexParser::FileContext* ctx) {
    
    auto file = std::make_shared<file_ast>();
    file->loc = loc(ctx);

    for (auto item : ctx->item()) {
        file->items.push_back(as_ast_node(visit(item)));
        std::cout << "Added item to file AST\n";
    }

    std::cout << "AST file node created with " << file->items.size() << " items.\n";

    return file;
}

antlrcpp::Any rex_ast_build::visitItem(RexParser::ItemContext* ctx) {
      std::cout << "visit function";
    if (ctx->functionDef()){
        return visit(ctx->functionDef());

    }

        

    if (ctx->typeDef())
        return visit(ctx->typeDef());
    std::cout << "Visiting statement item\n";
    return visit(ctx->statement());
}

// --------------------------------------------------
// Types (VERY minimal for now)
// --------------------------------------------------

antlrcpp::Any rex_ast_build::visitTypeDef(RexParser::TypeDefContext* ctx) {
    auto td = std::make_shared<type_decl>();
    td->loc = loc(ctx);
    td->name = ctx->ID()->getText();
    return td;
}

antlrcpp::Any rex_ast_build::visitArrayType(RexParser::ArrayTypeContext* ctx) { return nullptr; }
antlrcpp::Any rex_ast_build::visitNamedType(RexParser::NamedTypeContext* ctx) { return nullptr; }
antlrcpp::Any rex_ast_build::visitPrimeType(RexParser::PrimeTypeContext* ctx) { return nullptr; }
antlrcpp::Any rex_ast_build::visitTupleType(RexParser::TupleTypeContext* ctx) { return nullptr; }
antlrcpp::Any rex_ast_build::visitSliceType(RexParser::SliceTypeContext* ctx) { return nullptr; }
antlrcpp::Any rex_ast_build::visitPrimitiveType(RexParser::PrimitiveTypeContext* ctx) { return nullptr; }
antlrcpp::Any rex_ast_build::visitReturnType(RexParser::ReturnTypeContext* ctx) { return nullptr; }

// --------------------------------------------------
// Functions
// --------------------------------------------------

antlrcpp::Any rex_ast_build::visitFunctionDef(RexParser::FunctionDefContext* ctx) {
    auto fn = std::make_shared<function_decl>();
     std::cout << "here";
    std::string function_name = ctx->ID()->getText();
    fn->loc = loc(ctx);
    fn->name = function_name;
   

    if (ctx->paramList()) {
        auto function_paramaters = std::any_cast<std::vector<param_decl>>(visit(ctx->paramList()));
        fn->params = function_paramaters;
    }
    
    fn->return_type = std::make_shared<type_decl>(ctx->returnType()->getText());
    fn->body = std::any_cast<std::shared_ptr<block_expr>>(visit(ctx->block()));
    return fn;
}
antlrcpp::Any rex_ast_build::visitParam(RexParser::ParamContext* ctx) {
    param_decl param;
    std::string param_name = ctx->ID()->getText();
    param.name = param_name;
    
    if (ctx->type()) {
        std::string param_type = ctx->type()->getText();
        param.type = std::make_shared<type_decl>(param_type);
        param.type->loc = loc(ctx->type());
        param.type->name = param_type;
    } else {
        param.type = std::make_shared<type_decl>();
    }

    return param; // param_decl is returned by value
}

antlrcpp::Any rex_ast_build::visitParamList(RexParser::ParamListContext* ctx) {
    std::vector<param_decl> params;
    for (auto pctx : ctx->param()) {
        params.push_back(std::any_cast<param_decl>(visit(pctx)));
    }
    return params;
}

// --------------------------------------------------
// Statements
// --------------------------------------------------

antlrcpp::Any rex_ast_build::visitStatement(RexParser::StatementContext* ctx) {
    if (ctx->letStmt()) {return visit(ctx->letStmt());}
    if (ctx->returnStmt()) return visit(ctx->returnStmt());
    if (ctx->exprStmt()) return visit(ctx->exprStmt());
    if (ctx->loopStmt()) return visit(ctx->loopStmt());
    return nullptr;
}

antlrcpp::Any rex_ast_build::visitLetStmt(RexParser::LetStmtContext* ctx) {
    auto lets = std::make_shared<let_stmt>();
    lets->loc = loc(ctx);
    lets->name = ctx->pattern()->getText(); // simple for now
    lets->init = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr()));
   
    return std::dynamic_pointer_cast<stmt>(lets);
}

antlrcpp::Any rex_ast_build::visitReturnStmt(RexParser::ReturnStmtContext* ctx) {
    auto ret = std::make_shared<return_stmt>();
    ret->loc = loc(ctx);

    if (ctx->expr())
        ret->value = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr()));

    return ret;
}

antlrcpp::Any rex_ast_build::visitExprStmt(RexParser::ExprStmtContext* ctx) {
    std::cout << "Visiting expression statement\n";
    auto es = std::make_shared<expr_stmt>();
    es->loc = loc(ctx);
    es->value = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr()));
    return es;
}

antlrcpp::Any rex_ast_build::visitPattern(RexParser::PatternContext* ctx) {
    return nullptr;
}

antlrcpp::Any rex_ast_build::visitLoopStmt(RexParser::LoopStmtContext* ctx) {
    return nullptr;
}

// --------------------------------------------------
// Blocks
// --------------------------------------------------

antlrcpp::Any rex_ast_build::visitBlock(RexParser::BlockContext* ctx) {
    auto block = std::make_shared<block_expr>();
    block->loc = loc(ctx);

    for (auto state : ctx->statement()) {
        auto blc = std::any_cast<std::shared_ptr<stmt>>(visit(state));
        block->statements.push_back(blc);
    }

    if (ctx->expr())
        block->result = std::any_cast<std::shared_ptr<expr>>(visit(ctx->expr()));

    return block;
}

// --------------------------------------------------
// Expressions
// --------------------------------------------------

antlrcpp::Any rex_ast_build::visitIdExpr(RexParser::IdExprContext* ctx) {
    std::cout << "Visiting identifier expression\n";
    auto id = std::make_shared<id_expr>();
    id->loc = loc(ctx);
    id->name = ctx->ID()->getText();
    return std::dynamic_pointer_cast<expr>(id);
}

antlrcpp::Any rex_ast_build::visitLiteralExpr(RexParser::LiteralExprContext* ctx) {
    // Just delegating to visitLiteral
    return visit(ctx->literal());
}

// Way cleaner to use the generic binary expr builder beacuse it is all the same logic
antlrcpp::Any rex_ast_build::visitAddExpr(RexParser::AddExprContext* ctx) {return build_binary_expr(this, ctx);}
antlrcpp::Any rex_ast_build::visitMulExpr(RexParser::MulExprContext* ctx) {return build_binary_expr(this, ctx);}
antlrcpp::Any rex_ast_build::visitAndExpr(RexParser::AndExprContext* ctx) { return build_binary_expr(this, ctx); }
antlrcpp::Any rex_ast_build::visitOrExpr(RexParser::OrExprContext* ctx) { return build_binary_expr(this, ctx); }
antlrcpp::Any rex_ast_build::visitCompareExpr(RexParser::CompareExprContext *ctx) {return build_binary_expr(this, ctx);}
antlrcpp::Any rex_ast_build::visitEqualityExpr(RexParser::EqualityExprContext* ctx) { return build_binary_expr(this, ctx); }    

antlrcpp::Any rex_ast_build::visitPipeExpr(RexParser::PipeExprContext* ctx) { return nullptr;}
antlrcpp::Any rex_ast_build::visitRangeExpr(RexParser::RangeExprContext* ctx) { return nullptr; }
antlrcpp::Any rex_ast_build::visitIndexExpr(RexParser::IndexExprContext* ctx) { return nullptr; }

antlrcpp::Any rex_ast_build::visitCallExpr(RexParser::CallExprContext* ctx) {
    auto call = std::make_shared<call_expr>();
    call->loc = loc(ctx);
    call->callee = ctx->ID()->getText();

    if (ctx->argList()) {
        for (auto e : ctx->argList()->expr()) {
            call->args.push_back(
                std::any_cast<std::shared_ptr<expr>>(visit(e))
            );
        }
    }
    return std::dynamic_pointer_cast<expr>(call);
}

antlrcpp::Any rex_ast_build::visitParenExpr(RexParser::ParenExprContext* ctx) {
    return visit(ctx->expr());
}

antlrcpp::Any rex_ast_build::visitArgList(RexParser::ArgListContext* ctx) {
    return nullptr;
}

antlrcpp::Any rex_ast_build::visitLiteral(RexParser::LiteralContext* ctx) {
    // This is similar to visitLiteralExpr, but returns a literal directly
    // When we type promote WE NEED to check the actual type of the literal value and the type of the variable, to ensure compatibility.
    auto lit = std::make_shared<literal_expr>();
    lit->loc = loc(ctx);
    lit->text = ctx->getText();

    // Determine the kind based on the text
    lit->value_kind = classify_literal(lit->text);

    // 🔑 ALWAYS return base type
    return std::dynamic_pointer_cast<expr>(lit);
}
