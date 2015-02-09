#ifndef PARSER_PARSELIST_H
#define PARSER_PARSELIST_H

#include "token.h"

/* Current column number from lexer */
extern int col_num;

/* Current line number from lexer */
extern int line_num;

/* Retrieve next token from lexer */
extern token yylex();

char *token_name(token);
void error(token *, int);
void get_next_token();
void consume(token);
void skip_to(token *, int);

void rule_stmts();
void rule_stmts2();
void rule_stmt();
void rule_assign();
void rule_listexpr();
void rule_listexpr2();
void rule_listelem();
void rule_var();
void rule_items();
void rule_item();

#endif  // PARSER_PARSELIST_H
