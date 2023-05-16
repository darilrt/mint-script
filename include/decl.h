#pragma once

#include "ast.h"
#include "expr.h"
#include "token.h"
#include "object.h"
#include "mlist.h"

#include <list>
#include <string>

class AssignmentAST : public ASTNode {
public:
    Token type;
    ASTNode* declaration;
    ASTNode* expression;
    
    AssignmentAST(Token type, ASTNode* declaration, ASTNode* expression) : type(type), declaration(declaration), expression(expression) { }

    mList Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class VarDeclarationAST : public ASTNode {
public:
    bool isMutable;
    Token identifier;
    ASTNode* type;
    ASTNode* expression;

    VarDeclarationAST(bool isMutable, Token identifier, ASTNode* type, ASTNode* expression) : isMutable(isMutable), identifier(identifier), type(type), expression(expression) { }

    mList Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class LambdaAST : public ASTNode {
public:
    std::vector<ASTNode*> parameters;
    ASTNode* returnType;
    ASTNode* body;

    LambdaAST(std::vector<ASTNode*> parameters, ASTNode* returnType, ASTNode* body) : parameters(parameters), returnType(returnType), body(body) { }

    mList Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class ReturnAST : public ASTNode {
public:
    ASTNode* expression;

    ReturnAST(ASTNode* expression) : expression(expression) { }

    mList Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class ArgDeclAST : public ASTNode {
public:
    bool isMutable;
    Token identifier;
    ASTNode* type;
    ASTNode* defaultValue;

    ArgDeclAST(bool isMutable, Token identifier, ASTNode* type, ASTNode* defaultValue) : isMutable(isMutable), identifier(identifier), type(type), defaultValue(defaultValue) { }

    mList Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class FunctionAST : public ASTNode {
public:
    Token name;
    LambdaAST* lambda;

    FunctionAST(Token name, LambdaAST* lambda) : name(name), lambda(lambda) { }

    mList Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class BlockAST : public ASTNode {
public:
    std::vector<ASTNode*> statements;

    BlockAST(std::vector<ASTNode*> statements) : statements(statements) { }

    mList Accept(Visitor* visitor) override { return visitor->Visit(this); }
};

class IfAST : public ASTNode {
public:
    ASTNode* condition;
    ASTNode* body;
    std::vector<IfAST*> elseIfs;
    ASTNode* elseBody;

    IfAST() { }

    mList Accept(Visitor* visitor) override { return visitor->Visit(this); }
};