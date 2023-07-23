#pragma once

#include "token.hpp"
#include "value.hpp"

#include <exception>

namespace Msl {

class Exception : public std::exception {
public:
};

class ParsingException : public Exception {
public:
    ParsingException(Token token, const std::string& message)
        : token(token)
        , message(message)
    {
    }
    Token token;
    std::string message;
};

class RuntimeException : public Exception {
public:
    RuntimeException(const std::string& message)
        : message(message)
    {
    }
    std::string message;
};

class ReturnException : public Exception {
public:
    ReturnException(Value value)
        : value(value)
    {
    }
    Value value;
};

class BreakException : public Exception {
public:
};

class ContinueException : public Exception {
public:
};

}
