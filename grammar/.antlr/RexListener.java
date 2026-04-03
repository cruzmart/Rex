// Generated from /home/unknown/projects/Rex/grammar/Rex.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link RexParser}.
 */
public interface RexListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link RexParser#file}.
	 * @param ctx the parse tree
	 */
	void enterFile(RexParser.FileContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#file}.
	 * @param ctx the parse tree
	 */
	void exitFile(RexParser.FileContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#item}.
	 * @param ctx the parse tree
	 */
	void enterItem(RexParser.ItemContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#item}.
	 * @param ctx the parse tree
	 */
	void exitItem(RexParser.ItemContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#typeDef}.
	 * @param ctx the parse tree
	 */
	void enterTypeDef(RexParser.TypeDefContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#typeDef}.
	 * @param ctx the parse tree
	 */
	void exitTypeDef(RexParser.TypeDefContext ctx);
	/**
	 * Enter a parse tree produced by the {@code ArrayType}
	 * labeled alternative in {@link RexParser#type}.
	 * @param ctx the parse tree
	 */
	void enterArrayType(RexParser.ArrayTypeContext ctx);
	/**
	 * Exit a parse tree produced by the {@code ArrayType}
	 * labeled alternative in {@link RexParser#type}.
	 * @param ctx the parse tree
	 */
	void exitArrayType(RexParser.ArrayTypeContext ctx);
	/**
	 * Enter a parse tree produced by the {@code NamedType}
	 * labeled alternative in {@link RexParser#type}.
	 * @param ctx the parse tree
	 */
	void enterNamedType(RexParser.NamedTypeContext ctx);
	/**
	 * Exit a parse tree produced by the {@code NamedType}
	 * labeled alternative in {@link RexParser#type}.
	 * @param ctx the parse tree
	 */
	void exitNamedType(RexParser.NamedTypeContext ctx);
	/**
	 * Enter a parse tree produced by the {@code PrimeType}
	 * labeled alternative in {@link RexParser#type}.
	 * @param ctx the parse tree
	 */
	void enterPrimeType(RexParser.PrimeTypeContext ctx);
	/**
	 * Exit a parse tree produced by the {@code PrimeType}
	 * labeled alternative in {@link RexParser#type}.
	 * @param ctx the parse tree
	 */
	void exitPrimeType(RexParser.PrimeTypeContext ctx);
	/**
	 * Enter a parse tree produced by the {@code TupleType}
	 * labeled alternative in {@link RexParser#type}.
	 * @param ctx the parse tree
	 */
	void enterTupleType(RexParser.TupleTypeContext ctx);
	/**
	 * Exit a parse tree produced by the {@code TupleType}
	 * labeled alternative in {@link RexParser#type}.
	 * @param ctx the parse tree
	 */
	void exitTupleType(RexParser.TupleTypeContext ctx);
	/**
	 * Enter a parse tree produced by the {@code SliceType}
	 * labeled alternative in {@link RexParser#type}.
	 * @param ctx the parse tree
	 */
	void enterSliceType(RexParser.SliceTypeContext ctx);
	/**
	 * Exit a parse tree produced by the {@code SliceType}
	 * labeled alternative in {@link RexParser#type}.
	 * @param ctx the parse tree
	 */
	void exitSliceType(RexParser.SliceTypeContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#primitiveType}.
	 * @param ctx the parse tree
	 */
	void enterPrimitiveType(RexParser.PrimitiveTypeContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#primitiveType}.
	 * @param ctx the parse tree
	 */
	void exitPrimitiveType(RexParser.PrimitiveTypeContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#functionDef}.
	 * @param ctx the parse tree
	 */
	void enterFunctionDef(RexParser.FunctionDefContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#functionDef}.
	 * @param ctx the parse tree
	 */
	void exitFunctionDef(RexParser.FunctionDefContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#paramList}.
	 * @param ctx the parse tree
	 */
	void enterParamList(RexParser.ParamListContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#paramList}.
	 * @param ctx the parse tree
	 */
	void exitParamList(RexParser.ParamListContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#param}.
	 * @param ctx the parse tree
	 */
	void enterParam(RexParser.ParamContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#param}.
	 * @param ctx the parse tree
	 */
	void exitParam(RexParser.ParamContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#returnType}.
	 * @param ctx the parse tree
	 */
	void enterReturnType(RexParser.ReturnTypeContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#returnType}.
	 * @param ctx the parse tree
	 */
	void exitReturnType(RexParser.ReturnTypeContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#statement}.
	 * @param ctx the parse tree
	 */
	void enterStatement(RexParser.StatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#statement}.
	 * @param ctx the parse tree
	 */
	void exitStatement(RexParser.StatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#letStmt}.
	 * @param ctx the parse tree
	 */
	void enterLetStmt(RexParser.LetStmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#letStmt}.
	 * @param ctx the parse tree
	 */
	void exitLetStmt(RexParser.LetStmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#assignStmt}.
	 * @param ctx the parse tree
	 */
	void enterAssignStmt(RexParser.AssignStmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#assignStmt}.
	 * @param ctx the parse tree
	 */
	void exitAssignStmt(RexParser.AssignStmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#returnStmt}.
	 * @param ctx the parse tree
	 */
	void enterReturnStmt(RexParser.ReturnStmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#returnStmt}.
	 * @param ctx the parse tree
	 */
	void exitReturnStmt(RexParser.ReturnStmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#exprStmt}.
	 * @param ctx the parse tree
	 */
	void enterExprStmt(RexParser.ExprStmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#exprStmt}.
	 * @param ctx the parse tree
	 */
	void exitExprStmt(RexParser.ExprStmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#ifStmt}.
	 * @param ctx the parse tree
	 */
	void enterIfStmt(RexParser.IfStmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#ifStmt}.
	 * @param ctx the parse tree
	 */
	void exitIfStmt(RexParser.IfStmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#elifxChain}.
	 * @param ctx the parse tree
	 */
	void enterElifxChain(RexParser.ElifxChainContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#elifxChain}.
	 * @param ctx the parse tree
	 */
	void exitElifxChain(RexParser.ElifxChainContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#elseBlock}.
	 * @param ctx the parse tree
	 */
	void enterElseBlock(RexParser.ElseBlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#elseBlock}.
	 * @param ctx the parse tree
	 */
	void exitElseBlock(RexParser.ElseBlockContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#pattern}.
	 * @param ctx the parse tree
	 */
	void enterPattern(RexParser.PatternContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#pattern}.
	 * @param ctx the parse tree
	 */
	void exitPattern(RexParser.PatternContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#breakStmt}.
	 * @param ctx the parse tree
	 */
	void enterBreakStmt(RexParser.BreakStmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#breakStmt}.
	 * @param ctx the parse tree
	 */
	void exitBreakStmt(RexParser.BreakStmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#loopStmt}.
	 * @param ctx the parse tree
	 */
	void enterLoopStmt(RexParser.LoopStmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#loopStmt}.
	 * @param ctx the parse tree
	 */
	void exitLoopStmt(RexParser.LoopStmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#block}.
	 * @param ctx the parse tree
	 */
	void enterBlock(RexParser.BlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#block}.
	 * @param ctx the parse tree
	 */
	void exitBlock(RexParser.BlockContext ctx);
	/**
	 * Enter a parse tree produced by the {@code MulExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterMulExpr(RexParser.MulExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code MulExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitMulExpr(RexParser.MulExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code AndExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterAndExpr(RexParser.AndExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code AndExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitAndExpr(RexParser.AndExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code IdExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterIdExpr(RexParser.IdExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code IdExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitIdExpr(RexParser.IdExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code AddExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterAddExpr(RexParser.AddExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code AddExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitAddExpr(RexParser.AddExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code RangeExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterRangeExpr(RexParser.RangeExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code RangeExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitRangeExpr(RexParser.RangeExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code UnaryExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterUnaryExpr(RexParser.UnaryExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code UnaryExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitUnaryExpr(RexParser.UnaryExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code TupleExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterTupleExpr(RexParser.TupleExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code TupleExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitTupleExpr(RexParser.TupleExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code OrExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterOrExpr(RexParser.OrExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code OrExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitOrExpr(RexParser.OrExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code IndexExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterIndexExpr(RexParser.IndexExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code IndexExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitIndexExpr(RexParser.IndexExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code ArrayExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterArrayExpr(RexParser.ArrayExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code ArrayExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitArrayExpr(RexParser.ArrayExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code EqualityExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterEqualityExpr(RexParser.EqualityExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code EqualityExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitEqualityExpr(RexParser.EqualityExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code CompareExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterCompareExpr(RexParser.CompareExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code CompareExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitCompareExpr(RexParser.CompareExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code PipeExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterPipeExpr(RexParser.PipeExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code PipeExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitPipeExpr(RexParser.PipeExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code LiteralExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterLiteralExpr(RexParser.LiteralExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code LiteralExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitLiteralExpr(RexParser.LiteralExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code CallExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterCallExpr(RexParser.CallExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code CallExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitCallExpr(RexParser.CallExprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code ParenExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterParenExpr(RexParser.ParenExprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code ParenExpr}
	 * labeled alternative in {@link RexParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitParenExpr(RexParser.ParenExprContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#argList}.
	 * @param ctx the parse tree
	 */
	void enterArgList(RexParser.ArgListContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#argList}.
	 * @param ctx the parse tree
	 */
	void exitArgList(RexParser.ArgListContext ctx);
	/**
	 * Enter a parse tree produced by {@link RexParser#literal}.
	 * @param ctx the parse tree
	 */
	void enterLiteral(RexParser.LiteralContext ctx);
	/**
	 * Exit a parse tree produced by {@link RexParser#literal}.
	 * @param ctx the parse tree
	 */
	void exitLiteral(RexParser.LiteralContext ctx);
}