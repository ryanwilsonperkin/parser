#ifndef PARSER_PARSELIST_H
#define PARSER_PARSELIST_H

#include "token.h"

extern token yylex();
void get_next_token();

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
