
// Generated from /home/unknown/projects/Rex/grammar/Rex.g4 by ANTLR 4.13.0


#include "RexListener.h"
#include "RexVisitor.h"

#include "RexParser.h"


using namespace antlrcpp;
using namespace rex;

using namespace antlr4;

namespace {

struct RexParserStaticData final {
  RexParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  RexParserStaticData(const RexParserStaticData&) = delete;
  RexParserStaticData(RexParserStaticData&&) = delete;
  RexParserStaticData& operator=(const RexParserStaticData&) = delete;
  RexParserStaticData& operator=(RexParserStaticData&&) = delete;

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
RexParserStaticData *rexParserStaticData = nullptr;

void rexParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (rexParserStaticData != nullptr) {
    return;
  }
#else
  assert(rexParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<RexParserStaticData>(
    std::vector<std::string>{
      "file", "item", "typeDef", "type", "primitiveType", "functionDef", 
      "paramList", "param", "returnType", "statement", "letStmt", "assignStmt", 
      "returnStmt", "exprStmt", "pattern", "loopStmt", "block", "expr", 
      "argList", "literal"
    },
    std::vector<std::string>{
      "", "'function'", "'let'", "'mut'", "'for'", "'while'", "'loop'", 
      "'in'", "'return'", "'type'", "'Int'", "'Bool'", "'Char'", "'Real'", 
      "'String'", "'true'", "'false'", "'and'", "'or'", "'->'", "'|>'", 
      "'='", "'=='", "'!='", "'<'", "'>'", "'<='", "'>='", "'+'", "'-'", 
      "'*'", "'/'", "'%'", "'..'", "'('", "')'", "'{'", "'}'", "'['", "']'", 
      "','", "';'", "':'"
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
  	4,1,50,258,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,1,0,5,0,42,8,0,10,
  	0,12,0,45,9,0,1,0,1,0,1,1,1,1,1,1,3,1,52,8,1,1,2,1,2,1,2,1,2,1,2,1,2,
  	1,3,1,3,1,3,1,3,1,3,1,3,1,3,4,3,67,8,3,11,3,12,3,68,1,3,1,3,3,3,73,8,
  	3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,5,3,82,8,3,10,3,12,3,85,9,3,1,4,1,4,1,5,
  	1,5,1,5,1,5,3,5,93,8,5,1,5,1,5,3,5,97,8,5,1,5,1,5,1,6,1,6,1,6,5,6,104,
  	8,6,10,6,12,6,107,9,6,1,7,3,7,110,8,7,1,7,1,7,1,7,1,7,1,8,1,8,1,8,1,9,
  	1,9,1,9,1,9,1,9,3,9,124,8,9,1,10,1,10,1,10,1,10,3,10,130,8,10,1,10,1,
  	10,1,10,1,10,1,11,1,11,1,11,1,11,1,11,1,12,1,12,3,12,143,8,12,1,12,1,
  	12,1,13,1,13,1,13,1,14,1,14,1,14,1,14,1,14,4,14,155,8,14,11,14,12,14,
  	156,1,14,1,14,3,14,161,8,14,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,
  	15,1,15,1,15,1,15,3,15,175,8,15,1,16,1,16,5,16,179,8,16,10,16,12,16,182,
  	9,16,1,16,3,16,185,8,16,1,16,1,16,1,17,1,17,1,17,1,17,1,17,1,17,1,17,
  	1,17,1,17,4,17,198,8,17,11,17,12,17,199,1,17,1,17,1,17,1,17,1,17,3,17,
  	207,8,17,1,17,1,17,1,17,3,17,212,8,17,1,17,1,17,1,17,1,17,1,17,1,17,1,
  	17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,
  	17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,5,17,243,8,17,10,17,12,17,
  	246,9,17,1,18,1,18,1,18,5,18,251,8,18,10,18,12,18,254,9,18,1,19,1,19,
  	1,19,0,2,6,34,20,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,
  	38,0,6,1,0,10,14,1,0,30,32,1,0,28,29,1,0,24,27,1,0,22,23,2,0,15,16,44,
  	47,277,0,43,1,0,0,0,2,51,1,0,0,0,4,53,1,0,0,0,6,72,1,0,0,0,8,86,1,0,0,
  	0,10,88,1,0,0,0,12,100,1,0,0,0,14,109,1,0,0,0,16,115,1,0,0,0,18,123,1,
  	0,0,0,20,125,1,0,0,0,22,135,1,0,0,0,24,140,1,0,0,0,26,146,1,0,0,0,28,
  	160,1,0,0,0,30,174,1,0,0,0,32,176,1,0,0,0,34,211,1,0,0,0,36,247,1,0,0,
  	0,38,255,1,0,0,0,40,42,3,2,1,0,41,40,1,0,0,0,42,45,1,0,0,0,43,41,1,0,
  	0,0,43,44,1,0,0,0,44,46,1,0,0,0,45,43,1,0,0,0,46,47,5,0,0,1,47,1,1,0,
  	0,0,48,52,3,10,5,0,49,52,3,4,2,0,50,52,3,18,9,0,51,48,1,0,0,0,51,49,1,
  	0,0,0,51,50,1,0,0,0,52,3,1,0,0,0,53,54,5,9,0,0,54,55,5,43,0,0,55,56,5,
  	21,0,0,56,57,3,6,3,0,57,58,5,41,0,0,58,5,1,0,0,0,59,60,6,3,-1,0,60,73,
  	3,8,4,0,61,73,5,43,0,0,62,63,5,34,0,0,63,66,3,6,3,0,64,65,5,40,0,0,65,
  	67,3,6,3,0,66,64,1,0,0,0,67,68,1,0,0,0,68,66,1,0,0,0,68,69,1,0,0,0,69,
  	70,1,0,0,0,70,71,5,35,0,0,71,73,1,0,0,0,72,59,1,0,0,0,72,61,1,0,0,0,72,
  	62,1,0,0,0,73,83,1,0,0,0,74,75,10,3,0,0,75,76,5,38,0,0,76,82,5,39,0,0,
  	77,78,10,2,0,0,78,79,5,38,0,0,79,80,5,44,0,0,80,82,5,39,0,0,81,74,1,0,
  	0,0,81,77,1,0,0,0,82,85,1,0,0,0,83,81,1,0,0,0,83,84,1,0,0,0,84,7,1,0,
  	0,0,85,83,1,0,0,0,86,87,7,0,0,0,87,9,1,0,0,0,88,89,5,1,0,0,89,90,5,43,
  	0,0,90,92,5,34,0,0,91,93,3,12,6,0,92,91,1,0,0,0,92,93,1,0,0,0,93,94,1,
  	0,0,0,94,96,5,35,0,0,95,97,3,16,8,0,96,95,1,0,0,0,96,97,1,0,0,0,97,98,
  	1,0,0,0,98,99,3,32,16,0,99,11,1,0,0,0,100,105,3,14,7,0,101,102,5,40,0,
  	0,102,104,3,14,7,0,103,101,1,0,0,0,104,107,1,0,0,0,105,103,1,0,0,0,105,
  	106,1,0,0,0,106,13,1,0,0,0,107,105,1,0,0,0,108,110,5,3,0,0,109,108,1,
  	0,0,0,109,110,1,0,0,0,110,111,1,0,0,0,111,112,5,43,0,0,112,113,5,42,0,
  	0,113,114,3,6,3,0,114,15,1,0,0,0,115,116,5,19,0,0,116,117,3,6,3,0,117,
  	17,1,0,0,0,118,124,3,20,10,0,119,124,3,22,11,0,120,124,3,24,12,0,121,
  	124,3,30,15,0,122,124,3,26,13,0,123,118,1,0,0,0,123,119,1,0,0,0,123,120,
  	1,0,0,0,123,121,1,0,0,0,123,122,1,0,0,0,124,19,1,0,0,0,125,126,5,2,0,
  	0,126,129,3,28,14,0,127,128,5,42,0,0,128,130,3,6,3,0,129,127,1,0,0,0,
  	129,130,1,0,0,0,130,131,1,0,0,0,131,132,5,21,0,0,132,133,3,34,17,0,133,
  	134,5,41,0,0,134,21,1,0,0,0,135,136,3,34,17,0,136,137,5,21,0,0,137,138,
  	3,34,17,0,138,139,5,41,0,0,139,23,1,0,0,0,140,142,5,8,0,0,141,143,3,34,
  	17,0,142,141,1,0,0,0,142,143,1,0,0,0,143,144,1,0,0,0,144,145,5,41,0,0,
  	145,25,1,0,0,0,146,147,3,34,17,0,147,148,5,41,0,0,148,27,1,0,0,0,149,
  	161,5,43,0,0,150,151,5,34,0,0,151,154,3,28,14,0,152,153,5,40,0,0,153,
  	155,3,28,14,0,154,152,1,0,0,0,155,156,1,0,0,0,156,154,1,0,0,0,156,157,
  	1,0,0,0,157,158,1,0,0,0,158,159,5,35,0,0,159,161,1,0,0,0,160,149,1,0,
  	0,0,160,150,1,0,0,0,161,29,1,0,0,0,162,163,5,5,0,0,163,164,3,34,17,0,
  	164,165,3,32,16,0,165,175,1,0,0,0,166,167,5,4,0,0,167,168,5,43,0,0,168,
  	169,5,7,0,0,169,170,3,34,17,0,170,171,3,32,16,0,171,175,1,0,0,0,172,173,
  	5,6,0,0,173,175,3,32,16,0,174,162,1,0,0,0,174,166,1,0,0,0,174,172,1,0,
  	0,0,175,31,1,0,0,0,176,180,5,36,0,0,177,179,3,18,9,0,178,177,1,0,0,0,
  	179,182,1,0,0,0,180,178,1,0,0,0,180,181,1,0,0,0,181,184,1,0,0,0,182,180,
  	1,0,0,0,183,185,3,34,17,0,184,183,1,0,0,0,184,185,1,0,0,0,185,186,1,0,
  	0,0,186,187,5,37,0,0,187,33,1,0,0,0,188,189,6,17,-1,0,189,190,5,34,0,
  	0,190,191,3,34,17,0,191,192,5,35,0,0,192,212,1,0,0,0,193,194,5,34,0,0,
  	194,197,3,34,17,0,195,196,5,40,0,0,196,198,3,34,17,0,197,195,1,0,0,0,
  	198,199,1,0,0,0,199,197,1,0,0,0,199,200,1,0,0,0,200,201,1,0,0,0,201,202,
  	5,35,0,0,202,212,1,0,0,0,203,204,5,43,0,0,204,206,5,34,0,0,205,207,3,
  	36,18,0,206,205,1,0,0,0,206,207,1,0,0,0,207,208,1,0,0,0,208,212,5,35,
  	0,0,209,212,3,38,19,0,210,212,5,43,0,0,211,188,1,0,0,0,211,193,1,0,0,
  	0,211,203,1,0,0,0,211,209,1,0,0,0,211,210,1,0,0,0,212,244,1,0,0,0,213,
  	214,10,11,0,0,214,215,5,33,0,0,215,243,3,34,17,12,216,217,10,10,0,0,217,
  	218,7,1,0,0,218,243,3,34,17,11,219,220,10,9,0,0,220,221,7,2,0,0,221,243,
  	3,34,17,10,222,223,10,8,0,0,223,224,7,3,0,0,224,243,3,34,17,9,225,226,
  	10,7,0,0,226,227,7,4,0,0,227,243,3,34,17,8,228,229,10,6,0,0,229,230,5,
  	17,0,0,230,243,3,34,17,7,231,232,10,5,0,0,232,233,5,18,0,0,233,243,3,
  	34,17,6,234,235,10,4,0,0,235,236,5,20,0,0,236,243,3,34,17,5,237,238,10,
  	12,0,0,238,239,5,38,0,0,239,240,3,34,17,0,240,241,5,39,0,0,241,243,1,
  	0,0,0,242,213,1,0,0,0,242,216,1,0,0,0,242,219,1,0,0,0,242,222,1,0,0,0,
  	242,225,1,0,0,0,242,228,1,0,0,0,242,231,1,0,0,0,242,234,1,0,0,0,242,237,
  	1,0,0,0,243,246,1,0,0,0,244,242,1,0,0,0,244,245,1,0,0,0,245,35,1,0,0,
  	0,246,244,1,0,0,0,247,252,3,34,17,0,248,249,5,40,0,0,249,251,3,34,17,
  	0,250,248,1,0,0,0,251,254,1,0,0,0,252,250,1,0,0,0,252,253,1,0,0,0,253,
  	37,1,0,0,0,254,252,1,0,0,0,255,256,7,5,0,0,256,39,1,0,0,0,24,43,51,68,
  	72,81,83,92,96,105,109,123,129,142,156,160,174,180,184,199,206,211,242,
  	244,252
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

RexParser::RexParser(TokenStream *input) : RexParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

RexParser::RexParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  RexParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *rexParserStaticData->atn, rexParserStaticData->decisionToDFA, rexParserStaticData->sharedContextCache, options);
}

RexParser::~RexParser() {
  delete _interpreter;
}

const atn::ATN& RexParser::getATN() const {
  return *rexParserStaticData->atn;
}

std::string RexParser::getGrammarFileName() const {
  return "Rex.g4";
}

const std::vector<std::string>& RexParser::getRuleNames() const {
  return rexParserStaticData->ruleNames;
}

const dfa::Vocabulary& RexParser::getVocabulary() const {
  return rexParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView RexParser::getSerializedATN() const {
  return rexParserStaticData->serializedATN;
}


//----------------- FileContext ------------------------------------------------------------------

RexParser::FileContext::FileContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RexParser::FileContext::EOF() {
  return getToken(RexParser::EOF, 0);
}

std::vector<RexParser::ItemContext *> RexParser::FileContext::item() {
  return getRuleContexts<RexParser::ItemContext>();
}

RexParser::ItemContext* RexParser::FileContext::item(size_t i) {
  return getRuleContext<RexParser::ItemContext>(i);
}


size_t RexParser::FileContext::getRuleIndex() const {
  return RexParser::RuleFile;
}

void RexParser::FileContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFile(this);
}

void RexParser::FileContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFile(this);
}


std::any RexParser::FileContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitFile(this);
  else
    return visitor->visitChildren(this);
}

RexParser::FileContext* RexParser::file() {
  FileContext *_localctx = _tracker.createInstance<FileContext>(_ctx, getState());
  enterRule(_localctx, 0, RexParser::RuleFile);
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
    match(RexParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ItemContext ------------------------------------------------------------------

RexParser::ItemContext::ItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

RexParser::FunctionDefContext* RexParser::ItemContext::functionDef() {
  return getRuleContext<RexParser::FunctionDefContext>(0);
}

RexParser::TypeDefContext* RexParser::ItemContext::typeDef() {
  return getRuleContext<RexParser::TypeDefContext>(0);
}

RexParser::StatementContext* RexParser::ItemContext::statement() {
  return getRuleContext<RexParser::StatementContext>(0);
}


size_t RexParser::ItemContext::getRuleIndex() const {
  return RexParser::RuleItem;
}

void RexParser::ItemContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterItem(this);
}

void RexParser::ItemContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitItem(this);
}


std::any RexParser::ItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitItem(this);
  else
    return visitor->visitChildren(this);
}

