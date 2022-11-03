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
    case TokenType::GREATER:
        return Builder->CreateFCmpUGT(L, R, "cmptmp");
    case TokenType::GREATER_EQUAL:
        return Builder->CreateFCmpUGE(L, R, "cmptmp");
    case TokenType::LESS:
        return Builder->CreateFCmpULT(L, R, "cmptmp");
    case TokenType::LESS_EQUAL:
        return Builder->CreateFCmpULE(L, R, "cmptmp");
    case TokenType::EQUAL_EQUAL:
        return Builder->CreateFCmpUEQ(L, R, "cmptmp");
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

    if (expr.value.type == TokenType::TRUE) {
        return Builder->getInt1(true);
    }

    if (expr.value.type == TokenType::FALSE) {
        return Builder->getInt1(false);
    }

    if (expr.value.type == TokenType::STRING) {
        return llvm::ConstantDataArray::getString(*TheContext,
                                                  expr.value.lexeme);
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
    case TokenType::BANG:
        return Builder->CreateNot(operand, "nottmp");
    default:
        return error(expr.op, "invalid unary operator.");
    }
}