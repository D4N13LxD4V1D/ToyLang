#include "Compiler.hpp"

std::unique_ptr<llvm::LLVMContext> Compiler::TheContext =
    std::make_unique<llvm::LLVMContext>();
std::unique_ptr<llvm::Module> Compiler::TheModule =
    std::make_unique<llvm::Module>("toy", *TheContext);
std::unique_ptr<llvm::IRBuilder<>> Compiler::Builder =
    std::make_unique<llvm::IRBuilder<>>(*TheContext);

llvm::Value *Compiler::error(Token token, std::string message) {
    ToyLang::error(token, message);
    return nullptr;
}

llvm::Value *Compiler::codegen(std::unique_ptr<Expr<llvm::Value *>> &expr) {
    return expr->accept(*this);
}

llvm::Value *Compiler::visit(BinaryExpr<llvm::Value *> &expr) {
    llvm::Value *L = codegen(expr.left);
    llvm::Value *R = codegen(expr.right);
    if (L == nullptr || R == nullptr) {
        return nullptr;
    }
    switch (expr.op.type) {
    case TokenType::PLUS:
        return Builder->CreateFAdd(L, R, "addtmp");
    case TokenType::MINUS:
        return Builder->CreateFSub(L, R, "subtmp");
    case TokenType::STAR:
        return Builder->CreateFMul(L, R, "multmp");
    case TokenType::SLASH:
        return Builder->CreateFDiv(L, R, "divtmp");
    default:
        return error(expr.op, "Invalid binary operator.");
    }
}

llvm::Value *Compiler::visit(GroupingExpr<llvm::Value *> &expr) {
    return codegen(expr.expression);
}

llvm::Value *Compiler::visit(LiteralExpr<llvm::Value *> &expr) {
    if (expr.value.type == TokenType::NUMBER) {
        return llvm::ConstantFP::get(
            *TheContext, llvm::APFloat(std::stod(expr.value.lexeme)));
    }

    return error(expr.value, "Invalid literal.");
}

llvm::Value *Compiler::visit(UnaryExpr<llvm::Value *> &expr) {
    llvm::Value *operand = codegen(expr.right);
    if (operand == nullptr) {
        return nullptr;
    }
    switch (expr.op.type) {
    case TokenType::MINUS:
        return Builder->CreateFNeg(operand, "negtmp");
    default:
        return error(expr.op, "invalid unary operator.");
    }
}