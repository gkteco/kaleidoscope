#ifndef LLVM_WRAPPER_H
#define LLVM_WRAPPER_H
#include "kalidescope_jit.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <memory>

static llvm::ExitOnError ExitOnErr;

class LLVMWrapper {
public:
    static std::unique_ptr<llvm::LLVMContext> TheContext;
    static std::unique_ptr<llvm::Module> TheModule;
    static std::unique_ptr<llvm::IRBuilder<>> Builder;
    static std::map<std::string, llvm::Value *> NamedValues;
    //create new pass and analysis managers
    static std::unique_ptr<llvm::FunctionPassManager> TheFPM;
  static std::unique_ptr<llvm::LoopAnalysisManager> TheLAM; 
  static std::unique_ptr<llvm::FunctionAnalysisManager> TheFAM; 
  static std::unique_ptr<llvm::CGSCCAnalysisManager> TheCGAM; 
  static std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM; 
  static std::unique_ptr<llvm::PassInstrumentationCallbacks> ThePIC; 
  static std::unique_ptr<llvm::StandardInstrumentations> TheSI; 
  static std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;
};

#endif