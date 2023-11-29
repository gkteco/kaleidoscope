#include "parser.h"

int main() {
    Parser parser = Parser(Lexer(), LLVMWrapper());
    fprintf(stderr, "ready> ");
    parser.getNextToken();

    parser.initializeModule();
    parser.ParsingLoop();
    LLVMWrapper::TheModule->print(llvm::errs(), nullptr);
    return 0;
}