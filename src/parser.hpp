#pragma once

#include "ast.hpp"
#include "token.hpp"

#include <vector>

namespace Msl {

class Parser
{
public:
    Parser(std::vector<Token>& tokens);
    Program* parse();
    Program* parseProgram();
    Statement* parseStatement();
    Statement* parseExpressionStatement();
    Statement* parsePrintStatement();
    Statement* parseDeclaration();
    Statement* parseVariableDeclaration();
    Statement* parseBlockStatement();
    Statement* parseIfStatement();
    Statement* parseForStatement();
    Statement* parseWhileStatement();
    Statement* parseDoWhileStatement();
    Statement* parseReturnStatement();
    Expression* parseExpression();
    Expression* parsePrimary();
    Expression* parseUnary();
    Expression* parseFactor();
    Expression* parseTerm();
    Expression* parseComparison();
    Expression* parseEquality();
    Expression* parseAssignment();
    Expression* parseOr();
    Expression* parseAnd();
    Expression* parseSuffixUpdate();
    Expression* parseFunctionCallOrMember();
    Expression* parseFunctionExpression();
    Expression* parseObjectExpression();
    Expression* parseArrayExpression();
    Expression* parseFunctionCall(Expression* expression);
    Expression* parseObjectMember(Expression* expression);
    Expression* parseArrayMember(Expression* expression);

private:
    void synchronize();
    bool match(size_t count...);
    bool check(Token::Type type);
    bool isAtEnd();
    Token advance();
    Token peek(size_t i);
    Token previous();
    Token consume(Token::Type type, const std::string& message);
    size_t current { 0 };
    std::vector<Token> m_tokens;
};

}
