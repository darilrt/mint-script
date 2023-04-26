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

    mlist Accept(Visitor* visitor) override { return visitor->Visit(this); }
};