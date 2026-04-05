
// Generated from /home/unknown/projects/Rex/grammar/Rex.g4 by ANTLR 4.13.0


#include "RexListener.h"
#include "RexVisitor.h"

#include "Rex.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct RexStaticData final {
  RexStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  RexStaticData(const RexStaticData&) = delete;
  RexStaticData(RexStaticData&&) = delete;
  RexStaticData& operator=(const RexStaticData&) = delete;
  RexStaticData& operator=(RexStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag rexParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
RexStaticData *rexParserStaticData = nullptr;

void rexParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (rexParserStaticData != nullptr) {
    return;
  }
#else
  assert(rexParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<RexStaticData>(
    std::vector<std::string>{
      "file", "item", "typeDef", "type", "primitiveType", "functionDef", 
      "paramList", "param", "returnType", "statement", "letStmt", "assignStmt", 
      "returnStmt", "exprStmt", "pattern", "loopStmt", "block", "expr", 
      "argList", "literal"
    },
    std::vector<std::string>{
      "", "'fn'", "'let'", "'mut'", "'for'", "'while'", "'loop'", "'in'", 
      "'return'", "'type'", "'Int'", "'Bool'", "'Char'", "'Real'", "'String'", 
      "'true'", "'false'", "'and'", "'or'", "'->'", "'|>'", "'='", "'=='", 
      "'!='", "'<'", "'>'", "'<='", "'>='", "'+'", "'-'", "'*'", "'/'", 
      "'%'", "'..'", "'('", "')'", "'{'", "'}'", "'['", "']'", "','", "';'", 
      "':'"
    },
    std::vector<std::string>{
      "", "FN", "LET", "MUT", "FOR", "WHILE", "LOOP", "IN", "RETURN", "TYPE", 
      "INT", "BOOL", "CHAR", "REAL", "STRING", "TRUE", "FALSE", "AND", "OR", 
      "ARROW", "PIPE", "ASSIGN", "EQ", "NEQ", "LT", "GT", "LTE", "GTE", 
      "PLUS", "MINUS", "STAR", "DIV", "MOD", "RANGE", "LPAREN", "RPAREN", 
      "LBRACE", "RBRACE", "LBRACK", "RBRACK", "COMMA", "SEMI", "COLON", 
      "ID", "INT_LITERAL", "REAL_LITERAL", "STRING_LITERAL", "CHAR_LITERAL", 
      "WS", "LINE_COMMENT", "BLOCK_COMMENT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,50,244,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,1,0,5,0,42,8,0,10,
  	0,12,0,45,9,0,1,0,1,0,1,1,1,1,1,1,3,1,52,8,1,1,2,1,2,1,2,1,2,1,2,1,2,
  	1,3,1,3,1,3,1,3,1,3,1,3,1,3,4,3,67,8,3,11,3,12,3,68,1,3,1,3,3,3,73,8,
  	3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,5,3,82,8,3,10,3,12,3,85,9,3,1,4,1,4,1,5,
  	1,5,1,5,1,5,3,5,93,8,5,1,5,1,5,3,5,97,8,5,1,5,1,5,1,6,1,6,1,6,5,6,104,
  	8,6,10,6,12,6,107,9,6,1,7,3,7,110,8,7,1,7,1,7,1,7,1,7,1,8,1,8,1,8,1,9,
  	1,9,1,9,1,9,1,9,3,9,124,8,9,1,10,1,10,1,10,1,10,1,10,1,10,1,11,1,11,1,
  	11,1,11,1,11,1,12,1,12,3,12,139,8,12,1,12,1,12,1,13,1,13,1,13,1,14,1,
  	14,1,14,1,14,1,14,4,14,151,8,14,11,14,12,14,152,1,14,1,14,3,14,157,8,
  	14,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,3,15,171,
  	8,15,1,16,1,16,5,16,175,8,16,10,16,12,16,178,9,16,1,16,3,16,181,8,16,
  	1,16,1,16,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,3,17,193,8,17,1,17,
  	1,17,1,17,3,17,198,8,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,
  	1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,
  	1,17,1,17,1,17,1,17,1,17,1,17,5,17,229,8,17,10,17,12,17,232,9,17,1,18,
  	1,18,1,18,5,18,237,8,18,10,18,12,18,240,9,18,1,19,1,19,1,19,0,2,6,34,
  	20,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,0,6,1,0,10,
  	14,1,0,30,32,1,0,28,29,1,0,24,27,1,0,22,23,2,0,15,16,44,47,260,0,43,1,
  	0,0,0,2,51,1,0,0,0,4,53,1,0,0,0,6,72,1,0,0,0,8,86,1,0,0,0,10,88,1,0,0,
  	0,12,100,1,0,0,0,14,109,1,0,0,0,16,115,1,0,0,0,18,123,1,0,0,0,20,125,
  	1,0,0,0,22,131,1,0,0,0,24,136,1,0,0,0,26,142,1,0,0,0,28,156,1,0,0,0,30,
  	170,1,0,0,0,32,172,1,0,0,0,34,197,1,0,0,0,36,233,1,0,0,0,38,241,1,0,0,
  	0,40,42,3,2,1,0,41,40,1,0,0,0,42,45,1,0,0,0,43,41,1,0,0,0,43,44,1,0,0,
  	0,44,46,1,0,0,0,45,43,1,0,0,0,46,47,5,0,0,1,47,1,1,0,0,0,48,52,3,10,5,
  	0,49,52,3,4,2,0,50,52,3,18,9,0,51,48,1,0,0,0,51,49,1,0,0,0,51,50,1,0,
  	0,0,52,3,1,0,0,0,53,54,5,9,0,0,54,55,5,43,0,0,55,56,5,21,0,0,56,57,3,
  	6,3,0,57,58,5,41,0,0,58,5,1,0,0,0,59,60,6,3,-1,0,60,73,3,8,4,0,61,73,
  	5,43,0,0,62,63,5,34,0,0,63,66,3,6,3,0,64,65,5,40,0,0,65,67,3,6,3,0,66,
  	64,1,0,0,0,67,68,1,0,0,0,68,66,1,0,0,0,68,69,1,0,0,0,69,70,1,0,0,0,70,
  	71,5,35,0,0,71,73,1,0,0,0,72,59,1,0,0,0,72,61,1,0,0,0,72,62,1,0,0,0,73,
  	83,1,0,0,0,74,75,10,3,0,0,75,76,5,38,0,0,76,82,5,39,0,0,77,78,10,2,0,
  	0,78,79,5,38,0,0,79,80,5,44,0,0,80,82,5,39,0,0,81,74,1,0,0,0,81,77,1,
  	0,0,0,82,85,1,0,0,0,83,81,1,0,0,0,83,84,1,0,0,0,84,7,1,0,0,0,85,83,1,
  	0,0,0,86,87,7,0,0,0,87,9,1,0,0,0,88,89,5,1,0,0,89,90,5,43,0,0,90,92,5,
  	34,0,0,91,93,3,12,6,0,92,91,1,0,0,0,92,93,1,0,0,0,93,94,1,0,0,0,94,96,
  	5,35,0,0,95,97,3,16,8,0,96,95,1,0,0,0,96,97,1,0,0,0,97,98,1,0,0,0,98,
  	99,3,32,16,0,99,11,1,0,0,0,100,105,3,14,7,0,101,102,5,40,0,0,102,104,
  	3,14,7,0,103,101,1,0,0,0,104,107,1,0,0,0,105,103,1,0,0,0,105,106,1,0,
  	0,0,106,13,1,0,0,0,107,105,1,0,0,0,108,110,5,3,0,0,109,108,1,0,0,0,109,
  	110,1,0,0,0,110,111,1,0,0,0,111,112,5,43,0,0,112,113,5,42,0,0,113,114,
  	3,6,3,0,114,15,1,0,0,0,115,116,5,19,0,0,116,117,3,6,3,0,117,17,1,0,0,
  	0,118,124,3,20,10,0,119,124,3,22,11,0,120,124,3,24,12,0,121,124,3,30,
  	15,0,122,124,3,26,13,0,123,118,1,0,0,0,123,119,1,0,0,0,123,120,1,0,0,
  	0,123,121,1,0,0,0,123,122,1,0,0,0,124,19,1,0,0,0,125,126,5,2,0,0,126,
  	127,3,28,14,0,127,128,5,21,0,0,128,129,3,34,17,0,129,130,5,41,0,0,130,
  	21,1,0,0,0,131,132,3,34,17,0,132,133,5,21,0,0,133,134,3,34,17,0,134,135,
  	5,41,0,0,135,23,1,0,0,0,136,138,5,8,0,0,137,139,3,34,17,0,138,137,1,0,
  	0,0,138,139,1,0,0,0,139,140,1,0,0,0,140,141,5,41,0,0,141,25,1,0,0,0,142,
  	143,3,34,17,0,143,144,5,41,0,0,144,27,1,0,0,0,145,157,5,43,0,0,146,147,
  	5,34,0,0,147,150,3,28,14,0,148,149,5,40,0,0,149,151,3,28,14,0,150,148,
  	1,0,0,0,151,152,1,0,0,0,152,150,1,0,0,0,152,153,1,0,0,0,153,154,1,0,0,
  	0,154,155,5,35,0,0,155,157,1,0,0,0,156,145,1,0,0,0,156,146,1,0,0,0,157,
  	29,1,0,0,0,158,159,5,5,0,0,159,160,3,34,17,0,160,161,3,32,16,0,161,171,
  	1,0,0,0,162,163,5,4,0,0,163,164,5,43,0,0,164,165,5,7,0,0,165,166,3,34,
  	17,0,166,167,3,32,16,0,167,171,1,0,0,0,168,169,5,6,0,0,169,171,3,32,16,
  	0,170,158,1,0,0,0,170,162,1,0,0,0,170,168,1,0,0,0,171,31,1,0,0,0,172,
  	176,5,36,0,0,173,175,3,18,9,0,174,173,1,0,0,0,175,178,1,0,0,0,176,174,
  	1,0,0,0,176,177,1,0,0,0,177,180,1,0,0,0,178,176,1,0,0,0,179,181,3,34,
  	17,0,180,179,1,0,0,0,180,181,1,0,0,0,181,182,1,0,0,0,182,183,5,37,0,0,
  	183,33,1,0,0,0,184,185,6,17,-1,0,185,186,5,34,0,0,186,187,3,34,17,0,187,
  	188,5,35,0,0,188,198,1,0,0,0,189,190,5,43,0,0,190,192,5,34,0,0,191,193,
  	3,36,18,0,192,191,1,0,0,0,192,193,1,0,0,0,193,194,1,0,0,0,194,198,5,35,
  	0,0,195,198,3,38,19,0,196,198,5,43,0,0,197,184,1,0,0,0,197,189,1,0,0,
  	0,197,195,1,0,0,0,197,196,1,0,0,0,198,230,1,0,0,0,199,200,10,11,0,0,200,
  	201,5,33,0,0,201,229,3,34,17,12,202,203,10,10,0,0,203,204,7,1,0,0,204,
  	229,3,34,17,11,205,206,10,9,0,0,206,207,7,2,0,0,207,229,3,34,17,10,208,
  	209,10,8,0,0,209,210,7,3,0,0,210,229,3,34,17,9,211,212,10,7,0,0,212,213,
  	7,4,0,0,213,229,3,34,17,8,214,215,10,6,0,0,215,216,5,17,0,0,216,229,3,
  	34,17,7,217,218,10,5,0,0,218,219,5,18,0,0,219,229,3,34,17,6,220,221,10,
  	4,0,0,221,222,5,20,0,0,222,229,3,34,17,5,223,224,10,12,0,0,224,225,5,
  	38,0,0,225,226,3,34,17,0,226,227,5,39,0,0,227,229,1,0,0,0,228,199,1,0,
  	0,0,228,202,1,0,0,0,228,205,1,0,0,0,228,208,1,0,0,0,228,211,1,0,0,0,228,
  	214,1,0,0,0,228,217,1,0,0,0,228,220,1,0,0,0,228,223,1,0,0,0,229,232,1,
  	0,0,0,230,228,1,0,0,0,230,231,1,0,0,0,231,35,1,0,0,0,232,230,1,0,0,0,
  	233,238,3,34,17,0,234,235,5,40,0,0,235,237,3,34,17,0,236,234,1,0,0,0,
  	237,240,1,0,0,0,238,236,1,0,0,0,238,239,1,0,0,0,239,37,1,0,0,0,240,238,
  	1,0,0,0,241,242,7,5,0,0,242,39,1,0,0,0,22,43,51,68,72,81,83,92,96,105,
  	109,123,138,152,156,170,176,180,192,197,228,230,238
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  rexParserStaticData = staticData.release();
}

}

Rex::Rex(TokenStream *input) : Rex(input, antlr4::atn::ParserATNSimulatorOptions()) {}

Rex::Rex(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  Rex::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *rexParserStaticData->atn, rexParserStaticData->decisionToDFA, rexParserStaticData->sharedContextCache, options);
}

Rex::~Rex() {
  delete _interpreter;
}

const atn::ATN& Rex::getATN() const {
  return *rexParserStaticData->atn;
}

std::string Rex::getGrammarFileName() const {
  return "Rex.g4";
}

const std::vector<std::string>& Rex::getRuleNames() const {
  return rexParserStaticData->ruleNames;
}

const dfa::Vocabulary& Rex::getVocabulary() const {
  return rexParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView Rex::getSerializedATN() const {
  return rexParserStaticData->serializedATN;
}


//----------------- FileContext ------------------------------------------------------------------

Rex::FileContext::FileContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Rex::FileContext::EOF() {
  return getToken(Rex::EOF, 0);
}

std::vector<Rex::ItemContext *> Rex::FileContext::item() {
  return getRuleContexts<Rex::ItemContext>();
}

Rex::ItemContext* Rex::FileContext::item(size_t i) {
  return getRuleContext<Rex::ItemContext>(i);
}


size_t Rex::FileContext::getRuleIndex() const {
  return Rex::RuleFile;
}

void Rex::FileContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFile(this);
}

void Rex::FileContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFile(this);
}


