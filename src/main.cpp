#include <iostream>
#include <memory>
#include "lexer.h"
#include "ast.h"


int main() {
    // 2 + 3 * 4
    std::unique_ptr<ExprAST> prog = std::make_unique<BinOpExprAST>('+', 
        std::make_unique<NumberExprAST>(2), 
        std::make_unique<BinOpExprAST>('*', 
            std::make_unique<NumberExprAST>(3), 
            std::make_unique<NumberExprAST>(4)));
    prog->prettyPrint();
    return 0;
}