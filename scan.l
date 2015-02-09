%{
#include <stdlib.h>
#include <string.h>
#include "token.h"
#define YY_DECL token yylex(void)

int line_num = 1;
int col_num = 1;

char *line_buf = NULL;

void reset_line_buf();
void append_line_buf();
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
SPACE       [ \t]+
NEWLINE     \n

%%

{NEWLINE}   {reset_line_buf(); col_num = 1; line_num++;}
{SPACE}     {append_line_buf(); col_num += strlen(yytext);}
{INT}       {append_line_buf(); col_num += strlen(yytext); return INT;}
{REAL}      {append_line_buf(); col_num += strlen(yytext); return REAL;}
{LPAREN}    {append_line_buf(); col_num += strlen(yytext); return LPAREN;}
{RPAREN}    {append_line_buf(); col_num += strlen(yytext); return RPAREN;}
{CAR}       {append_line_buf(); col_num += strlen(yytext); return CAR;}
{CDR}       {append_line_buf(); col_num += strlen(yytext); return CDR;}
{SET}       {append_line_buf(); col_num += strlen(yytext); return SET;}
{PLUS}      {append_line_buf(); col_num += strlen(yytext); return PLUS;}
{SEMICOLON} {append_line_buf(); col_num += strlen(yytext); return SEMICOLON;}
{CHAR}      {append_line_buf(); col_num += strlen(yytext); return CHAR;}
{STR}       {append_line_buf(); col_num += strlen(yytext); return STR;}
<<EOF>>     {reset_line_buf(); return END;}

%%

/*
 * reset_line_buf
 * Free memory for global variable line_buf and reset string to '\0'
 */
void reset_line_buf()
{
        free(line_buf);
        line_buf = malloc(1);
        line_buf[0] = 0;
}

/*
 * append_line_buf
 * Add currently matched token to global variable line_buf
 * Allocates enough memory first
 */
void append_line_buf()
{
        size_t new_len;
        if (line_buf) {
                new_len = strlen(line_buf) + strlen(yytext) + 1;
                line_buf = realloc(line_buf, new_len);
        } else {
                new_len = strlen(yytext) + 1;
                line_buf = malloc(new_len);
        }
        line_buf = strcat(line_buf, yytext);
}
