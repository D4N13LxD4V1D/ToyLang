#ifndef TOYLANG_COMPILER_HPP
#define TOYLANG_COMPILER_HPP

#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>

#include "Expr.hpp"
#include "Token.hpp"
#include "ToyLang.hpp"

#include <map>
#include <memory>
#include <string>

class Compiler : public ExprVisitor<llvm::Value *> {
  private:
    static std::unique_ptr<llvm::LLVMContext> TheContext;
    static std::unique_ptr<llvm::Module> TheModule;
    static std::unique_ptr<llvm::IRBuilder<>> Builder;
    static std::map<std::string, llvm::Value *> NamedValues;

  public:
    llvm::Value *error(Token token, std::string message);
    llvm::Value *codegen(std::unique_ptr<Expr<llvm::Value *>> &expr);

    llvm::Value *visit(BinaryExpr<llvm::Value *> &expr) override;
    llvm::Value *visit(GroupingExpr<llvm::Value *> &expr) override;
    llvm::Value *visit(LiteralExpr<llvm::Value *> &expr) override;
    llvm::Value *visit(UnaryExpr<llvm::Value *> &expr) override;

    void print();
};

#endif