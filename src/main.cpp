#include "parser.h"

int main() {
    Parser parser = Parser(Lexer(), LLVMWrapper());
    fprintf(stderr, "ready> ");
    parser.getNextToken();

    parser.initializeModule();
    parser.ParsingLoop();
    return 0;
}