#include "interpreter.hpp"
#include "exceptions.hpp"
#include "function.hpp"
#include "heap.hpp"

#include <iostream>

namespace Msl {

Interpreter::Interpreter()
    : m_heap(*this)
    , m_stack({})
{
}

void Interpreter::run(Program* program)
{
    Environment globals;
    loadNativeFunctions(globals);
    program->execute(*this, globals);
}

Value Interpreter::getVariable(const std::string& name)
{
    for (ssize_t i = m_stack.size() - 1; i >= 0; --i) {
        auto it = m_stack[i].find(name);
        if (it != m_stack[i].end()) {
            return it->second;
        }
    }

    throw RuntimeException("Variable is undefined");
}

void Interpreter::declareVariable(const std::string& name, Msl::Value value)
{
    auto res = m_stack.back().emplace(name, value);
    if (!res.second) {
        throw RuntimeException("Variable already exists");
    }
}

Value Interpreter::updateVariable(const std::string& name, Msl::Value value)
{
    for (ssize_t i = m_stack.size() - 1; i >= 0; --i) {
        auto it = m_stack[i].find(name);
        if (it != m_stack[i].end()) {
            it->second = value;
            return it->second;
        }
    }

    throw RuntimeException("Variable doesn't Exist");
}

Heap& Interpreter::heap()
{
    return m_heap;
}

Stack& Interpreter::stack()
{
    return m_stack;
}

void Interpreter::loadNativeFunctions(Environment& environment)
{
    Print* print = m_heap.allocate<Print>();
    Read* read = m_heap.allocate<Read>();
    environment.emplace("Print", Value(print));
    environment.emplace("Read", Value(read));
}

}