RexParser::ItemContext* RexParser::item() {
  ItemContext *_localctx = _tracker.createInstance<ItemContext>(_ctx, getState());
  enterRule(_localctx, 2, RexParser::RuleItem);

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
      case RexParser::FN: {
        enterOuterAlt(_localctx, 1);
        setState(48);
        functionDef();
        break;
      }

      case RexParser::TYPE: {
        enterOuterAlt(_localctx, 2);
        setState(49);
        typeDef();
        break;
      }

      case RexParser::LET:
      case RexParser::FOR:
      case RexParser::WHILE:
      case RexParser::LOOP:
      case RexParser::RETURN:
      case RexParser::TRUE:
      case RexParser::FALSE:
      case RexParser::LPAREN:
      case RexParser::ID:
      case RexParser::INT_LITERAL:
      case RexParser::REAL_LITERAL:
      case RexParser::STRING_LITERAL:
      case RexParser::CHAR_LITERAL: {
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

RexParser::TypeDefContext::TypeDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RexParser::TypeDefContext::TYPE() {
  return getToken(RexParser::TYPE, 0);
}

tree::TerminalNode* RexParser::TypeDefContext::ID() {
  return getToken(RexParser::ID, 0);
}

tree::TerminalNode* RexParser::TypeDefContext::ASSIGN() {
  return getToken(RexParser::ASSIGN, 0);
}

RexParser::TypeContext* RexParser::TypeDefContext::type() {
  return getRuleContext<RexParser::TypeContext>(0);
}

tree::TerminalNode* RexParser::TypeDefContext::SEMI() {
  return getToken(RexParser::SEMI, 0);
}


size_t RexParser::TypeDefContext::getRuleIndex() const {
  return RexParser::RuleTypeDef;
}

void RexParser::TypeDefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeDef(this);
}

void RexParser::TypeDefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeDef(this);
}


