#include "parser.h"
#include <memory>


Parser::Parser(Lexer lexer, LLVMWrapper&& llvm_wrapper) : lexer(lexer), llvm_wrapper(std::move(llvm_wrapper)) {
    //install operator precedence
    BinOpPrecedence['<'] = 10;
    BinOpPrecedence['+'] = 20;
    BinOpPrecedence['-'] = 20;
    BinOpPrecedence['*'] = 40;
}

void Parser::initializeModule() {
  llvm_wrapper.TheContext = std::make_unique<llvm::LLVMContext>();
  llvm_wrapper.TheModule = std::make_unique<llvm::Module>("my cool jit", *llvm_wrapper.TheContext);
  // Create a new builder for the module.
  llvm_wrapper.Builder = std::make_unique<llvm::IRBuilder<>>(*llvm_wrapper.TheContext);
}

int Parser::getNextToken() {
    return CurTok = lexer.gettok();
}

std::unique_ptr<ExprAST> Parser::logError(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> Parser::logErrorP(const char *Str) {
    logError(Str);
    return nullptr;
}

// tok_number ..
// CurTok ^
//         CurTok ^
std::unique_ptr<ExprAST> Parser::ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(lexer.NumVal);
    getNextToken();
    return std::move(Result);
}

std::unique_ptr<ExprAST> Parser::ParseParenExpr() {
    getNextToken(); // eat (
    auto V = ParseExpression();
    if (!V) {
        return nullptr;
    }
    if (CurTok != ')') {
        return logError("expected ')'");
    }
    getNextToken(); // eat )
    return V;
}

std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr() {
    std::string IdName = lexer.IdentifierStr;
    getNextToken(); // eat identifier
    if (CurTok != '(') { // Simple variable ref
        return std::make_unique<VariableExprAST>(IdName);
    }
    // Call
    getNextToken(); // eat (
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != ')') {
        while (1) {
            if (auto Arg = ParseExpression()) {
                Args.push_back(std::move(Arg));
            } else {
                return nullptr;
            }
            if (CurTok == ')') {
                break;
            }
            if (CurTok != ',') {
                return logError("Expected ')' or ',' in argument list");
            }
            getNextToken();
        }
    }
    getNextToken(); // eat )
    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

std::unique_ptr<ExprAST> Parser::ParsePrimary() {
    switch (CurTok) {
        default:
            return logError("unknown token when expecting an expression");
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
    }
}

int Parser::getTokenPrecedence() {
    if (!isascii(CurTok)) {
        return -1;
    }
    int TokPrec = BinOpPrecedence[CurTok];
    if (TokPrec <= 0) {
        return -1;
    }
    return TokPrec;
}

std::unique_ptr<ExprAST> Parser::ParseExpression() {
    auto LHS = ParsePrimary();
    if (!LHS) {
        return nullptr;
    }
    return ParseBinOpRHS(0, std::move(LHS));
}

std::unique_ptr<ExprAST> Parser::ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
    while (1) {
        int TokPrec = getTokenPrecedence();
        if (TokPrec < ExprPrec) {
            return LHS;
        }
        int BinOp = CurTok;
        getNextToken();
        auto RHS = ParsePrimary();
        if (!RHS) {
            return nullptr;
        }
        int NextPrec = getTokenPrecedence();
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS) {
                return nullptr;
            }
        }
        LHS = std::make_unique<BinOpExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

std::unique_ptr<PrototypeAST> Parser::ParsePrototype() {
    if (CurTok != tok_identifier) {
        return logErrorP("Expected function name in prototype");
    }
    std::string FnName = lexer.IdentifierStr;
    getNextToken();
    if (CurTok != '(') {
        return logErrorP("Expected '(' in prototype");
    }
    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier) {
        ArgNames.push_back(lexer.IdentifierStr);
    }
    if (CurTok != ')') {
        return logErrorP("Expected ')' in prototype");
    }
    getNextToken(); // eat )
    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

std::unique_ptr<FunctionAST> Parser::ParseDefinition() {
    getNextToken(); // eat def
    auto Proto = ParsePrototype();
    if (!Proto) {
        return nullptr;
    }
    if (auto E = ParseExpression()) {
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

std::unique_ptr<PrototypeAST> Parser::ParseExtern() {
    getNextToken(); // eat extern
    return ParsePrototype();
}

std::unique_ptr<FunctionAST> Parser::ParseTopLevelExpr() {
    if (auto E = ParseExpression()) {
        auto Proto = std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

void Parser::HandleDefinition() {
    if (auto FnAST = ParseDefinition()) {
        if(auto *FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read function definition:");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    } else {
        getNextToken();
    }
}

void Parser::HandleExtern() {
    if (auto ExternAST = ParseExtern()) {
        if(auto *ExternIR = ExternAST->codegen()) {
            fprintf(stderr, "Read extern:");
            ExternIR->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    } else {
        getNextToken();
    }
}

void Parser::HandleTopLevelExpression() {
    if (auto TopLevelAST = ParseTopLevelExpr()) {
        if(auto *TopLevelASTIR = TopLevelAST->codegen()) {
            fprintf(stderr, "Read top-level expression:");
            TopLevelASTIR->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    } else {
        getNextToken();
    }
}

void Parser::ParsingLoop() {
    while (1) {
        fprintf(stderr, "ready> ");
        switch (CurTok) {
            case tok_eof:
                return;
            case ';':
                getNextToken();
                break;
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}