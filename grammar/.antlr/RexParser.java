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
		RETURN=11, TYPE=12, INT=13, BOOL=14, CHAR=15, REAL=16, STRING=17, TRUE=18, 
		FALSE=19, AND=20, OR=21, ARROW=22, PIPE=23, ASSIGN=24, EQ=25, NEQ=26, 
		LT=27, GT=28, LTE=29, GTE=30, PLUS=31, MINUS=32, STAR=33, DIV=34, MOD=35, 
		RANGE=36, LPAREN=37, RPAREN=38, LBRACE=39, RBRACE=40, LBRACK=41, RBRACK=42, 
		COMMA=43, SEMI=44, COLON=45, BREAK=46, ID=47, INT_LITERAL=48, REAL_LITERAL=49, 
		STRING_LITERAL=50, CHAR_LITERAL=51, WS=52, LINE_COMMENT=53, BLOCK_COMMENT=54;
	public static final int
		RULE_file = 0, RULE_item = 1, RULE_typeDef = 2, RULE_type = 3, RULE_primitiveType = 4, 
		RULE_functionDef = 5, RULE_paramList = 6, RULE_param = 7, RULE_returnType = 8, 
		RULE_statement = 9, RULE_letStmt = 10, RULE_assignStmt = 11, RULE_returnStmt = 12, 
		RULE_exprStmt = 13, RULE_ifStmt = 14, RULE_elifxChain = 15, RULE_elseBlock = 16, 
		RULE_pattern = 17, RULE_breakStmt = 18, RULE_loopStmt = 19, RULE_block = 20, 
		RULE_expr = 21, RULE_argList = 22, RULE_literal = 23;
	private static String[] makeRuleNames() {
		return new String[] {
			"file", "item", "typeDef", "type", "primitiveType", "functionDef", "paramList", 
			"param", "returnType", "statement", "letStmt", "assignStmt", "returnStmt", 
			"exprStmt", "ifStmt", "elifxChain", "elseBlock", "pattern", "breakStmt", 
			"loopStmt", "block", "expr", "argList", "literal"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'if'", "'elifx'", "'else'", "'function'", "'let'", "'mut'", "'for'", 
			"'while'", "'loop'", "'in'", "'return'", "'type'", "'Int'", "'Bool'", 
			"'Char'", "'Real'", "'String'", "'true'", "'false'", "'and'", "'or'", 
			"'->'", "'|>'", "'='", "'=='", "'!='", "'<'", "'>'", "'<='", "'>='", 
			"'+'", "'-'", "'*'", "'/'", "'%'", "'..'", "'('", "')'", "'{'", "'}'", 
			"'['", "']'", "','", "';'", "':'", "'break'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, "IF", "ELIFX", "ELSE", "FN", "LET", "MUT", "FOR", "WHILE", "LOOP", 
			"IN", "RETURN", "TYPE", "INT", "BOOL", "CHAR", "REAL", "STRING", "TRUE", 
			"FALSE", "AND", "OR", "ARROW", "PIPE", "ASSIGN", "EQ", "NEQ", "LT", "GT", 
			"LTE", "GTE", "PLUS", "MINUS", "STAR", "DIV", "MOD", "RANGE", "LPAREN", 
			"RPAREN", "LBRACE", "RBRACE", "LBRACK", "RBRACK", "COMMA", "SEMI", "COLON", 
			"BREAK", "ID", "INT_LITERAL", "REAL_LITERAL", "STRING_LITERAL", "CHAR_LITERAL", 
			"WS", "LINE_COMMENT", "BLOCK_COMMENT"
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
			setState(51);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 4435573788646322L) != 0)) {
				{
				{
				setState(48);
				item();
				}
				}
				setState(53);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(54);
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
			setState(58);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case FN:
				enterOuterAlt(_localctx, 1);
				{
				setState(56);
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
				setState(57);
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
			setState(60);
			match(TYPE);
			setState(61);
			match(ID);
			setState(62);
			match(ASSIGN);
			setState(63);
			type(0);
			setState(64);
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
			setState(79);
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

				setState(67);
				primitiveType();
				}
				break;
			case ID:
				{
				_localctx = new NamedTypeContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(68);
				match(ID);
				}
				break;
			case LPAREN:
				{
				_localctx = new TupleTypeContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(69);
				match(LPAREN);
				setState(70);
				type(0);
				setState(73); 
				_errHandler.sync(this);
				_la = _input.LA(1);
				do {
					{
					{
					setState(71);
					match(COMMA);
					setState(72);
					type(0);
					}
					}
					setState(75); 
					_errHandler.sync(this);
					_la = _input.LA(1);
				} while ( _la==COMMA );
				setState(77);
				match(RPAREN);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			_ctx.stop = _input.LT(-1);
			setState(90);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,5,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(88);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,4,_ctx) ) {
					case 1:
						{
						_localctx = new SliceTypeContext(new TypeContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_type);
						setState(81);
						if (!(precpred(_ctx, 3))) throw new FailedPredicateException(this, "precpred(_ctx, 3)");
						setState(82);
						match(LBRACK);
						setState(83);
						match(RBRACK);
						}
						break;
					case 2:
						{
						_localctx = new ArrayTypeContext(new TypeContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_type);
						setState(84);
						if (!(precpred(_ctx, 2))) throw new FailedPredicateException(this, "precpred(_ctx, 2)");
						setState(85);
						match(LBRACK);
						setState(86);
						match(INT_LITERAL);
						setState(87);
						match(RBRACK);
						}
						break;
					}
					} 
				}
				setState(92);
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
			setState(93);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 253952L) != 0)) ) {
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
			setState(95);
			match(FN);
			setState(96);
			match(ID);
			setState(97);
			match(LPAREN);
			setState(99);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==MUT || _la==ID) {
				{
				setState(98);
				paramList();
				}
			}

			setState(101);
			match(RPAREN);
			setState(103);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==ARROW) {
				{
				setState(102);
				returnType();
				}
			}

			setState(105);
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
			setState(107);
			param();
			setState(112);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(108);
				match(COMMA);
				setState(109);
				param();
				}
				}
				setState(114);
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
			setState(116);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==MUT) {
				{
				setState(115);
				match(MUT);
				}
			}

			setState(118);
			match(ID);
			setState(119);
			match(COLON);
			setState(120);
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
			setState(122);
			match(ARROW);
			setState(123);
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
			setState(134);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,10,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(125);
				letStmt();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(126);
				assignStmt();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(127);
				ifStmt();
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(128);
				loopStmt();
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(129);
				exprStmt();
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(130);
				typeDef();
				}
				break;
			case 7:
				enterOuterAlt(_localctx, 7);
				{
				setState(131);
				returnStmt();
				}
				break;
			case 8:
				enterOuterAlt(_localctx, 8);
				{
				setState(132);
				exprStmt();
				}
				break;
			case 9:
				enterOuterAlt(_localctx, 9);
				{
				setState(133);
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
			setState(136);
			match(LET);
			setState(137);
			pattern();
			setState(140);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COLON) {
				{
				setState(138);
				match(COLON);
				setState(139);
				type(0);
				}
			}

			setState(142);
			match(ASSIGN);
			setState(143);
			expr(0);
			setState(144);
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
		enterRule(_localctx, 22, RULE_assignStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(146);
			expr(0);
			setState(147);
			match(ASSIGN);
			setState(148);
			expr(0);
			setState(149);
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
		enterRule(_localctx, 24, RULE_returnStmt);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(151);
			match(RETURN);
			setState(153);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 4365205044461568L) != 0)) {
				{
				setState(152);
				expr(0);
				}
			}

			setState(155);
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
		enterRule(_localctx, 26, RULE_exprStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
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
		enterRule(_localctx, 28, RULE_ifStmt);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(160);
			match(IF);
			setState(161);
			expr(0);
			setState(162);
			block();
			setState(164);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==ELIFX) {
				{
				setState(163);
				elifxChain();
				}
			}

			setState(167);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==ELSE) {
				{
				setState(166);
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
		enterRule(_localctx, 30, RULE_elifxChain);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(173); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(169);
				match(ELIFX);
				setState(170);
				expr(0);
				setState(171);
				block();
				}
				}
				setState(175); 
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
		enterRule(_localctx, 32, RULE_elseBlock);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(177);
			match(ELSE);
			setState(178);
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
		enterRule(_localctx, 34, RULE_pattern);
		int _la;
		try {
			setState(190);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case ID:
				enterOuterAlt(_localctx, 1);
				{
				setState(180);
				match(ID);
				}
				break;
			case LPAREN:
				enterOuterAlt(_localctx, 2);
				{
				setState(181);
				match(LPAREN);
				setState(182);
				match(ID);
				setState(185); 
				_errHandler.sync(this);
				_la = _input.LA(1);
				do {
					{
					{
					setState(183);
					match(COMMA);
					setState(184);
					match(ID);
					}
					}
					setState(187); 
					_errHandler.sync(this);
					_la = _input.LA(1);
				} while ( _la==COMMA );
				setState(189);
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
		enterRule(_localctx, 36, RULE_breakStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(192);
			match(BREAK);
			setState(193);
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
		enterRule(_localctx, 38, RULE_loopStmt);
		try {
			setState(207);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case WHILE:
				enterOuterAlt(_localctx, 1);
				{
				setState(195);
				match(WHILE);
				setState(196);
				expr(0);
				setState(197);
				block();
				}
				break;
			case FOR:
				enterOuterAlt(_localctx, 2);
				{
				setState(199);
				match(FOR);
				setState(200);
				match(ID);
				setState(201);
				match(IN);
				setState(202);
				expr(0);
				setState(203);
				block();
				}
				break;
			case LOOP:
				enterOuterAlt(_localctx, 3);
				{
				setState(205);
				match(LOOP);
				setState(206);
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
		enterRule(_localctx, 40, RULE_block);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(209);
			match(LBRACE);
			setState(213);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 4435573788646306L) != 0)) {
				{
				{
				setState(210);
				statement();
				}
				}
				setState(215);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(216);
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
		int _startState = 42;
		enterRecursionRule(_localctx, 42, RULE_expr, _p);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(255);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,24,_ctx) ) {
			case 1:
				{
				_localctx = new UnaryExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;

				setState(219);
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
				setState(220);
				expr(16);
				}
				break;
			case 2:
				{
				_localctx = new ParenExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(221);
				match(LPAREN);
				setState(222);
				expr(0);
				setState(223);
				match(RPAREN);
				}
				break;
			case 3:
				{
				_localctx = new TupleExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(225);
				match(LPAREN);
				setState(226);
				expr(0);
				setState(229); 
				_errHandler.sync(this);
				_la = _input.LA(1);
				do {
					{
					{
					setState(227);
					match(COMMA);
					setState(228);
					expr(0);
					}
					}
					setState(231); 
					_errHandler.sync(this);
					_la = _input.LA(1);
				} while ( _la==COMMA );
				setState(233);
				match(RPAREN);
				}
				break;
			case 4:
				{
				_localctx = new ArrayExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(235);
				match(LBRACK);
				setState(244);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 4365205044461568L) != 0)) {
					{
					setState(236);
					expr(0);
					setState(241);
					_errHandler.sync(this);
					_la = _input.LA(1);
					while (_la==COMMA) {
						{
						{
						setState(237);
						match(COMMA);
						setState(238);
						expr(0);
						}
						}
						setState(243);
						_errHandler.sync(this);
						_la = _input.LA(1);
					}
					}
				}

				setState(246);
				match(RBRACK);
				}
				break;
			case 5:
				{
				_localctx = new CallExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(247);
				match(ID);
				setState(248);
				match(LPAREN);
				setState(250);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 4365205044461568L) != 0)) {
					{
					setState(249);
					argList();
					}
				}

				setState(252);
				match(RPAREN);
				}
				break;
			case 6:
				{
				_localctx = new LiteralExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(253);
				literal();
				}
				break;
			case 7:
				{
				_localctx = new IdExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(254);
				match(ID);
				}
				break;
			}
			_ctx.stop = _input.LT(-1);
			setState(288);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,26,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(286);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,25,_ctx) ) {
					case 1:
						{
						_localctx = new MulExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(257);
						if (!(precpred(_ctx, 11))) throw new FailedPredicateException(this, "precpred(_ctx, 11)");
						setState(258);
						((MulExprContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 60129542144L) != 0)) ) {
							((MulExprContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(259);
						expr(12);
						}
						break;
					case 2:
						{
						_localctx = new AddExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(260);
						if (!(precpred(_ctx, 10))) throw new FailedPredicateException(this, "precpred(_ctx, 10)");
						setState(261);
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
						setState(262);
						expr(11);
						}
						break;
					case 3:
						{
						_localctx = new CompareExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(263);
						if (!(precpred(_ctx, 9))) throw new FailedPredicateException(this, "precpred(_ctx, 9)");
						setState(264);
						((CompareExprContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 2013265920L) != 0)) ) {
							((CompareExprContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(265);
						expr(10);
						}
						break;
					case 4:
						{
						_localctx = new EqualityExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(266);
						if (!(precpred(_ctx, 8))) throw new FailedPredicateException(this, "precpred(_ctx, 8)");
						setState(267);
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
						setState(268);
						expr(9);
						}
						break;
					case 5:
						{
						_localctx = new AndExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(269);
						if (!(precpred(_ctx, 7))) throw new FailedPredicateException(this, "precpred(_ctx, 7)");
						setState(270);
						((AndExprContext)_localctx).op = match(AND);
						setState(271);
						expr(8);
						}
						break;
					case 6:
						{
						_localctx = new OrExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(272);
						if (!(precpred(_ctx, 6))) throw new FailedPredicateException(this, "precpred(_ctx, 6)");
						setState(273);
						((OrExprContext)_localctx).op = match(OR);
						setState(274);
						expr(7);
						}
						break;
					case 7:
						{
						_localctx = new RangeExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(275);
						if (!(precpred(_ctx, 5))) throw new FailedPredicateException(this, "precpred(_ctx, 5)");
						setState(276);
						match(RANGE);
						setState(277);
						expr(6);
						}
						break;
					case 8:
						{
						_localctx = new PipeExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(278);
						if (!(precpred(_ctx, 4))) throw new FailedPredicateException(this, "precpred(_ctx, 4)");
						setState(279);
						match(PIPE);
						setState(280);
						expr(5);
						}
						break;
					case 9:
						{
						_localctx = new IndexExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(281);
						if (!(precpred(_ctx, 12))) throw new FailedPredicateException(this, "precpred(_ctx, 12)");
						setState(282);
						match(LBRACK);
						setState(283);
						expr(0);
						setState(284);
						match(RBRACK);
						}
						break;
					}
					} 
				}
				setState(290);
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
		enterRule(_localctx, 44, RULE_argList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(291);
			expr(0);
			setState(296);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(292);
				match(COMMA);
				setState(293);
				expr(0);
				}
				}
				setState(298);
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
		enterRule(_localctx, 46, RULE_literal);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(299);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 4222124651446272L) != 0)) ) {
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
		case 21:
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
		"\u0004\u00016\u012e\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001\u0002"+
		"\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004\u0007\u0004\u0002"+
		"\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007\u0007\u0007\u0002"+
		"\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b\u0007\u000b\u0002"+
		"\f\u0007\f\u0002\r\u0007\r\u0002\u000e\u0007\u000e\u0002\u000f\u0007\u000f"+
		"\u0002\u0010\u0007\u0010\u0002\u0011\u0007\u0011\u0002\u0012\u0007\u0012"+
		"\u0002\u0013\u0007\u0013\u0002\u0014\u0007\u0014\u0002\u0015\u0007\u0015"+
		"\u0002\u0016\u0007\u0016\u0002\u0017\u0007\u0017\u0001\u0000\u0005\u0000"+
		"2\b\u0000\n\u0000\f\u00005\t\u0000\u0001\u0000\u0001\u0000\u0001\u0001"+
		"\u0001\u0001\u0003\u0001;\b\u0001\u0001\u0002\u0001\u0002\u0001\u0002"+
		"\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0003\u0001\u0003\u0001\u0003"+
		"\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0004\u0003J\b\u0003"+
		"\u000b\u0003\f\u0003K\u0001\u0003\u0001\u0003\u0003\u0003P\b\u0003\u0001"+
		"\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001"+
		"\u0003\u0005\u0003Y\b\u0003\n\u0003\f\u0003\\\t\u0003\u0001\u0004\u0001"+
		"\u0004\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0003\u0005d\b"+
		"\u0005\u0001\u0005\u0001\u0005\u0003\u0005h\b\u0005\u0001\u0005\u0001"+
		"\u0005\u0001\u0006\u0001\u0006\u0001\u0006\u0005\u0006o\b\u0006\n\u0006"+
		"\f\u0006r\t\u0006\u0001\u0007\u0003\u0007u\b\u0007\u0001\u0007\u0001\u0007"+
		"\u0001\u0007\u0001\u0007\u0001\b\u0001\b\u0001\b\u0001\t\u0001\t\u0001"+
		"\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0003\t\u0087\b\t\u0001"+
		"\n\u0001\n\u0001\n\u0001\n\u0003\n\u008d\b\n\u0001\n\u0001\n\u0001\n\u0001"+
		"\n\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0001\f"+
		"\u0001\f\u0003\f\u009a\b\f\u0001\f\u0001\f\u0001\r\u0001\r\u0001\r\u0001"+
		"\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0003\u000e\u00a5\b\u000e\u0001"+
		"\u000e\u0003\u000e\u00a8\b\u000e\u0001\u000f\u0001\u000f\u0001\u000f\u0001"+
		"\u000f\u0004\u000f\u00ae\b\u000f\u000b\u000f\f\u000f\u00af\u0001\u0010"+
		"\u0001\u0010\u0001\u0010\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0011"+
		"\u0001\u0011\u0004\u0011\u00ba\b\u0011\u000b\u0011\f\u0011\u00bb\u0001"+
		"\u0011\u0003\u0011\u00bf\b\u0011\u0001\u0012\u0001\u0012\u0001\u0012\u0001"+
		"\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001"+
		"\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0003"+
		"\u0013\u00d0\b\u0013\u0001\u0014\u0001\u0014\u0005\u0014\u00d4\b\u0014"+
		"\n\u0014\f\u0014\u00d7\t\u0014\u0001\u0014\u0001\u0014\u0001\u0015\u0001"+
		"\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001"+
		"\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0004\u0015\u00e6\b\u0015\u000b"+
		"\u0015\f\u0015\u00e7\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001"+
		"\u0015\u0001\u0015\u0005\u0015\u00f0\b\u0015\n\u0015\f\u0015\u00f3\t\u0015"+
		"\u0003\u0015\u00f5\b\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0003\u0015\u00fb\b\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0003\u0015"+
		"\u0100\b\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0005\u0015\u011f\b\u0015\n\u0015\f\u0015\u0122\t\u0015\u0001\u0016\u0001"+
		"\u0016\u0001\u0016\u0005\u0016\u0127\b\u0016\n\u0016\f\u0016\u012a\t\u0016"+
		"\u0001\u0017\u0001\u0017\u0001\u0017\u0000\u0002\u0006*\u0018\u0000\u0002"+
		"\u0004\u0006\b\n\f\u000e\u0010\u0012\u0014\u0016\u0018\u001a\u001c\u001e"+
		" \"$&(*,.\u0000\u0006\u0001\u0000\r\u0011\u0001\u0000\u001f \u0001\u0000"+
		"!#\u0001\u0000\u001b\u001e\u0001\u0000\u0019\u001a\u0002\u0000\u0012\u0013"+
		"03\u0146\u00003\u0001\u0000\u0000\u0000\u0002:\u0001\u0000\u0000\u0000"+
		"\u0004<\u0001\u0000\u0000\u0000\u0006O\u0001\u0000\u0000\u0000\b]\u0001"+
		"\u0000\u0000\u0000\n_\u0001\u0000\u0000\u0000\fk\u0001\u0000\u0000\u0000"+
		"\u000et\u0001\u0000\u0000\u0000\u0010z\u0001\u0000\u0000\u0000\u0012\u0086"+
		"\u0001\u0000\u0000\u0000\u0014\u0088\u0001\u0000\u0000\u0000\u0016\u0092"+
		"\u0001\u0000\u0000\u0000\u0018\u0097\u0001\u0000\u0000\u0000\u001a\u009d"+
		"\u0001\u0000\u0000\u0000\u001c\u00a0\u0001\u0000\u0000\u0000\u001e\u00ad"+
		"\u0001\u0000\u0000\u0000 \u00b1\u0001\u0000\u0000\u0000\"\u00be\u0001"+
		"\u0000\u0000\u0000$\u00c0\u0001\u0000\u0000\u0000&\u00cf\u0001\u0000\u0000"+
		"\u0000(\u00d1\u0001\u0000\u0000\u0000*\u00ff\u0001\u0000\u0000\u0000,"+
		"\u0123\u0001\u0000\u0000\u0000.\u012b\u0001\u0000\u0000\u000002\u0003"+
		"\u0002\u0001\u000010\u0001\u0000\u0000\u000025\u0001\u0000\u0000\u0000"+
		"31\u0001\u0000\u0000\u000034\u0001\u0000\u0000\u000046\u0001\u0000\u0000"+
		"\u000053\u0001\u0000\u0000\u000067\u0005\u0000\u0000\u00017\u0001\u0001"+
		"\u0000\u0000\u00008;\u0003\n\u0005\u00009;\u0003\u0012\t\u0000:8\u0001"+
		"\u0000\u0000\u0000:9\u0001\u0000\u0000\u0000;\u0003\u0001\u0000\u0000"+
		"\u0000<=\u0005\f\u0000\u0000=>\u0005/\u0000\u0000>?\u0005\u0018\u0000"+
		"\u0000?@\u0003\u0006\u0003\u0000@A\u0005,\u0000\u0000A\u0005\u0001\u0000"+
		"\u0000\u0000BC\u0006\u0003\uffff\uffff\u0000CP\u0003\b\u0004\u0000DP\u0005"+
		"/\u0000\u0000EF\u0005%\u0000\u0000FI\u0003\u0006\u0003\u0000GH\u0005+"+
		"\u0000\u0000HJ\u0003\u0006\u0003\u0000IG\u0001\u0000\u0000\u0000JK\u0001"+
		"\u0000\u0000\u0000KI\u0001\u0000\u0000\u0000KL\u0001\u0000\u0000\u0000"+
		"LM\u0001\u0000\u0000\u0000MN\u0005&\u0000\u0000NP\u0001\u0000\u0000\u0000"+
		"OB\u0001\u0000\u0000\u0000OD\u0001\u0000\u0000\u0000OE\u0001\u0000\u0000"+
		"\u0000PZ\u0001\u0000\u0000\u0000QR\n\u0003\u0000\u0000RS\u0005)\u0000"+
		"\u0000SY\u0005*\u0000\u0000TU\n\u0002\u0000\u0000UV\u0005)\u0000\u0000"+
		"VW\u00050\u0000\u0000WY\u0005*\u0000\u0000XQ\u0001\u0000\u0000\u0000X"+
		"T\u0001\u0000\u0000\u0000Y\\\u0001\u0000\u0000\u0000ZX\u0001\u0000\u0000"+
		"\u0000Z[\u0001\u0000\u0000\u0000[\u0007\u0001\u0000\u0000\u0000\\Z\u0001"+
		"\u0000\u0000\u0000]^\u0007\u0000\u0000\u0000^\t\u0001\u0000\u0000\u0000"+
		"_`\u0005\u0004\u0000\u0000`a\u0005/\u0000\u0000ac\u0005%\u0000\u0000b"+
		"d\u0003\f\u0006\u0000cb\u0001\u0000\u0000\u0000cd\u0001\u0000\u0000\u0000"+
		"de\u0001\u0000\u0000\u0000eg\u0005&\u0000\u0000fh\u0003\u0010\b\u0000"+
		"gf\u0001\u0000\u0000\u0000gh\u0001\u0000\u0000\u0000hi\u0001\u0000\u0000"+
		"\u0000ij\u0003(\u0014\u0000j\u000b\u0001\u0000\u0000\u0000kp\u0003\u000e"+
		"\u0007\u0000lm\u0005+\u0000\u0000mo\u0003\u000e\u0007\u0000nl\u0001\u0000"+
		"\u0000\u0000or\u0001\u0000\u0000\u0000pn\u0001\u0000\u0000\u0000pq\u0001"+
		"\u0000\u0000\u0000q\r\u0001\u0000\u0000\u0000rp\u0001\u0000\u0000\u0000"+
		"su\u0005\u0006\u0000\u0000ts\u0001\u0000\u0000\u0000tu\u0001\u0000\u0000"+
		"\u0000uv\u0001\u0000\u0000\u0000vw\u0005/\u0000\u0000wx\u0005-\u0000\u0000"+
		"xy\u0003\u0006\u0003\u0000y\u000f\u0001\u0000\u0000\u0000z{\u0005\u0016"+
		"\u0000\u0000{|\u0003\u0006\u0003\u0000|\u0011\u0001\u0000\u0000\u0000"+
		"}\u0087\u0003\u0014\n\u0000~\u0087\u0003\u0016\u000b\u0000\u007f\u0087"+
		"\u0003\u001c\u000e\u0000\u0080\u0087\u0003&\u0013\u0000\u0081\u0087\u0003"+
		"\u001a\r\u0000\u0082\u0087\u0003\u0004\u0002\u0000\u0083\u0087\u0003\u0018"+
		"\f\u0000\u0084\u0087\u0003\u001a\r\u0000\u0085\u0087\u0003$\u0012\u0000"+
		"\u0086}\u0001\u0000\u0000\u0000\u0086~\u0001\u0000\u0000\u0000\u0086\u007f"+
		"\u0001\u0000\u0000\u0000\u0086\u0080\u0001\u0000\u0000\u0000\u0086\u0081"+
		"\u0001\u0000\u0000\u0000\u0086\u0082\u0001\u0000\u0000\u0000\u0086\u0083"+
		"\u0001\u0000\u0000\u0000\u0086\u0084\u0001\u0000\u0000\u0000\u0086\u0085"+
		"\u0001\u0000\u0000\u0000\u0087\u0013\u0001\u0000\u0000\u0000\u0088\u0089"+
		"\u0005\u0005\u0000\u0000\u0089\u008c\u0003\"\u0011\u0000\u008a\u008b\u0005"+
		"-\u0000\u0000\u008b\u008d\u0003\u0006\u0003\u0000\u008c\u008a\u0001\u0000"+
		"\u0000\u0000\u008c\u008d\u0001\u0000\u0000\u0000\u008d\u008e\u0001\u0000"+
		"\u0000\u0000\u008e\u008f\u0005\u0018\u0000\u0000\u008f\u0090\u0003*\u0015"+
		"\u0000\u0090\u0091\u0005,\u0000\u0000\u0091\u0015\u0001\u0000\u0000\u0000"+
		"\u0092\u0093\u0003*\u0015\u0000\u0093\u0094\u0005\u0018\u0000\u0000\u0094"+
		"\u0095\u0003*\u0015\u0000\u0095\u0096\u0005,\u0000\u0000\u0096\u0017\u0001"+
		"\u0000\u0000\u0000\u0097\u0099\u0005\u000b\u0000\u0000\u0098\u009a\u0003"+
		"*\u0015\u0000\u0099\u0098\u0001\u0000\u0000\u0000\u0099\u009a\u0001\u0000"+
		"\u0000\u0000\u009a\u009b\u0001\u0000\u0000\u0000\u009b\u009c\u0005,\u0000"+
		"\u0000\u009c\u0019\u0001\u0000\u0000\u0000\u009d\u009e\u0003*\u0015\u0000"+
		"\u009e\u009f\u0005,\u0000\u0000\u009f\u001b\u0001\u0000\u0000\u0000\u00a0"+
		"\u00a1\u0005\u0001\u0000\u0000\u00a1\u00a2\u0003*\u0015\u0000\u00a2\u00a4"+
		"\u0003(\u0014\u0000\u00a3\u00a5\u0003\u001e\u000f\u0000\u00a4\u00a3\u0001"+
		"\u0000\u0000\u0000\u00a4\u00a5\u0001\u0000\u0000\u0000\u00a5\u00a7\u0001"+
		"\u0000\u0000\u0000\u00a6\u00a8\u0003 \u0010\u0000\u00a7\u00a6\u0001\u0000"+
		"\u0000\u0000\u00a7\u00a8\u0001\u0000\u0000\u0000\u00a8\u001d\u0001\u0000"+
		"\u0000\u0000\u00a9\u00aa\u0005\u0002\u0000\u0000\u00aa\u00ab\u0003*\u0015"+
		"\u0000\u00ab\u00ac\u0003(\u0014\u0000\u00ac\u00ae\u0001\u0000\u0000\u0000"+
		"\u00ad\u00a9\u0001\u0000\u0000\u0000\u00ae\u00af\u0001\u0000\u0000\u0000"+
		"\u00af\u00ad\u0001\u0000\u0000\u0000\u00af\u00b0\u0001\u0000\u0000\u0000"+
		"\u00b0\u001f\u0001\u0000\u0000\u0000\u00b1\u00b2\u0005\u0003\u0000\u0000"+
		"\u00b2\u00b3\u0003(\u0014\u0000\u00b3!\u0001\u0000\u0000\u0000\u00b4\u00bf"+
		"\u0005/\u0000\u0000\u00b5\u00b6\u0005%\u0000\u0000\u00b6\u00b9\u0005/"+
		"\u0000\u0000\u00b7\u00b8\u0005+\u0000\u0000\u00b8\u00ba\u0005/\u0000\u0000"+
		"\u00b9\u00b7\u0001\u0000\u0000\u0000\u00ba\u00bb\u0001\u0000\u0000\u0000"+
		"\u00bb\u00b9\u0001\u0000\u0000\u0000\u00bb\u00bc\u0001\u0000\u0000\u0000"+
		"\u00bc\u00bd\u0001\u0000\u0000\u0000\u00bd\u00bf\u0005&\u0000\u0000\u00be"+
		"\u00b4\u0001\u0000\u0000\u0000\u00be\u00b5\u0001\u0000\u0000\u0000\u00bf"+
		"#\u0001\u0000\u0000\u0000\u00c0\u00c1\u0005.\u0000\u0000\u00c1\u00c2\u0005"+
		",\u0000\u0000\u00c2%\u0001\u0000\u0000\u0000\u00c3\u00c4\u0005\b\u0000"+
		"\u0000\u00c4\u00c5\u0003*\u0015\u0000\u00c5\u00c6\u0003(\u0014\u0000\u00c6"+
		"\u00d0\u0001\u0000\u0000\u0000\u00c7\u00c8\u0005\u0007\u0000\u0000\u00c8"+
		"\u00c9\u0005/\u0000\u0000\u00c9\u00ca\u0005\n\u0000\u0000\u00ca\u00cb"+
		"\u0003*\u0015\u0000\u00cb\u00cc\u0003(\u0014\u0000\u00cc\u00d0\u0001\u0000"+
		"\u0000\u0000\u00cd\u00ce\u0005\t\u0000\u0000\u00ce\u00d0\u0003(\u0014"+
		"\u0000\u00cf\u00c3\u0001\u0000\u0000\u0000\u00cf\u00c7\u0001\u0000\u0000"+
		"\u0000\u00cf\u00cd\u0001\u0000\u0000\u0000\u00d0\'\u0001\u0000\u0000\u0000"+
		"\u00d1\u00d5\u0005\'\u0000\u0000\u00d2\u00d4\u0003\u0012\t\u0000\u00d3"+
		"\u00d2\u0001\u0000\u0000\u0000\u00d4\u00d7\u0001\u0000\u0000\u0000\u00d5"+
		"\u00d3\u0001\u0000\u0000\u0000\u00d5\u00d6\u0001\u0000\u0000\u0000\u00d6"+
		"\u00d8\u0001\u0000\u0000\u0000\u00d7\u00d5\u0001\u0000\u0000\u0000\u00d8"+
		"\u00d9\u0005(\u0000\u0000\u00d9)\u0001\u0000\u0000\u0000\u00da\u00db\u0006"+
		"\u0015\uffff\uffff\u0000\u00db\u00dc\u0007\u0001\u0000\u0000\u00dc\u0100"+
		"\u0003*\u0015\u0010\u00dd\u00de\u0005%\u0000\u0000\u00de\u00df\u0003*"+
		"\u0015\u0000\u00df\u00e0\u0005&\u0000\u0000\u00e0\u0100\u0001\u0000\u0000"+
		"\u0000\u00e1\u00e2\u0005%\u0000\u0000\u00e2\u00e5\u0003*\u0015\u0000\u00e3"+
		"\u00e4\u0005+\u0000\u0000\u00e4\u00e6\u0003*\u0015\u0000\u00e5\u00e3\u0001"+
		"\u0000\u0000\u0000\u00e6\u00e7\u0001\u0000\u0000\u0000\u00e7\u00e5\u0001"+
		"\u0000\u0000\u0000\u00e7\u00e8\u0001\u0000\u0000\u0000\u00e8\u00e9\u0001"+
		"\u0000\u0000\u0000\u00e9\u00ea\u0005&\u0000\u0000\u00ea\u0100\u0001\u0000"+
		"\u0000\u0000\u00eb\u00f4\u0005)\u0000\u0000\u00ec\u00f1\u0003*\u0015\u0000"+
		"\u00ed\u00ee\u0005+\u0000\u0000\u00ee\u00f0\u0003*\u0015\u0000\u00ef\u00ed"+
		"\u0001\u0000\u0000\u0000\u00f0\u00f3\u0001\u0000\u0000\u0000\u00f1\u00ef"+
		"\u0001\u0000\u0000\u0000\u00f1\u00f2\u0001\u0000\u0000\u0000\u00f2\u00f5"+
		"\u0001\u0000\u0000\u0000\u00f3\u00f1\u0001\u0000\u0000\u0000\u00f4\u00ec"+
		"\u0001\u0000\u0000\u0000\u00f4\u00f5\u0001\u0000\u0000\u0000\u00f5\u00f6"+
		"\u0001\u0000\u0000\u0000\u00f6\u0100\u0005*\u0000\u0000\u00f7\u00f8\u0005"+
		"/\u0000\u0000\u00f8\u00fa\u0005%\u0000\u0000\u00f9\u00fb\u0003,\u0016"+
		"\u0000\u00fa\u00f9\u0001\u0000\u0000\u0000\u00fa\u00fb\u0001\u0000\u0000"+
		"\u0000\u00fb\u00fc\u0001\u0000\u0000\u0000\u00fc\u0100\u0005&\u0000\u0000"+
		"\u00fd\u0100\u0003.\u0017\u0000\u00fe\u0100\u0005/\u0000\u0000\u00ff\u00da"+
		"\u0001\u0000\u0000\u0000\u00ff\u00dd\u0001\u0000\u0000\u0000\u00ff\u00e1"+
		"\u0001\u0000\u0000\u0000\u00ff\u00eb\u0001\u0000\u0000\u0000\u00ff\u00f7"+
		"\u0001\u0000\u0000\u0000\u00ff\u00fd\u0001\u0000\u0000\u0000\u00ff\u00fe"+
		"\u0001\u0000\u0000\u0000\u0100\u0120\u0001\u0000\u0000\u0000\u0101\u0102"+
		"\n\u000b\u0000\u0000\u0102\u0103\u0007\u0002\u0000\u0000\u0103\u011f\u0003"+
		"*\u0015\f\u0104\u0105\n\n\u0000\u0000\u0105\u0106\u0007\u0001\u0000\u0000"+
		"\u0106\u011f\u0003*\u0015\u000b\u0107\u0108\n\t\u0000\u0000\u0108\u0109"+
		"\u0007\u0003\u0000\u0000\u0109\u011f\u0003*\u0015\n\u010a\u010b\n\b\u0000"+
		"\u0000\u010b\u010c\u0007\u0004\u0000\u0000\u010c\u011f\u0003*\u0015\t"+
		"\u010d\u010e\n\u0007\u0000\u0000\u010e\u010f\u0005\u0014\u0000\u0000\u010f"+
		"\u011f\u0003*\u0015\b\u0110\u0111\n\u0006\u0000\u0000\u0111\u0112\u0005"+
		"\u0015\u0000\u0000\u0112\u011f\u0003*\u0015\u0007\u0113\u0114\n\u0005"+
		"\u0000\u0000\u0114\u0115\u0005$\u0000\u0000\u0115\u011f\u0003*\u0015\u0006"+
		"\u0116\u0117\n\u0004\u0000\u0000\u0117\u0118\u0005\u0017\u0000\u0000\u0118"+
		"\u011f\u0003*\u0015\u0005\u0119\u011a\n\f\u0000\u0000\u011a\u011b\u0005"+
		")\u0000\u0000\u011b\u011c\u0003*\u0015\u0000\u011c\u011d\u0005*\u0000"+
		"\u0000\u011d\u011f\u0001\u0000\u0000\u0000\u011e\u0101\u0001\u0000\u0000"+
		"\u0000\u011e\u0104\u0001\u0000\u0000\u0000\u011e\u0107\u0001\u0000\u0000"+
		"\u0000\u011e\u010a\u0001\u0000\u0000\u0000\u011e\u010d\u0001\u0000\u0000"+
		"\u0000\u011e\u0110\u0001\u0000\u0000\u0000\u011e\u0113\u0001\u0000\u0000"+
		"\u0000\u011e\u0116\u0001\u0000\u0000\u0000\u011e\u0119\u0001\u0000\u0000"+
		"\u0000\u011f\u0122\u0001\u0000\u0000\u0000\u0120\u011e\u0001\u0000\u0000"+
		"\u0000\u0120\u0121\u0001\u0000\u0000\u0000\u0121+\u0001\u0000\u0000\u0000"+
		"\u0122\u0120\u0001\u0000\u0000\u0000\u0123\u0128\u0003*\u0015\u0000\u0124"+
		"\u0125\u0005+\u0000\u0000\u0125\u0127\u0003*\u0015\u0000\u0126\u0124\u0001"+
		"\u0000\u0000\u0000\u0127\u012a\u0001\u0000\u0000\u0000\u0128\u0126\u0001"+
		"\u0000\u0000\u0000\u0128\u0129\u0001\u0000\u0000\u0000\u0129-\u0001\u0000"+
		"\u0000\u0000\u012a\u0128\u0001\u0000\u0000\u0000\u012b\u012c\u0007\u0005"+
		"\u0000\u0000\u012c/\u0001\u0000\u0000\u0000\u001c3:KOXZcgpt\u0086\u008c"+
		"\u0099\u00a4\u00a7\u00af\u00bb\u00be\u00cf\u00d5\u00e7\u00f1\u00f4\u00fa"+
		"\u00ff\u011e\u0120\u0128";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}