std::any RexParser::TypeDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitTypeDef(this);
  else
    return visitor->visitChildren(this);
}

RexParser::TypeDefContext* RexParser::typeDef() {
  TypeDefContext *_localctx = _tracker.createInstance<TypeDefContext>(_ctx, getState());
  enterRule(_localctx, 4, RexParser::RuleTypeDef);

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
    match(RexParser::TYPE);
    setState(54);
    match(RexParser::ID);
    setState(55);
    match(RexParser::ASSIGN);
    setState(56);
    type(0);
    setState(57);
    match(RexParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

RexParser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t RexParser::TypeContext::getRuleIndex() const {
  return RexParser::RuleType;
}

void RexParser::TypeContext::copyFrom(TypeContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ArrayTypeContext ------------------------------------------------------------------

RexParser::TypeContext* RexParser::ArrayTypeContext::type() {
  return getRuleContext<RexParser::TypeContext>(0);
}

tree::TerminalNode* RexParser::ArrayTypeContext::LBRACK() {
  return getToken(RexParser::LBRACK, 0);
}

tree::TerminalNode* RexParser::ArrayTypeContext::INT_LITERAL() {
  return getToken(RexParser::INT_LITERAL, 0);
}

tree::TerminalNode* RexParser::ArrayTypeContext::RBRACK() {
  return getToken(RexParser::RBRACK, 0);
}

RexParser::ArrayTypeContext::ArrayTypeContext(TypeContext *ctx) { copyFrom(ctx); }

void RexParser::ArrayTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArrayType(this);
}
void RexParser::ArrayTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArrayType(this);
}

std::any RexParser::ArrayTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitArrayType(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NamedTypeContext ------------------------------------------------------------------

tree::TerminalNode* RexParser::NamedTypeContext::ID() {
  return getToken(RexParser::ID, 0);
}

RexParser::NamedTypeContext::NamedTypeContext(TypeContext *ctx) { copyFrom(ctx); }

void RexParser::NamedTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNamedType(this);
}
void RexParser::NamedTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNamedType(this);
}

std::any RexParser::NamedTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitNamedType(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PrimeTypeContext ------------------------------------------------------------------

RexParser::PrimitiveTypeContext* RexParser::PrimeTypeContext::primitiveType() {
  return getRuleContext<RexParser::PrimitiveTypeContext>(0);
}

RexParser::PrimeTypeContext::PrimeTypeContext(TypeContext *ctx) { copyFrom(ctx); }

void RexParser::PrimeTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrimeType(this);
}
void RexParser::PrimeTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrimeType(this);
}

std::any RexParser::PrimeTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitPrimeType(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TupleTypeContext ------------------------------------------------------------------

tree::TerminalNode* RexParser::TupleTypeContext::LPAREN() {
  return getToken(RexParser::LPAREN, 0);
}

std::vector<RexParser::TypeContext *> RexParser::TupleTypeContext::type() {
  return getRuleContexts<RexParser::TypeContext>();
}

RexParser::TypeContext* RexParser::TupleTypeContext::type(size_t i) {
  return getRuleContext<RexParser::TypeContext>(i);
}

tree::TerminalNode* RexParser::TupleTypeContext::RPAREN() {
  return getToken(RexParser::RPAREN, 0);
}

std::vector<tree::TerminalNode *> RexParser::TupleTypeContext::COMMA() {
  return getTokens(RexParser::COMMA);
}

tree::TerminalNode* RexParser::TupleTypeContext::COMMA(size_t i) {
  return getToken(RexParser::COMMA, i);
}

RexParser::TupleTypeContext::TupleTypeContext(TypeContext *ctx) { copyFrom(ctx); }

void RexParser::TupleTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTupleType(this);
}
void RexParser::TupleTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTupleType(this);
}

std::any RexParser::TupleTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitTupleType(this);
  else
    return visitor->visitChildren(this);
}
//----------------- SliceTypeContext ------------------------------------------------------------------

RexParser::TypeContext* RexParser::SliceTypeContext::type() {
  return getRuleContext<RexParser::TypeContext>(0);
}

tree::TerminalNode* RexParser::SliceTypeContext::LBRACK() {
  return getToken(RexParser::LBRACK, 0);
}

tree::TerminalNode* RexParser::SliceTypeContext::RBRACK() {
  return getToken(RexParser::RBRACK, 0);
}

RexParser::SliceTypeContext::SliceTypeContext(TypeContext *ctx) { copyFrom(ctx); }

void RexParser::SliceTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSliceType(this);
}
void RexParser::SliceTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSliceType(this);
}

