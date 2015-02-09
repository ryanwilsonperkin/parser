#include <stdio.h>
#include <stdlib.h>

#include "parselist.h"
#include "token.h"

#define ARR_LEN(arr) sizeof(arr) / sizeof(arr[0])

token tok;

char *token_name(token t)
{
        switch (t) {
        case INT: return "INT";
        case REAL: return "REAL";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case CAR: return "CAR";
        case CDR: return "CDR";
        case SET: return "SET";
        case PLUS: return "PLUS";
        case SEMICOLON: return "SEMICOLON";
        case CHAR: return "CHAR";
        case STR: return "STR";
        case END: return "END";
        }
}

void error(token *expected, int n_expected)
{
        int i;
        fprintf(stderr, "line %d: col %d: error: expected ", line_num, col_num);
        for (i = 0; i < n_expected; i++) {
                fprintf(stderr, "%s, ", token_name(expected[i]));
        }
        fprintf(stderr, "received %s\n", token_name(tok));
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

void skip_to(token *toks, int n_toks)
{
        int i;
        while (tok != END) {
                get_next_token();
                for (i = 0; i < n_toks; i++) {
                        if (tok == toks[i]) return;
                }
        }
}

void rule_stmts()
{
        token first[] = {LPAREN, CDR, CHAR, SET};
        token follow[] = {END};
        switch (tok) {
        case LPAREN:
        case CDR:
        case CHAR:
        case SET:
                rule_stmt();
                rule_stmts2();
                break;
        default:
                error(first, ARR_LEN(first));
                skip_to(follow, ARR_LEN(follow));
        }
}

void rule_stmts2()
{
        token first_and_follow[] = {LPAREN, CDR, CHAR, SET, END};
        token follow[] = {END};
        switch (tok) {
        case LPAREN:
        case CDR:
        case CHAR:
        case SET:
                rule_stmt();
                rule_stmts2();
                break;
        case END: return;  // epsilon
        default:
                error(first_and_follow, ARR_LEN(first_and_follow));
                skip_to(follow, ARR_LEN(follow));
        }
}

void rule_stmt()
{
        token first[] = {LPAREN, CDR, CHAR, SET};
        token follow[] = {LPAREN, CDR, CHAR, SET};
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
        default:
                error(first, ARR_LEN(first));
                skip_to(follow, ARR_LEN(follow));
        }
}

void rule_assign()
{
        token first[] = {SET};
        token follow[] = {SEMICOLON};
        switch (tok) {
        case SET:
                consume(SET);
                rule_var();
                rule_listexpr();
                break;
        default:
                error(first, ARR_LEN(first));
                skip_to(follow, ARR_LEN(follow));
        }
}

void rule_listexpr()
{
        token first[] = {LPAREN, CDR, CHAR};
        token follow[] = {SEMICOLON, LPAREN, RPAREN, CAR, CDR, CHAR, INT, REAL,
                STR};
        switch (tok) {
        case LPAREN:
        case CDR:
        case CHAR:
                rule_listelem();
                rule_listexpr2();
                break;
        default:
                error(first, ARR_LEN(first));
                skip_to(follow, ARR_LEN(follow));
        }
}

void rule_listexpr2()
{
        token first_and_follow[] = {PLUS, SEMICOLON, LPAREN, RPAREN, CAR, CDR,
                CHAR, INT, REAL, STR};
        token follow[] = {SEMICOLON, LPAREN, RPAREN, CAR, CDR, CHAR, INT, REAL,
                STR};
        switch (tok) {
        case PLUS:
                consume(PLUS);
                rule_listelem();
                rule_listexpr2();
                break;
        case SEMICOLON: 
        case LPAREN:
        case RPAREN:
        case CAR:
        case CDR:
        case CHAR:
        case INT:
        case REAL:
        case STR:
                return;  // epsilon
        default:
                error(first_and_follow, ARR_LEN(first_and_follow));
                skip_to(follow, ARR_LEN(follow));
        }
}

void rule_listelem()
{
        token first[] = {LPAREN, CDR, CHAR};
        token follow[] = {PLUS, SEMICOLON};
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
        default:
                error(first, ARR_LEN(first));
                skip_to(follow, ARR_LEN(follow));
        }
}

void rule_var()
{
        token first[] = {CHAR};
        token follow[] = {PLUS, SEMICOLON, LPAREN, CDR, CHAR};
        switch (tok) {
        case CHAR:
                consume(CHAR);
                break;
        default:
                error(first, ARR_LEN(first));
                skip_to(follow, ARR_LEN(follow));
        }
}

void rule_items()
{
        token first_and_follow[] = {LPAREN, CAR, CDR, CHAR, INT, REAL, STR,
                RPAREN};
        token follow[] = {RPAREN};
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
        case RPAREN: return;  // epsilon
        default:
                error(first_and_follow, ARR_LEN(first_and_follow));
                skip_to(follow, ARR_LEN(follow));
        }
}

void rule_item()
{
        token first[] = {LPAREN, CAR, CDR, CHAR, INT, REAL, STR};
        token follow[] = {LPAREN, CAR, CDR, CHAR, INT, REAL, STR};
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
        default:
                error(first, ARR_LEN(first));
                skip_to(follow, ARR_LEN(follow));
        }
}

int main()
{
        get_next_token();
        rule_stmts();
        consume(END);
        return 0;
}