std::any Rex::FileContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitFile(this);
  else
    return visitor->visitChildren(this);
}

Rex::FileContext* Rex::file() {
  FileContext *_localctx = _tracker.createInstance<FileContext>(_ctx, getState());
  enterRule(_localctx, 0, Rex::RuleFile);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(43);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 272696063656822) != 0)) {
      setState(40);
      item();
      setState(45);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(46);
    match(Rex::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ItemContext ------------------------------------------------------------------

Rex::ItemContext::ItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

Rex::FunctionDefContext* Rex::ItemContext::functionDef() {
  return getRuleContext<Rex::FunctionDefContext>(0);
}

Rex::TypeDefContext* Rex::ItemContext::typeDef() {
  return getRuleContext<Rex::TypeDefContext>(0);
}

Rex::StatementContext* Rex::ItemContext::statement() {
  return getRuleContext<Rex::StatementContext>(0);
}


size_t Rex::ItemContext::getRuleIndex() const {
  return Rex::RuleItem;
}

void Rex::ItemContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterItem(this);
}

void Rex::ItemContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitItem(this);
}


std::any Rex::ItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitItem(this);
  else
    return visitor->visitChildren(this);
}

Rex::ItemContext* Rex::item() {
  ItemContext *_localctx = _tracker.createInstance<ItemContext>(_ctx, getState());
  enterRule(_localctx, 2, Rex::RuleItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(51);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Rex::FN: {
        enterOuterAlt(_localctx, 1);
        setState(48);
        functionDef();
        break;
      }

      case Rex::TYPE: {
        enterOuterAlt(_localctx, 2);
        setState(49);
        typeDef();
        break;
      }

      case Rex::LET:
      case Rex::FOR:
      case Rex::WHILE:
      case Rex::LOOP:
      case Rex::RETURN:
      case Rex::TRUE:
      case Rex::FALSE:
      case Rex::LPAREN:
      case Rex::ID:
      case Rex::INT_LITERAL:
      case Rex::REAL_LITERAL:
      case Rex::STRING_LITERAL:
      case Rex::CHAR_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(50);
        statement();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeDefContext ------------------------------------------------------------------

Rex::TypeDefContext::TypeDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Rex::TypeDefContext::TYPE() {
  return getToken(Rex::TYPE, 0);
}

tree::TerminalNode* Rex::TypeDefContext::ID() {
  return getToken(Rex::ID, 0);
}

tree::TerminalNode* Rex::TypeDefContext::ASSIGN() {
  return getToken(Rex::ASSIGN, 0);
}

Rex::TypeContext* Rex::TypeDefContext::type() {
  return getRuleContext<Rex::TypeContext>(0);
}

tree::TerminalNode* Rex::TypeDefContext::SEMI() {
  return getToken(Rex::SEMI, 0);
}


size_t Rex::TypeDefContext::getRuleIndex() const {
  return Rex::RuleTypeDef;
}

void Rex::TypeDefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeDef(this);
}

void Rex::TypeDefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeDef(this);
}


