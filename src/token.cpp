#include "token.hpp"

namespace Msl {

// this map is for debugging purposes

const std::unordered_map<Token::Type, std::string> Token::tokenTypeStrings = {
    { Token::Type::OpenParen, "OpenParen" },
    { Token::Type::CloseParen, "CloseParen" },
    { Token::Type::OpenBracket, "OpenBracket" },
    { Token::Type::CloseBracket, "CloseBracket" },
    { Token::Type::OpenBrace, "OpenBrace" },
    { Token::Type::CloseBrace, "CloseBrace" },
    { Token::Type::Comma, "Comma" },
    { Token::Type::Dot, "Dot" },
    { Token::Type::Colon, "Colon" },
    { Token::Type::SemiColon, "SemiColon" },
    { Token::Type::Plus, "Plus" },
    { Token::Type::Minus, "Minus" },
    { Token::Type::Asterisk, "Asterisk" },
    { Token::Type::Slash, "Slash" },
    { Token::Type::Percent, "Percent" },
    { Token::Type::Equal, "Equal" },
    { Token::Type::Greater, "Greater" },
    { Token::Type::Less, "Less" },
    { Token::Type::Bang, "Bang" },
    { Token::Type::PlusPlus, "PlusPlus" },
    { Token::Type::MinusMinus, "MinusMinus" },
    { Token::Type::EqualEqual, "EqualEqual" },
    { Token::Type::BangEqual, "BangEqual" },
    { Token::Type::GreaterEqual, "GreaterEqual" },
    { Token::Type::LessEqual, "LessEqual" },
    { Token::Type::PlusEqual, "PlusEqual" },
    { Token::Type::MinusEqual, "MinusEqual" },
    { Token::Type::AsteriskEqual, "AsteriskEqual" },
    { Token::Type::SlashEqual, "SlashEqual" },
    { Token::Type::PercentEqual, "PercentEqual" },
    { Token::Type::And, "And" },
    { Token::Type::Or, "Or" },
    { Token::Type::Let, "Let" },
    { Token::Type::Null, "Null" },
    { Token::Type::False, "False" },
    { Token::Type::True, "True" },
    { Token::Type::If, "If" },
    { Token::Type::Else, "Else" },
    { Token::Type::For, "For" },
    { Token::Type::While, "While" },
    { Token::Type::Do, "Do" },
    { Token::Type::Continue, "Continue" },
    { Token::Type::Break, "Break" },
    { Token::Type::Return, "Return" },
    { Token::Type::Print, "Print" },
    { Token::Type::Identifier, "Identifier" },
    { Token::Type::StringLiteral, "StringLiteral" },
    { Token::Type::NumberLiteral, "NumberLiteral" },
    { Token::Type::Eof, "Eof" },
};

Token::Token(Type type, const std::string& str, size_t line, size_t column)
    : m_type(type)
    , m_str(str)
    , m_line(line)
    , m_column(column)
{
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    os << "[ "
       << Token::tokenTypeStrings.at(token.m_type)
       << " (" << token.m_line
       << ":" << token.m_column
       << ") '" << token.m_str
       << "' ]";
    return os;
}

Token::Type Token::type() const
{
    return m_type;
}

std::string Token::str() const
{
    return m_str;
}

size_t Token::line() const
{
    return m_line;
}

size_t Token::column() const
{
    return m_column;
}

}
