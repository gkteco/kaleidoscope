#include "parser.h"

int main() {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    Parser parser = Parser(Lexer(), LLVMWrapper());
    fprintf(stderr, "ready> ");
    parser.getNextToken();
    LLVMWrapper::TheJIT = ExitOnErr(llvm::orc::KaleidoscopeJIT::Create());
    parser.initializeModule();
    parser.ParsingLoop();
    LLVMWrapper::TheModule->print(llvm::errs(), nullptr);
    return 0;
}