std::any Rex::TypeDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitTypeDef(this);
  else
    return visitor->visitChildren(this);
}

Rex::TypeDefContext* Rex::typeDef() {
  TypeDefContext *_localctx = _tracker.createInstance<TypeDefContext>(_ctx, getState());
  enterRule(_localctx, 4, Rex::RuleTypeDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(53);
    match(Rex::TYPE);
    setState(54);
    match(Rex::ID);
    setState(55);
    match(Rex::ASSIGN);
    setState(56);
    type(0);
    setState(57);
    match(Rex::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

Rex::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t Rex::TypeContext::getRuleIndex() const {
  return Rex::RuleType;
}

void Rex::TypeContext::copyFrom(TypeContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ArrayTypeContext ------------------------------------------------------------------

Rex::TypeContext* Rex::ArrayTypeContext::type() {
  return getRuleContext<Rex::TypeContext>(0);
}

tree::TerminalNode* Rex::ArrayTypeContext::LBRACK() {
  return getToken(Rex::LBRACK, 0);
}

tree::TerminalNode* Rex::ArrayTypeContext::INT_LITERAL() {
  return getToken(Rex::INT_LITERAL, 0);
}

tree::TerminalNode* Rex::ArrayTypeContext::RBRACK() {
  return getToken(Rex::RBRACK, 0);
}

Rex::ArrayTypeContext::ArrayTypeContext(TypeContext *ctx) { copyFrom(ctx); }

void Rex::ArrayTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArrayType(this);
}
void Rex::ArrayTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArrayType(this);
}

std::any Rex::ArrayTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitArrayType(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NamedTypeContext ------------------------------------------------------------------

tree::TerminalNode* Rex::NamedTypeContext::ID() {
  return getToken(Rex::ID, 0);
}

Rex::NamedTypeContext::NamedTypeContext(TypeContext *ctx) { copyFrom(ctx); }

void Rex::NamedTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNamedType(this);
}
void Rex::NamedTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNamedType(this);
}

std::any Rex::NamedTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitNamedType(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PrimeTypeContext ------------------------------------------------------------------

Rex::PrimitiveTypeContext* Rex::PrimeTypeContext::primitiveType() {
  return getRuleContext<Rex::PrimitiveTypeContext>(0);
}

Rex::PrimeTypeContext::PrimeTypeContext(TypeContext *ctx) { copyFrom(ctx); }

void Rex::PrimeTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrimeType(this);
}
void Rex::PrimeTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrimeType(this);
}

std::any Rex::PrimeTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitPrimeType(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TupleTypeContext ------------------------------------------------------------------

tree::TerminalNode* Rex::TupleTypeContext::LPAREN() {
  return getToken(Rex::LPAREN, 0);
}

std::vector<Rex::TypeContext *> Rex::TupleTypeContext::type() {
  return getRuleContexts<Rex::TypeContext>();
}

Rex::TypeContext* Rex::TupleTypeContext::type(size_t i) {
  return getRuleContext<Rex::TypeContext>(i);
}

tree::TerminalNode* Rex::TupleTypeContext::RPAREN() {
  return getToken(Rex::RPAREN, 0);
}

std::vector<tree::TerminalNode *> Rex::TupleTypeContext::COMMA() {
  return getTokens(Rex::COMMA);
}

tree::TerminalNode* Rex::TupleTypeContext::COMMA(size_t i) {
  return getToken(Rex::COMMA, i);
}

Rex::TupleTypeContext::TupleTypeContext(TypeContext *ctx) { copyFrom(ctx); }

void Rex::TupleTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTupleType(this);
}
void Rex::TupleTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTupleType(this);
}

std::any Rex::TupleTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitTupleType(this);
  else
    return visitor->visitChildren(this);
}
//----------------- SliceTypeContext ------------------------------------------------------------------

Rex::TypeContext* Rex::SliceTypeContext::type() {
  return getRuleContext<Rex::TypeContext>(0);
}

tree::TerminalNode* Rex::SliceTypeContext::LBRACK() {
  return getToken(Rex::LBRACK, 0);
}

tree::TerminalNode* Rex::SliceTypeContext::RBRACK() {
  return getToken(Rex::RBRACK, 0);
}

Rex::SliceTypeContext::SliceTypeContext(TypeContext *ctx) { copyFrom(ctx); }