std::any RexParser::SliceTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitSliceType(this);
  else
    return visitor->visitChildren(this);
}

RexParser::TypeContext* RexParser::type() {
   return type(0);
}

RexParser::TypeContext* RexParser::type(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  RexParser::TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, parentState);
  RexParser::TypeContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 6;
  enterRecursionRule(_localctx, 6, RexParser::RuleType, precedence);

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
      case RexParser::INT:
      case RexParser::BOOL:
      case RexParser::CHAR:
      case RexParser::REAL:
      case RexParser::STRING: {
        _localctx = _tracker.createInstance<PrimeTypeContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(60);
        primitiveType();
        break;
      }

      case RexParser::ID: {
        _localctx = _tracker.createInstance<NamedTypeContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(61);
        match(RexParser::ID);
        break;
      }

      case RexParser::LPAREN: {
        _localctx = _tracker.createInstance<TupleTypeContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(62);
        match(RexParser::LPAREN);
        setState(63);
        type(0);
        setState(66); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(64);
          match(RexParser::COMMA);
          setState(65);
          type(0);
          setState(68); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (_la == RexParser::COMMA);
        setState(70);
        match(RexParser::RPAREN);
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
          match(RexParser::LBRACK);
          setState(76);
          match(RexParser::RBRACK);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<ArrayTypeContext>(_tracker.createInstance<TypeContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleType);
          setState(77);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(78);
          match(RexParser::LBRACK);
          setState(79);
          match(RexParser::INT_LITERAL);
          setState(80);
          match(RexParser::RBRACK);
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

RexParser::PrimitiveTypeContext::PrimitiveTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RexParser::PrimitiveTypeContext::INT() {
  return getToken(RexParser::INT, 0);
}

tree::TerminalNode* RexParser::PrimitiveTypeContext::BOOL() {
  return getToken(RexParser::BOOL, 0);
}

tree::TerminalNode* RexParser::PrimitiveTypeContext::CHAR() {
  return getToken(RexParser::CHAR, 0);
}

tree::TerminalNode* RexParser::PrimitiveTypeContext::REAL() {
  return getToken(RexParser::REAL, 0);
}

tree::TerminalNode* RexParser::PrimitiveTypeContext::STRING() {
  return getToken(RexParser::STRING, 0);
}


size_t RexParser::PrimitiveTypeContext::getRuleIndex() const {
  return RexParser::RulePrimitiveType;
}

void RexParser::PrimitiveTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrimitiveType(this);
}

void RexParser::PrimitiveTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrimitiveType(this);
}


std::any RexParser::PrimitiveTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitPrimitiveType(this);
  else
    return visitor->visitChildren(this);
}

RexParser::PrimitiveTypeContext* RexParser::primitiveType() {
  PrimitiveTypeContext *_localctx = _tracker.createInstance<PrimitiveTypeContext>(_ctx, getState());
  enterRule(_localctx, 8, RexParser::RulePrimitiveType);
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

RexParser::FunctionDefContext::FunctionDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RexParser::FunctionDefContext::FN() {
  return getToken(RexParser::FN, 0);
}

tree::TerminalNode* RexParser::FunctionDefContext::ID() {
  return getToken(RexParser::ID, 0);
}

tree::TerminalNode* RexParser::FunctionDefContext::LPAREN() {
  return getToken(RexParser::LPAREN, 0);
}

tree::TerminalNode* RexParser::FunctionDefContext::RPAREN() {
  return getToken(RexParser::RPAREN, 0);
}

RexParser::BlockContext* RexParser::FunctionDefContext::block() {
  return getRuleContext<RexParser::BlockContext>(0);
}

RexParser::ParamListContext* RexParser::FunctionDefContext::paramList() {
  return getRuleContext<RexParser::ParamListContext>(0);
}

RexParser::ReturnTypeContext* RexParser::FunctionDefContext::returnType() {
  return getRuleContext<RexParser::ReturnTypeContext>(0);
}


size_t RexParser::FunctionDefContext::getRuleIndex() const {
  return RexParser::RuleFunctionDef;
}

void RexParser::FunctionDefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionDef(this);
}

void RexParser::FunctionDefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionDef(this);
}


std::any RexParser::FunctionDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitFunctionDef(this);
  else
    return visitor->visitChildren(this);
}

RexParser::FunctionDefContext* RexParser::functionDef() {
  FunctionDefContext *_localctx = _tracker.createInstance<FunctionDefContext>(_ctx, getState());
  enterRule(_localctx, 10, RexParser::RuleFunctionDef);
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
    match(RexParser::FN);
    setState(89);
    match(RexParser::ID);
    setState(90);
    match(RexParser::LPAREN);
    setState(92);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == RexParser::MUT

    || _la == RexParser::ID) {
      setState(91);
      paramList();
    }
    setState(94);
    match(RexParser::RPAREN);
    setState(96);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == RexParser::ARROW) {
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

RexParser::ParamListContext::ParamListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<RexParser::ParamContext *> RexParser::ParamListContext::param() {
  return getRuleContexts<RexParser::ParamContext>();
}

RexParser::ParamContext* RexParser::ParamListContext::param(size_t i) {
  return getRuleContext<RexParser::ParamContext>(i);
}

std::vector<tree::TerminalNode *> RexParser::ParamListContext::COMMA() {
  return getTokens(RexParser::COMMA);
}

tree::TerminalNode* RexParser::ParamListContext::COMMA(size_t i) {
  return getToken(RexParser::COMMA, i);
}


size_t RexParser::ParamListContext::getRuleIndex() const {
  return RexParser::RuleParamList;
}

void RexParser::ParamListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParamList(this);
}

void RexParser::ParamListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParamList(this);
}


std::any RexParser::ParamListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitParamList(this);
  else
    return visitor->visitChildren(this);
}

RexParser::ParamListContext* RexParser::paramList() {
  ParamListContext *_localctx = _tracker.createInstance<ParamListContext>(_ctx, getState());
  enterRule(_localctx, 12, RexParser::RuleParamList);
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
    while (_la == RexParser::COMMA) {
      setState(101);
      match(RexParser::COMMA);
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

RexParser::ParamContext::ParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RexParser::ParamContext::ID() {
  return getToken(RexParser::ID, 0);
}

tree::TerminalNode* RexParser::ParamContext::COLON() {
  return getToken(RexParser::COLON, 0);
}

RexParser::TypeContext* RexParser::ParamContext::type() {
  return getRuleContext<RexParser::TypeContext>(0);
}

tree::TerminalNode* RexParser::ParamContext::MUT() {
  return getToken(RexParser::MUT, 0);
}


size_t RexParser::ParamContext::getRuleIndex() const {
  return RexParser::RuleParam;
}

void RexParser::ParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParam(this);
}

void RexParser::ParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParam(this);
}


std::any RexParser::ParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitParam(this);
  else
    return visitor->visitChildren(this);
}

RexParser::ParamContext* RexParser::param() {
  ParamContext *_localctx = _tracker.createInstance<ParamContext>(_ctx, getState());
  enterRule(_localctx, 14, RexParser::RuleParam);
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
    if (_la == RexParser::MUT) {
      setState(108);
      match(RexParser::MUT);
    }
    setState(111);
    match(RexParser::ID);
    setState(112);
    match(RexParser::COLON);
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

RexParser::ReturnTypeContext::ReturnTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RexParser::ReturnTypeContext::ARROW() {
  return getToken(RexParser::ARROW, 0);
}

RexParser::TypeContext* RexParser::ReturnTypeContext::type() {
  return getRuleContext<RexParser::TypeContext>(0);
}


size_t RexParser::ReturnTypeContext::getRuleIndex() const {
  return RexParser::RuleReturnType;
}

void RexParser::ReturnTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReturnType(this);
}

