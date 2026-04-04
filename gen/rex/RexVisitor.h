
// Generated from /home/unknown/projects/Rex/grammar/Rex.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"
#include "RexParser.h"


namespace rex {

/**
 * This class defines an abstract visitor for a parse tree
 * produced by RexParser.
 */
class  RexVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by RexParser.
   */
    virtual std::any visitFile(RexParser::FileContext *context) = 0;

    virtual std::any visitItem(RexParser::ItemContext *context) = 0;

    virtual std::any visitTypeDef(RexParser::TypeDefContext *context) = 0;

    virtual std::any visitArrayType(RexParser::ArrayTypeContext *context) = 0;

    virtual std::any visitNamedType(RexParser::NamedTypeContext *context) = 0;

    virtual std::any visitPrimeType(RexParser::PrimeTypeContext *context) = 0;

    virtual std::any visitTupleType(RexParser::TupleTypeContext *context) = 0;

    virtual std::any visitSliceType(RexParser::SliceTypeContext *context) = 0;

    virtual std::any visitPrimitiveType(RexParser::PrimitiveTypeContext *context) = 0;

    virtual std::any visitFunctionDef(RexParser::FunctionDefContext *context) = 0;

    virtual std::any visitParamList(RexParser::ParamListContext *context) = 0;

    virtual std::any visitParam(RexParser::ParamContext *context) = 0;

    virtual std::any visitReturnType(RexParser::ReturnTypeContext *context) = 0;

    virtual std::any visitStatement(RexParser::StatementContext *context) = 0;

    virtual std::any visitLetStmt(RexParser::LetStmtContext *context) = 0;

    virtual std::any visitPrintStmt(RexParser::PrintStmtContext *context) = 0;

    virtual std::any visitAssignStmt(RexParser::AssignStmtContext *context) = 0;

    virtual std::any visitReturnStmt(RexParser::ReturnStmtContext *context) = 0;

    virtual std::any visitExprStmt(RexParser::ExprStmtContext *context) = 0;

    virtual std::any visitIfStmt(RexParser::IfStmtContext *context) = 0;

    virtual std::any visitElifxChain(RexParser::ElifxChainContext *context) = 0;

    virtual std::any visitElseBlock(RexParser::ElseBlockContext *context) = 0;

    virtual std::any visitPattern(RexParser::PatternContext *context) = 0;

    virtual std::any visitBreakStmt(RexParser::BreakStmtContext *context) = 0;

    virtual std::any visitLoopStmt(RexParser::LoopStmtContext *context) = 0;

    virtual std::any visitBlock(RexParser::BlockContext *context) = 0;

    virtual std::any visitMulExpr(RexParser::MulExprContext *context) = 0;

    virtual std::any visitAndExpr(RexParser::AndExprContext *context) = 0;

    virtual std::any visitIdExpr(RexParser::IdExprContext *context) = 0;

    virtual std::any visitAddExpr(RexParser::AddExprContext *context) = 0;

    virtual std::any visitRangeExpr(RexParser::RangeExprContext *context) = 0;

    virtual std::any visitUnaryExpr(RexParser::UnaryExprContext *context) = 0;

    virtual std::any visitTupleExpr(RexParser::TupleExprContext *context) = 0;

    virtual std::any visitOrExpr(RexParser::OrExprContext *context) = 0;

    virtual std::any visitIndexExpr(RexParser::IndexExprContext *context) = 0;

    virtual std::any visitArrayExpr(RexParser::ArrayExprContext *context) = 0;

    virtual std::any visitEqualityExpr(RexParser::EqualityExprContext *context) = 0;

    virtual std::any visitCompareExpr(RexParser::CompareExprContext *context) = 0;

    virtual std::any visitPipeExpr(RexParser::PipeExprContext *context) = 0;

    virtual std::any visitLiteralExpr(RexParser::LiteralExprContext *context) = 0;

    virtual std::any visitCallExpr(RexParser::CallExprContext *context) = 0;

    virtual std::any visitParenExpr(RexParser::ParenExprContext *context) = 0;

    virtual std::any visitArgList(RexParser::ArgListContext *context) = 0;

    virtual std::any visitLiteral(RexParser::LiteralContext *context) = 0;


};

}  // namespace rex