void Rex::SliceTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSliceType(this);
}
void Rex::SliceTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSliceType(this);
}

std::any Rex::SliceTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitSliceType(this);
  else
    return visitor->visitChildren(this);
}

Rex::TypeContext* Rex::type() {
   return type(0);
}

Rex::TypeContext* Rex::type(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  Rex::TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, parentState);
  Rex::TypeContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 6;
  enterRecursionRule(_localctx, 6, Rex::RuleType, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(72);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Rex::INT:
      case Rex::BOOL:
      case Rex::CHAR:
      case Rex::REAL:
      case Rex::STRING: {
        _localctx = _tracker.createInstance<PrimeTypeContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(60);
        primitiveType();
        break;
      }

      case Rex::ID: {
        _localctx = _tracker.createInstance<NamedTypeContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(61);
        match(Rex::ID);
        break;
      }

      case Rex::LPAREN: {
        _localctx = _tracker.createInstance<TupleTypeContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(62);
        match(Rex::LPAREN);
        setState(63);
        type(0);
        setState(66); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(64);
          match(Rex::COMMA);
          setState(65);
          type(0);
          setState(68); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (_la == Rex::COMMA);
        setState(70);
        match(Rex::RPAREN);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(83);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(81);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<SliceTypeContext>(_tracker.createInstance<TypeContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleType);
          setState(74);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(75);
          match(Rex::LBRACK);
          setState(76);
          match(Rex::RBRACK);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<ArrayTypeContext>(_tracker.createInstance<TypeContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleType);
          setState(77);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(78);
          match(Rex::LBRACK);
          setState(79);
          match(Rex::INT_LITERAL);
          setState(80);
          match(Rex::RBRACK);
          break;
        }

        default:
          break;
        } 
      }
      setState(85);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- PrimitiveTypeContext ------------------------------------------------------------------

Rex::PrimitiveTypeContext::PrimitiveTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Rex::PrimitiveTypeContext::INT() {
  return getToken(Rex::INT, 0);
}

tree::TerminalNode* Rex::PrimitiveTypeContext::BOOL() {
  return getToken(Rex::BOOL, 0);
}

tree::TerminalNode* Rex::PrimitiveTypeContext::CHAR() {
  return getToken(Rex::CHAR, 0);
}

tree::TerminalNode* Rex::PrimitiveTypeContext::REAL() {
  return getToken(Rex::REAL, 0);
}

tree::TerminalNode* Rex::PrimitiveTypeContext::STRING() {
  return getToken(Rex::STRING, 0);
}


size_t Rex::PrimitiveTypeContext::getRuleIndex() const {
  return Rex::RulePrimitiveType;
}

void Rex::PrimitiveTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrimitiveType(this);
}

void Rex::PrimitiveTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrimitiveType(this);
}


std::any Rex::PrimitiveTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitPrimitiveType(this);
  else
    return visitor->visitChildren(this);
}

Rex::PrimitiveTypeContext* Rex::primitiveType() {
  PrimitiveTypeContext *_localctx = _tracker.createInstance<PrimitiveTypeContext>(_ctx, getState());
  enterRule(_localctx, 8, Rex::RulePrimitiveType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(86);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 31744) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDefContext ------------------------------------------------------------------

Rex::FunctionDefContext::FunctionDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Rex::FunctionDefContext::FN() {
  return getToken(Rex::FN, 0);
}

tree::TerminalNode* Rex::FunctionDefContext::ID() {
  return getToken(Rex::ID, 0);
}

tree::TerminalNode* Rex::FunctionDefContext::LPAREN() {
  return getToken(Rex::LPAREN, 0);
}

tree::TerminalNode* Rex::FunctionDefContext::RPAREN() {
  return getToken(Rex::RPAREN, 0);
}

Rex::BlockContext* Rex::FunctionDefContext::block() {
  return getRuleContext<Rex::BlockContext>(0);
}

Rex::ParamListContext* Rex::FunctionDefContext::paramList() {
  return getRuleContext<Rex::ParamListContext>(0);
}

Rex::ReturnTypeContext* Rex::FunctionDefContext::returnType() {
  return getRuleContext<Rex::ReturnTypeContext>(0);
}


size_t Rex::FunctionDefContext::getRuleIndex() const {
  return Rex::RuleFunctionDef;
}

void Rex::FunctionDefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionDef(this);
}

void Rex::FunctionDefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionDef(this);
}


std::any Rex::FunctionDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitFunctionDef(this);
  else
    return visitor->visitChildren(this);
}

Rex::FunctionDefContext* Rex::functionDef() {
  FunctionDefContext *_localctx = _tracker.createInstance<FunctionDefContext>(_ctx, getState());
  enterRule(_localctx, 10, Rex::RuleFunctionDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(88);
    match(Rex::FN);
    setState(89);
    match(Rex::ID);
    setState(90);
    match(Rex::LPAREN);
    setState(92);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == Rex::MUT

    || _la == Rex::ID) {
      setState(91);
      paramList();
    }
    setState(94);
    match(Rex::RPAREN);
    setState(96);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == Rex::ARROW) {
      setState(95);
      returnType();
    }
    setState(98);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamListContext ------------------------------------------------------------------

Rex::ParamListContext::ParamListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<Rex::ParamContext *> Rex::ParamListContext::param() {
  return getRuleContexts<Rex::ParamContext>();
}

Rex::ParamContext* Rex::ParamListContext::param(size_t i) {
  return getRuleContext<Rex::ParamContext>(i);
}

std::vector<tree::TerminalNode *> Rex::ParamListContext::COMMA() {
  return getTokens(Rex::COMMA);
}

tree::TerminalNode* Rex::ParamListContext::COMMA(size_t i) {
  return getToken(Rex::COMMA, i);
}


size_t Rex::ParamListContext::getRuleIndex() const {
  return Rex::RuleParamList;
}

void Rex::ParamListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParamList(this);
}

void Rex::ParamListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParamList(this);
}


std::any Rex::ParamListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitParamList(this);
  else
    return visitor->visitChildren(this);
}

Rex::ParamListContext* Rex::paramList() {
  ParamListContext *_localctx = _tracker.createInstance<ParamListContext>(_ctx, getState());
  enterRule(_localctx, 12, Rex::RuleParamList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(100);
    param();
    setState(105);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == Rex::COMMA) {
      setState(101);
      match(Rex::COMMA);
      setState(102);
      param();
      setState(107);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamContext ------------------------------------------------------------------

Rex::ParamContext::ParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Rex::ParamContext::ID() {
  return getToken(Rex::ID, 0);
}

tree::TerminalNode* Rex::ParamContext::COLON() {
  return getToken(Rex::COLON, 0);
}

Rex::TypeContext* Rex::ParamContext::type() {
  return getRuleContext<Rex::TypeContext>(0);
}

tree::TerminalNode* Rex::ParamContext::MUT() {
  return getToken(Rex::MUT, 0);
}


size_t Rex::ParamContext::getRuleIndex() const {
  return Rex::RuleParam;
}

void Rex::ParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParam(this);
}

void Rex::ParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParam(this);
}


std::any Rex::ParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitParam(this);
  else
    return visitor->visitChildren(this);
}