void RexParser::ReturnTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReturnType(this);
}


std::any RexParser::ReturnTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitReturnType(this);
  else
    return visitor->visitChildren(this);
}

RexParser::ReturnTypeContext* RexParser::returnType() {
  ReturnTypeContext *_localctx = _tracker.createInstance<ReturnTypeContext>(_ctx, getState());
  enterRule(_localctx, 16, RexParser::RuleReturnType);

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
    match(RexParser::ARROW);
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

RexParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

RexParser::LetStmtContext* RexParser::StatementContext::letStmt() {
  return getRuleContext<RexParser::LetStmtContext>(0);
}

RexParser::AssignStmtContext* RexParser::StatementContext::assignStmt() {
  return getRuleContext<RexParser::AssignStmtContext>(0);
}

RexParser::ReturnStmtContext* RexParser::StatementContext::returnStmt() {
  return getRuleContext<RexParser::ReturnStmtContext>(0);
}

RexParser::LoopStmtContext* RexParser::StatementContext::loopStmt() {
  return getRuleContext<RexParser::LoopStmtContext>(0);
}

RexParser::ExprStmtContext* RexParser::StatementContext::exprStmt() {
  return getRuleContext<RexParser::ExprStmtContext>(0);
}


size_t RexParser::StatementContext::getRuleIndex() const {
  return RexParser::RuleStatement;
}

void RexParser::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void RexParser::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}


std::any RexParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

RexParser::StatementContext* RexParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 18, RexParser::RuleStatement);

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

RexParser::LetStmtContext::LetStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RexParser::LetStmtContext::LET() {
  return getToken(RexParser::LET, 0);
}

RexParser::PatternContext* RexParser::LetStmtContext::pattern() {
  return getRuleContext<RexParser::PatternContext>(0);
}

tree::TerminalNode* RexParser::LetStmtContext::ASSIGN() {
  return getToken(RexParser::ASSIGN, 0);
}

RexParser::ExprContext* RexParser::LetStmtContext::expr() {
  return getRuleContext<RexParser::ExprContext>(0);
}

tree::TerminalNode* RexParser::LetStmtContext::SEMI() {
  return getToken(RexParser::SEMI, 0);
}

tree::TerminalNode* RexParser::LetStmtContext::COLON() {
  return getToken(RexParser::COLON, 0);
}

RexParser::TypeContext* RexParser::LetStmtContext::type() {
  return getRuleContext<RexParser::TypeContext>(0);
}


size_t RexParser::LetStmtContext::getRuleIndex() const {
  return RexParser::RuleLetStmt;
}

void RexParser::LetStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLetStmt(this);
}

void RexParser::LetStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLetStmt(this);
}


std::any RexParser::LetStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitLetStmt(this);
  else
    return visitor->visitChildren(this);
}

RexParser::LetStmtContext* RexParser::letStmt() {
  LetStmtContext *_localctx = _tracker.createInstance<LetStmtContext>(_ctx, getState());
  enterRule(_localctx, 20, RexParser::RuleLetStmt);
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
    setState(125);
    match(RexParser::LET);
    setState(126);
    pattern();
    setState(129);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == RexParser::COLON) {
      setState(127);
      match(RexParser::COLON);
      setState(128);
      type(0);
    }
    setState(131);
    match(RexParser::ASSIGN);
    setState(132);
    expr(0);
    setState(133);
    match(RexParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignStmtContext ------------------------------------------------------------------

RexParser::AssignStmtContext::AssignStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<RexParser::ExprContext *> RexParser::AssignStmtContext::expr() {
  return getRuleContexts<RexParser::ExprContext>();
}

RexParser::ExprContext* RexParser::AssignStmtContext::expr(size_t i) {
  return getRuleContext<RexParser::ExprContext>(i);
}

tree::TerminalNode* RexParser::AssignStmtContext::ASSIGN() {
  return getToken(RexParser::ASSIGN, 0);
}

tree::TerminalNode* RexParser::AssignStmtContext::SEMI() {
  return getToken(RexParser::SEMI, 0);
}


size_t RexParser::AssignStmtContext::getRuleIndex() const {
  return RexParser::RuleAssignStmt;
}

void RexParser::AssignStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignStmt(this);
}

void RexParser::AssignStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignStmt(this);
}


std::any RexParser::AssignStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitAssignStmt(this);
  else
    return visitor->visitChildren(this);
}

RexParser::AssignStmtContext* RexParser::assignStmt() {
  AssignStmtContext *_localctx = _tracker.createInstance<AssignStmtContext>(_ctx, getState());
  enterRule(_localctx, 22, RexParser::RuleAssignStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(135);
    expr(0);
    setState(136);
    match(RexParser::ASSIGN);
    setState(137);
    expr(0);
    setState(138);
    match(RexParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStmtContext ------------------------------------------------------------------

RexParser::ReturnStmtContext::ReturnStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RexParser::ReturnStmtContext::RETURN() {
  return getToken(RexParser::RETURN, 0);
}

tree::TerminalNode* RexParser::ReturnStmtContext::SEMI() {
  return getToken(RexParser::SEMI, 0);
}

RexParser::ExprContext* RexParser::ReturnStmtContext::expr() {
  return getRuleContext<RexParser::ExprContext>(0);
}


size_t RexParser::ReturnStmtContext::getRuleIndex() const {
  return RexParser::RuleReturnStmt;
}

void RexParser::ReturnStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReturnStmt(this);
}

void RexParser::ReturnStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReturnStmt(this);
}


std::any RexParser::ReturnStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitReturnStmt(this);
  else
    return visitor->visitChildren(this);
}

RexParser::ReturnStmtContext* RexParser::returnStmt() {
  ReturnStmtContext *_localctx = _tracker.createInstance<ReturnStmtContext>(_ctx, getState());
  enterRule(_localctx, 24, RexParser::RuleReturnStmt);
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
    setState(140);
    match(RexParser::RETURN);
    setState(142);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 272696063655936) != 0)) {
      setState(141);
      expr(0);
    }
    setState(144);
    match(RexParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprStmtContext ------------------------------------------------------------------

RexParser::ExprStmtContext::ExprStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

RexParser::ExprContext* RexParser::ExprStmtContext::expr() {
  return getRuleContext<RexParser::ExprContext>(0);
}

tree::TerminalNode* RexParser::ExprStmtContext::SEMI() {
  return getToken(RexParser::SEMI, 0);
}


size_t RexParser::ExprStmtContext::getRuleIndex() const {
  return RexParser::RuleExprStmt;
}

void RexParser::ExprStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExprStmt(this);
}

void RexParser::ExprStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExprStmt(this);
}


std::any RexParser::ExprStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitExprStmt(this);
  else
    return visitor->visitChildren(this);
}

