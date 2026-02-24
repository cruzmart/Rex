grammar Rex;


// -------------------------------------------------
// Entry point
// -------------------------------------------------


// -------------------------------------------------
// ------------------ Parser Rules -----------------
// -------------------------------------------------

file
    : item* EOF
    ;

item
    : functionDef
    | typeDef
    | statement
    ;

// -------------------------------------------------
// Type system
// -------------------------------------------------

typeDef
    : TYPE ID '=' type ';'
    ;

type
    : primitiveType                    #PrimeType
    | ID                               #NamedType
    | type '[' ']'                     #SliceType
    | type '[' INT_LITERAL ']'         #ArrayType
    | '(' type (',' type)+ ')'         #TupleType
    ;

primitiveType
    : INT
    | BOOL
    | CHAR
    | REAL
    | STRING
    ;

// -------------------------------------------------
// Functions
// -------------------------------------------------

functionDef
    : FN ID '(' paramList? ')' returnType? block
    ;

paramList
    : param (',' param)*
    ;

param
    : MUT? ID ':' type
    ;

returnType
    : ARROW type
    ;

// -------------------------------------------------
// Statements
// -------------------------------------------------

statement
    : letStmt
    | assignStmt
    | returnStmt
    | ifStmt          
    | loopStmt
    | exprStmt
    ;

letStmt
    : LET pattern (':' type)? '=' expr ';'
    ;

assignStmt
    : expr '=' expr ';'
    ;

returnStmt
    : RETURN expr? ';'
    ;

exprStmt
    : expr ';'
    ;
ifStmt
    : IF  expr block elifxChain? elseBlock?  
    ;

elifxChain
    : (ELIFX expr block)+                   
    ;

elseBlock
    : ELSE block                            
    ;

// -------------------------------------------------
// Patterns (for destructuring)
// -------------------------------------------------

pattern
    : ID
    | '(' pattern (',' pattern)+ ')'
    ;


// -------------------------------------------------
// Loops
// -------------------------------------------------

loopStmt
    : WHILE expr block
    | FOR ID IN expr block
    | LOOP block
    ;

// -------------------------------------------------
// Blocks (expression-oriented)
// -------------------------------------------------

block
    : '{' statement* expr? '}'
    ;

// -------------------------------------------------
// Expressions
// -------------------------------------------------

expr
    : op=(MINUS | PLUS) expr               #UnaryExpr
    | '(' expr ')'                         #ParenExpr
    |'(' expr (',' expr)+ ')'              #TupleExpr
    | expr '[' expr ']'                    #IndexExpr
    | expr RANGE expr                       #RangeExpr
    | expr op=(STAR | DIV | MOD) expr       #MulExpr
    | expr op=(PLUS | MINUS) expr             #AddExpr
    | expr op=(LT | GT | LTE | GTE) expr #CompareExpr
    | expr op=(EQ | NEQ) expr            #EqualityExpr
    | expr op=AND expr                     #AndExpr
    | expr op=OR expr                      #OrExpr
    | expr PIPE expr                       #PipeExpr
    | ID '(' argList? ')'                  #CallExpr
    | literal                              #LiteralExpr
    | ID                                   #IdExpr
    ;

argList
    : expr (',' expr)*
    ;

// -------------------------------------------------
// Literals
// -------------------------------------------------

literal
    : INT_LITERAL
    | REAL_LITERAL
    | STRING_LITERAL
    | CHAR_LITERAL
    | TRUE
    | FALSE
    ;


// -------------------------------------------------
// Keywords
// -------------------------------------------------

IF      : 'if';
ELIFX   : 'elifx';
ELSE    : 'else';
FN      : 'function';
LET     : 'let';
MUT     : 'mut';
FOR     : 'for';
WHILE   : 'while';
LOOP    : 'loop';
IN      : 'in';
RETURN  : 'return';
TYPE    : 'type';

INT     : 'Int';
BOOL    : 'Bool';
CHAR    : 'Char';
REAL    : 'Real';
STRING  : 'String';

TRUE    : 'true';
FALSE   : 'false';

AND     : 'and';
OR      : 'or';

// -------------------------------------------------
// Symbols
// -------------------------------------------------

ARROW   : '->';
PIPE    : '|>';

ASSIGN  : '=';
EQ      : '==';
NEQ     : '!=';

LT      : '<';
GT      : '>';
LTE     : '<=';
GTE     : '>=';

PLUS    : '+';
MINUS   : '-';
STAR    : '*';
DIV     : '/';
MOD     : '%';

RANGE   : '..';

LPAREN  : '(';
RPAREN  : ')';
LBRACE  : '{';
RBRACE  : '}';
LBRACK  : '[';
RBRACK  : ']';

COMMA   : ',';
SEMI    : ';';
COLON   : ':';

// -------------------------------------------------
// Identifiers & literals
// -------------------------------------------------

ID : [a-zA-Z_][a-zA-Z0-9_]*;

INT_LITERAL
    : [0-9]+
    ;

REAL_LITERAL
    : [0-9]+ '.' [0-9]*
    ;

STRING_LITERAL
    : '"' (ESC | ~["\\\r\n])* '"'
    ;

CHAR_LITERAL
    : '\'' (ESC | ~['\\\r\n]) '\''
    ;

fragment ESC
    : '\\' [nrt"\\']
    ;

// -------------------------------------------------
// Whitespace & comments
// -------------------------------------------------

WS : [ \t\r\n]+ -> skip ;
LINE_COMMENT : '//' .*? ('\n' | EOF) -> skip ;
BLOCK_COMMENT : '/*' .*? '*/' -> skip ;