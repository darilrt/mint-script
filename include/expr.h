#pragma once

#include "ast.h"
#include "token.h"
#include "mlist.h"

#include <string>

class NumberExprAST : public ASTNode {
public:
    enum class Type {
        Int,
        Float
    };

    Type type;
    double value;

    NumberExprAST(double value) : type(Type::Float), value(value) {}
    NumberExprAST(int value) : type(Type::Int), value(value) {}

    mlist Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class StringExprAST : public ASTNode {
public:
    std::string value;

    StringExprAST(const std::string& value) : value(value) {}

    mlist Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class BoolExprAST : public ASTNode {
public:
    bool value;

    BoolExprAST(bool value) : value(value) {}

    mlist Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class NullExprAST : public ASTNode {
public:
    NullExprAST() = default;

    mlist Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class LambdaExprAST : public ASTNode {
public:
    LambdaExprAST() = default;

    mlist Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class PropertyExprAST : public ASTNode {
public:
    std::string name;
    PropertyExprAST* next = nullptr;

    PropertyExprAST(const std::string& name) : name(name) {}
    PropertyExprAST(const std::string& name, PropertyExprAST* next) : name(name), next(next) {}

    mlist Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class IndexExprAST : public ASTNode {
public:
    ASTNode* expr;
    ASTNode* index;

    IndexExprAST(ASTNode* expr, ASTNode* index) : expr(expr), index(index) {}

    mlist Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class CallExprAST : public ASTNode {
public:
    ASTNode* property;

    CallExprAST(ASTNode* property) : property(property) {}

    mlist Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class UnaryExprAST : public ASTNode {
public:
    Token op;
    ASTNode* expr;
    bool isPrefix;

    UnaryExprAST(Token op, ASTNode* expr) : op(op), expr(expr), isPrefix(true) {}
    UnaryExprAST(Token op, ASTNode* expr, bool isPrefix) : op(op), expr(expr), isPrefix(isPrefix) {}

    mlist Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class BinaryExprAST : public ASTNode {
public:
    Token op;
    ASTNode* lhs;
    ASTNode* rhs;

    BinaryExprAST(Token op, ASTNode* lhs, ASTNode* rhs) : op(op), lhs(lhs), rhs(rhs) {}

    mlist Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class TernaryExprAST : public ASTNode {
public:
    ASTNode* condition;
    ASTNode* trueExpr;
    ASTNode* falseExpr;

    TernaryExprAST(ASTNode* condition, ASTNode* trueExpr, ASTNode* falseExpr) : condition(condition), trueExpr(trueExpr), falseExpr(falseExpr) {}

    mlist Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class ParenExprAST : public ASTNode {
public:
    ASTNode* expr;

    ParenExprAST(ASTNode* expr) : expr(expr) {}

    mlist Accept(Visitor* visitor) override { return visitor->Visit(this); }
};