Rex::ParamContext* Rex::param() {
  ParamContext *_localctx = _tracker.createInstance<ParamContext>(_ctx, getState());
  enterRule(_localctx, 14, Rex::RuleParam);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(109);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == Rex::MUT) {
      setState(108);
      match(Rex::MUT);
    }
    setState(111);
    match(Rex::ID);
    setState(112);
    match(Rex::COLON);
    setState(113);
    type(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnTypeContext ------------------------------------------------------------------

Rex::ReturnTypeContext::ReturnTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Rex::ReturnTypeContext::ARROW() {
  return getToken(Rex::ARROW, 0);
}

Rex::TypeContext* Rex::ReturnTypeContext::type() {
  return getRuleContext<Rex::TypeContext>(0);
}


size_t Rex::ReturnTypeContext::getRuleIndex() const {
  return Rex::RuleReturnType;
}

void Rex::ReturnTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReturnType(this);
}

void Rex::ReturnTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReturnType(this);
}


std::any Rex::ReturnTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitReturnType(this);
  else
    return visitor->visitChildren(this);
}

Rex::ReturnTypeContext* Rex::returnType() {
  ReturnTypeContext *_localctx = _tracker.createInstance<ReturnTypeContext>(_ctx, getState());
  enterRule(_localctx, 16, Rex::RuleReturnType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(115);
    match(Rex::ARROW);
    setState(116);
    type(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

Rex::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

Rex::LetStmtContext* Rex::StatementContext::letStmt() {
  return getRuleContext<Rex::LetStmtContext>(0);
}

Rex::AssignStmtContext* Rex::StatementContext::assignStmt() {
  return getRuleContext<Rex::AssignStmtContext>(0);
}

Rex::ReturnStmtContext* Rex::StatementContext::returnStmt() {
  return getRuleContext<Rex::ReturnStmtContext>(0);
}

Rex::LoopStmtContext* Rex::StatementContext::loopStmt() {
  return getRuleContext<Rex::LoopStmtContext>(0);
}

Rex::ExprStmtContext* Rex::StatementContext::exprStmt() {
  return getRuleContext<Rex::ExprStmtContext>(0);
}


size_t Rex::StatementContext::getRuleIndex() const {
  return Rex::RuleStatement;
}

void Rex::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void Rex::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}


std::any Rex::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

Rex::StatementContext* Rex::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 18, Rex::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(123);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(118);
      letStmt();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(119);
      assignStmt();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(120);
      returnStmt();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(121);
      loopStmt();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(122);
      exprStmt();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LetStmtContext ------------------------------------------------------------------

Rex::LetStmtContext::LetStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Rex::LetStmtContext::LET() {
  return getToken(Rex::LET, 0);
}

Rex::PatternContext* Rex::LetStmtContext::pattern() {
  return getRuleContext<Rex::PatternContext>(0);
}

tree::TerminalNode* Rex::LetStmtContext::ASSIGN() {
  return getToken(Rex::ASSIGN, 0);
}

Rex::ExprContext* Rex::LetStmtContext::expr() {
  return getRuleContext<Rex::ExprContext>(0);
}

tree::TerminalNode* Rex::LetStmtContext::SEMI() {
  return getToken(Rex::SEMI, 0);
}


size_t Rex::LetStmtContext::getRuleIndex() const {
  return Rex::RuleLetStmt;
}

void Rex::LetStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLetStmt(this);
}

void Rex::LetStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLetStmt(this);
}


std::any Rex::LetStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitLetStmt(this);
  else
    return visitor->visitChildren(this);
}

Rex::LetStmtContext* Rex::letStmt() {
  LetStmtContext *_localctx = _tracker.createInstance<LetStmtContext>(_ctx, getState());
  enterRule(_localctx, 20, Rex::RuleLetStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(125);
    match(Rex::LET);
    setState(126);
    pattern();
    setState(127);
    match(Rex::ASSIGN);
    setState(128);
    expr(0);
    setState(129);
    match(Rex::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignStmtContext ------------------------------------------------------------------

Rex::AssignStmtContext::AssignStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<Rex::ExprContext *> Rex::AssignStmtContext::expr() {
  return getRuleContexts<Rex::ExprContext>();
}

Rex::ExprContext* Rex::AssignStmtContext::expr(size_t i) {
  return getRuleContext<Rex::ExprContext>(i);
}

tree::TerminalNode* Rex::AssignStmtContext::ASSIGN() {
  return getToken(Rex::ASSIGN, 0);
}

tree::TerminalNode* Rex::AssignStmtContext::SEMI() {
  return getToken(Rex::SEMI, 0);
}


size_t Rex::AssignStmtContext::getRuleIndex() const {
  return Rex::RuleAssignStmt;
}

void Rex::AssignStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignStmt(this);
}

void Rex::AssignStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignStmt(this);
}


std::any Rex::AssignStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitAssignStmt(this);
  else
    return visitor->visitChildren(this);
}

Rex::AssignStmtContext* Rex::assignStmt() {
  AssignStmtContext *_localctx = _tracker.createInstance<AssignStmtContext>(_ctx, getState());
  enterRule(_localctx, 22, Rex::RuleAssignStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(131);
    expr(0);
    setState(132);
    match(Rex::ASSIGN);
    setState(133);
    expr(0);
    setState(134);
    match(Rex::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStmtContext ------------------------------------------------------------------

Rex::ReturnStmtContext::ReturnStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Rex::ReturnStmtContext::RETURN() {
  return getToken(Rex::RETURN, 0);
}

tree::TerminalNode* Rex::ReturnStmtContext::SEMI() {
  return getToken(Rex::SEMI, 0);
}

Rex::ExprContext* Rex::ReturnStmtContext::expr() {
  return getRuleContext<Rex::ExprContext>(0);
}


size_t Rex::ReturnStmtContext::getRuleIndex() const {
  return Rex::RuleReturnStmt;
}

void Rex::ReturnStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReturnStmt(this);
}

void Rex::ReturnStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReturnStmt(this);
}


std::any Rex::ReturnStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitReturnStmt(this);
  else
    return visitor->visitChildren(this);
}

Rex::ReturnStmtContext* Rex::returnStmt() {
  ReturnStmtContext *_localctx = _tracker.createInstance<ReturnStmtContext>(_ctx, getState());
  enterRule(_localctx, 24, Rex::RuleReturnStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(136);
    match(Rex::RETURN);
    setState(138);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 272696063655936) != 0)) {
      setState(137);
      expr(0);
    }
    setState(140);
    match(Rex::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprStmtContext ------------------------------------------------------------------

Rex::ExprStmtContext::ExprStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

Rex::ExprContext* Rex::ExprStmtContext::expr() {
  return getRuleContext<Rex::ExprContext>(0);
}

tree::TerminalNode* Rex::ExprStmtContext::SEMI() {
  return getToken(Rex::SEMI, 0);
}


size_t Rex::ExprStmtContext::getRuleIndex() const {
  return Rex::RuleExprStmt;
}

void Rex::ExprStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExprStmt(this);
}

void Rex::ExprStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExprStmt(this);
}


