// Generated from /home/unknown/projects/Rex/grammar/Rex.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue"})
public class RexParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.13.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		IF=1, ELIFX=2, ELSE=3, FN=4, LET=5, MUT=6, FOR=7, WHILE=8, LOOP=9, IN=10, 
		RETURN=11, TYPE=12, PRINT=13, INT=14, BOOL=15, CHAR=16, REAL=17, STRING=18, 
		TRUE=19, FALSE=20, AND=21, OR=22, ARROW=23, PIPE=24, ASSIGN=25, EQ=26, 
		NEQ=27, LT=28, GT=29, LTE=30, GTE=31, PLUS=32, MINUS=33, STAR=34, DIV=35, 
		MOD=36, RANGE=37, LPAREN=38, RPAREN=39, LBRACE=40, RBRACE=41, LBRACK=42, 
		RBRACK=43, COMMA=44, SEMI=45, COLON=46, BREAK=47, ID=48, INT_LITERAL=49, 
		REAL_LITERAL=50, STRING_LITERAL=51, CHAR_LITERAL=52, WS=53, LINE_COMMENT=54, 
		BLOCK_COMMENT=55;
	public static final int
		RULE_file = 0, RULE_item = 1, RULE_typeDef = 2, RULE_type = 3, RULE_primitiveType = 4, 
		RULE_functionDef = 5, RULE_paramList = 6, RULE_param = 7, RULE_returnType = 8, 
		RULE_statement = 9, RULE_letStmt = 10, RULE_printStmt = 11, RULE_assignStmt = 12, 
		RULE_returnStmt = 13, RULE_exprStmt = 14, RULE_ifStmt = 15, RULE_elifxChain = 16, 
		RULE_elseBlock = 17, RULE_pattern = 18, RULE_breakStmt = 19, RULE_loopStmt = 20, 
		RULE_block = 21, RULE_expr = 22, RULE_argList = 23, RULE_literal = 24;
	private static String[] makeRuleNames() {
		return new String[] {
			"file", "item", "typeDef", "type", "primitiveType", "functionDef", "paramList", 
			"param", "returnType", "statement", "letStmt", "printStmt", "assignStmt", 
			"returnStmt", "exprStmt", "ifStmt", "elifxChain", "elseBlock", "pattern", 
			"breakStmt", "loopStmt", "block", "expr", "argList", "literal"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'if'", "'elifx'", "'else'", "'function'", "'let'", "'mut'", "'for'", 
			"'while'", "'loop'", "'in'", "'return'", "'type'", "'print'", "'Int'", 
			"'Bool'", "'Char'", "'Real'", "'String'", "'true'", "'false'", "'and'", 
			"'or'", "'->'", "'|>'", "'='", "'=='", "'!='", "'<'", "'>'", "'<='", 
			"'>='", "'+'", "'-'", "'*'", "'/'", "'%'", "'..'", "'('", "')'", "'{'", 
			"'}'", "'['", "']'", "','", "';'", "':'", "'break'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, "IF", "ELIFX", "ELSE", "FN", "LET", "MUT", "FOR", "WHILE", "LOOP", 
			"IN", "RETURN", "TYPE", "PRINT", "INT", "BOOL", "CHAR", "REAL", "STRING", 
			"TRUE", "FALSE", "AND", "OR", "ARROW", "PIPE", "ASSIGN", "EQ", "NEQ", 
			"LT", "GT", "LTE", "GTE", "PLUS", "MINUS", "STAR", "DIV", "MOD", "RANGE", 
			"LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACK", "RBRACK", "COMMA", 
			"SEMI", "COLON", "BREAK", "ID", "INT_LITERAL", "REAL_LITERAL", "STRING_LITERAL", 
			"CHAR_LITERAL", "WS", "LINE_COMMENT", "BLOCK_COMMENT"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "Rex.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public RexParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FileContext extends ParserRuleContext {
		public TerminalNode EOF() { return getToken(RexParser.EOF, 0); }
		public List<ItemContext> item() {
			return getRuleContexts(ItemContext.class);
		}
		public ItemContext item(int i) {
			return getRuleContext(ItemContext.class,i);
		}
		public FileContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_file; }
	}

	public final FileContext file() throws RecognitionException {
		FileContext _localctx = new FileContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_file);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(53);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 8871147577293746L) != 0)) {
				{
				{
				setState(50);
				item();
				}
				}
				setState(55);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(56);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ItemContext extends ParserRuleContext {
		public FunctionDefContext functionDef() {
			return getRuleContext(FunctionDefContext.class,0);
		}
		public StatementContext statement() {
			return getRuleContext(StatementContext.class,0);
		}
		public ItemContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_item; }
	}

	public final ItemContext item() throws RecognitionException {
		ItemContext _localctx = new ItemContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_item);
		try {
			setState(60);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case FN:
				enterOuterAlt(_localctx, 1);
				{
				setState(58);
				functionDef();
				}
				break;
			case IF:
			case LET:
			case FOR:
			case WHILE:
			case LOOP:
			case RETURN:
			case TYPE:
			case PRINT:
			case TRUE:
			case FALSE:
			case PLUS:
			case MINUS:
			case LPAREN:
			case LBRACK:
			case BREAK:
			case ID:
			case INT_LITERAL:
			case REAL_LITERAL:
			case STRING_LITERAL:
			case CHAR_LITERAL:
				enterOuterAlt(_localctx, 2);
				{
				setState(59);
				statement();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class TypeDefContext extends ParserRuleContext {
		public TerminalNode TYPE() { return getToken(RexParser.TYPE, 0); }
		public TerminalNode ID() { return getToken(RexParser.ID, 0); }
		public TerminalNode ASSIGN() { return getToken(RexParser.ASSIGN, 0); }
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public TerminalNode SEMI() { return getToken(RexParser.SEMI, 0); }
		public TypeDefContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_typeDef; }
	}

	public final TypeDefContext typeDef() throws RecognitionException {
		TypeDefContext _localctx = new TypeDefContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_typeDef);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(62);
			match(TYPE);
			setState(63);
			match(ID);
			setState(64);
			match(ASSIGN);
			setState(65);
			type(0);
			setState(66);
			match(SEMI);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class TypeContext extends ParserRuleContext {
		public TypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_type; }
	 
		public TypeContext() { }
		public void copyFrom(TypeContext ctx) {
			super.copyFrom(ctx);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class ArrayTypeContext extends TypeContext {
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public TerminalNode LBRACK() { return getToken(RexParser.LBRACK, 0); }
		public TerminalNode INT_LITERAL() { return getToken(RexParser.INT_LITERAL, 0); }
		public TerminalNode RBRACK() { return getToken(RexParser.RBRACK, 0); }
		public ArrayTypeContext(TypeContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class NamedTypeContext extends TypeContext {
		public TerminalNode ID() { return getToken(RexParser.ID, 0); }
		public NamedTypeContext(TypeContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class PrimeTypeContext extends TypeContext {
		public PrimitiveTypeContext primitiveType() {
			return getRuleContext(PrimitiveTypeContext.class,0);
		}
		public PrimeTypeContext(TypeContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class TupleTypeContext extends TypeContext {
		public TerminalNode LPAREN() { return getToken(RexParser.LPAREN, 0); }
		public List<TypeContext> type() {
			return getRuleContexts(TypeContext.class);
		}
		public TypeContext type(int i) {
			return getRuleContext(TypeContext.class,i);
		}
		public TerminalNode RPAREN() { return getToken(RexParser.RPAREN, 0); }
		public List<TerminalNode> COMMA() { return getTokens(RexParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(RexParser.COMMA, i);
		}
		public TupleTypeContext(TypeContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class SliceTypeContext extends TypeContext {
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public TerminalNode LBRACK() { return getToken(RexParser.LBRACK, 0); }
		public TerminalNode RBRACK() { return getToken(RexParser.RBRACK, 0); }
		public SliceTypeContext(TypeContext ctx) { copyFrom(ctx); }
	}

	public final TypeContext type() throws RecognitionException {
		return type(0);
	}

	private TypeContext type(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		TypeContext _localctx = new TypeContext(_ctx, _parentState);
		TypeContext _prevctx = _localctx;
		int _startState = 6;
		enterRecursionRule(_localctx, 6, RULE_type, _p);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(81);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case INT:
			case BOOL:
			case CHAR:
			case REAL:
			case STRING:
				{
				_localctx = new PrimeTypeContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;

				setState(69);
				primitiveType();
				}
				break;
			case ID:
				{
				_localctx = new NamedTypeContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(70);
				match(ID);
				}
				break;
			case LPAREN:
				{
				_localctx = new TupleTypeContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(71);
				match(LPAREN);
				setState(72);
				type(0);
				setState(75); 
				_errHandler.sync(this);
				_la = _input.LA(1);
				do {
					{
					{
					setState(73);
					match(COMMA);
					setState(74);
					type(0);
					}
					}
					setState(77); 
					_errHandler.sync(this);
					_la = _input.LA(1);
				} while ( _la==COMMA );
				setState(79);
				match(RPAREN);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			_ctx.stop = _input.LT(-1);
			setState(92);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,5,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(90);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,4,_ctx) ) {
					case 1:
						{
						_localctx = new SliceTypeContext(new TypeContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_type);
						setState(83);
						if (!(precpred(_ctx, 3))) throw new FailedPredicateException(this, "precpred(_ctx, 3)");
						setState(84);
						match(LBRACK);
						setState(85);
						match(RBRACK);
						}
						break;
					case 2:
						{
						_localctx = new ArrayTypeContext(new TypeContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_type);
						setState(86);
						if (!(precpred(_ctx, 2))) throw new FailedPredicateException(this, "precpred(_ctx, 2)");
						setState(87);
						match(LBRACK);
						setState(88);
						match(INT_LITERAL);
						setState(89);
						match(RBRACK);
						}
						break;
					}
					} 
				}
				setState(94);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,5,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class PrimitiveTypeContext extends ParserRuleContext {
		public TerminalNode INT() { return getToken(RexParser.INT, 0); }
		public TerminalNode BOOL() { return getToken(RexParser.BOOL, 0); }
		public TerminalNode CHAR() { return getToken(RexParser.CHAR, 0); }
		public TerminalNode REAL() { return getToken(RexParser.REAL, 0); }
		public TerminalNode STRING() { return getToken(RexParser.STRING, 0); }
		public PrimitiveTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_primitiveType; }
	}

	public final PrimitiveTypeContext primitiveType() throws RecognitionException {
		PrimitiveTypeContext _localctx = new PrimitiveTypeContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_primitiveType);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(95);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 507904L) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FunctionDefContext extends ParserRuleContext {
		public TerminalNode FN() { return getToken(RexParser.FN, 0); }
		public TerminalNode ID() { return getToken(RexParser.ID, 0); }
		public TerminalNode LPAREN() { return getToken(RexParser.LPAREN, 0); }
		public TerminalNode RPAREN() { return getToken(RexParser.RPAREN, 0); }
		public BlockContext block() {
			return getRuleContext(BlockContext.class,0);
		}
		public ParamListContext paramList() {
			return getRuleContext(ParamListContext.class,0);
		}
		public ReturnTypeContext returnType() {
			return getRuleContext(ReturnTypeContext.class,0);
		}
		public FunctionDefContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_functionDef; }
	}

	public final FunctionDefContext functionDef() throws RecognitionException {
		FunctionDefContext _localctx = new FunctionDefContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_functionDef);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(97);
			match(FN);
			setState(98);
			match(ID);
			setState(99);
			match(LPAREN);
			setState(101);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==MUT || _la==ID) {
				{
				setState(100);
				paramList();
				}
			}

			setState(103);
			match(RPAREN);
			setState(105);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==ARROW) {
				{
				setState(104);
				returnType();
				}
			}

			setState(107);
			block();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ParamListContext extends ParserRuleContext {
		public List<ParamContext> param() {
			return getRuleContexts(ParamContext.class);
		}
		public ParamContext param(int i) {
			return getRuleContext(ParamContext.class,i);
		}
		public List<TerminalNode> COMMA() { return getTokens(RexParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(RexParser.COMMA, i);
		}
		public ParamListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_paramList; }
	}

	public final ParamListContext paramList() throws RecognitionException {
		ParamListContext _localctx = new ParamListContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_paramList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(109);
			param();
			setState(114);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(110);
				match(COMMA);
				setState(111);
				param();
				}
				}
				setState(116);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ParamContext extends ParserRuleContext {
		public TerminalNode ID() { return getToken(RexParser.ID, 0); }
		public TerminalNode COLON() { return getToken(RexParser.COLON, 0); }
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public TerminalNode MUT() { return getToken(RexParser.MUT, 0); }
		public ParamContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_param; }
	}

	public final ParamContext param() throws RecognitionException {
		ParamContext _localctx = new ParamContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_param);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(118);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==MUT) {
				{
				setState(117);
				match(MUT);
				}
			}

			setState(120);
			match(ID);
			setState(121);
			match(COLON);
			setState(122);
			type(0);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ReturnTypeContext extends ParserRuleContext {
		public TerminalNode ARROW() { return getToken(RexParser.ARROW, 0); }
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public ReturnTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_returnType; }
	}

	public final ReturnTypeContext returnType() throws RecognitionException {
		ReturnTypeContext _localctx = new ReturnTypeContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_returnType);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(124);
			match(ARROW);
			setState(125);
			type(0);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class StatementContext extends ParserRuleContext {
		public LetStmtContext letStmt() {
			return getRuleContext(LetStmtContext.class,0);
		}
		public AssignStmtContext assignStmt() {
			return getRuleContext(AssignStmtContext.class,0);
		}
		public IfStmtContext ifStmt() {
			return getRuleContext(IfStmtContext.class,0);
		}
		public LoopStmtContext loopStmt() {
			return getRuleContext(LoopStmtContext.class,0);
		}
		public ExprStmtContext exprStmt() {
			return getRuleContext(ExprStmtContext.class,0);
		}
		public TypeDefContext typeDef() {
			return getRuleContext(TypeDefContext.class,0);
		}
		public PrintStmtContext printStmt() {
			return getRuleContext(PrintStmtContext.class,0);
		}
		public ReturnStmtContext returnStmt() {
			return getRuleContext(ReturnStmtContext.class,0);
		}
		public BreakStmtContext breakStmt() {
			return getRuleContext(BreakStmtContext.class,0);
		}
		public StatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_statement; }
	}

	public final StatementContext statement() throws RecognitionException {
		StatementContext _localctx = new StatementContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_statement);
		try {
			setState(137);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,10,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(127);
				letStmt();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(128);
				assignStmt();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(129);
				ifStmt();
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(130);
				loopStmt();
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(131);
				exprStmt();
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(132);
				typeDef();
				}
				break;
			case 7:
				enterOuterAlt(_localctx, 7);
				{
				setState(133);
				printStmt();
				}
				break;
			case 8:
				enterOuterAlt(_localctx, 8);
				{
				setState(134);
				returnStmt();
				}
				break;
			case 9:
				enterOuterAlt(_localctx, 9);
				{
				setState(135);
				exprStmt();
				}
				break;
			case 10:
				enterOuterAlt(_localctx, 10);
				{
				setState(136);
				breakStmt();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class LetStmtContext extends ParserRuleContext {
		public TerminalNode LET() { return getToken(RexParser.LET, 0); }
		public PatternContext pattern() {
			return getRuleContext(PatternContext.class,0);
		}
		public TerminalNode ASSIGN() { return getToken(RexParser.ASSIGN, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public TerminalNode SEMI() { return getToken(RexParser.SEMI, 0); }
		public TerminalNode COLON() { return getToken(RexParser.COLON, 0); }
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public LetStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_letStmt; }
	}

	public final LetStmtContext letStmt() throws RecognitionException {
		LetStmtContext _localctx = new LetStmtContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_letStmt);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(139);
			match(LET);
			setState(140);
			pattern();
			setState(143);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COLON) {
				{
				setState(141);
				match(COLON);
				setState(142);
				type(0);
				}
			}

			setState(145);
			match(ASSIGN);
			setState(146);
			expr(0);
			setState(147);
			match(SEMI);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class PrintStmtContext extends ParserRuleContext {
		public TerminalNode PRINT() { return getToken(RexParser.PRINT, 0); }
		public TerminalNode LPAREN() { return getToken(RexParser.LPAREN, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public TerminalNode RPAREN() { return getToken(RexParser.RPAREN, 0); }
		public TerminalNode SEMI() { return getToken(RexParser.SEMI, 0); }
		public PrintStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_printStmt; }
	}

	public final PrintStmtContext printStmt() throws RecognitionException {
		PrintStmtContext _localctx = new PrintStmtContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_printStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(149);
			match(PRINT);
			setState(150);
			match(LPAREN);
			setState(151);
			expr(0);
			setState(152);
			match(RPAREN);
			setState(153);
			match(SEMI);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class AssignStmtContext extends ParserRuleContext {
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode ASSIGN() { return getToken(RexParser.ASSIGN, 0); }
		public TerminalNode SEMI() { return getToken(RexParser.SEMI, 0); }
		public AssignStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_assignStmt; }
	}

	public final AssignStmtContext assignStmt() throws RecognitionException {
		AssignStmtContext _localctx = new AssignStmtContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_assignStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(155);
			expr(0);
			setState(156);
			match(ASSIGN);
			setState(157);
			expr(0);
			setState(158);
			match(SEMI);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ReturnStmtContext extends ParserRuleContext {
		public TerminalNode RETURN() { return getToken(RexParser.RETURN, 0); }
		public TerminalNode SEMI() { return getToken(RexParser.SEMI, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public ReturnStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_returnStmt; }
	}

	public final ReturnStmtContext returnStmt() throws RecognitionException {
		ReturnStmtContext _localctx = new ReturnStmtContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_returnStmt);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(160);
			match(RETURN);
			setState(162);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 8730410088923136L) != 0)) {
				{
				setState(161);
				expr(0);
				}
			}

			setState(164);
			match(SEMI);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ExprStmtContext extends ParserRuleContext {
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public TerminalNode SEMI() { return getToken(RexParser.SEMI, 0); }
		public ExprStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_exprStmt; }
	}

	public final ExprStmtContext exprStmt() throws RecognitionException {
		ExprStmtContext _localctx = new ExprStmtContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_exprStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(166);
			expr(0);
			setState(167);
			match(SEMI);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class IfStmtContext extends ParserRuleContext {
		public TerminalNode IF() { return getToken(RexParser.IF, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public BlockContext block() {
			return getRuleContext(BlockContext.class,0);
		}
		public ElifxChainContext elifxChain() {
			return getRuleContext(ElifxChainContext.class,0);
		}
		public ElseBlockContext elseBlock() {
			return getRuleContext(ElseBlockContext.class,0);
		}
		public IfStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_ifStmt; }
	}

	public final IfStmtContext ifStmt() throws RecognitionException {
		IfStmtContext _localctx = new IfStmtContext(_ctx, getState());
		enterRule(_localctx, 30, RULE_ifStmt);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(169);
			match(IF);
			setState(170);
			expr(0);
			setState(171);
			block();
			setState(173);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==ELIFX) {
				{
				setState(172);
				elifxChain();
				}
			}

			setState(176);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==ELSE) {
				{
				setState(175);
				elseBlock();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ElifxChainContext extends ParserRuleContext {
		public List<TerminalNode> ELIFX() { return getTokens(RexParser.ELIFX); }
		public TerminalNode ELIFX(int i) {
			return getToken(RexParser.ELIFX, i);
		}
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public List<BlockContext> block() {
			return getRuleContexts(BlockContext.class);
		}
		public BlockContext block(int i) {
			return getRuleContext(BlockContext.class,i);
		}
		public ElifxChainContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_elifxChain; }
	}

	public final ElifxChainContext elifxChain() throws RecognitionException {
		ElifxChainContext _localctx = new ElifxChainContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_elifxChain);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(182); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(178);
				match(ELIFX);
				setState(179);
				expr(0);
				setState(180);
				block();
				}
				}
				setState(184); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==ELIFX );
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ElseBlockContext extends ParserRuleContext {
		public TerminalNode ELSE() { return getToken(RexParser.ELSE, 0); }
		public BlockContext block() {
			return getRuleContext(BlockContext.class,0);
		}
		public ElseBlockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_elseBlock; }
	}

	public final ElseBlockContext elseBlock() throws RecognitionException {
		ElseBlockContext _localctx = new ElseBlockContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_elseBlock);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(186);
			match(ELSE);
			setState(187);
			block();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class PatternContext extends ParserRuleContext {
		public List<TerminalNode> ID() { return getTokens(RexParser.ID); }
		public TerminalNode ID(int i) {
			return getToken(RexParser.ID, i);
		}
		public TerminalNode LPAREN() { return getToken(RexParser.LPAREN, 0); }
		public TerminalNode RPAREN() { return getToken(RexParser.RPAREN, 0); }
		public List<TerminalNode> COMMA() { return getTokens(RexParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(RexParser.COMMA, i);
		}
		public PatternContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_pattern; }
	}

	public final PatternContext pattern() throws RecognitionException {
		PatternContext _localctx = new PatternContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_pattern);
		int _la;
		try {
			setState(199);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case ID:
				enterOuterAlt(_localctx, 1);
				{
				setState(189);
				match(ID);
				}
				break;
			case LPAREN:
				enterOuterAlt(_localctx, 2);
				{
				setState(190);
				match(LPAREN);
				setState(191);
				match(ID);
				setState(194); 
				_errHandler.sync(this);
				_la = _input.LA(1);
				do {
					{
					{
					setState(192);
					match(COMMA);
					setState(193);
					match(ID);
					}
					}
					setState(196); 
					_errHandler.sync(this);
					_la = _input.LA(1);
				} while ( _la==COMMA );
				setState(198);
				match(RPAREN);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class BreakStmtContext extends ParserRuleContext {
		public TerminalNode BREAK() { return getToken(RexParser.BREAK, 0); }
		public TerminalNode SEMI() { return getToken(RexParser.SEMI, 0); }
		public BreakStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_breakStmt; }
	}

	public final BreakStmtContext breakStmt() throws RecognitionException {
		BreakStmtContext _localctx = new BreakStmtContext(_ctx, getState());
		enterRule(_localctx, 38, RULE_breakStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(201);
			match(BREAK);
			setState(202);
			match(SEMI);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class LoopStmtContext extends ParserRuleContext {
		public TerminalNode WHILE() { return getToken(RexParser.WHILE, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public BlockContext block() {
			return getRuleContext(BlockContext.class,0);
		}
		public TerminalNode FOR() { return getToken(RexParser.FOR, 0); }
		public TerminalNode ID() { return getToken(RexParser.ID, 0); }
		public TerminalNode IN() { return getToken(RexParser.IN, 0); }
		public TerminalNode LOOP() { return getToken(RexParser.LOOP, 0); }
		public LoopStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_loopStmt; }
	}

	public final LoopStmtContext loopStmt() throws RecognitionException {
		LoopStmtContext _localctx = new LoopStmtContext(_ctx, getState());
		enterRule(_localctx, 40, RULE_loopStmt);
		try {
			setState(216);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case WHILE:
				enterOuterAlt(_localctx, 1);
				{
				setState(204);
				match(WHILE);
				setState(205);
				expr(0);
				setState(206);
				block();
				}
				break;
			case FOR:
				enterOuterAlt(_localctx, 2);
				{
				setState(208);
				match(FOR);
				setState(209);
				match(ID);
				setState(210);
				match(IN);
				setState(211);
				expr(0);
				setState(212);
				block();
				}
				break;
			case LOOP:
				enterOuterAlt(_localctx, 3);
				{
				setState(214);
				match(LOOP);
				setState(215);
				block();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class BlockContext extends ParserRuleContext {
		public TerminalNode LBRACE() { return getToken(RexParser.LBRACE, 0); }
		public TerminalNode RBRACE() { return getToken(RexParser.RBRACE, 0); }
		public List<StatementContext> statement() {
			return getRuleContexts(StatementContext.class);
		}
		public StatementContext statement(int i) {
			return getRuleContext(StatementContext.class,i);
		}
		public BlockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_block; }
	}

	public final BlockContext block() throws RecognitionException {
		BlockContext _localctx = new BlockContext(_ctx, getState());
		enterRule(_localctx, 42, RULE_block);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(218);
			match(LBRACE);
			setState(222);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 8871147577293730L) != 0)) {
				{
				{
				setState(219);
				statement();
				}
				}
				setState(224);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(225);
			match(RBRACE);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ExprContext extends ParserRuleContext {
		public ExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expr; }
	 
		public ExprContext() { }
		public void copyFrom(ExprContext ctx) {
			super.copyFrom(ctx);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class MulExprContext extends ExprContext {
		public Token op;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode STAR() { return getToken(RexParser.STAR, 0); }
		public TerminalNode DIV() { return getToken(RexParser.DIV, 0); }
		public TerminalNode MOD() { return getToken(RexParser.MOD, 0); }
		public MulExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class AndExprContext extends ExprContext {
		public Token op;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode AND() { return getToken(RexParser.AND, 0); }
		public AndExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class IdExprContext extends ExprContext {
		public TerminalNode ID() { return getToken(RexParser.ID, 0); }
		public IdExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class AddExprContext extends ExprContext {
		public Token op;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode PLUS() { return getToken(RexParser.PLUS, 0); }
		public TerminalNode MINUS() { return getToken(RexParser.MINUS, 0); }
		public AddExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class RangeExprContext extends ExprContext {
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode RANGE() { return getToken(RexParser.RANGE, 0); }
		public RangeExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class UnaryExprContext extends ExprContext {
		public Token op;
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public TerminalNode MINUS() { return getToken(RexParser.MINUS, 0); }
		public TerminalNode PLUS() { return getToken(RexParser.PLUS, 0); }
		public UnaryExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class TupleExprContext extends ExprContext {
		public TerminalNode LPAREN() { return getToken(RexParser.LPAREN, 0); }
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode RPAREN() { return getToken(RexParser.RPAREN, 0); }
		public List<TerminalNode> COMMA() { return getTokens(RexParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(RexParser.COMMA, i);
		}
		public TupleExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class OrExprContext extends ExprContext {
		public Token op;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode OR() { return getToken(RexParser.OR, 0); }
		public OrExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class IndexExprContext extends ExprContext {
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode LBRACK() { return getToken(RexParser.LBRACK, 0); }
		public TerminalNode RBRACK() { return getToken(RexParser.RBRACK, 0); }
		public IndexExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class ArrayExprContext extends ExprContext {
		public TerminalNode LBRACK() { return getToken(RexParser.LBRACK, 0); }
		public TerminalNode RBRACK() { return getToken(RexParser.RBRACK, 0); }
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public List<TerminalNode> COMMA() { return getTokens(RexParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(RexParser.COMMA, i);
		}
		public ArrayExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class EqualityExprContext extends ExprContext {
		public Token op;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode EQ() { return getToken(RexParser.EQ, 0); }
		public TerminalNode NEQ() { return getToken(RexParser.NEQ, 0); }
		public EqualityExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class CompareExprContext extends ExprContext {
		public Token op;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode LT() { return getToken(RexParser.LT, 0); }
		public TerminalNode GT() { return getToken(RexParser.GT, 0); }
		public TerminalNode LTE() { return getToken(RexParser.LTE, 0); }
		public TerminalNode GTE() { return getToken(RexParser.GTE, 0); }
		public CompareExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class PipeExprContext extends ExprContext {
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode PIPE() { return getToken(RexParser.PIPE, 0); }
		public PipeExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class LiteralExprContext extends ExprContext {
		public LiteralContext literal() {
			return getRuleContext(LiteralContext.class,0);
		}
		public LiteralExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class CallExprContext extends ExprContext {
		public TerminalNode ID() { return getToken(RexParser.ID, 0); }
		public TerminalNode LPAREN() { return getToken(RexParser.LPAREN, 0); }
		public TerminalNode RPAREN() { return getToken(RexParser.RPAREN, 0); }
		public ArgListContext argList() {
			return getRuleContext(ArgListContext.class,0);
		}
		public CallExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class ParenExprContext extends ExprContext {
		public TerminalNode LPAREN() { return getToken(RexParser.LPAREN, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public TerminalNode RPAREN() { return getToken(RexParser.RPAREN, 0); }
		public ParenExprContext(ExprContext ctx) { copyFrom(ctx); }
	}

	public final ExprContext expr() throws RecognitionException {
		return expr(0);
	}

	private ExprContext expr(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		ExprContext _localctx = new ExprContext(_ctx, _parentState);
		ExprContext _prevctx = _localctx;
		int _startState = 44;
		enterRecursionRule(_localctx, 44, RULE_expr, _p);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(264);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,24,_ctx) ) {
			case 1:
				{
				_localctx = new UnaryExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;

				setState(228);
				((UnaryExprContext)_localctx).op = _input.LT(1);
				_la = _input.LA(1);
				if ( !(_la==PLUS || _la==MINUS) ) {
					((UnaryExprContext)_localctx).op = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(229);
				expr(16);
				}
				break;
			case 2:
				{
				_localctx = new ParenExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(230);
				match(LPAREN);
				setState(231);
				expr(0);
				setState(232);
				match(RPAREN);
				}
				break;
			case 3:
				{
				_localctx = new TupleExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(234);
				match(LPAREN);
				setState(235);
				expr(0);
				setState(238); 
				_errHandler.sync(this);
				_la = _input.LA(1);
				do {
					{
					{
					setState(236);
					match(COMMA);
					setState(237);
					expr(0);
					}
					}
					setState(240); 
					_errHandler.sync(this);
					_la = _input.LA(1);
				} while ( _la==COMMA );
				setState(242);
				match(RPAREN);
				}
				break;
			case 4:
				{
				_localctx = new ArrayExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(244);
				match(LBRACK);
				setState(253);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 8730410088923136L) != 0)) {
					{
					setState(245);
					expr(0);
					setState(250);
					_errHandler.sync(this);
					_la = _input.LA(1);
					while (_la==COMMA) {
						{
						{
						setState(246);
						match(COMMA);
						setState(247);
						expr(0);
						}
						}
						setState(252);
						_errHandler.sync(this);
						_la = _input.LA(1);
					}
					}
				}

				setState(255);
				match(RBRACK);
				}
				break;
			case 5:
				{
				_localctx = new CallExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(256);
				match(ID);
				setState(257);
				match(LPAREN);
				setState(259);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 8730410088923136L) != 0)) {
					{
					setState(258);
					argList();
					}
				}

				setState(261);
				match(RPAREN);
				}
				break;
			case 6:
				{
				_localctx = new LiteralExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(262);
				literal();
				}
				break;
			case 7:
				{
				_localctx = new IdExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(263);
				match(ID);
				}
				break;
			}
			_ctx.stop = _input.LT(-1);
			setState(297);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,26,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(295);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,25,_ctx) ) {
					case 1:
						{
						_localctx = new MulExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(266);
						if (!(precpred(_ctx, 11))) throw new FailedPredicateException(this, "precpred(_ctx, 11)");
						setState(267);
						((MulExprContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 120259084288L) != 0)) ) {
							((MulExprContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(268);
						expr(12);
						}
						break;
					case 2:
						{
						_localctx = new AddExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(269);
						if (!(precpred(_ctx, 10))) throw new FailedPredicateException(this, "precpred(_ctx, 10)");
						setState(270);
						((AddExprContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==PLUS || _la==MINUS) ) {
							((AddExprContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(271);
						expr(11);
						}
						break;
					case 3:
						{
						_localctx = new CompareExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(272);
						if (!(precpred(_ctx, 9))) throw new FailedPredicateException(this, "precpred(_ctx, 9)");
						setState(273);
						((CompareExprContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 4026531840L) != 0)) ) {
							((CompareExprContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(274);
						expr(10);
						}
						break;
					case 4:
						{
						_localctx = new EqualityExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(275);
						if (!(precpred(_ctx, 8))) throw new FailedPredicateException(this, "precpred(_ctx, 8)");
						setState(276);
						((EqualityExprContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==EQ || _la==NEQ) ) {
							((EqualityExprContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(277);
						expr(9);
						}
						break;
					case 5:
						{
						_localctx = new AndExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(278);
						if (!(precpred(_ctx, 7))) throw new FailedPredicateException(this, "precpred(_ctx, 7)");
						setState(279);
						((AndExprContext)_localctx).op = match(AND);
						setState(280);
						expr(8);
						}
						break;
					case 6:
						{
						_localctx = new OrExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(281);
						if (!(precpred(_ctx, 6))) throw new FailedPredicateException(this, "precpred(_ctx, 6)");
						setState(282);
						((OrExprContext)_localctx).op = match(OR);
						setState(283);
						expr(7);
						}
						break;
					case 7:
						{
						_localctx = new RangeExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(284);
						if (!(precpred(_ctx, 5))) throw new FailedPredicateException(this, "precpred(_ctx, 5)");
						setState(285);
						match(RANGE);
						setState(286);
						expr(6);
						}
						break;
					case 8:
						{
						_localctx = new PipeExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(287);
						if (!(precpred(_ctx, 4))) throw new FailedPredicateException(this, "precpred(_ctx, 4)");
						setState(288);
						match(PIPE);
						setState(289);
						expr(5);
						}
						break;
					case 9:
						{
						_localctx = new IndexExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(290);
						if (!(precpred(_ctx, 12))) throw new FailedPredicateException(this, "precpred(_ctx, 12)");
						setState(291);
						match(LBRACK);
						setState(292);
						expr(0);
						setState(293);
						match(RBRACK);
						}
						break;
					}
					} 
				}
				setState(299);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,26,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ArgListContext extends ParserRuleContext {
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public List<TerminalNode> COMMA() { return getTokens(RexParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(RexParser.COMMA, i);
		}
		public ArgListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_argList; }
	}

	public final ArgListContext argList() throws RecognitionException {
		ArgListContext _localctx = new ArgListContext(_ctx, getState());
		enterRule(_localctx, 46, RULE_argList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(300);
			expr(0);
			setState(305);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(301);
				match(COMMA);
				setState(302);
				expr(0);
				}
				}
				setState(307);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class LiteralContext extends ParserRuleContext {
		public TerminalNode INT_LITERAL() { return getToken(RexParser.INT_LITERAL, 0); }
		public TerminalNode REAL_LITERAL() { return getToken(RexParser.REAL_LITERAL, 0); }
		public TerminalNode STRING_LITERAL() { return getToken(RexParser.STRING_LITERAL, 0); }
		public TerminalNode CHAR_LITERAL() { return getToken(RexParser.CHAR_LITERAL, 0); }
		public TerminalNode TRUE() { return getToken(RexParser.TRUE, 0); }
		public TerminalNode FALSE() { return getToken(RexParser.FALSE, 0); }
		public LiteralContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_literal; }
	}

	public final LiteralContext literal() throws RecognitionException {
		LiteralContext _localctx = new LiteralContext(_ctx, getState());
		enterRule(_localctx, 48, RULE_literal);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(308);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 8444249302892544L) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public boolean sempred(RuleContext _localctx, int ruleIndex, int predIndex) {
		switch (ruleIndex) {
		case 3:
			return type_sempred((TypeContext)_localctx, predIndex);
		case 22:
			return expr_sempred((ExprContext)_localctx, predIndex);
		}
		return true;
	}
	private boolean type_sempred(TypeContext _localctx, int predIndex) {
		switch (predIndex) {
		case 0:
			return precpred(_ctx, 3);
		case 1:
			return precpred(_ctx, 2);
		}
		return true;
	}
	private boolean expr_sempred(ExprContext _localctx, int predIndex) {
		switch (predIndex) {
		case 2:
			return precpred(_ctx, 11);
		case 3:
			return precpred(_ctx, 10);
		case 4:
			return precpred(_ctx, 9);
		case 5:
			return precpred(_ctx, 8);
		case 6:
			return precpred(_ctx, 7);
		case 7:
			return precpred(_ctx, 6);
		case 8:
			return precpred(_ctx, 5);
		case 9:
			return precpred(_ctx, 4);
		case 10:
			return precpred(_ctx, 12);
		}
		return true;
	}

	public static final String _serializedATN =
		"\u0004\u00017\u0137\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001\u0002"+
		"\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004\u0007\u0004\u0002"+
		"\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007\u0007\u0007\u0002"+
		"\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b\u0007\u000b\u0002"+
		"\f\u0007\f\u0002\r\u0007\r\u0002\u000e\u0007\u000e\u0002\u000f\u0007\u000f"+
		"\u0002\u0010\u0007\u0010\u0002\u0011\u0007\u0011\u0002\u0012\u0007\u0012"+
		"\u0002\u0013\u0007\u0013\u0002\u0014\u0007\u0014\u0002\u0015\u0007\u0015"+
		"\u0002\u0016\u0007\u0016\u0002\u0017\u0007\u0017\u0002\u0018\u0007\u0018"+
		"\u0001\u0000\u0005\u00004\b\u0000\n\u0000\f\u00007\t\u0000\u0001\u0000"+
		"\u0001\u0000\u0001\u0001\u0001\u0001\u0003\u0001=\b\u0001\u0001\u0002"+
		"\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0003"+
		"\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003"+
		"\u0004\u0003L\b\u0003\u000b\u0003\f\u0003M\u0001\u0003\u0001\u0003\u0003"+
		"\u0003R\b\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001"+
		"\u0003\u0001\u0003\u0001\u0003\u0005\u0003[\b\u0003\n\u0003\f\u0003^\t"+
		"\u0003\u0001\u0004\u0001\u0004\u0001\u0005\u0001\u0005\u0001\u0005\u0001"+
		"\u0005\u0003\u0005f\b\u0005\u0001\u0005\u0001\u0005\u0003\u0005j\b\u0005"+
		"\u0001\u0005\u0001\u0005\u0001\u0006\u0001\u0006\u0001\u0006\u0005\u0006"+
		"q\b\u0006\n\u0006\f\u0006t\t\u0006\u0001\u0007\u0003\u0007w\b\u0007\u0001"+
		"\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\b\u0001\b\u0001\b\u0001"+
		"\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001"+
		"\t\u0003\t\u008a\b\t\u0001\n\u0001\n\u0001\n\u0001\n\u0003\n\u0090\b\n"+
		"\u0001\n\u0001\n\u0001\n\u0001\n\u0001\u000b\u0001\u000b\u0001\u000b\u0001"+
		"\u000b\u0001\u000b\u0001\u000b\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f"+
		"\u0001\r\u0001\r\u0003\r\u00a3\b\r\u0001\r\u0001\r\u0001\u000e\u0001\u000e"+
		"\u0001\u000e\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0003\u000f"+
		"\u00ae\b\u000f\u0001\u000f\u0003\u000f\u00b1\b\u000f\u0001\u0010\u0001"+
		"\u0010\u0001\u0010\u0001\u0010\u0004\u0010\u00b7\b\u0010\u000b\u0010\f"+
		"\u0010\u00b8\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0012\u0001\u0012"+
		"\u0001\u0012\u0001\u0012\u0001\u0012\u0004\u0012\u00c3\b\u0012\u000b\u0012"+
		"\f\u0012\u00c4\u0001\u0012\u0003\u0012\u00c8\b\u0012\u0001\u0013\u0001"+
		"\u0013\u0001\u0013\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001"+
		"\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001"+
		"\u0014\u0001\u0014\u0003\u0014\u00d9\b\u0014\u0001\u0015\u0001\u0015\u0005"+
		"\u0015\u00dd\b\u0015\n\u0015\f\u0015\u00e0\t\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016"+
		"\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0004\u0016"+
		"\u00ef\b\u0016\u000b\u0016\f\u0016\u00f0\u0001\u0016\u0001\u0016\u0001"+
		"\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0005\u0016\u00f9\b\u0016\n"+
		"\u0016\f\u0016\u00fc\t\u0016\u0003\u0016\u00fe\b\u0016\u0001\u0016\u0001"+
		"\u0016\u0001\u0016\u0001\u0016\u0003\u0016\u0104\b\u0016\u0001\u0016\u0001"+
		"\u0016\u0001\u0016\u0003\u0016\u0109\b\u0016\u0001\u0016\u0001\u0016\u0001"+
		"\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001"+
		"\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001"+
		"\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001"+
		"\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001"+
		"\u0016\u0001\u0016\u0001\u0016\u0005\u0016\u0128\b\u0016\n\u0016\f\u0016"+
		"\u012b\t\u0016\u0001\u0017\u0001\u0017\u0001\u0017\u0005\u0017\u0130\b"+
		"\u0017\n\u0017\f\u0017\u0133\t\u0017\u0001\u0018\u0001\u0018\u0001\u0018"+
		"\u0000\u0002\u0006,\u0019\u0000\u0002\u0004\u0006\b\n\f\u000e\u0010\u0012"+
		"\u0014\u0016\u0018\u001a\u001c\u001e \"$&(*,.0\u0000\u0006\u0001\u0000"+
		"\u000e\u0012\u0001\u0000 !\u0001\u0000\"$\u0001\u0000\u001c\u001f\u0001"+
		"\u0000\u001a\u001b\u0002\u0000\u0013\u001414\u014f\u00005\u0001\u0000"+
		"\u0000\u0000\u0002<\u0001\u0000\u0000\u0000\u0004>\u0001\u0000\u0000\u0000"+
		"\u0006Q\u0001\u0000\u0000\u0000\b_\u0001\u0000\u0000\u0000\na\u0001\u0000"+
		"\u0000\u0000\fm\u0001\u0000\u0000\u0000\u000ev\u0001\u0000\u0000\u0000"+
		"\u0010|\u0001\u0000\u0000\u0000\u0012\u0089\u0001\u0000\u0000\u0000\u0014"+
		"\u008b\u0001\u0000\u0000\u0000\u0016\u0095\u0001\u0000\u0000\u0000\u0018"+
		"\u009b\u0001\u0000\u0000\u0000\u001a\u00a0\u0001\u0000\u0000\u0000\u001c"+
		"\u00a6\u0001\u0000\u0000\u0000\u001e\u00a9\u0001\u0000\u0000\u0000 \u00b6"+
		"\u0001\u0000\u0000\u0000\"\u00ba\u0001\u0000\u0000\u0000$\u00c7\u0001"+
		"\u0000\u0000\u0000&\u00c9\u0001\u0000\u0000\u0000(\u00d8\u0001\u0000\u0000"+
		"\u0000*\u00da\u0001\u0000\u0000\u0000,\u0108\u0001\u0000\u0000\u0000."+
		"\u012c\u0001\u0000\u0000\u00000\u0134\u0001\u0000\u0000\u000024\u0003"+
		"\u0002\u0001\u000032\u0001\u0000\u0000\u000047\u0001\u0000\u0000\u0000"+
		"53\u0001\u0000\u0000\u000056\u0001\u0000\u0000\u000068\u0001\u0000\u0000"+
		"\u000075\u0001\u0000\u0000\u000089\u0005\u0000\u0000\u00019\u0001\u0001"+
		"\u0000\u0000\u0000:=\u0003\n\u0005\u0000;=\u0003\u0012\t\u0000<:\u0001"+
		"\u0000\u0000\u0000<;\u0001\u0000\u0000\u0000=\u0003\u0001\u0000\u0000"+
		"\u0000>?\u0005\f\u0000\u0000?@\u00050\u0000\u0000@A\u0005\u0019\u0000"+
		"\u0000AB\u0003\u0006\u0003\u0000BC\u0005-\u0000\u0000C\u0005\u0001\u0000"+
		"\u0000\u0000DE\u0006\u0003\uffff\uffff\u0000ER\u0003\b\u0004\u0000FR\u0005"+
		"0\u0000\u0000GH\u0005&\u0000\u0000HK\u0003\u0006\u0003\u0000IJ\u0005,"+
		"\u0000\u0000JL\u0003\u0006\u0003\u0000KI\u0001\u0000\u0000\u0000LM\u0001"+
		"\u0000\u0000\u0000MK\u0001\u0000\u0000\u0000MN\u0001\u0000\u0000\u0000"+
		"NO\u0001\u0000\u0000\u0000OP\u0005\'\u0000\u0000PR\u0001\u0000\u0000\u0000"+
		"QD\u0001\u0000\u0000\u0000QF\u0001\u0000\u0000\u0000QG\u0001\u0000\u0000"+
		"\u0000R\\\u0001\u0000\u0000\u0000ST\n\u0003\u0000\u0000TU\u0005*\u0000"+
		"\u0000U[\u0005+\u0000\u0000VW\n\u0002\u0000\u0000WX\u0005*\u0000\u0000"+
		"XY\u00051\u0000\u0000Y[\u0005+\u0000\u0000ZS\u0001\u0000\u0000\u0000Z"+
		"V\u0001\u0000\u0000\u0000[^\u0001\u0000\u0000\u0000\\Z\u0001\u0000\u0000"+
		"\u0000\\]\u0001\u0000\u0000\u0000]\u0007\u0001\u0000\u0000\u0000^\\\u0001"+
		"\u0000\u0000\u0000_`\u0007\u0000\u0000\u0000`\t\u0001\u0000\u0000\u0000"+
		"ab\u0005\u0004\u0000\u0000bc\u00050\u0000\u0000ce\u0005&\u0000\u0000d"+
		"f\u0003\f\u0006\u0000ed\u0001\u0000\u0000\u0000ef\u0001\u0000\u0000\u0000"+
		"fg\u0001\u0000\u0000\u0000gi\u0005\'\u0000\u0000hj\u0003\u0010\b\u0000"+
		"ih\u0001\u0000\u0000\u0000ij\u0001\u0000\u0000\u0000jk\u0001\u0000\u0000"+
		"\u0000kl\u0003*\u0015\u0000l\u000b\u0001\u0000\u0000\u0000mr\u0003\u000e"+
		"\u0007\u0000no\u0005,\u0000\u0000oq\u0003\u000e\u0007\u0000pn\u0001\u0000"+
		"\u0000\u0000qt\u0001\u0000\u0000\u0000rp\u0001\u0000\u0000\u0000rs\u0001"+
		"\u0000\u0000\u0000s\r\u0001\u0000\u0000\u0000tr\u0001\u0000\u0000\u0000"+
		"uw\u0005\u0006\u0000\u0000vu\u0001\u0000\u0000\u0000vw\u0001\u0000\u0000"+
		"\u0000wx\u0001\u0000\u0000\u0000xy\u00050\u0000\u0000yz\u0005.\u0000\u0000"+
		"z{\u0003\u0006\u0003\u0000{\u000f\u0001\u0000\u0000\u0000|}\u0005\u0017"+
		"\u0000\u0000}~\u0003\u0006\u0003\u0000~\u0011\u0001\u0000\u0000\u0000"+
		"\u007f\u008a\u0003\u0014\n\u0000\u0080\u008a\u0003\u0018\f\u0000\u0081"+
		"\u008a\u0003\u001e\u000f\u0000\u0082\u008a\u0003(\u0014\u0000\u0083\u008a"+
		"\u0003\u001c\u000e\u0000\u0084\u008a\u0003\u0004\u0002\u0000\u0085\u008a"+
		"\u0003\u0016\u000b\u0000\u0086\u008a\u0003\u001a\r\u0000\u0087\u008a\u0003"+
		"\u001c\u000e\u0000\u0088\u008a\u0003&\u0013\u0000\u0089\u007f\u0001\u0000"+
		"\u0000\u0000\u0089\u0080\u0001\u0000\u0000\u0000\u0089\u0081\u0001\u0000"+
		"\u0000\u0000\u0089\u0082\u0001\u0000\u0000\u0000\u0089\u0083\u0001\u0000"+
		"\u0000\u0000\u0089\u0084\u0001\u0000\u0000\u0000\u0089\u0085\u0001\u0000"+
		"\u0000\u0000\u0089\u0086\u0001\u0000\u0000\u0000\u0089\u0087\u0001\u0000"+
		"\u0000\u0000\u0089\u0088\u0001\u0000\u0000\u0000\u008a\u0013\u0001\u0000"+
		"\u0000\u0000\u008b\u008c\u0005\u0005\u0000\u0000\u008c\u008f\u0003$\u0012"+
		"\u0000\u008d\u008e\u0005.\u0000\u0000\u008e\u0090\u0003\u0006\u0003\u0000"+
		"\u008f\u008d\u0001\u0000\u0000\u0000\u008f\u0090\u0001\u0000\u0000\u0000"+
		"\u0090\u0091\u0001\u0000\u0000\u0000\u0091\u0092\u0005\u0019\u0000\u0000"+
		"\u0092\u0093\u0003,\u0016\u0000\u0093\u0094\u0005-\u0000\u0000\u0094\u0015"+
		"\u0001\u0000\u0000\u0000\u0095\u0096\u0005\r\u0000\u0000\u0096\u0097\u0005"+
		"&\u0000\u0000\u0097\u0098\u0003,\u0016\u0000\u0098\u0099\u0005\'\u0000"+
		"\u0000\u0099\u009a\u0005-\u0000\u0000\u009a\u0017\u0001\u0000\u0000\u0000"+
		"\u009b\u009c\u0003,\u0016\u0000\u009c\u009d\u0005\u0019\u0000\u0000\u009d"+
		"\u009e\u0003,\u0016\u0000\u009e\u009f\u0005-\u0000\u0000\u009f\u0019\u0001"+
		"\u0000\u0000\u0000\u00a0\u00a2\u0005\u000b\u0000\u0000\u00a1\u00a3\u0003"+
		",\u0016\u0000\u00a2\u00a1\u0001\u0000\u0000\u0000\u00a2\u00a3\u0001\u0000"+
		"\u0000\u0000\u00a3\u00a4\u0001\u0000\u0000\u0000\u00a4\u00a5\u0005-\u0000"+
		"\u0000\u00a5\u001b\u0001\u0000\u0000\u0000\u00a6\u00a7\u0003,\u0016\u0000"+
		"\u00a7\u00a8\u0005-\u0000\u0000\u00a8\u001d\u0001\u0000\u0000\u0000\u00a9"+
		"\u00aa\u0005\u0001\u0000\u0000\u00aa\u00ab\u0003,\u0016\u0000\u00ab\u00ad"+
		"\u0003*\u0015\u0000\u00ac\u00ae\u0003 \u0010\u0000\u00ad\u00ac\u0001\u0000"+
		"\u0000\u0000\u00ad\u00ae\u0001\u0000\u0000\u0000\u00ae\u00b0\u0001\u0000"+
		"\u0000\u0000\u00af\u00b1\u0003\"\u0011\u0000\u00b0\u00af\u0001\u0000\u0000"+
		"\u0000\u00b0\u00b1\u0001\u0000\u0000\u0000\u00b1\u001f\u0001\u0000\u0000"+
		"\u0000\u00b2\u00b3\u0005\u0002\u0000\u0000\u00b3\u00b4\u0003,\u0016\u0000"+
		"\u00b4\u00b5\u0003*\u0015\u0000\u00b5\u00b7\u0001\u0000\u0000\u0000\u00b6"+
		"\u00b2\u0001\u0000\u0000\u0000\u00b7\u00b8\u0001\u0000\u0000\u0000\u00b8"+
		"\u00b6\u0001\u0000\u0000\u0000\u00b8\u00b9\u0001\u0000\u0000\u0000\u00b9"+
		"!\u0001\u0000\u0000\u0000\u00ba\u00bb\u0005\u0003\u0000\u0000\u00bb\u00bc"+
		"\u0003*\u0015\u0000\u00bc#\u0001\u0000\u0000\u0000\u00bd\u00c8\u00050"+
		"\u0000\u0000\u00be\u00bf\u0005&\u0000\u0000\u00bf\u00c2\u00050\u0000\u0000"+
		"\u00c0\u00c1\u0005,\u0000\u0000\u00c1\u00c3\u00050\u0000\u0000\u00c2\u00c0"+
		"\u0001\u0000\u0000\u0000\u00c3\u00c4\u0001\u0000\u0000\u0000\u00c4\u00c2"+
		"\u0001\u0000\u0000\u0000\u00c4\u00c5\u0001\u0000\u0000\u0000\u00c5\u00c6"+
		"\u0001\u0000\u0000\u0000\u00c6\u00c8\u0005\'\u0000\u0000\u00c7\u00bd\u0001"+
		"\u0000\u0000\u0000\u00c7\u00be\u0001\u0000\u0000\u0000\u00c8%\u0001\u0000"+
		"\u0000\u0000\u00c9\u00ca\u0005/\u0000\u0000\u00ca\u00cb\u0005-\u0000\u0000"+
		"\u00cb\'\u0001\u0000\u0000\u0000\u00cc\u00cd\u0005\b\u0000\u0000\u00cd"+
		"\u00ce\u0003,\u0016\u0000\u00ce\u00cf\u0003*\u0015\u0000\u00cf\u00d9\u0001"+
		"\u0000\u0000\u0000\u00d0\u00d1\u0005\u0007\u0000\u0000\u00d1\u00d2\u0005"+
		"0\u0000\u0000\u00d2\u00d3\u0005\n\u0000\u0000\u00d3\u00d4\u0003,\u0016"+
		"\u0000\u00d4\u00d5\u0003*\u0015\u0000\u00d5\u00d9\u0001\u0000\u0000\u0000"+
		"\u00d6\u00d7\u0005\t\u0000\u0000\u00d7\u00d9\u0003*\u0015\u0000\u00d8"+
		"\u00cc\u0001\u0000\u0000\u0000\u00d8\u00d0\u0001\u0000\u0000\u0000\u00d8"+
		"\u00d6\u0001\u0000\u0000\u0000\u00d9)\u0001\u0000\u0000\u0000\u00da\u00de"+
		"\u0005(\u0000\u0000\u00db\u00dd\u0003\u0012\t\u0000\u00dc\u00db\u0001"+
		"\u0000\u0000\u0000\u00dd\u00e0\u0001\u0000\u0000\u0000\u00de\u00dc\u0001"+
		"\u0000\u0000\u0000\u00de\u00df\u0001\u0000\u0000\u0000\u00df\u00e1\u0001"+
		"\u0000\u0000\u0000\u00e0\u00de\u0001\u0000\u0000\u0000\u00e1\u00e2\u0005"+
		")\u0000\u0000\u00e2+\u0001\u0000\u0000\u0000\u00e3\u00e4\u0006\u0016\uffff"+
		"\uffff\u0000\u00e4\u00e5\u0007\u0001\u0000\u0000\u00e5\u0109\u0003,\u0016"+
		"\u0010\u00e6\u00e7\u0005&\u0000\u0000\u00e7\u00e8\u0003,\u0016\u0000\u00e8"+
		"\u00e9\u0005\'\u0000\u0000\u00e9\u0109\u0001\u0000\u0000\u0000\u00ea\u00eb"+
		"\u0005&\u0000\u0000\u00eb\u00ee\u0003,\u0016\u0000\u00ec\u00ed\u0005,"+
		"\u0000\u0000\u00ed\u00ef\u0003,\u0016\u0000\u00ee\u00ec\u0001\u0000\u0000"+
		"\u0000\u00ef\u00f0\u0001\u0000\u0000\u0000\u00f0\u00ee\u0001\u0000\u0000"+
		"\u0000\u00f0\u00f1\u0001\u0000\u0000\u0000\u00f1\u00f2\u0001\u0000\u0000"+
		"\u0000\u00f2\u00f3\u0005\'\u0000\u0000\u00f3\u0109\u0001\u0000\u0000\u0000"+
		"\u00f4\u00fd\u0005*\u0000\u0000\u00f5\u00fa\u0003,\u0016\u0000\u00f6\u00f7"+
		"\u0005,\u0000\u0000\u00f7\u00f9\u0003,\u0016\u0000\u00f8\u00f6\u0001\u0000"+
		"\u0000\u0000\u00f9\u00fc\u0001\u0000\u0000\u0000\u00fa\u00f8\u0001\u0000"+
		"\u0000\u0000\u00fa\u00fb\u0001\u0000\u0000\u0000\u00fb\u00fe\u0001\u0000"+
		"\u0000\u0000\u00fc\u00fa\u0001\u0000\u0000\u0000\u00fd\u00f5\u0001\u0000"+
		"\u0000\u0000\u00fd\u00fe\u0001\u0000\u0000\u0000\u00fe\u00ff\u0001\u0000"+
		"\u0000\u0000\u00ff\u0109\u0005+\u0000\u0000\u0100\u0101\u00050\u0000\u0000"+
		"\u0101\u0103\u0005&\u0000\u0000\u0102\u0104\u0003.\u0017\u0000\u0103\u0102"+
		"\u0001\u0000\u0000\u0000\u0103\u0104\u0001\u0000\u0000\u0000\u0104\u0105"+
		"\u0001\u0000\u0000\u0000\u0105\u0109\u0005\'\u0000\u0000\u0106\u0109\u0003"+
		"0\u0018\u0000\u0107\u0109\u00050\u0000\u0000\u0108\u00e3\u0001\u0000\u0000"+
		"\u0000\u0108\u00e6\u0001\u0000\u0000\u0000\u0108\u00ea\u0001\u0000\u0000"+
		"\u0000\u0108\u00f4\u0001\u0000\u0000\u0000\u0108\u0100\u0001\u0000\u0000"+
		"\u0000\u0108\u0106\u0001\u0000\u0000\u0000\u0108\u0107\u0001\u0000\u0000"+
		"\u0000\u0109\u0129\u0001\u0000\u0000\u0000\u010a\u010b\n\u000b\u0000\u0000"+
		"\u010b\u010c\u0007\u0002\u0000\u0000\u010c\u0128\u0003,\u0016\f\u010d"+
		"\u010e\n\n\u0000\u0000\u010e\u010f\u0007\u0001\u0000\u0000\u010f\u0128"+
		"\u0003,\u0016\u000b\u0110\u0111\n\t\u0000\u0000\u0111\u0112\u0007\u0003"+
		"\u0000\u0000\u0112\u0128\u0003,\u0016\n\u0113\u0114\n\b\u0000\u0000\u0114"+
		"\u0115\u0007\u0004\u0000\u0000\u0115\u0128\u0003,\u0016\t\u0116\u0117"+
		"\n\u0007\u0000\u0000\u0117\u0118\u0005\u0015\u0000\u0000\u0118\u0128\u0003"+
		",\u0016\b\u0119\u011a\n\u0006\u0000\u0000\u011a\u011b\u0005\u0016\u0000"+
		"\u0000\u011b\u0128\u0003,\u0016\u0007\u011c\u011d\n\u0005\u0000\u0000"+
		"\u011d\u011e\u0005%\u0000\u0000\u011e\u0128\u0003,\u0016\u0006\u011f\u0120"+
		"\n\u0004\u0000\u0000\u0120\u0121\u0005\u0018\u0000\u0000\u0121\u0128\u0003"+
		",\u0016\u0005\u0122\u0123\n\f\u0000\u0000\u0123\u0124\u0005*\u0000\u0000"+
		"\u0124\u0125\u0003,\u0016\u0000\u0125\u0126\u0005+\u0000\u0000\u0126\u0128"+
		"\u0001\u0000\u0000\u0000\u0127\u010a\u0001\u0000\u0000\u0000\u0127\u010d"+
		"\u0001\u0000\u0000\u0000\u0127\u0110\u0001\u0000\u0000\u0000\u0127\u0113"+
		"\u0001\u0000\u0000\u0000\u0127\u0116\u0001\u0000\u0000\u0000\u0127\u0119"+
		"\u0001\u0000\u0000\u0000\u0127\u011c\u0001\u0000\u0000\u0000\u0127\u011f"+
		"\u0001\u0000\u0000\u0000\u0127\u0122\u0001\u0000\u0000\u0000\u0128\u012b"+
		"\u0001\u0000\u0000\u0000\u0129\u0127\u0001\u0000\u0000\u0000\u0129\u012a"+
		"\u0001\u0000\u0000\u0000\u012a-\u0001\u0000\u0000\u0000\u012b\u0129\u0001"+
		"\u0000\u0000\u0000\u012c\u0131\u0003,\u0016\u0000\u012d\u012e\u0005,\u0000"+
		"\u0000\u012e\u0130\u0003,\u0016\u0000\u012f\u012d\u0001\u0000\u0000\u0000"+
		"\u0130\u0133\u0001\u0000\u0000\u0000\u0131\u012f\u0001\u0000\u0000\u0000"+
		"\u0131\u0132\u0001\u0000\u0000\u0000\u0132/\u0001\u0000\u0000\u0000\u0133"+
		"\u0131\u0001\u0000\u0000\u0000\u0134\u0135\u0007\u0005\u0000\u0000\u0135"+
		"1\u0001\u0000\u0000\u0000\u001c5<MQZ\\eirv\u0089\u008f\u00a2\u00ad\u00b0"+
		"\u00b8\u00c4\u00c7\u00d8\u00de\u00f0\u00fa\u00fd\u0103\u0108\u0127\u0129"+
		"\u0131";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}