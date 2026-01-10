
// Generated from /home/unknown/projects/Rex/grammar/Rex.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"


namespace rex {


class  RexLexer : public antlr4::Lexer {
public:
  enum {
    FN = 1, LET = 2, MUT = 3, FOR = 4, WHILE = 5, LOOP = 6, IN = 7, RETURN = 8, 
    TYPE = 9, INT = 10, BOOL = 11, CHAR = 12, REAL = 13, STRING = 14, TRUE = 15, 
    FALSE = 16, AND = 17, OR = 18, ARROW = 19, PIPE = 20, ASSIGN = 21, EQ = 22, 
    NEQ = 23, LT = 24, GT = 25, LTE = 26, GTE = 27, PLUS = 28, MINUS = 29, 
    STAR = 30, DIV = 31, MOD = 32, RANGE = 33, LPAREN = 34, RPAREN = 35, 
    LBRACE = 36, RBRACE = 37, LBRACK = 38, RBRACK = 39, COMMA = 40, SEMI = 41, 
    COLON = 42, ID = 43, INT_LITERAL = 44, REAL_LITERAL = 45, STRING_LITERAL = 46, 
    CHAR_LITERAL = 47, WS = 48, LINE_COMMENT = 49, BLOCK_COMMENT = 50
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