std::any Rex::ExprStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitExprStmt(this);
  else
    return visitor->visitChildren(this);
}

Rex::ExprStmtContext* Rex::exprStmt() {
  ExprStmtContext *_localctx = _tracker.createInstance<ExprStmtContext>(_ctx, getState());
  enterRule(_localctx, 26, Rex::RuleExprStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(142);
    expr(0);
    setState(143);
    match(Rex::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PatternContext ------------------------------------------------------------------

Rex::PatternContext::PatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Rex::PatternContext::ID() {
  return getToken(Rex::ID, 0);
}

tree::TerminalNode* Rex::PatternContext::LPAREN() {
  return getToken(Rex::LPAREN, 0);
}

std::vector<Rex::PatternContext *> Rex::PatternContext::pattern() {
  return getRuleContexts<Rex::PatternContext>();
}

Rex::PatternContext* Rex::PatternContext::pattern(size_t i) {
  return getRuleContext<Rex::PatternContext>(i);
}

tree::TerminalNode* Rex::PatternContext::RPAREN() {
  return getToken(Rex::RPAREN, 0);
}

std::vector<tree::TerminalNode *> Rex::PatternContext::COMMA() {
  return getTokens(Rex::COMMA);
}

tree::TerminalNode* Rex::PatternContext::COMMA(size_t i) {
  return getToken(Rex::COMMA, i);
}


size_t Rex::PatternContext::getRuleIndex() const {
  return Rex::RulePattern;
}

void Rex::PatternContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPattern(this);
}

void Rex::PatternContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPattern(this);
}


std::any Rex::PatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitPattern(this);
  else
    return visitor->visitChildren(this);
}

Rex::PatternContext* Rex::pattern() {
  PatternContext *_localctx = _tracker.createInstance<PatternContext>(_ctx, getState());
  enterRule(_localctx, 28, Rex::RulePattern);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(156);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Rex::ID: {
        enterOuterAlt(_localctx, 1);
        setState(145);
        match(Rex::ID);
        break;
      }

      case Rex::LPAREN: {
        enterOuterAlt(_localctx, 2);
        setState(146);
        match(Rex::LPAREN);
        setState(147);
        pattern();
        setState(150); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(148);
          match(Rex::COMMA);
          setState(149);
          pattern();
          setState(152); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (_la == Rex::COMMA);
        setState(154);
        match(Rex::RPAREN);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LoopStmtContext ------------------------------------------------------------------

Rex::LoopStmtContext::LoopStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Rex::LoopStmtContext::WHILE() {
  return getToken(Rex::WHILE, 0);
}

Rex::ExprContext* Rex::LoopStmtContext::expr() {
  return getRuleContext<Rex::ExprContext>(0);
}

Rex::BlockContext* Rex::LoopStmtContext::block() {
  return getRuleContext<Rex::BlockContext>(0);
}

tree::TerminalNode* Rex::LoopStmtContext::FOR() {
  return getToken(Rex::FOR, 0);
}

tree::TerminalNode* Rex::LoopStmtContext::ID() {
  return getToken(Rex::ID, 0);
}

tree::TerminalNode* Rex::LoopStmtContext::IN() {
  return getToken(Rex::IN, 0);
}

tree::TerminalNode* Rex::LoopStmtContext::LOOP() {
  return getToken(Rex::LOOP, 0);
}


size_t Rex::LoopStmtContext::getRuleIndex() const {
  return Rex::RuleLoopStmt;
}

void Rex::LoopStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLoopStmt(this);
}

void Rex::LoopStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLoopStmt(this);
}


std::any Rex::LoopStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitLoopStmt(this);
  else
    return visitor->visitChildren(this);
}

Rex::LoopStmtContext* Rex::loopStmt() {
  LoopStmtContext *_localctx = _tracker.createInstance<LoopStmtContext>(_ctx, getState());
  enterRule(_localctx, 30, Rex::RuleLoopStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(170);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case Rex::WHILE: {
        enterOuterAlt(_localctx, 1);
        setState(158);
        match(Rex::WHILE);
        setState(159);
        expr(0);
        setState(160);
        block();
        break;
      }

      case Rex::FOR: {
        enterOuterAlt(_localctx, 2);
        setState(162);
        match(Rex::FOR);
        setState(163);
        match(Rex::ID);
        setState(164);
        match(Rex::IN);
        setState(165);
        expr(0);
        setState(166);
        block();
        break;
      }

      case Rex::LOOP: {
        enterOuterAlt(_localctx, 3);
        setState(168);
        match(Rex::LOOP);
        setState(169);
        block();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

Rex::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Rex::BlockContext::LBRACE() {
  return getToken(Rex::LBRACE, 0);
}

tree::TerminalNode* Rex::BlockContext::RBRACE() {
  return getToken(Rex::RBRACE, 0);
}

std::vector<Rex::StatementContext *> Rex::BlockContext::statement() {
  return getRuleContexts<Rex::StatementContext>();
}

Rex::StatementContext* Rex::BlockContext::statement(size_t i) {
  return getRuleContext<Rex::StatementContext>(i);
}

Rex::ExprContext* Rex::BlockContext::expr() {
  return getRuleContext<Rex::ExprContext>(0);
}


size_t Rex::BlockContext::getRuleIndex() const {
  return Rex::RuleBlock;
}

void Rex::BlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlock(this);
}

void Rex::BlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlock(this);
}


std::any Rex::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

Rex::BlockContext* Rex::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 32, Rex::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(172);
    match(Rex::LBRACE);
    setState(176);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(173);
        statement(); 
      }
      setState(178);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx);
    }
    setState(180);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 272696063655936) != 0)) {
      setState(179);
      expr(0);
    }
    setState(182);
    match(Rex::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

Rex::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t Rex::ExprContext::getRuleIndex() const {
  return Rex::RuleExpr;
}

void Rex::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- MulExprContext ------------------------------------------------------------------

std::vector<Rex::ExprContext *> Rex::MulExprContext::expr() {
  return getRuleContexts<Rex::ExprContext>();
}

Rex::ExprContext* Rex::MulExprContext::expr(size_t i) {
  return getRuleContext<Rex::ExprContext>(i);
}

tree::TerminalNode* Rex::MulExprContext::STAR() {
  return getToken(Rex::STAR, 0);
}

tree::TerminalNode* Rex::MulExprContext::DIV() {
  return getToken(Rex::DIV, 0);
}

tree::TerminalNode* Rex::MulExprContext::MOD() {
  return getToken(Rex::MOD, 0);
}

Rex::MulExprContext::MulExprContext(ExprContext *ctx) { copyFrom(ctx); }

void Rex::MulExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMulExpr(this);
}
void Rex::MulExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMulExpr(this);
}

std::any Rex::MulExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitMulExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AndExprContext ------------------------------------------------------------------

std::vector<Rex::ExprContext *> Rex::AndExprContext::expr() {
  return getRuleContexts<Rex::ExprContext>();
}

Rex::ExprContext* Rex::AndExprContext::expr(size_t i) {
  return getRuleContext<Rex::ExprContext>(i);
}

tree::TerminalNode* Rex::AndExprContext::AND() {
  return getToken(Rex::AND, 0);
}

