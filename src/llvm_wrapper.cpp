#include "llvm_wrapper.h"

//initialize llvm data structures
std::unique_ptr<llvm::LLVMContext> LLVMWrapper::TheContext;
std::unique_ptr<llvm::Module> LLVMWrapper::TheModule;
std::unique_ptr<llvm::IRBuilder<>> LLVMWrapper::Builder;
std::map<std::string, llvm::Value *> LLVMWrapper::NamedValues;