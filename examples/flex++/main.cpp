#include <iostream>
#include <FlexLexer.h>

// cpps-macro flex++ a.l

int main()
{

    FlexLexer* lexer = new yyFlexLexer;
    lexer->yylex();

    return 0;
}
