#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <iostream>

class Lexer {
    public:
        double NumVal;
        std::string IdentifierStr = "";
        std::string BinOp = "";
        int gettok();
};


#endif