#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "token.hpp"

namespace Msl {

class Lexer {
public:
    Lexer(const std::string& code);
    std::vector<Token> lexTokens();

private:
    static const std::unordered_map<std::string_view, Token::Type> keywords;

    std::string m_code;
    size_t m_start;
    size_t m_current;
    size_t m_line;
    size_t m_lineBegin;
    std::vector<Token> m_tokens;

    void lexNextToken();
    bool atEnd() const;
    char peek() const;
    char peekSecond() const;
    char advance();
    bool match(char expected);
    void addToken(Token::Type type);
    void skipWhitespaces();
    void lexString();
    void lexNumber();
    void lexIdentifier();
    size_t getTokenColumn() const;
    std::string getTokenString() const;
};

}