Rex::AndExprContext::AndExprContext(ExprContext *ctx) { copyFrom(ctx); }

void Rex::AndExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAndExpr(this);
}
void Rex::AndExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAndExpr(this);
}

std::any Rex::AndExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitAndExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IdExprContext ------------------------------------------------------------------

tree::TerminalNode* Rex::IdExprContext::ID() {
  return getToken(Rex::ID, 0);
}

Rex::IdExprContext::IdExprContext(ExprContext *ctx) { copyFrom(ctx); }

void Rex::IdExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIdExpr(this);
}
void Rex::IdExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIdExpr(this);
}

std::any Rex::IdExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitIdExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AddExprContext ------------------------------------------------------------------

std::vector<Rex::ExprContext *> Rex::AddExprContext::expr() {
  return getRuleContexts<Rex::ExprContext>();
}

Rex::ExprContext* Rex::AddExprContext::expr(size_t i) {
  return getRuleContext<Rex::ExprContext>(i);
}

tree::TerminalNode* Rex::AddExprContext::PLUS() {
  return getToken(Rex::PLUS, 0);
}

tree::TerminalNode* Rex::AddExprContext::MINUS() {
  return getToken(Rex::MINUS, 0);
}

Rex::AddExprContext::AddExprContext(ExprContext *ctx) { copyFrom(ctx); }

void Rex::AddExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAddExpr(this);
}
void Rex::AddExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAddExpr(this);
}

std::any Rex::AddExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitAddExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RangeExprContext ------------------------------------------------------------------

std::vector<Rex::ExprContext *> Rex::RangeExprContext::expr() {
  return getRuleContexts<Rex::ExprContext>();
}

Rex::ExprContext* Rex::RangeExprContext::expr(size_t i) {
  return getRuleContext<Rex::ExprContext>(i);
}

tree::TerminalNode* Rex::RangeExprContext::RANGE() {
  return getToken(Rex::RANGE, 0);
}

Rex::RangeExprContext::RangeExprContext(ExprContext *ctx) { copyFrom(ctx); }

void Rex::RangeExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRangeExpr(this);
}
void Rex::RangeExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRangeExpr(this);
}

std::any Rex::RangeExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitRangeExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OrExprContext ------------------------------------------------------------------

std::vector<Rex::ExprContext *> Rex::OrExprContext::expr() {
  return getRuleContexts<Rex::ExprContext>();
}

Rex::ExprContext* Rex::OrExprContext::expr(size_t i) {
  return getRuleContext<Rex::ExprContext>(i);
}

tree::TerminalNode* Rex::OrExprContext::OR() {
  return getToken(Rex::OR, 0);
}

Rex::OrExprContext::OrExprContext(ExprContext *ctx) { copyFrom(ctx); }

void Rex::OrExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOrExpr(this);
}
void Rex::OrExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOrExpr(this);
}

std::any Rex::OrExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitOrExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IndexExprContext ------------------------------------------------------------------

std::vector<Rex::ExprContext *> Rex::IndexExprContext::expr() {
  return getRuleContexts<Rex::ExprContext>();
}

Rex::ExprContext* Rex::IndexExprContext::expr(size_t i) {
  return getRuleContext<Rex::ExprContext>(i);
}

tree::TerminalNode* Rex::IndexExprContext::LBRACK() {
  return getToken(Rex::LBRACK, 0);
}

tree::TerminalNode* Rex::IndexExprContext::RBRACK() {
  return getToken(Rex::RBRACK, 0);
}

Rex::IndexExprContext::IndexExprContext(ExprContext *ctx) { copyFrom(ctx); }

void Rex::IndexExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIndexExpr(this);
}
void Rex::IndexExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIndexExpr(this);
}

std::any Rex::IndexExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitIndexExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- EqualityExprContext ------------------------------------------------------------------

std::vector<Rex::ExprContext *> Rex::EqualityExprContext::expr() {
  return getRuleContexts<Rex::ExprContext>();
}

Rex::ExprContext* Rex::EqualityExprContext::expr(size_t i) {
  return getRuleContext<Rex::ExprContext>(i);
}

tree::TerminalNode* Rex::EqualityExprContext::EQ() {
  return getToken(Rex::EQ, 0);
}

tree::TerminalNode* Rex::EqualityExprContext::NEQ() {
  return getToken(Rex::NEQ, 0);
}

Rex::EqualityExprContext::EqualityExprContext(ExprContext *ctx) { copyFrom(ctx); }

void Rex::EqualityExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEqualityExpr(this);
}
void Rex::EqualityExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEqualityExpr(this);
}

std::any Rex::EqualityExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitEqualityExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CompareExprContext ------------------------------------------------------------------

std::vector<Rex::ExprContext *> Rex::CompareExprContext::expr() {
  return getRuleContexts<Rex::ExprContext>();
}

Rex::ExprContext* Rex::CompareExprContext::expr(size_t i) {
  return getRuleContext<Rex::ExprContext>(i);
}

tree::TerminalNode* Rex::CompareExprContext::LT() {
  return getToken(Rex::LT, 0);
}

tree::TerminalNode* Rex::CompareExprContext::GT() {
  return getToken(Rex::GT, 0);
}

tree::TerminalNode* Rex::CompareExprContext::LTE() {
  return getToken(Rex::LTE, 0);
}

tree::TerminalNode* Rex::CompareExprContext::GTE() {
  return getToken(Rex::GTE, 0);
}

Rex::CompareExprContext::CompareExprContext(ExprContext *ctx) { copyFrom(ctx); }

void Rex::CompareExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCompareExpr(this);
}
void Rex::CompareExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCompareExpr(this);
}

std::any Rex::CompareExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitCompareExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PipeExprContext ------------------------------------------------------------------

std::vector<Rex::ExprContext *> Rex::PipeExprContext::expr() {
  return getRuleContexts<Rex::ExprContext>();
}

Rex::ExprContext* Rex::PipeExprContext::expr(size_t i) {
  return getRuleContext<Rex::ExprContext>(i);
}

tree::TerminalNode* Rex::PipeExprContext::PIPE() {
  return getToken(Rex::PIPE, 0);
}

Rex::PipeExprContext::PipeExprContext(ExprContext *ctx) { copyFrom(ctx); }

void Rex::PipeExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPipeExpr(this);
}
void Rex::PipeExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPipeExpr(this);
}

std::any Rex::PipeExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitPipeExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LiteralExprContext ------------------------------------------------------------------

Rex::LiteralContext* Rex::LiteralExprContext::literal() {
  return getRuleContext<Rex::LiteralContext>(0);
}

Rex::LiteralExprContext::LiteralExprContext(ExprContext *ctx) { copyFrom(ctx); }

void Rex::LiteralExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLiteralExpr(this);
}
void Rex::LiteralExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLiteralExpr(this);
}

std::any Rex::LiteralExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitLiteralExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CallExprContext ------------------------------------------------------------------

tree::TerminalNode* Rex::CallExprContext::ID() {
  return getToken(Rex::ID, 0);
}

tree::TerminalNode* Rex::CallExprContext::LPAREN() {
  return getToken(Rex::LPAREN, 0);
}

