#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

namespace Msl {

class Token {
public:
    enum class Type {
        OpenParen,
        CloseParen,
        OpenBracket,
        CloseBracket,
        OpenBrace,
        CloseBrace,
        Comma,
        Dot,
        Colon,
        SemiColon,
        Plus,
        Minus,
        Asterisk,
        Slash,
        Percent,
        Equal,
        Greater,
        Less,
        Bang,

        PlusPlus,
        MinusMinus,
        EqualEqual,
        BangEqual,
        GreaterEqual,
        LessEqual,
        PlusEqual,
        MinusEqual,
        AsteriskEqual,
        SlashEqual,
        PercentEqual,
        And,
        Or,

        Let,
        Null,
        False,
        True,
        If,
        Else,
        For,
        While,
        Do,
        Continue,
        Break,
        Return,
        Print,

        Identifier,
        StringLiteral,
        NumberLiteral,

        Eof
    };

public:
    Token(Type type, const std::string& str, size_t line, size_t column);
    friend std::ostream& operator<<(std::ostream& os, const Token& tok);
    Type type() const;
    std::string str() const;
    size_t line() const;
    size_t column() const;

private:
    static const std::unordered_map<Type, std::string> tokenTypeStrings;

    Type m_type;
    std::string m_str;
    size_t m_line;
    size_t m_column;
};
}
