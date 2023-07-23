#include "function.hpp"
#include "exceptions.hpp"
#include "interpreter.hpp"

#include <iostream>

namespace Msl {

Function::Function(BlockStatement* body, std::vector<std::string> params)
    : m_variadic(false)
    , m_params(params)
    , m_body(body)
{
}

Value Function::execute(Interpreter& interpreter, std::vector<Value> arguments)
{
    std::unordered_map<std::string, Value> environment = {};

    for (size_t i = 0; i < m_params.size(); ++i) {
        environment.emplace(m_params[i], arguments[i]);
    }

    Value ret;
    try {
        m_body->execute(interpreter, environment);
    } catch (ReturnException& e) {
        ret = e.value;
    }
    return ret;
}

BlockStatement* Function::body()
{
    return m_body;
}

size_t Function::paramCount() const
{
    return m_params.size();
}

bool Function::variadic() const
{
    return m_variadic;
}

Print::Print()
{
    m_variadic = true;
}

Value Print::execute(Interpreter&, std::vector<Value> arguments)
{
    for (const auto& argument : arguments) {
        std::cout << argument << " ";
    }
    std::cout << std::endl;
    return Value();
}

Read::Read()
{
    m_variadic = true;
}

Value Read::execute(Interpreter&, std::vector<Value>)
{
    std::string line;
    std::getline(std::cin, line);
    return Value(line);
}
}
