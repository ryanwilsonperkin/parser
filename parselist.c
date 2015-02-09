#include <stdio.h>

#include "parselist.h"
#include "token.h"

/* Common macro for calculating number of elements in array */
#define ARR_LEN(arr) sizeof(arr) / sizeof(arr[0])

/* Globally accesible current token variable */
token tok;

/*
 * token_name
 * Get the printable name of a token.
 *
 * Accepts:
 *      t: the token
 *
 * Returns: the name associated with the token
 */
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

/*
 * error
 * Print a message to stdout when current token doesn't match expected values
 *
 * Accepts:
 *      expected: array of expected tokens
 *      n_expected: number of tokens in expected array
 */
void error(token *expected, int n_expected)
{
        int i;
        printf("line %d: col %d: error: expected ", line_num, col_num);
        for (i = 0; i < n_expected; i++) {
                printf("%s, ", token_name(expected[i]));
        }
        printf("received %s\n", token_name(tok));
}

/*
 * get_next_token
 * Fetch the next token from lex and set to global variable
 */
void get_next_token()
{
        tok = yylex();
}

/*
 * consume
 * Advance to next token as long as current token matches
 *
 * Accepts:
 *      t: the token to match
 */
void consume(token t)
{
        token expected[] = {t};
        if (t == tok) get_next_token();
        else error(expected, ARR_LEN(expected));
}

/*
 * skip_to
 * Advance lexer until token matches value in list or EOF
 *
 * Accepts:
 *      toks: list of tokens to match
 *      n_toks: number of tokens in toks list
 */
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

/*
 * rule_stmts
 * Recursive descent for STMTS rule
 */
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

/*
 * rule_stmts2
 * Recursive descent for STMTS2 rule
 */
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

/*
 * rule_stmt
 * Recursive descent for STMT rule
 */
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

/*
 * rule_assign
 * Recursive descent for ASSIGN rule
 */
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

/*
 * rule_listexpr
 * Recursive descent for LISTEXPR rule
 */
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

/*
 * rule_listexpr2
 * Recursive descent for LISTEXPR2 rule
 */
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

/*
 * rule_listelem
 * Recursive descent for LISTELEM rule
 */
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

/*
 * rule_var
 * Recursive descent for VAR rule
 */
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

/*
 * rule_items
 * Recursive descent for ITEMS rule
 */
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

/*
 * rule_item
 * Recursive descent for ITEM rule
 */
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
