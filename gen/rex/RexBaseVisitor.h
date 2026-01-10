
// Generated from /home/unknown/projects/Rex/grammar/Rex.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"
#include "RexVisitor.h"


namespace rex {

/**
 * This class provides an empty implementation of RexVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  RexBaseVisitor : public RexVisitor {
public:

  virtual std::any visitFile(RexParser::FileContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitItem(RexParser::ItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypeDef(RexParser::TypeDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArrayType(RexParser::ArrayTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNamedType(RexParser::NamedTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimeType(RexParser::PrimeTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTupleType(RexParser::TupleTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSliceType(RexParser::SliceTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimitiveType(RexParser::PrimitiveTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionDef(RexParser::FunctionDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParamList(RexParser::ParamListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParam(RexParser::ParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnType(RexParser::ReturnTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatement(RexParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLetStmt(RexParser::LetStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignStmt(RexParser::AssignStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnStmt(RexParser::ReturnStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprStmt(RexParser::ExprStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPattern(RexParser::PatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoopStmt(RexParser::LoopStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(RexParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMulExpr(RexParser::MulExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAndExpr(RexParser::AndExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIdExpr(RexParser::IdExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAddExpr(RexParser::AddExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRangeExpr(RexParser::RangeExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrExpr(RexParser::OrExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIndexExpr(RexParser::IndexExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEqualityExpr(RexParser::EqualityExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCompareExpr(RexParser::CompareExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPipeExpr(RexParser::PipeExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteralExpr(RexParser::LiteralExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCallExpr(RexParser::CallExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParenExpr(RexParser::ParenExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArgList(RexParser::ArgListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral(RexParser::LiteralContext *ctx) override {
    return visitChildren(ctx);
  }


};

}  // namespace rex
