
// Generated from /home/unknown/projects/Rex/grammar/Rex.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"
#include "RexParser.h"


namespace rex {

/**
 * This interface defines an abstract listener for a parse tree produced by RexParser.
 */
class  RexListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterFile(RexParser::FileContext *ctx) = 0;
  virtual void exitFile(RexParser::FileContext *ctx) = 0;

  virtual void enterItem(RexParser::ItemContext *ctx) = 0;
  virtual void exitItem(RexParser::ItemContext *ctx) = 0;

  virtual void enterTypeDef(RexParser::TypeDefContext *ctx) = 0;
  virtual void exitTypeDef(RexParser::TypeDefContext *ctx) = 0;

  virtual void enterArrayType(RexParser::ArrayTypeContext *ctx) = 0;
  virtual void exitArrayType(RexParser::ArrayTypeContext *ctx) = 0;

  virtual void enterNamedType(RexParser::NamedTypeContext *ctx) = 0;
  virtual void exitNamedType(RexParser::NamedTypeContext *ctx) = 0;

  virtual void enterPrimeType(RexParser::PrimeTypeContext *ctx) = 0;
  virtual void exitPrimeType(RexParser::PrimeTypeContext *ctx) = 0;

  virtual void enterTupleType(RexParser::TupleTypeContext *ctx) = 0;
  virtual void exitTupleType(RexParser::TupleTypeContext *ctx) = 0;

  virtual void enterSliceType(RexParser::SliceTypeContext *ctx) = 0;
  virtual void exitSliceType(RexParser::SliceTypeContext *ctx) = 0;

  virtual void enterPrimitiveType(RexParser::PrimitiveTypeContext *ctx) = 0;
  virtual void exitPrimitiveType(RexParser::PrimitiveTypeContext *ctx) = 0;

  virtual void enterFunctionDef(RexParser::FunctionDefContext *ctx) = 0;
  virtual void exitFunctionDef(RexParser::FunctionDefContext *ctx) = 0;

  virtual void enterParamList(RexParser::ParamListContext *ctx) = 0;
  virtual void exitParamList(RexParser::ParamListContext *ctx) = 0;

  virtual void enterParam(RexParser::ParamContext *ctx) = 0;
  virtual void exitParam(RexParser::ParamContext *ctx) = 0;

  virtual void enterReturnType(RexParser::ReturnTypeContext *ctx) = 0;
  virtual void exitReturnType(RexParser::ReturnTypeContext *ctx) = 0;

  virtual void enterStatement(RexParser::StatementContext *ctx) = 0;
  virtual void exitStatement(RexParser::StatementContext *ctx) = 0;

  virtual void enterLetStmt(RexParser::LetStmtContext *ctx) = 0;
  virtual void exitLetStmt(RexParser::LetStmtContext *ctx) = 0;

  virtual void enterAssignStmt(RexParser::AssignStmtContext *ctx) = 0;
  virtual void exitAssignStmt(RexParser::AssignStmtContext *ctx) = 0;

  virtual void enterReturnStmt(RexParser::ReturnStmtContext *ctx) = 0;
  virtual void exitReturnStmt(RexParser::ReturnStmtContext *ctx) = 0;

  virtual void enterExprStmt(RexParser::ExprStmtContext *ctx) = 0;
  virtual void exitExprStmt(RexParser::ExprStmtContext *ctx) = 0;

  virtual void enterIfStmt(RexParser::IfStmtContext *ctx) = 0;
  virtual void exitIfStmt(RexParser::IfStmtContext *ctx) = 0;

  virtual void enterElifxChain(RexParser::ElifxChainContext *ctx) = 0;
  virtual void exitElifxChain(RexParser::ElifxChainContext *ctx) = 0;

  virtual void enterElseBlock(RexParser::ElseBlockContext *ctx) = 0;
  virtual void exitElseBlock(RexParser::ElseBlockContext *ctx) = 0;

  virtual void enterPattern(RexParser::PatternContext *ctx) = 0;
  virtual void exitPattern(RexParser::PatternContext *ctx) = 0;

  virtual void enterLoopStmt(RexParser::LoopStmtContext *ctx) = 0;
  virtual void exitLoopStmt(RexParser::LoopStmtContext *ctx) = 0;

  virtual void enterBlock(RexParser::BlockContext *ctx) = 0;
  virtual void exitBlock(RexParser::BlockContext *ctx) = 0;

  virtual void enterMulExpr(RexParser::MulExprContext *ctx) = 0;
  virtual void exitMulExpr(RexParser::MulExprContext *ctx) = 0;

  virtual void enterAndExpr(RexParser::AndExprContext *ctx) = 0;
  virtual void exitAndExpr(RexParser::AndExprContext *ctx) = 0;

  virtual void enterIdExpr(RexParser::IdExprContext *ctx) = 0;
  virtual void exitIdExpr(RexParser::IdExprContext *ctx) = 0;

  virtual void enterAddExpr(RexParser::AddExprContext *ctx) = 0;
  virtual void exitAddExpr(RexParser::AddExprContext *ctx) = 0;

  virtual void enterRangeExpr(RexParser::RangeExprContext *ctx) = 0;
  virtual void exitRangeExpr(RexParser::RangeExprContext *ctx) = 0;

  virtual void enterUnaryExpr(RexParser::UnaryExprContext *ctx) = 0;
  virtual void exitUnaryExpr(RexParser::UnaryExprContext *ctx) = 0;

  virtual void enterTupleExpr(RexParser::TupleExprContext *ctx) = 0;
  virtual void exitTupleExpr(RexParser::TupleExprContext *ctx) = 0;

  virtual void enterOrExpr(RexParser::OrExprContext *ctx) = 0;
  virtual void exitOrExpr(RexParser::OrExprContext *ctx) = 0;

  virtual void enterIndexExpr(RexParser::IndexExprContext *ctx) = 0;
  virtual void exitIndexExpr(RexParser::IndexExprContext *ctx) = 0;

  virtual void enterEqualityExpr(RexParser::EqualityExprContext *ctx) = 0;
  virtual void exitEqualityExpr(RexParser::EqualityExprContext *ctx) = 0;

  virtual void enterCompareExpr(RexParser::CompareExprContext *ctx) = 0;
  virtual void exitCompareExpr(RexParser::CompareExprContext *ctx) = 0;

  virtual void enterPipeExpr(RexParser::PipeExprContext *ctx) = 0;
  virtual void exitPipeExpr(RexParser::PipeExprContext *ctx) = 0;

  virtual void enterLiteralExpr(RexParser::LiteralExprContext *ctx) = 0;
  virtual void exitLiteralExpr(RexParser::LiteralExprContext *ctx) = 0;

  virtual void enterCallExpr(RexParser::CallExprContext *ctx) = 0;
  virtual void exitCallExpr(RexParser::CallExprContext *ctx) = 0;

  virtual void enterParenExpr(RexParser::ParenExprContext *ctx) = 0;
  virtual void exitParenExpr(RexParser::ParenExprContext *ctx) = 0;

  virtual void enterArgList(RexParser::ArgListContext *ctx) = 0;
  virtual void exitArgList(RexParser::ArgListContext *ctx) = 0;

  virtual void enterLiteral(RexParser::LiteralContext *ctx) = 0;
  virtual void exitLiteral(RexParser::LiteralContext *ctx) = 0;


};

}  // namespace rex
