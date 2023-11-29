#include "ast.h"

llvm::Value* NumberExprAST::codegen() {
    return llvm::ConstantFP::get(*LLVMWrapper::TheContext, llvm::APFloat(Val));
}

llvm::Value* VariableExprAST::codegen() {
    llvm::Value* V = LLVMWrapper::NamedValues[Name];
    if (!V) {
        std::cerr << "Unknown variable name " << Name << std::endl;
        return nullptr;
    }
    return V;
}

llvm::Value* BinOpExprAST::codegen() {
    llvm::Value* L = LHS->codegen();
    llvm::Value* R = RHS->codegen();
    if (!L || !R) {
        return nullptr;
    }
    switch (Op) {
        case '+':
            return LLVMWrapper::Builder->CreateFAdd(L, R, "addtmp");
        case '-':
            return LLVMWrapper::Builder->CreateFSub(L, R, "subtmp");
        case '*':
            return LLVMWrapper::Builder->CreateFMul(L, R, "multmp");
        case '<':
            L = LLVMWrapper::Builder->CreateFCmpULT(L, R, "cmptmp");
            return LLVMWrapper::Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*LLVMWrapper::TheContext), "booltmp");
        default:
            std::cerr << "Invalid binary operator " << Op << std::endl;
            return nullptr;
    }
}

llvm::Value* CallExprAST::codegen() {
    llvm::Function* CalleeF = LLVMWrapper::TheModule->getFunction(Callee);
    if (!CalleeF) {
        std::cerr << "Unknown function referenced " << Callee << std::endl;
        return nullptr;
    }
    if (CalleeF->arg_size() != Args.size()) {
        std::cerr << "Incorrect # arguments passed" << std::endl;
        return nullptr;
    }
    std::vector<llvm::Value*> ArgsV;
    for (unsigned i = 0, e = Args.size(); i != e; ++i) {
        ArgsV.push_back(Args[i]->codegen());
        if (!ArgsV.back()) {
            return nullptr;
        }
    }
    return LLVMWrapper::Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}


llvm::Function* PrototypeAST::codegen() {
    std::vector<llvm::Type*> Doubles(Args.size(), llvm::Type::getDoubleTy(*LLVMWrapper::TheContext));
    llvm::FunctionType* FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(*LLVMWrapper::TheContext), Doubles, false);
    llvm::Function* F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, LLVMWrapper::TheModule.get());
    unsigned Idx = 0;
    for (auto &Arg : F->args()) {
        Arg.setName(Args[Idx++]);
    }
    return F;
}

llvm::Function* FunctionAST::codegen() {
    llvm::Function* TheFunction = LLVMWrapper::TheModule->getFunction(Proto->getName());
    if (!TheFunction) {
        TheFunction = Proto->codegen();
    }
    if (!TheFunction) {
        return nullptr;
    }
    if (!TheFunction->empty()) {
        std::cerr << "Function cannot be redefined" << std::endl;
        return nullptr;
    }
    llvm::BasicBlock* BB = llvm::BasicBlock::Create(*LLVMWrapper::TheContext, "entry", TheFunction);
    LLVMWrapper::Builder->SetInsertPoint(BB);
    LLVMWrapper::NamedValues.clear();
    for (auto &Arg : TheFunction->args()) {
        LLVMWrapper::NamedValues[std::string(Arg.getName())] = &Arg;
    }
    if (llvm::Value* RetVal = Body->codegen()) {
        LLVMWrapper::Builder->CreateRet(RetVal);
        llvm::verifyFunction(*TheFunction);
        LLVMWrapper::TheFPM->run(*TheFunction, *LLVMWrapper::TheFAM);
        return TheFunction;
    }
    TheFunction->eraseFromParent();
    return nullptr;
}