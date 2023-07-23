#pragma once

#include "ast.hpp"
#include "object.hpp"

#include <vector>

namespace Msl {

class Function : public Object {
public:
    Function(BlockStatement* body, std::vector<std::string> params);
    virtual Value execute(Interpreter& interpreter, std::vector<Value> arguments);
    BlockStatement* body();
    virtual size_t paramCount() const;
    bool variadic() const;

protected:
    Function() {};
    bool m_variadic;

private:
    std::vector<std::string> m_params;
    BlockStatement* m_body;
};

class Print final : public Function {
public:
    Print();
    virtual Value execute(Interpreter& interpreter, std::vector<Value> arguments) override;
};

class Read final : public Function {
public:
    Read();
    virtual Value execute(Interpreter& interpreter, std::vector<Value> arguments) override;
};
}
