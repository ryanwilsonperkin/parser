#ifndef PARSER_TOKEN_H
#define PARSER_TOKEN_H

typedef enum token_t {
        INT,
        REAL,
        LPAREN,
        RPAREN,
        CAR,
        CDR,
        SET,
        PLUS,
        SEMICOLON,
        CHAR,
        STR
} token;

#endif  // PARSER_TOKEN_H
