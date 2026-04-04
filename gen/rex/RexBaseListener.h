
// Generated from /home/unknown/projects/Rex/grammar/Rex.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"
#include "RexListener.h"


namespace rex {

/**
 * This class provides an empty implementation of RexListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  RexBaseListener : public RexListener {
public:

  virtual void enterFile(RexParser::FileContext * /*ctx*/) override { }
  virtual void exitFile(RexParser::FileContext * /*ctx*/) override { }

  virtual void enterItem(RexParser::ItemContext * /*ctx*/) override { }
  virtual void exitItem(RexParser::ItemContext * /*ctx*/) override { }

  virtual void enterTypeDef(RexParser::TypeDefContext * /*ctx*/) override { }
  virtual void exitTypeDef(RexParser::TypeDefContext * /*ctx*/) override { }

  virtual void enterArrayType(RexParser::ArrayTypeContext * /*ctx*/) override { }
  virtual void exitArrayType(RexParser::ArrayTypeContext * /*ctx*/) override { }

  virtual void enterNamedType(RexParser::NamedTypeContext * /*ctx*/) override { }
  virtual void exitNamedType(RexParser::NamedTypeContext * /*ctx*/) override { }

  virtual void enterPrimeType(RexParser::PrimeTypeContext * /*ctx*/) override { }
  virtual void exitPrimeType(RexParser::PrimeTypeContext * /*ctx*/) override { }

  virtual void enterTupleType(RexParser::TupleTypeContext * /*ctx*/) override { }
  virtual void exitTupleType(RexParser::TupleTypeContext * /*ctx*/) override { }

  virtual void enterSliceType(RexParser::SliceTypeContext * /*ctx*/) override { }
  virtual void exitSliceType(RexParser::SliceTypeContext * /*ctx*/) override { }

  virtual void enterPrimitiveType(RexParser::PrimitiveTypeContext * /*ctx*/) override { }
  virtual void exitPrimitiveType(RexParser::PrimitiveTypeContext * /*ctx*/) override { }

  virtual void enterFunctionDef(RexParser::FunctionDefContext * /*ctx*/) override { }
  virtual void exitFunctionDef(RexParser::FunctionDefContext * /*ctx*/) override { }

  virtual void enterParamList(RexParser::ParamListContext * /*ctx*/) override { }
  virtual void exitParamList(RexParser::ParamListContext * /*ctx*/) override { }

  virtual void enterParam(RexParser::ParamContext * /*ctx*/) override { }
  virtual void exitParam(RexParser::ParamContext * /*ctx*/) override { }

  virtual void enterReturnType(RexParser::ReturnTypeContext * /*ctx*/) override { }
  virtual void exitReturnType(RexParser::ReturnTypeContext * /*ctx*/) override { }

  virtual void enterStatement(RexParser::StatementContext * /*ctx*/) override { }
  virtual void exitStatement(RexParser::StatementContext * /*ctx*/) override { }

  virtual void enterLetStmt(RexParser::LetStmtContext * /*ctx*/) override { }
  virtual void exitLetStmt(RexParser::LetStmtContext * /*ctx*/) override { }

  virtual void enterPrintStmt(RexParser::PrintStmtContext * /*ctx*/) override { }
  virtual void exitPrintStmt(RexParser::PrintStmtContext * /*ctx*/) override { }

  virtual void enterAssignStmt(RexParser::AssignStmtContext * /*ctx*/) override { }
  virtual void exitAssignStmt(RexParser::AssignStmtContext * /*ctx*/) override { }

  virtual void enterReturnStmt(RexParser::ReturnStmtContext * /*ctx*/) override { }
  virtual void exitReturnStmt(RexParser::ReturnStmtContext * /*ctx*/) override { }

  virtual void enterExprStmt(RexParser::ExprStmtContext * /*ctx*/) override { }
  virtual void exitExprStmt(RexParser::ExprStmtContext * /*ctx*/) override { }

  virtual void enterIfStmt(RexParser::IfStmtContext * /*ctx*/) override { }
  virtual void exitIfStmt(RexParser::IfStmtContext * /*ctx*/) override { }

  virtual void enterElifxChain(RexParser::ElifxChainContext * /*ctx*/) override { }
  virtual void exitElifxChain(RexParser::ElifxChainContext * /*ctx*/) override { }

  virtual void enterElseBlock(RexParser::ElseBlockContext * /*ctx*/) override { }
  virtual void exitElseBlock(RexParser::ElseBlockContext * /*ctx*/) override { }

  virtual void enterPattern(RexParser::PatternContext * /*ctx*/) override { }
  virtual void exitPattern(RexParser::PatternContext * /*ctx*/) override { }

  virtual void enterBreakStmt(RexParser::BreakStmtContext * /*ctx*/) override { }
  virtual void exitBreakStmt(RexParser::BreakStmtContext * /*ctx*/) override { }

  virtual void enterLoopStmt(RexParser::LoopStmtContext * /*ctx*/) override { }
  virtual void exitLoopStmt(RexParser::LoopStmtContext * /*ctx*/) override { }

  virtual void enterBlock(RexParser::BlockContext * /*ctx*/) override { }
  virtual void exitBlock(RexParser::BlockContext * /*ctx*/) override { }

  virtual void enterMulExpr(RexParser::MulExprContext * /*ctx*/) override { }
  virtual void exitMulExpr(RexParser::MulExprContext * /*ctx*/) override { }

  virtual void enterAndExpr(RexParser::AndExprContext * /*ctx*/) override { }
  virtual void exitAndExpr(RexParser::AndExprContext * /*ctx*/) override { }

  virtual void enterIdExpr(RexParser::IdExprContext * /*ctx*/) override { }
  virtual void exitIdExpr(RexParser::IdExprContext * /*ctx*/) override { }

  virtual void enterAddExpr(RexParser::AddExprContext * /*ctx*/) override { }
  virtual void exitAddExpr(RexParser::AddExprContext * /*ctx*/) override { }

  virtual void enterRangeExpr(RexParser::RangeExprContext * /*ctx*/) override { }
  virtual void exitRangeExpr(RexParser::RangeExprContext * /*ctx*/) override { }

  virtual void enterUnaryExpr(RexParser::UnaryExprContext * /*ctx*/) override { }
  virtual void exitUnaryExpr(RexParser::UnaryExprContext * /*ctx*/) override { }

  virtual void enterTupleExpr(RexParser::TupleExprContext * /*ctx*/) override { }
  virtual void exitTupleExpr(RexParser::TupleExprContext * /*ctx*/) override { }

  virtual void enterOrExpr(RexParser::OrExprContext * /*ctx*/) override { }
  virtual void exitOrExpr(RexParser::OrExprContext * /*ctx*/) override { }

  virtual void enterIndexExpr(RexParser::IndexExprContext * /*ctx*/) override { }
  virtual void exitIndexExpr(RexParser::IndexExprContext * /*ctx*/) override { }

  virtual void enterArrayExpr(RexParser::ArrayExprContext * /*ctx*/) override { }
  virtual void exitArrayExpr(RexParser::ArrayExprContext * /*ctx*/) override { }

  virtual void enterEqualityExpr(RexParser::EqualityExprContext * /*ctx*/) override { }
  virtual void exitEqualityExpr(RexParser::EqualityExprContext * /*ctx*/) override { }

  virtual void enterCompareExpr(RexParser::CompareExprContext * /*ctx*/) override { }
  virtual void exitCompareExpr(RexParser::CompareExprContext * /*ctx*/) override { }

  virtual void enterPipeExpr(RexParser::PipeExprContext * /*ctx*/) override { }
  virtual void exitPipeExpr(RexParser::PipeExprContext * /*ctx*/) override { }

  virtual void enterLiteralExpr(RexParser::LiteralExprContext * /*ctx*/) override { }
  virtual void exitLiteralExpr(RexParser::LiteralExprContext * /*ctx*/) override { }

  virtual void enterCallExpr(RexParser::CallExprContext * /*ctx*/) override { }
  virtual void exitCallExpr(RexParser::CallExprContext * /*ctx*/) override { }

  virtual void enterParenExpr(RexParser::ParenExprContext * /*ctx*/) override { }
  virtual void exitParenExpr(RexParser::ParenExprContext * /*ctx*/) override { }

  virtual void enterArgList(RexParser::ArgListContext * /*ctx*/) override { }
  virtual void exitArgList(RexParser::ArgListContext * /*ctx*/) override { }

  virtual void enterLiteral(RexParser::LiteralContext * /*ctx*/) override { }
  virtual void exitLiteral(RexParser::LiteralContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

}  // namespace rex