tree::TerminalNode* Rex::CallExprContext::RPAREN() {
  return getToken(Rex::RPAREN, 0);
}

Rex::ArgListContext* Rex::CallExprContext::argList() {
  return getRuleContext<Rex::ArgListContext>(0);
}

Rex::CallExprContext::CallExprContext(ExprContext *ctx) { copyFrom(ctx); }

void Rex::CallExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCallExpr(this);
}
void Rex::CallExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCallExpr(this);
}

std::any Rex::CallExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitCallExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParenExprContext ------------------------------------------------------------------

tree::TerminalNode* Rex::ParenExprContext::LPAREN() {
  return getToken(Rex::LPAREN, 0);
}

Rex::ExprContext* Rex::ParenExprContext::expr() {
  return getRuleContext<Rex::ExprContext>(0);
}

tree::TerminalNode* Rex::ParenExprContext::RPAREN() {
  return getToken(Rex::RPAREN, 0);
}

Rex::ParenExprContext::ParenExprContext(ExprContext *ctx) { copyFrom(ctx); }

void Rex::ParenExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParenExpr(this);
}
void Rex::ParenExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParenExpr(this);
}

std::any Rex::ParenExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitParenExpr(this);
  else
    return visitor->visitChildren(this);
}

Rex::ExprContext* Rex::expr() {
   return expr(0);
}

Rex::ExprContext* Rex::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  Rex::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  Rex::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 34;
  enterRecursionRule(_localctx, 34, Rex::RuleExpr, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(197);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<ParenExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(185);
      match(Rex::LPAREN);
      setState(186);
      expr(0);
      setState(187);
      match(Rex::RPAREN);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<CallExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(189);
      match(Rex::ID);
      setState(190);
      match(Rex::LPAREN);
      setState(192);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 272696063655936) != 0)) {
        setState(191);
        argList();
      }
      setState(194);
      match(Rex::RPAREN);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<LiteralExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(195);
      literal();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<IdExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(196);
      match(Rex::ID);
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(230);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(228);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<RangeExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(199);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(200);
          match(Rex::RANGE);
          setState(201);
          expr(12);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<MulExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(202);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(203);
          antlrcpp::downCast<MulExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 7516192768) != 0))) {
            antlrcpp::downCast<MulExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(204);
          expr(11);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<AddExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(205);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(206);
          antlrcpp::downCast<AddExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == Rex::PLUS

          || _la == Rex::MINUS)) {
            antlrcpp::downCast<AddExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(207);
          expr(10);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<CompareExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(208);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(209);
          antlrcpp::downCast<CompareExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 251658240) != 0))) {
            antlrcpp::downCast<CompareExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(210);
          expr(9);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<EqualityExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(211);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(212);
          antlrcpp::downCast<EqualityExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == Rex::EQ

          || _la == Rex::NEQ)) {
            antlrcpp::downCast<EqualityExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(213);
          expr(8);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<AndExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(214);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(215);
          antlrcpp::downCast<AndExprContext *>(_localctx)->op = match(Rex::AND);
          setState(216);
          expr(7);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<OrExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(217);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(218);
          antlrcpp::downCast<OrExprContext *>(_localctx)->op = match(Rex::OR);
          setState(219);
          expr(6);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<PipeExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(220);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(221);
          match(Rex::PIPE);
          setState(222);
          expr(5);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<IndexExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(223);

          if (!(precpred(_ctx, 12))) throw FailedPredicateException(this, "precpred(_ctx, 12)");
          setState(224);
          match(Rex::LBRACK);
          setState(225);
          expr(0);
          setState(226);
          match(Rex::RBRACK);
          break;
        }

        default:
          break;
        } 
      }
      setState(232);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- ArgListContext ------------------------------------------------------------------

Rex::ArgListContext::ArgListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<Rex::ExprContext *> Rex::ArgListContext::expr() {
  return getRuleContexts<Rex::ExprContext>();
}

Rex::ExprContext* Rex::ArgListContext::expr(size_t i) {
  return getRuleContext<Rex::ExprContext>(i);
}

std::vector<tree::TerminalNode *> Rex::ArgListContext::COMMA() {
  return getTokens(Rex::COMMA);
}

tree::TerminalNode* Rex::ArgListContext::COMMA(size_t i) {
  return getToken(Rex::COMMA, i);
}


size_t Rex::ArgListContext::getRuleIndex() const {
  return Rex::RuleArgList;
}

void Rex::ArgListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArgList(this);
}

void Rex::ArgListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArgList(this);
}


std::any Rex::ArgListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitArgList(this);
  else
    return visitor->visitChildren(this);
}

Rex::ArgListContext* Rex::argList() {
  ArgListContext *_localctx = _tracker.createInstance<ArgListContext>(_ctx, getState());
  enterRule(_localctx, 36, Rex::RuleArgList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(233);
    expr(0);
    setState(238);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == Rex::COMMA) {
      setState(234);
      match(Rex::COMMA);
      setState(235);
      expr(0);
      setState(240);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LiteralContext ------------------------------------------------------------------

Rex::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* Rex::LiteralContext::INT_LITERAL() {
  return getToken(Rex::INT_LITERAL, 0);
}

tree::TerminalNode* Rex::LiteralContext::REAL_LITERAL() {
  return getToken(Rex::REAL_LITERAL, 0);
}

tree::TerminalNode* Rex::LiteralContext::STRING_LITERAL() {
  return getToken(Rex::STRING_LITERAL, 0);
}

tree::TerminalNode* Rex::LiteralContext::CHAR_LITERAL() {
  return getToken(Rex::CHAR_LITERAL, 0);
}

tree::TerminalNode* Rex::LiteralContext::TRUE() {
  return getToken(Rex::TRUE, 0);
}

tree::TerminalNode* Rex::LiteralContext::FALSE() {
  return getToken(Rex::FALSE, 0);
}


size_t Rex::LiteralContext::getRuleIndex() const {
  return Rex::RuleLiteral;
}

void Rex::LiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLiteral(this);
}

void Rex::LiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLiteral(this);
}


std::any Rex::LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitLiteral(this);
  else
    return visitor->visitChildren(this);
}

Rex::LiteralContext* Rex::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 38, Rex::RuleLiteral);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(241);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 263882790764544) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool Rex::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 3: return typeSempred(antlrcpp::downCast<TypeContext *>(context), predicateIndex);
    case 17: return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool Rex::typeSempred(TypeContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 3);
    case 1: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

bool Rex::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 2: return precpred(_ctx, 11);
    case 3: return precpred(_ctx, 10);
    case 4: return precpred(_ctx, 9);
    case 5: return precpred(_ctx, 8);
    case 6: return precpred(_ctx, 7);
    case 7: return precpred(_ctx, 6);
    case 8: return precpred(_ctx, 5);
    case 9: return precpred(_ctx, 4);
    case 10: return precpred(_ctx, 12);

  default:
    break;
  }
  return true;
}

void Rex::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  rexParserInitialize();
#else
  ::antlr4::internal::call_once(rexParserOnceFlag, rexParserInitialize);
#endif
}