RexParser::ExprStmtContext* RexParser::exprStmt() {
  ExprStmtContext *_localctx = _tracker.createInstance<ExprStmtContext>(_ctx, getState());
  enterRule(_localctx, 26, RexParser::RuleExprStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(146);
    expr(0);
    setState(147);
    match(RexParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PatternContext ------------------------------------------------------------------

RexParser::PatternContext::PatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RexParser::PatternContext::ID() {
  return getToken(RexParser::ID, 0);
}

tree::TerminalNode* RexParser::PatternContext::LPAREN() {
  return getToken(RexParser::LPAREN, 0);
}

std::vector<RexParser::PatternContext *> RexParser::PatternContext::pattern() {
  return getRuleContexts<RexParser::PatternContext>();
}

RexParser::PatternContext* RexParser::PatternContext::pattern(size_t i) {
  return getRuleContext<RexParser::PatternContext>(i);
}

tree::TerminalNode* RexParser::PatternContext::RPAREN() {
  return getToken(RexParser::RPAREN, 0);
}

std::vector<tree::TerminalNode *> RexParser::PatternContext::COMMA() {
  return getTokens(RexParser::COMMA);
}

tree::TerminalNode* RexParser::PatternContext::COMMA(size_t i) {
  return getToken(RexParser::COMMA, i);
}


size_t RexParser::PatternContext::getRuleIndex() const {
  return RexParser::RulePattern;
}

void RexParser::PatternContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPattern(this);
}

void RexParser::PatternContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPattern(this);
}


std::any RexParser::PatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitPattern(this);
  else
    return visitor->visitChildren(this);
}

RexParser::PatternContext* RexParser::pattern() {
  PatternContext *_localctx = _tracker.createInstance<PatternContext>(_ctx, getState());
  enterRule(_localctx, 28, RexParser::RulePattern);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(160);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case RexParser::ID: {
        enterOuterAlt(_localctx, 1);
        setState(149);
        match(RexParser::ID);
        break;
      }

      case RexParser::LPAREN: {
        enterOuterAlt(_localctx, 2);
        setState(150);
        match(RexParser::LPAREN);
        setState(151);
        pattern();
        setState(154); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(152);
          match(RexParser::COMMA);
          setState(153);
          pattern();
          setState(156); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (_la == RexParser::COMMA);
        setState(158);
        match(RexParser::RPAREN);
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

RexParser::LoopStmtContext::LoopStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RexParser::LoopStmtContext::WHILE() {
  return getToken(RexParser::WHILE, 0);
}

RexParser::ExprContext* RexParser::LoopStmtContext::expr() {
  return getRuleContext<RexParser::ExprContext>(0);
}

RexParser::BlockContext* RexParser::LoopStmtContext::block() {
  return getRuleContext<RexParser::BlockContext>(0);
}

tree::TerminalNode* RexParser::LoopStmtContext::FOR() {
  return getToken(RexParser::FOR, 0);
}

tree::TerminalNode* RexParser::LoopStmtContext::ID() {
  return getToken(RexParser::ID, 0);
}

tree::TerminalNode* RexParser::LoopStmtContext::IN() {
  return getToken(RexParser::IN, 0);
}

tree::TerminalNode* RexParser::LoopStmtContext::LOOP() {
  return getToken(RexParser::LOOP, 0);
}


size_t RexParser::LoopStmtContext::getRuleIndex() const {
  return RexParser::RuleLoopStmt;
}

void RexParser::LoopStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLoopStmt(this);
}

void RexParser::LoopStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLoopStmt(this);
}


std::any RexParser::LoopStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitLoopStmt(this);
  else
    return visitor->visitChildren(this);
}

RexParser::LoopStmtContext* RexParser::loopStmt() {
  LoopStmtContext *_localctx = _tracker.createInstance<LoopStmtContext>(_ctx, getState());
  enterRule(_localctx, 30, RexParser::RuleLoopStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(174);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case RexParser::WHILE: {
        enterOuterAlt(_localctx, 1);
        setState(162);
        match(RexParser::WHILE);
        setState(163);
        expr(0);
        setState(164);
        block();
        break;
      }

      case RexParser::FOR: {
        enterOuterAlt(_localctx, 2);
        setState(166);
        match(RexParser::FOR);
        setState(167);
        match(RexParser::ID);
        setState(168);
        match(RexParser::IN);
        setState(169);
        expr(0);
        setState(170);
        block();
        break;
      }

      case RexParser::LOOP: {
        enterOuterAlt(_localctx, 3);
        setState(172);
        match(RexParser::LOOP);
        setState(173);
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

RexParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RexParser::BlockContext::LBRACE() {
  return getToken(RexParser::LBRACE, 0);
}

tree::TerminalNode* RexParser::BlockContext::RBRACE() {
  return getToken(RexParser::RBRACE, 0);
}

std::vector<RexParser::StatementContext *> RexParser::BlockContext::statement() {
  return getRuleContexts<RexParser::StatementContext>();
}

RexParser::StatementContext* RexParser::BlockContext::statement(size_t i) {
  return getRuleContext<RexParser::StatementContext>(i);
}

RexParser::ExprContext* RexParser::BlockContext::expr() {
  return getRuleContext<RexParser::ExprContext>(0);
}


size_t RexParser::BlockContext::getRuleIndex() const {
  return RexParser::RuleBlock;
}

void RexParser::BlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlock(this);
}

void RexParser::BlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlock(this);
}


std::any RexParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

RexParser::BlockContext* RexParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 32, RexParser::RuleBlock);
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
    setState(176);
    match(RexParser::LBRACE);
    setState(180);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(177);
        statement(); 
      }
      setState(182);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx);
    }
    setState(184);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 272696063655936) != 0)) {
      setState(183);
      expr(0);
    }
    setState(186);
    match(RexParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

RexParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t RexParser::ExprContext::getRuleIndex() const {
  return RexParser::RuleExpr;
}

void RexParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- MulExprContext ------------------------------------------------------------------

std::vector<RexParser::ExprContext *> RexParser::MulExprContext::expr() {
  return getRuleContexts<RexParser::ExprContext>();
}

RexParser::ExprContext* RexParser::MulExprContext::expr(size_t i) {
  return getRuleContext<RexParser::ExprContext>(i);
}

tree::TerminalNode* RexParser::MulExprContext::STAR() {
  return getToken(RexParser::STAR, 0);
}

tree::TerminalNode* RexParser::MulExprContext::DIV() {
  return getToken(RexParser::DIV, 0);
}

tree::TerminalNode* RexParser::MulExprContext::MOD() {
  return getToken(RexParser::MOD, 0);
}

RexParser::MulExprContext::MulExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::MulExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMulExpr(this);
}
void RexParser::MulExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMulExpr(this);
}

std::any RexParser::MulExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitMulExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AndExprContext ------------------------------------------------------------------

std::vector<RexParser::ExprContext *> RexParser::AndExprContext::expr() {
  return getRuleContexts<RexParser::ExprContext>();
}

RexParser::ExprContext* RexParser::AndExprContext::expr(size_t i) {
  return getRuleContext<RexParser::ExprContext>(i);
}

tree::TerminalNode* RexParser::AndExprContext::AND() {
  return getToken(RexParser::AND, 0);
}

