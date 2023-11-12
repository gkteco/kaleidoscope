#include <iostream>
#include "lexer.h"


int main() {
    Lexer lexer = Lexer();
    int token = lexer.gettok();
    do {
        switch(token) {
            default:
                //do nothing
                break;
            case tok_def:
                std::cout << "DEF" << std::endl;
                break;
            case tok_identifier:
                std::cout << "IDEN, " << lexer.IdentifierStr << std::endl;
                break;
            case tok_number:
                std::cout << "NUM_IDEN, " << lexer.NumVal << std::endl;
                break;
        }
        token = lexer.gettok();
    } while (token != tok_eof);
    return 0;
}