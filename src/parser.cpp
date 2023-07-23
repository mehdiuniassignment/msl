#include "parser.hpp"
#include "cassert"
#include "exceptions.hpp"
#include "token.hpp"

#include <cstdarg>
#include <iostream>

namespace Msl {

Parser::Parser(std::vector<Token>& tokens)
    : m_tokens(std::move(tokens))
{
}

Program* Parser::parse()
{
    return parseProgram();
}

Program* Parser::parseProgram()
{
    std::vector<Statement*> statements;

    while (!isAtEnd()) {
        auto declaration = parseDeclaration();
        statements.push_back(declaration);
    }

    return new Program(std::move(statements));
}

Statement* Parser::parseDeclaration()
{
    try {
        if (match(1, Token::Type::Let))
            return parseVariableDeclaration();
        return parseStatement();
    } catch (ParsingException& e) {
        synchronize();
        throw e;
    }
    assert(false);
}

Statement* Parser::parseVariableDeclaration()
{
    std::vector<VariableDeclarator*> declarators = {};

    do {
        auto name = consume(Token::Type::Identifier, "Expected variable name.");
        auto identifier = new Identifier(name.str());
        Expression* init = nullptr;
        if (match(1, Token::Type::Equal)) {
            init = parseExpression();
        }
        if (init)
            declarators.push_back(new VariableDeclarator(identifier, init));
        else
            declarators.push_back(new VariableDeclarator(identifier));
    } while (match(1, Token::Type::Comma));

    consume(Token::Type::SemiColon, "Expected ';' after declaration.");

    return new VariableDeclaration(std::move(declarators));
}

Statement* Parser::parseStatement()
{
    if (match(1, Token::Type::Continue)) {
        consume(Token::Type::SemiColon, "Expected ';' after continue statement.");
        return new ContinueStatement();
    }
    if (match(1, Token::Type::Break)) {
        consume(Token::Type::SemiColon, "Expected ';' after break statement.");
        return new BreakStatement();
    }
    if (match(1, Token::Type::Do))
        return parseDoWhileStatement();
    if (match(1, Token::Type::For))
        return parseForStatement();
    if (match(1, Token::Type::If))
        return parseIfStatement();
    if (match(1, Token::Type::Print))
        return parsePrintStatement();
    if (match(1, Token::Type::Return))
        return parseReturnStatement();
    if (match(1, Token::Type::While))
        return parseWhileStatement();
    if (match(1, Token::Type::OpenBrace))
        return parseBlockStatement();
    return parseExpressionStatement();
}

Statement* Parser::parseReturnStatement()
{
    Expression* argument = nullptr;
    if (!check(Token::Type::SemiColon))
        argument = parseExpression();
    consume(Token::Type::SemiColon, "Expected ';' after return statement.");
    return new ReturnStatement(argument);
}

Statement* Parser::parseDoWhileStatement()
{
    auto body = parseStatement();
    consume(Token::Type::While, "Expected 'while' after do while block.");
    consume(Token::Type::OpenParen, "Expected '(' after while.");
    auto condition = parseExpression();
    consume(Token::Type::CloseParen, "Expected ')' after do while loop condition.");
    consume(Token::Type::SemiColon, "Expected ';' after do while loop.");

    return new DoWhileLoopStatement(condition, body);
}

Statement* Parser::parseWhileStatement()
{
    consume(Token::Type::OpenParen, "Expected '(' after while .");
    auto condition = parseExpression();
    consume(Token::Type::CloseParen, "Expected ')' after while loop condition .");
    auto body = parseStatement();
    return new WhileLoopStatement(condition, body);
}

Statement* Parser::parseForStatement()
{
    consume(Token::Type::OpenParen, "Expected '(' after for .");

    Statement* init = nullptr;
    if (match(1, Token::Type::SemiColon)) {
        init = nullptr;
    } else if (match(1, Token::Type::Let)) {
        init = parseVariableDeclaration();
    } else {
        init = parseExpressionStatement();
    }

    Expression* condition = nullptr;
    if (!check(Token::Type::SemiColon))
        condition = parseExpression();

    consume(Token::Type::SemiColon, "Expected ';' for loop condition .");

    Expression* increment = nullptr;
    if (!check(Token::Type::CloseParen))
        increment = parseExpression();

    consume(Token::Type::CloseParen, "Expected ')' for loop condition .");

    auto body = parseStatement();

    return new ForLoopStatement(init, condition, increment, body);
}

Statement* Parser::parseIfStatement()
{
    consume(Token::Type::OpenParen, "Expected '(' after if .");
    auto condition = parseExpression();
    consume(Token::Type::CloseParen, "Expected ')' after if condition.");

    auto ifBranch = parseStatement();
    Statement* elseBranch = nullptr;
    if (match(1, Token::Type::Else))
        elseBranch = parseStatement();

    if (elseBranch)
        return new IfElseStatement(condition, ifBranch, elseBranch);

    return new IfElseStatement(condition, ifBranch);
}

Statement* Parser::parseBlockStatement()
{
    std::vector<Statement*> statements;
    while (!check(Token::Type::CloseBrace) && !isAtEnd()) {
        statements.push_back(parseDeclaration());
    }
    consume(Token::Type::CloseBrace, "Expected '}' after block.");
    return new BlockStatement(std::move(statements));
}

Statement* Parser::parsePrintStatement()
{
    auto argument = parseExpression();
    consume(Token::Type::SemiColon, "Expected ';' after print argument.");
    return new PrintStatement(argument);
}

Statement* Parser::parseExpressionStatement()
{
    auto expression = parseExpression();
    consume(Token::Type::SemiColon, "Exected ';' after expression");
    return new ExpressionStatement(expression);
}

Expression* Parser::parseExpression()
{
    return parseAssignment();
}

Expression* Parser::parseAssignment()
{
    auto expression = parseOr();
    if (match(4, Token::Type::Equal, Token::Type::PlusEqual,
            Token::Type::MinusEqual, Token::Type::AsteriskEqual,
            Token::Type::SlashEqual, Token::Type::PercentEqual)) {
        AssignmentExpression::Operator op;
        Token operatorToken = previous();
        switch (operatorToken.type()) {
        case Token::Type::Equal:
            op = AssignmentExpression::Operator::Equals;
            break;
        case Token::Type::PlusEqual:
            op = AssignmentExpression::Operator::PlusEquals;
            break;
        case Token::Type::MinusEqual:
            op = AssignmentExpression::Operator::MinusEquals;
            break;
        case Token::Type::AsteriskEqual:
            op = AssignmentExpression::Operator::AsteriskEquals;
            break;
        case Token::Type::SlashEqual:
            op = AssignmentExpression::Operator::SlashEquals;
            break;
        case Token::Type::Percent:
            op = AssignmentExpression::Operator::ModuloEquals;
            break;
        default:
            assert(false);
        }
        auto right = parseAssignment();

        if (expression->isIdentifier())
            return new AssignmentExpression(op,
                static_cast<Identifier*>(expression), right);
        if (expression->isMemberExpression())
            return new AssignmentExpression(op,
                static_cast<MemberExpression*>(expression), right);
        if (expression->isArrayMemberExpression())
            return new AssignmentExpression(op,
                static_cast<ArrayMemberExpression*>(expression), right);

        throw ParsingException(operatorToken, "Invalid assignment target");
    }

    return expression;
}

Expression* Parser::parseOr()
{
    auto expression = parseAnd();

    while (match(1, Token::Type::Or)) {
        auto right = parseAnd();
        expression = new LogicalExpression(LogicalExpression::Operator::Or,
            expression, right);
    }
    return expression;
}

Expression* Parser::parseAnd()
{
    auto expression = parseEquality();

    while (match(1, Token::Type::And)) {
        auto right = parseEquality();
        expression = new LogicalExpression(LogicalExpression::Operator::And,
            expression, right);
    }
    return expression;
}

Expression* Parser::parseEquality()
{
    auto expr = parseComparison();

    while (match(2, Token::Type::EqualEqual, Token::Type::BangEqual)) {
        auto op = previous().type() == Token::Type::EqualEqual
            ? BinaryExpression::Operator::Equals
            : BinaryExpression::Operator::Inequals;
        auto right = parseComparison();
        expr = new BinaryExpression(op, expr, right);
    }

    return expr;
}

Expression* Parser::parseComparison()
{
    auto expr = parseTerm();
    while (match(4, Token::Type::Greater, Token::Type::GreaterEqual,
        Token::Type::Less, Token::Type::LessEqual)) {
        BinaryExpression::Operator op;
        switch (previous().type()) {
        case Token::Type::Greater:
            op = BinaryExpression::Operator::GreaterThan;
            break;
        case Token::Type::GreaterEqual:
            op = BinaryExpression::Operator::GreaterThanEquals;
            break;
        case Token::Type::Less:
            op = BinaryExpression::Operator::LessThan;
            break;
        case Token::Type::LessEqual:
            op = BinaryExpression::Operator::LessThanEquals;
            break;
        default:
            assert(false);
        }
        auto right = parseTerm();
        expr = new BinaryExpression(op, expr, right);
    }

    return expr;
}

Expression* Parser::parseTerm()
{
    auto expr = parseFactor();
    while (match(2, Token::Type::Plus, Token::Type::Minus)) {
        auto op = previous().type() == Token::Type::Plus
            ? BinaryExpression::Operator::Addition
            : BinaryExpression::Operator::Subtraction;
        auto right = parseFactor();
        expr = new BinaryExpression(op, expr, right);
    }

    return expr;
}

Expression* Parser::parseFactor()
{
    auto expr = parseUnary();
    while (match(3, Token::Type::Slash, Token::Type::Asterisk,
        Token::Type::Percent)) {
        BinaryExpression::Operator op;
        switch (previous().type()) {
        case Token::Type::Slash:
            op = BinaryExpression::Operator::Division;
            break;
        case Token::Type::Asterisk:
            op = BinaryExpression::Operator::Multiplication;
            break;
        case Token::Type::Percent:
            op = BinaryExpression::Operator::Modulo;
            break;
        default:
            assert(false);
        }
        auto right = parseUnary();
        expr = new BinaryExpression(op, expr, right);
    }

    return expr;
}

Expression* Parser::parseUnary()
{

    if (match(1, Token::Type::Bang)) {
        auto right = parseUnary();
        return new UnaryExpression(UnaryExpression::Operator::Not, right);
    }

    if (match(1, Token::Type::Minus)) {
        auto right = parseUnary();
        return new UnaryExpression(UnaryExpression::Operator::Minus, right);
    }
    if (match(1, Token::Type::Plus)) {
        auto right = parseUnary();
        return new UnaryExpression(UnaryExpression::Operator::Plus, right);
    }

    if (match(1, Token::Type::PlusPlus)) {
        auto right = parseUnary();
        return new UpdateExpression(UpdateExpression::Operation::Increment, true, right);
    }

    if (match(1, Token::Type::MinusMinus)) {
        auto right = parseUnary();
        return new UpdateExpression(UpdateExpression::Operation::Decrement, true, right);
    }

    return parseFunctionCallOrMember();
}

Expression* Parser::parseFunctionCallOrMember()
{
    auto expression = parsePrimary();

    while (true) {
        if (match(1, Token::Type::OpenParen)) {
            expression = parseFunctionCall(expression);
        } else if (match(1, Token::Type::Dot)) {
            expression = parseObjectMember(expression);
        } else if (match(1, Token::Type::OpenBracket)) {
            expression = parseArrayMember(expression);

        } else if (match(1, Token::Type::PlusPlus)) {
            expression = new UpdateExpression(UpdateExpression::Operation::Increment, false, expression);
        } else if (match(1, Token::Type::MinusMinus)) {
            expression = new UpdateExpression(UpdateExpression::Operation::Decrement, false, expression);
        } else {
            break;
        }
    }

    return expression;
}

Expression* Parser::parseFunctionCall(Expression* expression)
{
    std::vector<Expression*> args;
    if (!check(Token::Type::CloseParen)) {
        do {
            args.push_back(parseExpression());
        } while (match(1, Token::Type::Comma));
    }
    consume(Token::Type::CloseParen, "Expected ')' after function  arguments");
    return new CallExpression(expression, args);
}

Expression* Parser::parseObjectMember(Expression* expression)
{
    auto identifier = consume(Token::Type::Identifier,
        "Expected identifier after . to access property.");
    auto property = new Identifier(identifier.str());
    return new MemberExpression(expression, property);
}

Expression* Parser::parseArrayMember(Expression* expression)
{
    auto index = parseExpression();
    consume(Token::Type::CloseBracket, "Expected ']' after array index");
    return new ArrayMemberExpression(expression, index);
}

Expression* Parser::parsePrimary()
{
    if (match(1, Token::Type::Null))
        return new Literal(Value());
    if (match(1, Token::Type::False))
        return new Literal(Value(false));
    if (match(1, Token::Type::True))
        return new Literal(Value(true));
    if (match(1, Token::Type::NumberLiteral))
        return new Literal(Value(std::stod(previous().str())));
    if (match(1, Token::Type::StringLiteral)) {
        auto str = previous().str();
        assert(str.size() >= 2);
        str.pop_back();
        str.erase(str.begin());
        return new Literal(Value(str));
    }
    if (match(1, Token::Type::Identifier))
        return new Identifier(previous().str());

    if (match(1, Token::Type::OpenParen)) {
        if ((peek(0).type() == Token::Type::CloseParen)
            || (peek(0).type() == Token::Type::Identifier
                && peek(1).type() == Token::Type::Comma)
            || (peek(0).type() == Token::Type::Identifier
                && peek(1).type() == Token::Type::CloseParen
                && peek(2).type() == Token::Type::OpenBrace)) {
            return parseFunctionExpression();
        }
        auto expression = parseExpression();
        consume(Token::Type::CloseParen, "Expected ')' after expression");
        return expression;
    }

    if (match(1, Token::Type::OpenBrace))
        return parseObjectExpression();
    if (match(1, Token::Type::OpenBracket))
        return parseArrayExpression();

    throw ParsingException(peek(0), "Expected expression");
}

Expression* Parser::parseArrayExpression()
{
    std::vector<Expression*> elements;
    if (!check(Token::Type::CloseBracket)) {
        do {
            auto element = parseExpression();
            elements.push_back(element);
        } while (match(1, Token::Type::Comma));
    }
    consume(Token::Type::CloseBracket, "Expceted ']' after array elements.");

    return new ArrayExpression(elements);
}

Expression* Parser::parseObjectExpression()
{
    std::vector<ObjectProperty*> properties;
    if (!check(Token::Type::CloseBrace)) {
        do {
            auto tok = consume(Token::Type::Identifier,
                "Expected property identifier name in object expression");
            auto name = new Identifier(tok.str());
            consume(Token::Type::Colon,
                "Expected ':' after proprety name in object expression");
            auto init = parseExpression();
            properties.push_back(new ObjectProperty(name, init));
        } while (match(1, Token::Type::Comma));
    }
    consume(Token::Type::CloseBrace, "Expceted '}' after object expression.");

    return new ObjectExpression(properties);
}

Expression* Parser::parseFunctionExpression()
{
    std::vector<Identifier*> params;

    if (!check(Token::Type::CloseParen)) {
        do {
            auto param = consume(Token::Type::Identifier,
                "Expected parameter name after '(' in function expression");
            params.push_back(new Identifier(param.str()));
        } while (match(1, Token::Type::Comma));
    }

    consume(Token::Type::CloseParen, "Expected ')' after function params");
    consume(Token::Type::OpenBrace, "Expected block after function params");
    auto* body = dynamic_cast<BlockStatement*>(parseBlockStatement());
    return new FunctionExpression(params, body);
}

void Parser::synchronize()
{
    advance();
    while (!isAtEnd()) {
        if (previous().type() == Token::Type::SemiColon)
            return;

        switch (peek(0).type()) {
        case Token::Type::Let:
        case Token::Type::For:
        case Token::Type::If:
        case Token::Type::While:
        case Token::Type::Do:
        case Token::Type::Print:
            return;
        default:
            break;
        }

        advance();
    }
}

bool Parser::match(size_t count...)
{
    va_list args;
    va_start(args, count);
    for (size_t i = 0; i < count; ++i) {
        auto type = va_arg(args, Token::Type);
        if (check(type)) {
            advance();
            va_end(args);
            return true;
        }
    }

    va_end(args);
    return false;
}

bool Parser::check(Token::Type type)
{
    if (isAtEnd())
        return false;
    return peek(0).type() == type;
}

Token Parser::advance()
{
    if (!isAtEnd())
        current++;
    return previous();
}

bool Parser::isAtEnd()
{
    return peek(0).type() == Token::Type::Eof;
}

Token Parser::peek(size_t i)
{
    if (current + i > m_tokens.size())
        return m_tokens.back();

    return m_tokens.at(current + i);
}

Token Parser::previous()
{
    return m_tokens.at(current - 1);
}

Token Parser::consume(Token::Type type, const std::string& message)
{
    if (check(type))
        return advance();
    throw ParsingException(peek(0), message);
}

}