RexParser::AndExprContext::AndExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::AndExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAndExpr(this);
}
void RexParser::AndExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAndExpr(this);
}

std::any RexParser::AndExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitAndExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IdExprContext ------------------------------------------------------------------

tree::TerminalNode* RexParser::IdExprContext::ID() {
  return getToken(RexParser::ID, 0);
}

RexParser::IdExprContext::IdExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::IdExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIdExpr(this);
}
void RexParser::IdExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIdExpr(this);
}

std::any RexParser::IdExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitIdExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AddExprContext ------------------------------------------------------------------

std::vector<RexParser::ExprContext *> RexParser::AddExprContext::expr() {
  return getRuleContexts<RexParser::ExprContext>();
}

RexParser::ExprContext* RexParser::AddExprContext::expr(size_t i) {
  return getRuleContext<RexParser::ExprContext>(i);
}

tree::TerminalNode* RexParser::AddExprContext::PLUS() {
  return getToken(RexParser::PLUS, 0);
}

tree::TerminalNode* RexParser::AddExprContext::MINUS() {
  return getToken(RexParser::MINUS, 0);
}

RexParser::AddExprContext::AddExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::AddExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAddExpr(this);
}
void RexParser::AddExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAddExpr(this);
}

std::any RexParser::AddExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitAddExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RangeExprContext ------------------------------------------------------------------

std::vector<RexParser::ExprContext *> RexParser::RangeExprContext::expr() {
  return getRuleContexts<RexParser::ExprContext>();
}

RexParser::ExprContext* RexParser::RangeExprContext::expr(size_t i) {
  return getRuleContext<RexParser::ExprContext>(i);
}

tree::TerminalNode* RexParser::RangeExprContext::RANGE() {
  return getToken(RexParser::RANGE, 0);
}

RexParser::RangeExprContext::RangeExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::RangeExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRangeExpr(this);
}
void RexParser::RangeExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRangeExpr(this);
}

std::any RexParser::RangeExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitRangeExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TupleExprContext ------------------------------------------------------------------

tree::TerminalNode* RexParser::TupleExprContext::LPAREN() {
  return getToken(RexParser::LPAREN, 0);
}

std::vector<RexParser::ExprContext *> RexParser::TupleExprContext::expr() {
  return getRuleContexts<RexParser::ExprContext>();
}

RexParser::ExprContext* RexParser::TupleExprContext::expr(size_t i) {
  return getRuleContext<RexParser::ExprContext>(i);
}

tree::TerminalNode* RexParser::TupleExprContext::RPAREN() {
  return getToken(RexParser::RPAREN, 0);
}

std::vector<tree::TerminalNode *> RexParser::TupleExprContext::COMMA() {
  return getTokens(RexParser::COMMA);
}

tree::TerminalNode* RexParser::TupleExprContext::COMMA(size_t i) {
  return getToken(RexParser::COMMA, i);
}

RexParser::TupleExprContext::TupleExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::TupleExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTupleExpr(this);
}
void RexParser::TupleExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTupleExpr(this);
}

std::any RexParser::TupleExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitTupleExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OrExprContext ------------------------------------------------------------------

std::vector<RexParser::ExprContext *> RexParser::OrExprContext::expr() {
  return getRuleContexts<RexParser::ExprContext>();
}

RexParser::ExprContext* RexParser::OrExprContext::expr(size_t i) {
  return getRuleContext<RexParser::ExprContext>(i);
}

tree::TerminalNode* RexParser::OrExprContext::OR() {
  return getToken(RexParser::OR, 0);
}

RexParser::OrExprContext::OrExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::OrExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOrExpr(this);
}
void RexParser::OrExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOrExpr(this);
}

std::any RexParser::OrExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitOrExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IndexExprContext ------------------------------------------------------------------

std::vector<RexParser::ExprContext *> RexParser::IndexExprContext::expr() {
  return getRuleContexts<RexParser::ExprContext>();
}

RexParser::ExprContext* RexParser::IndexExprContext::expr(size_t i) {
  return getRuleContext<RexParser::ExprContext>(i);
}

tree::TerminalNode* RexParser::IndexExprContext::LBRACK() {
  return getToken(RexParser::LBRACK, 0);
}

tree::TerminalNode* RexParser::IndexExprContext::RBRACK() {
  return getToken(RexParser::RBRACK, 0);
}

RexParser::IndexExprContext::IndexExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::IndexExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIndexExpr(this);
}
void RexParser::IndexExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIndexExpr(this);
}

std::any RexParser::IndexExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitIndexExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- EqualityExprContext ------------------------------------------------------------------

std::vector<RexParser::ExprContext *> RexParser::EqualityExprContext::expr() {
  return getRuleContexts<RexParser::ExprContext>();
}

RexParser::ExprContext* RexParser::EqualityExprContext::expr(size_t i) {
  return getRuleContext<RexParser::ExprContext>(i);
}

tree::TerminalNode* RexParser::EqualityExprContext::EQ() {
  return getToken(RexParser::EQ, 0);
}

tree::TerminalNode* RexParser::EqualityExprContext::NEQ() {
  return getToken(RexParser::NEQ, 0);
}

RexParser::EqualityExprContext::EqualityExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::EqualityExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEqualityExpr(this);
}
void RexParser::EqualityExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEqualityExpr(this);
}

std::any RexParser::EqualityExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitEqualityExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CompareExprContext ------------------------------------------------------------------

std::vector<RexParser::ExprContext *> RexParser::CompareExprContext::expr() {
  return getRuleContexts<RexParser::ExprContext>();
}

RexParser::ExprContext* RexParser::CompareExprContext::expr(size_t i) {
  return getRuleContext<RexParser::ExprContext>(i);
}

tree::TerminalNode* RexParser::CompareExprContext::LT() {
  return getToken(RexParser::LT, 0);
}

tree::TerminalNode* RexParser::CompareExprContext::GT() {
  return getToken(RexParser::GT, 0);
}

tree::TerminalNode* RexParser::CompareExprContext::LTE() {
  return getToken(RexParser::LTE, 0);
}

tree::TerminalNode* RexParser::CompareExprContext::GTE() {
  return getToken(RexParser::GTE, 0);
}

RexParser::CompareExprContext::CompareExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::CompareExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCompareExpr(this);
}
void RexParser::CompareExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCompareExpr(this);
}

std::any RexParser::CompareExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitCompareExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PipeExprContext ------------------------------------------------------------------

std::vector<RexParser::ExprContext *> RexParser::PipeExprContext::expr() {
  return getRuleContexts<RexParser::ExprContext>();
}

RexParser::ExprContext* RexParser::PipeExprContext::expr(size_t i) {
  return getRuleContext<RexParser::ExprContext>(i);
}

tree::TerminalNode* RexParser::PipeExprContext::PIPE() {
  return getToken(RexParser::PIPE, 0);
}

RexParser::PipeExprContext::PipeExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::PipeExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPipeExpr(this);
}
void RexParser::PipeExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPipeExpr(this);
}

std::any RexParser::PipeExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitPipeExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LiteralExprContext ------------------------------------------------------------------

RexParser::LiteralContext* RexParser::LiteralExprContext::literal() {
  return getRuleContext<RexParser::LiteralContext>(0);
}

