#include "llvm_wrapper.h"
#include <memory>
#include <mutex>

//initialize llvm data structures
std::unique_ptr<llvm::LLVMContext> LLVMWrapper::TheContext;
std::unique_ptr<llvm::Module> LLVMWrapper::TheModule;
std::unique_ptr<llvm::IRBuilder<>> LLVMWrapper::Builder;
std::map<std::string, llvm::Value *> LLVMWrapper::NamedValues;
std::unique_ptr<llvm::FunctionPassManager> LLVMWrapper::TheFPM;
std::unique_ptr<llvm::LoopAnalysisManager> LLVMWrapper::TheLAM; 
std::unique_ptr<llvm::FunctionAnalysisManager> LLVMWrapper::TheFAM; 
std::unique_ptr<llvm::CGSCCAnalysisManager> LLVMWrapper::TheCGAM; 
std::unique_ptr<llvm::ModuleAnalysisManager> LLVMWrapper::TheMAM; 
std::unique_ptr<llvm::PassInstrumentationCallbacks> LLVMWrapper::ThePIC; 
std::unique_ptr<llvm::StandardInstrumentations> LLVMWrapper::TheSI; 
std::unique_ptr<llvm::orc::KaleidoscopeJIT> LLVMWrapper::TheJIT;