#include <stdio.h>
#include <stdlib.h>

#include "parselist.h"
#include "token.h"

token tok;

void error(char *str)
{
        fprintf(stderr, "error: %s: %d\n", str, tok);
        exit(1);
}

void get_next_token()
{
        tok = yylex();
}

void consume(token t)
{
        if (t == tok) get_next_token();
        else error("consume");
}

void rule_stmts()
{
        switch (tok) {
        case LPAREN:
        case CDR:
        case CHAR:
        case SET:
                rule_stmt();
                rule_stmts2();
                break;
        default: error("rule_stmts");
        }
}

void rule_stmts2()
{
        switch (tok) {
        case LPAREN:
        case CDR:
        case CHAR:
        case SET:
                rule_stmt();
                rule_stmts2();
                break;
        default: return;  // epsilon
        }
}

void rule_stmt()
{
        switch (tok) {
        case LPAREN:
        case CDR:
        case CHAR:
                rule_listexpr();
                consume(SEMICOLON);
                break;
        case SET:
                rule_assign();
                consume(SEMICOLON);
                break;
        default: error("rule_stmt");
        }
}

void rule_assign()
{
        switch (tok) {
        case SET:
                consume(SET);
                rule_var();
                rule_listexpr();
                break;
        default: error("rule_assign");
        }
}

void rule_listexpr()
{
        switch (tok) {
        case LPAREN:
        case CDR:
        case CHAR:
                rule_listelem();
                rule_listexpr2();
                break;
        default: error("rule_listexpr");
        }
}

void rule_listexpr2()
{
        switch (tok) {
        case PLUS:
                consume(PLUS);
                rule_listelem();
                rule_listexpr2();
                break;
        default: return;  // epsilon
        }
}

void rule_listelem()
{
        switch (tok) {
        case LPAREN:
                consume(LPAREN);
                rule_items();
                consume(RPAREN);
                break;
        case CDR:
                consume(CDR);
                rule_listelem();
                break;
        case CHAR:
                rule_var();
                break;
        default: error("rule_listelem");
        }
}

void rule_var()
{
        switch (tok) {
        case CHAR:
                consume(CHAR);
                break;
        default: error("rule_var");
        }
}

void rule_items()
{
        switch (tok) {
        case LPAREN:
        case CAR:
        case CDR:
        case CHAR:
        case INT:
        case REAL:
        case STR:
                rule_item();
                rule_items();
                break;
        default: return;  // epsilon
        }
}

void rule_item()
{
        switch (tok) {
        case LPAREN:
        case CDR:
        case CHAR:
                rule_listexpr();
                break;
        case CAR:
                consume(CAR);
                rule_listexpr();
                break;
        case INT:
                consume(INT);
                break;
        case REAL:
                consume(REAL);
                break;
        case STR:
                consume(STR);
                break;
        default: error("rule_item");
        }
}

int main()
{
        get_next_token();
        rule_stmts();
        return 0;
}