RexParser::LiteralExprContext::LiteralExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::LiteralExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLiteralExpr(this);
}
void RexParser::LiteralExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLiteralExpr(this);
}

std::any RexParser::LiteralExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitLiteralExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CallExprContext ------------------------------------------------------------------

tree::TerminalNode* RexParser::CallExprContext::ID() {
  return getToken(RexParser::ID, 0);
}

tree::TerminalNode* RexParser::CallExprContext::LPAREN() {
  return getToken(RexParser::LPAREN, 0);
}

tree::TerminalNode* RexParser::CallExprContext::RPAREN() {
  return getToken(RexParser::RPAREN, 0);
}

RexParser::ArgListContext* RexParser::CallExprContext::argList() {
  return getRuleContext<RexParser::ArgListContext>(0);
}

RexParser::CallExprContext::CallExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::CallExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCallExpr(this);
}
void RexParser::CallExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCallExpr(this);
}

std::any RexParser::CallExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitCallExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParenExprContext ------------------------------------------------------------------

tree::TerminalNode* RexParser::ParenExprContext::LPAREN() {
  return getToken(RexParser::LPAREN, 0);
}

RexParser::ExprContext* RexParser::ParenExprContext::expr() {
  return getRuleContext<RexParser::ExprContext>(0);
}

tree::TerminalNode* RexParser::ParenExprContext::RPAREN() {
  return getToken(RexParser::RPAREN, 0);
}

RexParser::ParenExprContext::ParenExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RexParser::ParenExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParenExpr(this);
}
void RexParser::ParenExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParenExpr(this);
}

std::any RexParser::ParenExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitParenExpr(this);
  else
    return visitor->visitChildren(this);
}

RexParser::ExprContext* RexParser::expr() {
   return expr(0);
}

RexParser::ExprContext* RexParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  RexParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  RexParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 34;
  enterRecursionRule(_localctx, 34, RexParser::RuleExpr, precedence);

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
    setState(211);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<ParenExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(189);
      match(RexParser::LPAREN);
      setState(190);
      expr(0);
      setState(191);
      match(RexParser::RPAREN);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<TupleExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(193);
      match(RexParser::LPAREN);
      setState(194);
      expr(0);
      setState(197); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(195);
        match(RexParser::COMMA);
        setState(196);
        expr(0);
        setState(199); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == RexParser::COMMA);
      setState(201);
      match(RexParser::RPAREN);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<CallExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(203);
      match(RexParser::ID);
      setState(204);
      match(RexParser::LPAREN);
      setState(206);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 272696063655936) != 0)) {
        setState(205);
        argList();
      }
      setState(208);
      match(RexParser::RPAREN);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<LiteralExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(209);
      literal();
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<IdExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(210);
      match(RexParser::ID);
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(244);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(242);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<RangeExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(213);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(214);
          match(RexParser::RANGE);
          setState(215);
          expr(12);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<MulExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(216);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(217);
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
          setState(218);
          expr(11);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<AddExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(219);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(220);
          antlrcpp::downCast<AddExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == RexParser::PLUS

          || _la == RexParser::MINUS)) {
            antlrcpp::downCast<AddExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(221);
          expr(10);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<CompareExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(222);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(223);
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
          setState(224);
          expr(9);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<EqualityExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(225);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(226);
          antlrcpp::downCast<EqualityExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == RexParser::EQ

          || _la == RexParser::NEQ)) {
            antlrcpp::downCast<EqualityExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(227);
          expr(8);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<AndExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(228);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(229);
          antlrcpp::downCast<AndExprContext *>(_localctx)->op = match(RexParser::AND);
          setState(230);
          expr(7);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<OrExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(231);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(232);
          antlrcpp::downCast<OrExprContext *>(_localctx)->op = match(RexParser::OR);
          setState(233);
          expr(6);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<PipeExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(234);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(235);
          match(RexParser::PIPE);
          setState(236);
          expr(5);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<IndexExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(237);

          if (!(precpred(_ctx, 12))) throw FailedPredicateException(this, "precpred(_ctx, 12)");
          setState(238);
          match(RexParser::LBRACK);
          setState(239);
          expr(0);
          setState(240);
          match(RexParser::RBRACK);
          break;
        }

        default:
          break;
        } 
      }
      setState(246);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
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

RexParser::ArgListContext::ArgListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<RexParser::ExprContext *> RexParser::ArgListContext::expr() {
  return getRuleContexts<RexParser::ExprContext>();
}

RexParser::ExprContext* RexParser::ArgListContext::expr(size_t i) {
  return getRuleContext<RexParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> RexParser::ArgListContext::COMMA() {
  return getTokens(RexParser::COMMA);
}

tree::TerminalNode* RexParser::ArgListContext::COMMA(size_t i) {
  return getToken(RexParser::COMMA, i);
}


size_t RexParser::ArgListContext::getRuleIndex() const {
  return RexParser::RuleArgList;
}

void RexParser::ArgListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArgList(this);
}

void RexParser::ArgListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArgList(this);
}


std::any RexParser::ArgListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitArgList(this);
  else
    return visitor->visitChildren(this);
}

RexParser::ArgListContext* RexParser::argList() {
  ArgListContext *_localctx = _tracker.createInstance<ArgListContext>(_ctx, getState());
  enterRule(_localctx, 36, RexParser::RuleArgList);
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
    setState(247);
    expr(0);
    setState(252);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == RexParser::COMMA) {
      setState(248);
      match(RexParser::COMMA);
      setState(249);
      expr(0);
      setState(254);
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

RexParser::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RexParser::LiteralContext::INT_LITERAL() {
  return getToken(RexParser::INT_LITERAL, 0);
}

tree::TerminalNode* RexParser::LiteralContext::REAL_LITERAL() {
  return getToken(RexParser::REAL_LITERAL, 0);
}

tree::TerminalNode* RexParser::LiteralContext::STRING_LITERAL() {
  return getToken(RexParser::STRING_LITERAL, 0);
}

tree::TerminalNode* RexParser::LiteralContext::CHAR_LITERAL() {
  return getToken(RexParser::CHAR_LITERAL, 0);
}

tree::TerminalNode* RexParser::LiteralContext::TRUE() {
  return getToken(RexParser::TRUE, 0);
}

tree::TerminalNode* RexParser::LiteralContext::FALSE() {
  return getToken(RexParser::FALSE, 0);
}


size_t RexParser::LiteralContext::getRuleIndex() const {
  return RexParser::RuleLiteral;
}

void RexParser::LiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLiteral(this);
}

void RexParser::LiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLiteral(this);
}


std::any RexParser::LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RexVisitor*>(visitor))
    return parserVisitor->visitLiteral(this);
  else
    return visitor->visitChildren(this);
}

RexParser::LiteralContext* RexParser::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 38, RexParser::RuleLiteral);
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
    setState(255);
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

bool RexParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 3: return typeSempred(antlrcpp::downCast<TypeContext *>(context), predicateIndex);
    case 17: return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool RexParser::typeSempred(TypeContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 3);
    case 1: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

bool RexParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
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

void RexParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  rexParserInitialize();
#else
  ::antlr4::internal::call_once(rexParserOnceFlag, rexParserInitialize);
#endif
}
