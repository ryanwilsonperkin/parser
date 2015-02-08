%{
#include "token.h"
#define YY_DECL token yylex(void)
%}

INT         -?(0|[1-9][0-9]*)
REAL        -?(0|[1-9][0-9]*)\.[0-9]+([eE][1-9][0-9]*)?
LPAREN      \(
RPAREN      \)
CAR         car
CDR         cdr
SET         set
PLUS        \+
SEMICOLON   ;
CHAR        [^[:space:]]
STR         [^[:space:]][^[:space:]]+
SPACE       [[:space:]]+

%%

{SPACE}     /* Ignore whitespace */
{INT}       return INT;
{REAL}      return REAL;
{LPAREN}    return LPAREN;
{RPAREN}    return RPAREN;
{CAR}       return CAR;
{CDR}       return CDR;
{SET}       return SET;
{PLUS}      return PLUS; 
{SEMICOLON} return SEMICOLON; 
{CHAR}      return CHAR;
{STR}       return STR;
<<EOF>>     return END;
