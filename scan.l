%{
#include <string.h>
#include "token.h"
#define YY_DECL token yylex(void)

int line_num = 1;
int col_num = 1;
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
CHAR        [^[:space:]\(\)\+;]
STR         [^[:space:]\(\)\+;][^[:space:]\(\)\+;]+
SPACE       [[:space:]]+
NEWLINE     \n

%%

{NEWLINE}   {col_num = 1; line_num++;}
{SPACE}     {col_num += strlen(yytext);}
{INT}       {col_num += strlen(yytext); return INT;}
{REAL}      {col_num += strlen(yytext); return REAL;}
{LPAREN}    {col_num += strlen(yytext); return LPAREN;}
{RPAREN}    {col_num += strlen(yytext); return RPAREN;}
{CAR}       {col_num += strlen(yytext); return CAR;}
{CDR}       {col_num += strlen(yytext); return CDR;}
{SET}       {col_num += strlen(yytext); return SET;}
{PLUS}      {col_num += strlen(yytext); return PLUS;}
{SEMICOLON} {col_num += strlen(yytext); return SEMICOLON;}
{CHAR}      {col_num += strlen(yytext); return CHAR;}
{STR}       {col_num += strlen(yytext); return STR;}
<<EOF>>     {col_num += strlen(yytext); return END;}
