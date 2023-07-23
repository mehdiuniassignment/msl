#include <cctype>

#include "error.hpp"
#include "lexer.hpp"

namespace Msl {

const std::unordered_map<std::string_view, Token::Type> Lexer::keywords = {
    { "let", Token::Type::Let },
    { "null", Token::Type::Null },
    { "true", Token::Type::True },
    { "false", Token::Type::False },
    { "if", Token::Type::If },
    { "else", Token::Type::Else },
    { "for", Token::Type::For },
    { "while", Token::Type::While },
    { "do", Token::Type::Do },
    { "continue", Token::Type::Continue },
    { "break", Token::Type::Break },
    { "return", Token::Type::Return },
    { "print", Token::Type::Print }
};

Lexer::Lexer(const std::string& code)
    : m_code(code)
    , m_start(0)
    , m_current(0)
    , m_line(1)
    , m_lineBegin(0)
{
}

std::vector<Token> Lexer::lexTokens()
{
    while (!atEnd())
        lexNextToken();

    m_tokens.push_back(Token(Token::Type::Eof, "", m_line, getTokenColumn()));

    return m_tokens;
}

void Lexer::lexNextToken()
{
    m_start = m_current;
    skipWhitespaces();
    m_start = m_current;

    char c = advance();
    switch (c) {
    case '\0':
        addToken(Token::Type::Eof);
        break;
    case '(':
        addToken(Token::Type::OpenParen);
        break;
    case ')':
        addToken(Token::Type::CloseParen);
        break;
    case '[':
        addToken(Token::Type::OpenBracket);
        break;
    case ']':
        addToken(Token::Type::CloseBracket);
        break;
    case '{':
        addToken(Token::Type::OpenBrace);
        break;
    case '}':
        addToken(Token::Type::CloseBrace);
        break;
    case ',':
        addToken(Token::Type::Comma);
        break;
    case '.':
        addToken(Token::Type::Dot);
        break;
    case ':':
        addToken(Token::Type::Colon);
        break;
    case ';':
        addToken(Token::Type::SemiColon);
        break;
    case '+':
        if (match('=')) {
            addToken(Token::Type::PlusEqual);
        } else if (match('+')) {
            addToken(Token::Type::PlusPlus);
        } else {
            addToken(Token::Type::Plus);
        }
        break;
    case '-':
        if (match('=')) {
            addToken(Token::Type::MinusEqual);
        } else if (match('-')) {
            addToken(Token::Type::MinusMinus);
        } else {
            addToken(Token::Type::Minus);
        }
        break;
    case '*':
        addToken(match('=') ? Token::Type::AsteriskEqual
                            : Token::Type::Asterisk);
        break;
    case '/':
        addToken(match('=') ? Token::Type::SlashEqual
                            : Token::Type::Slash);
        break;
    case '%':
        addToken(match('=') ? Token::Type::PercentEqual
                            : Token::Type::Percent);
        break;
    case '=':
        addToken(match('=') ? Token::Type::EqualEqual
                            : Token::Type::Equal);
        break;
    case '>':
        addToken(match('=') ? Token::Type::GreaterEqual
                            : Token::Type::Greater);
        break;
    case '<':
        addToken(match('=') ? Token::Type::LessEqual
                            : Token::Type::Less);
        break;
    case '!':
        addToken(match('=') ? Token::Type::BangEqual
                            : Token::Type::Bang);
        break;
    case '&':
        if (match('&')) {
            addToken(Token::Type::And);
        } else {
            error(m_line, getTokenColumn(), "Unexpected token: '&'.");
        }
        break;
    case '|':
        if (match('|')) {
            addToken(Token::Type::Or);
        } else {
            error(m_line, getTokenColumn(), "Unexpected token: '|'.");
        }
        break;
    case '"':
        lexString();
        break;
    default:
        if (std::isdigit(c)) {
            lexNumber();
            break;
        }
        if (std::isalpha(c)) {
            lexIdentifier();
            break;
        }

        error(m_line, getTokenColumn(),
            "Unexpected token '" + getTokenString() + "'.");
        break;
    }
}

bool Lexer::atEnd() const
{
    return m_current >= m_code.size();
}

char Lexer::peek() const
{
    if (atEnd())
        return '\0';

    return m_code.at(m_current);
}

char Lexer::peekSecond() const
{
    if (m_current + 1 >= m_code.size())
        return '\0';

    return m_code.at(m_current + 1);
}

char Lexer::advance()
{
    if (atEnd())
        return '\0';

    return m_code.at(m_current++);
}

bool Lexer::match(char expected)
{
    if (atEnd())
        return false;
    if (m_code.at(m_current) != expected)
        return false;

    advance();
    return true;
}

void Lexer::addToken(Token::Type type)
{
    m_tokens.push_back(Token(type, getTokenString(),
        m_line, getTokenColumn()));
}

void Lexer::skipWhitespaces()
{
    for (;;) {
        switch (peek()) {
        case ' ':
        case '\t':
        case '\v':
        case '\f':
        case '\r':
            advance();
            break;
        case '/':
            if (peekSecond() == '/') {
                while (peek() != '\n' && !atEnd())
                    advance();
            } else {
                return;
            }
            break;
        case '\n':
            m_line++;
            m_lineBegin = m_current + 1;
            advance();
            break;
        default:
            return;
        }
    }
}

void Lexer::lexString()
{
    while (peek() != '"' && !atEnd()) {
        if (peek() == '\n') {
            m_line++;
            m_lineBegin = m_current + 1;
        }
        advance();
    }

    if (atEnd()) {
        error(m_line, getTokenColumn(), "Unterminated string.");
        return;
    }
    advance();
    addToken(Token::Type::StringLiteral);
}

void Lexer::lexNumber()
{
    while (std::isdigit(peek()))
        advance();

    if (peek() == '.' && std::isdigit(peekSecond())) {
        advance();
        while (std::isdigit(peek()))
            advance();
    }

    addToken(Token::Type::NumberLiteral);
}

void Lexer::lexIdentifier()
{
    while (std::isalnum(peek()))
        advance();
    try {
        auto text = getTokenString();
        Token::Type type = keywords.at(text);
        addToken(type);
    } catch (...) {
        addToken(Token::Type::Identifier);
    }
}

std::string Lexer::getTokenString() const
{
    return m_code.substr(m_start, m_current - m_start);
}

size_t Lexer::getTokenColumn() const
{
    return m_start - m_lineBegin + 1;
}

}
