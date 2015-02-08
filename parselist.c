#include <stdio.h>
#include <stdlib.h>

#include "parselist.h"
#include "token.h"

#define ARR_LEN(arr) sizeof(arr) / sizeof(arr[0])

token tok;

void error(token *expected, int n_expected)
{
        int i;
        fprintf(stderr, "line %d: error: expected ", line_num);
        for (i = 0; i < n_expected; i++) {
                fprintf(stderr, "%d, ", expected[i]);
        }
        fprintf(stderr, "received %d\n", tok);
        exit(1);
}

void get_next_token()
{
        tok = yylex();
}

void consume(token t)
{
        token expected[] = {t};
        if (t == tok) get_next_token();
        else error(expected, ARR_LEN(expected));
}

void rule_stmts()
{
        token first[] = {LPAREN, CDR, CHAR, SET};
        switch (tok) {
        case LPAREN:
        case CDR:
        case CHAR:
        case SET:
                rule_stmt();
                rule_stmts2();
                break;
        default: error(first, ARR_LEN(first));
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
        token first[] = {LPAREN, CDR, CHAR, SET};
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
        default: error(first, ARR_LEN(first));
        }
}

void rule_assign()
{
        token first[] = {SET};
        switch (tok) {
        case SET:
                consume(SET);
                rule_var();
                rule_listexpr();
                break;
        default: error(first, ARR_LEN(first));
        }
}

void rule_listexpr()
{
        token first[] = {LPAREN, CDR, CHAR};
        switch (tok) {
        case LPAREN:
        case CDR:
        case CHAR:
                rule_listelem();
                rule_listexpr2();
                break;
        default: error(first, ARR_LEN(first));
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
        token first[] = {LPAREN, CDR, CHAR};
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
        default: error(first, ARR_LEN(first));
        }
}

void rule_var()
{
        token first[] = {CHAR};
        switch (tok) {
        case CHAR:
                consume(CHAR);
                break;
        default: error(first, ARR_LEN(first));
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
        token first[] = {LPAREN, CAR, CDR, CHAR, INT, REAL, STR};
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
        default: error(first, ARR_LEN(first));
        }
}

int main()
{
        get_next_token();
        rule_stmts();
        return 0;
}
