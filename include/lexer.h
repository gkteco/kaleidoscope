#ifndef LEXER_H
#define LEXER_H

#include "token.h"

class Lexer {
    public:
        double NumVal;
        std::string IdentifierStr = "";
        int gettok();
};


#endif