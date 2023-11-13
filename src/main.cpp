#include "parser.h"



int main() {
    // 2 + 3 * 4
    Parser parser = Parser(Lexer());
     // Prime the first token.
    fprintf(stderr, "ready> ");
    parser.getNextToken();

    parser.ParsingLoop();
    return 0;
}