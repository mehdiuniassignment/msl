#pragma once

#include "ast.hpp"
#include "heap.hpp"
#include <unordered_map>

namespace Msl {

typedef std::unordered_map<std::string, Value> Environment;
typedef std::vector<Environment> Stack;

class Interpreter {
public:
    Interpreter();
    void run(Program* program);
    Heap& heap();
    Stack& stack();
    Value getVariable(const std::string& name);
    void declareVariable(const std::string& name, Value value);
    Value updateVariable(const std::string& name, Value value);

private:
    Heap m_heap;
    Stack m_stack;

    void loadNativeFunctions(Environment& environment);
};
}
