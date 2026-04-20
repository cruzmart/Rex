#pragma once
#include "Any.h"
#include "RexParser.h"
#include "RexBaseVisitor.h"
#include "rex_ast_nodes.h"
#include <stdexcept>

using namespace rex;

class rex_ast_build : public RexBaseVisitor {
public:
    // ---------------------- TOP LEVEL ----------------------
    std::unique_ptr<FileAst> build(RexParser::FileContext* ctx);
    SourceLocation loc(antlr4::ParserRuleContext* ctx);

private:
    virtual antlrcpp::Any visitFile(RexParser::FileContext *ctx) override;
    virtual antlrcpp::Any visitItem(RexParser::ItemContext *ctx) override;

    // ------------------------ TYPES ------------------------
    virtual antlrcpp::Any visitTypeDef(RexParser::TypeDefContext *ctx) override;
    virtual antlrcpp::Any visitPrimitiveType(RexParser::PrimitiveTypeContext *ctx) override;
    virtual antlrcpp::Any visitPrimeType(RexParser::PrimeTypeContext *ctx) override;
    virtual antlrcpp::Any visitNamedType(RexParser::NamedTypeContext *ctx) override;
    virtual antlrcpp::Any visitArrayType(RexParser::ArrayTypeContext *ctx) override;
    virtual antlrcpp::Any visitSliceType(RexParser::SliceTypeContext *ctx) override;
    virtual antlrcpp::Any visitTupleType(RexParser::TupleTypeContext *ctx) override;

    // ---------------------- FUNCTIONS ----------------------
    virtual antlrcpp::Any visitFunctionDef(RexParser::FunctionDefContext *ctx) override;
    virtual antlrcpp::Any visitParam(RexParser::ParamContext *ctx) override;
    virtual antlrcpp::Any visitParamList(RexParser::ParamListContext *ctx) override;
    virtual antlrcpp::Any visitReturnType(RexParser::ReturnTypeContext *ctx) override;

    // --------------------- STATEMENTS ---------------------
    virtual antlrcpp::Any visitStatement(RexParser::StatementContext *ctx) override;
    virtual antlrcpp::Any visitLetStmt(RexParser::LetStmtContext *ctx) override;
    virtual antlrcpp::Any visitPattern(RexParser::PatternContext *ctx) override;
    virtual antlrcpp::Any visitReturnStmt(RexParser::ReturnStmtContext *ctx) override;
    virtual antlrcpp::Any visitExprStmt(RexParser::ExprStmtContext *ctx) override;
    virtual antlrcpp::Any visitAssignStmt(RexParser::AssignStmtContext *ctx) override;
    virtual antlrcpp::Any visitIfStmt(RexParser::IfStmtContext *ctx) override;
    virtual antlrcpp::Any visitLoopStmt(RexParser::LoopStmtContext *ctx) override;
    virtual antlrcpp::Any visitBreakStmt(RexParser::BreakStmtContext * ctx) override;
    virtual antlrcpp::Any visitPrintStmt(RexParser::PrintStmtContext * ctx) override;

    // ------------------------ BLOCKS -----------------------
    virtual antlrcpp::Any visitBlock(RexParser::BlockContext *ctx) override;

    // ---------------------- EXPRESSIONS -------------------
    virtual antlrcpp::Any visitIdExpr(RexParser::IdExprContext *ctx) override;
    virtual antlrcpp::Any visitLiteralExpr(RexParser::LiteralExprContext *ctx) override;
    virtual antlrcpp::Any visitLiteral(RexParser::LiteralContext *ctx) override;
    virtual antlrcpp::Any visitTupleExpr(RexParser::TupleExprContext *ctx) override;
    virtual antlrcpp::Any visitArrayExpr(RexParser::ArrayExprContext *ctx) override;
    virtual antlrcpp::Any visitCallExpr(RexParser::CallExprContext *ctx) override;
    virtual antlrcpp::Any visitParenExpr(RexParser::ParenExprContext *ctx) override;
    virtual antlrcpp::Any visitUnaryExpr(RexParser::UnaryExprContext *ctx) override;



    // Binary / operators
    virtual antlrcpp::Any visitAddExpr(RexParser::AddExprContext *ctx) override;
    virtual antlrcpp::Any visitMulExpr(RexParser::MulExprContext *ctx) override;
    virtual antlrcpp::Any visitAndExpr(RexParser::AndExprContext *ctx) override;
    virtual antlrcpp::Any visitOrExpr(RexParser::OrExprContext *ctx) override;
    virtual antlrcpp::Any visitCompareExpr(RexParser::CompareExprContext *ctx) override;
    virtual antlrcpp::Any visitEqualityExpr(RexParser::EqualityExprContext *ctx) override;
    virtual antlrcpp::Any visitPipeExpr(RexParser::PipeExprContext *ctx) override;
    virtual antlrcpp::Any visitRangeExpr(RexParser::RangeExprContext *ctx) override;
    virtual antlrcpp::Any visitIndexExpr(RexParser::IndexExprContext *ctx) override;
    virtual antlrcpp::Any visitIndexTupleExpr(RexParser::IndexTupleExprContext *ctx) override;

};
