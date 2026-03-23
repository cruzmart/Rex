
// Generated from /home/unknown/projects/Rex/grammar/Rex.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"


namespace rex {


class  RexLexer : public antlr4::Lexer {
public:
  enum {
    IF = 1, ELIFX = 2, ELSE = 3, FN = 4, LET = 5, MUT = 6, FOR = 7, WHILE = 8, 
    LOOP = 9, IN = 10, RETURN = 11, TYPE = 12, INT = 13, BOOL = 14, CHAR = 15, 
    REAL = 16, STRING = 17, TRUE = 18, FALSE = 19, AND = 20, OR = 21, ARROW = 22, 
    PIPE = 23, ASSIGN = 24, EQ = 25, NEQ = 26, LT = 27, GT = 28, LTE = 29, 
    GTE = 30, PLUS = 31, MINUS = 32, STAR = 33, DIV = 34, MOD = 35, RANGE = 36, 
    LPAREN = 37, RPAREN = 38, LBRACE = 39, RBRACE = 40, LBRACK = 41, RBRACK = 42, 
    COMMA = 43, SEMI = 44, COLON = 45, BREAK = 46, ID = 47, INT_LITERAL = 48, 
    REAL_LITERAL = 49, STRING_LITERAL = 50, CHAR_LITERAL = 51, WS = 52, 
    LINE_COMMENT = 53, BLOCK_COMMENT = 54
  };

  explicit RexLexer(antlr4::CharStream *input);

  ~RexLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

}  // namespace rex
