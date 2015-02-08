#include "parselist.h"
#include "token.h"

token tok;

void get_next_token()
{
        tok = yylex();
}

int main()
{
        get_next_token();
        return 0;
}
