#ifndef PARSER_H
#define PARSER_H
#include "ast.h"
#include "lexer.h"
#include <memory>
#include <map>

class Parser {
public:
    void ParsingLoop();
    Parser(Lexer lexer, LLVMWrapper&& llvm_wraper);

    //token buffer, lookahead = 1
    int getNextToken();
    void initializeModule();

private:
    int CurTok;
    Lexer lexer;
    LLVMWrapper llvm_wrapper;
    //operator precedence table
    std::map <char, int> BinOpPrecedence;
    //get precedence of current token
    int getTokenPrecedence();


    //error logging
    std::unique_ptr<ExprAST> logError(const char *Str);
    std::unique_ptr<PrototypeAST> logErrorP(const char *Str);

    //parsing routines
    std::unique_ptr<ExprAST> ParseNumberExpr();
    std::unique_ptr<ExprAST> ParseParenExpr();
    std::unique_ptr<ExprAST> ParseIdentifierExpr();
    std::unique_ptr<ExprAST> ParsePrimary();
    std::unique_ptr<ExprAST> ParseExpression();
    std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);
    std::unique_ptr<PrototypeAST> ParsePrototype();
    std::unique_ptr<FunctionAST> ParseDefinition();
    std::unique_ptr<PrototypeAST> ParseExtern();
    std::unique_ptr<FunctionAST> ParseTopLevelExpr();

    //top level parsing
    void HandleDefinition();
    void HandleExtern();
    void HandleTopLevelExpression();
};

#endif