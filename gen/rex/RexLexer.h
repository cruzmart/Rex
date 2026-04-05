
// Generated from /home/unknown/projects/Rex/grammar/Rex.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"


namespace rex {


class  RexLexer : public antlr4::Lexer {
public:
  enum {
    IF = 1, ELIFX = 2, ELSE = 3, FN = 4, LET = 5, MUT = 6, FOR = 7, WHILE = 8, 
    LOOP = 9, IN = 10, RETURN = 11, TYPE = 12, PRINT = 13, INT = 14, BOOL = 15, 
    CHAR = 16, REAL = 17, STRING = 18, TRUE = 19, FALSE = 20, AND = 21, 
    OR = 22, ARROW = 23, PIPE = 24, ASSIGN = 25, EQ = 26, NEQ = 27, LT = 28, 
    GT = 29, LTE = 30, GTE = 31, PLUS = 32, MINUS = 33, STAR = 34, DIV = 35, 
    MOD = 36, RANGE = 37, LPAREN = 38, RPAREN = 39, LBRACE = 40, RBRACE = 41, 
    LBRACK = 42, RBRACK = 43, COMMA = 44, SEMI = 45, COLON = 46, BREAK = 47, 
    ID = 48, INT_LITERAL = 49, REAL_LITERAL = 50, STRING_LITERAL = 51, CHAR_LITERAL = 52, 
    WS = 53, LINE_COMMENT = 54, BLOCK_COMMENT = 55
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
