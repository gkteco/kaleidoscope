#ifndef AST_H
#define AST_H
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class ExprAST {
public:
  virtual ~ExprAST() = default;
  virtual void prettyPrint() = 0;
};

class NumberExprAST : public ExprAST {
    public:
        double Val;
        NumberExprAST(double Val) : Val(Val) {}
        void prettyPrint() override {
            std::cout << Val;
        }
};

class VariableExprAST : public ExprAST {
    std::string Name;
    public:
        VariableExprAST(const std::string &Name) : Name(Name) {}
        void prettyPrint() override {
            std::cout << Name;
        }
};

class BinOpExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;
    public:
        BinOpExprAST(char Op, 
            std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS) : Op(Op), 
                                            LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        void prettyPrint() override {
            std::cout << "(";
            LHS->prettyPrint();
            std::cout << " " << Op << " ";
            RHS->prettyPrint();
            std::cout << ")";
        }
};

class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;
    public:
        CallExprAST(const std::string &Callee, 
            std::vector<std::unique_ptr<ExprAST>> Args) : Callee(Callee), Args(std::move(Args)) {}

        void prettyPrint() override {
            std::cout << Callee << "(";
            for (auto &Arg : Args) {
                Arg->prettyPrint();
                std::cout << ", ";
            }
            std::cout << ")";
        }
};

class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
    public:
        PrototypeAST(const std::string &Name, 
            std::vector<std::string> Args) : Name(Name), Args(std::move(Args)) {}
        void prettyPrint() {
            std::cout << Name << "(";
            for (auto &Arg : Args) {
                std::cout << Arg << ", ";
            }
            std::cout << ")";
        }
};

class FuncitonAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;
    public:
        FuncitonAST(std::unique_ptr<PrototypeAST> Proto, 
            std::unique_ptr<ExprAST> Body) : Proto(std::move(Proto)), Body(std::move(Body)) {}
        void prettyPrint() {
            Proto->prettyPrint();
            std::cout << " = ";
            Body->prettyPrint();
        }
};

#endif


























