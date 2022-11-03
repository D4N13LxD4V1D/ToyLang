#include "StringifyAST.hpp"

std::string StringifyAST::toString(std::unique_ptr<Expr<std::string>> &expr) {
    return expr->accept(*this);
}

std::string StringifyAST::visit(BinaryExpr<std::string> &expr) {
    std::vector<std::unique_ptr<Expr<std::string>>> exprs;
    exprs.push_back(std::move(expr.left));
    exprs.push_back(std::move(expr.right));
    return parenthesize(expr.op.lexeme, exprs);
}

std::string StringifyAST::visit(GroupingExpr<std::string> &expr) {
    std::vector<std::unique_ptr<Expr<std::string>>> exprs;
    exprs.push_back(std::move(expr.expression));
    return parenthesize("group", exprs);
}

std::string StringifyAST::visit(LiteralExpr<std::string> &expr) {
    return expr.value.toString();
}

std::string StringifyAST::visit(UnaryExpr<std::string> &expr) {
    std::vector<std::unique_ptr<Expr<std::string>>> exprs;
    exprs.push_back(std::move(expr.right));
    return parenthesize(expr.op.lexeme, exprs);
}

std::string StringifyAST::parenthesize(
    const std::string &name,
    std::vector<std::unique_ptr<Expr<std::string>>> &exprs) {
    std::string result = "(" + name;
    for (auto &expr : exprs) {
        result += " ";
        result += expr->accept(*this);
    }
    result += ")";
    return result;
}