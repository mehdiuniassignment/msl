#include "ast.hpp"

#include "array.hpp"
#include "exceptions.hpp"
#include "function.hpp"
#include "interpreter.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

namespace Msl {

void printIndentation(uint32_t indentLevel)
{
    for (size_t i = 0; i < indentLevel; ++i) {
        std::cout << "  ";
    }
}

Ast::Ast() { }
Ast::~Ast() { }
bool Ast::isIdentifier() const
{
    return false;
}

bool Ast::isMemberExpression() const
{
    return false;
}

bool Ast::isArrayMemberExpression() const
{
    return false;
}

Scope::Scope(std::vector<Statement*> body)
    : m_body(body)
{
}

Scope::~Scope()
{
    for (auto& statement : m_body) {
        delete statement;
    }
}

void Scope::append(Statement* statement)
{
    m_body.push_back(statement);
}

Program::Program(std::vector<Statement*> body)
    : Scope::Scope(body)
{
}

BlockStatement::BlockStatement(std::vector<Statement*> body)
    : Scope::Scope(body)
{
}

ExpressionStatement::ExpressionStatement(Expression* expression)
    : m_expression(expression)
{
}

ExpressionStatement::~ExpressionStatement()
{
    delete m_expression;
}

Literal::Literal(Value value)
    : m_value(value)
{
}

BinaryExpression::BinaryExpression(Operator op, Expression* left,
    Expression* right)
    : m_op(op)
    , m_left(left)
    , m_right(right)
{
}

BinaryExpression::~BinaryExpression()
{
    delete m_left;
    delete m_right;
}

UnaryExpression::UnaryExpression(Operator op, Expression* right)
    : m_op(op)
    , m_right(right)
{
}

UnaryExpression::~UnaryExpression()
{
    delete m_right;
}

Identifier::Identifier(const std::string& name)
    : m_name(name)
{
}

bool Identifier::isIdentifier() const
{
    return true;
}

const std::string& Identifier::name() const
{
    return m_name;
}

FunctionExpression::FunctionExpression(
    std::vector<Identifier*> m_params,
    BlockStatement* m_body)
    : m_params(m_params)
    , m_body(m_body)
{
}

FunctionExpression::~FunctionExpression()
{
    for (auto& param : m_params) {
        delete param;
    }
    delete m_body;
}

const std::vector<Identifier*>& FunctionExpression::params() const
{
    return m_params;
}

ReturnStatement::ReturnStatement(Expression* argument)
    : m_argument(argument)
{
}

ReturnStatement::~ReturnStatement()
{
    delete m_argument;
}

VariableDeclarator::VariableDeclarator(Identifier* name, Expression* init)
    : m_name(name)
    , m_init(init)
{
}

VariableDeclarator::~VariableDeclarator()
{
    delete m_name;
    delete m_init;
}

VariableDeclaration::VariableDeclaration(
    std::vector<VariableDeclarator*> declarators)
    : m_declarators(declarators)
{
}

VariableDeclaration::~VariableDeclaration()
{
    for (auto& declarator : m_declarators) {
        delete declarator;
    }
}

CallExpression::CallExpression(Expression* name, std::vector<Expression*> arguments)
    : m_name(name)
    , m_arguments(arguments)
{
}

CallExpression::~CallExpression()
{
    delete m_name;
    for (auto& argument : m_arguments) {
        delete argument;
    }
}

PrintStatement::PrintStatement(Expression* argument)
    : m_argument(argument)
{
}

PrintStatement::~PrintStatement()
{
    delete m_argument;
}

AssignmentExpression::AssignmentExpression(Operator op,
    Expression* left, Expression* right)
    : m_op(op)
    , m_left(left)
    , m_right(right)
{
}

AssignmentExpression::~AssignmentExpression()
{
    delete m_left;
    delete m_right;
}

LogicalExpression::LogicalExpression(Operator op,
    Expression* left, Expression* right)
    : m_op(op)
    , m_left(left)
    , m_right(right)
{
}

LogicalExpression::~LogicalExpression()
{
    delete m_left;
    delete m_right;
}

void Scope::prettyPrint(int32_t indentLevel) const
{
    for (const auto& statement : m_body) {
        statement->prettyPrint(indentLevel);
    }
}

IfElseStatement::IfElseStatement(Expression* condition, Statement* ifBranch,
    Statement* elseBranch)
    : m_condition(condition)
    , m_ifBranch(ifBranch)
    , m_elseBranch(elseBranch)
{
}
IfElseStatement::IfElseStatement(Expression* condition, Statement* ifBranch)
    : m_condition(condition)
    , m_ifBranch(ifBranch)
    , m_elseBranch(nullptr)
{
}

IfElseStatement::~IfElseStatement()
{
    delete m_condition;
    delete m_ifBranch;
    delete m_elseBranch;
}

ForLoopStatement::ForLoopStatement(Statement* init, Expression* condition,
    Expression* increment, Statement* body)
    : m_init(init)
    , m_condition(condition)
    , m_increment(increment)
    , m_body(body)
{
}

ForLoopStatement::~ForLoopStatement()
{
    delete m_init;
    delete m_condition;
    delete m_increment;
    delete m_body;
}

UpdateExpression::UpdateExpression(Operation op, bool prefix, Expression* argument)
    : m_op(op)
    , m_prefix(prefix)
    , m_argument(argument)
{
}

UpdateExpression::~UpdateExpression()
{
    delete m_argument;
}

DoWhileLoopStatement::DoWhileLoopStatement(Expression* condition, Statement* body)
    : m_condition(condition)
    , m_body(body)
{
}

DoWhileLoopStatement::~DoWhileLoopStatement()
{
    delete m_condition;
    delete m_body;
}

WhileLoopStatement::WhileLoopStatement(Expression* condition, Statement* body)
    : m_condition(condition)
    , m_body(body)
{
}

WhileLoopStatement::~WhileLoopStatement()
{
    delete m_condition;
    delete m_body;
}

BreakStatement::BreakStatement() { }
BreakStatement::~BreakStatement() { }

ContinueStatement::ContinueStatement() { }
ContinueStatement::~ContinueStatement() { }

ArrayMemberExpression::ArrayMemberExpression(Expression* array, Expression* index)
    : m_array(array)
    , m_index(index)
{
}

ArrayMemberExpression::~ArrayMemberExpression()
{
    delete m_array;
    delete m_index;
}

bool ArrayMemberExpression::isArrayMemberExpression() const
{
    return true;
}

Expression* ArrayMemberExpression::array() const
{
    return m_array;
}

Expression* ArrayMemberExpression::index() const
{
    return m_index;
}

MemberExpression::MemberExpression(Expression* object, Identifier* property)
    : m_object(object)
    , m_property(property)
{
}

MemberExpression::~MemberExpression()
{
    delete m_object;
    delete m_property;
}

Expression* MemberExpression::object()
{
    return m_object;
}

Identifier* MemberExpression::property()
{
    return m_property;
}

bool MemberExpression::isMemberExpression() const
{
    return true;
}

ArrayExpression::ArrayExpression(std::vector<Expression*> elements)
    : m_elements(elements)
{
}
ArrayExpression::~ArrayExpression()
{
    for (auto& element : m_elements) {
        delete element;
    }
}

ObjectExpression::ObjectExpression(std::vector<ObjectProperty*> properties)
    : m_properties(properties)
{
}

ObjectExpression::~ObjectExpression()
{
    for (auto& property : m_properties) {
        delete property;
    }
}

const std::vector<ObjectProperty*>& ObjectExpression::properties() const
{
    return m_properties;
}

ObjectProperty::ObjectProperty(Identifier* name, Expression* value)
    : m_name(name)
    , m_value(value)
{
}

ObjectProperty::~ObjectProperty()
{
    delete m_name;
    delete m_value;
}

Identifier* ObjectProperty::name()
{
    return m_name;
}

Expression* ObjectProperty::value()
{
    return m_value;
}

void Program::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "Program:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- body:" << std::endl;
    Scope::prettyPrint(indentLevel + 2);
}

void BlockStatement::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "Block:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- body:" << std::endl;
    Scope::prettyPrint(indentLevel + 2);
}

void ExpressionStatement::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "ExpressionStatement:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- expression:" << std::endl;
    m_expression->prettyPrint(indentLevel + 2);
}

void Literal::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "Literal:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- value:" << std::endl;
    printIndentation(indentLevel + 2);
    std::cout << m_value << std::endl;
}

void BinaryExpression::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "BinaryExpression:" << std::endl;

    printIndentation(indentLevel + 1);
    std::cout << "- op:" << std::endl;
    printIndentation(indentLevel + 2);
    std::cout << static_cast<int>(m_op) << std::endl;

    printIndentation(indentLevel + 1);
    std::cout << "- left:" << std::endl;
    m_left->prettyPrint(indentLevel + 2);

    printIndentation(indentLevel + 1);
    std::cout << "- rigth:" << std::endl;
    m_right->prettyPrint(indentLevel + 2);
}

void UnaryExpression::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "UnaryExpression:" << std::endl;

    printIndentation(indentLevel + 1);
    std::cout << "- op:" << std::endl;
    printIndentation(indentLevel + 2);
    std::cout << static_cast<int>(m_op) << std::endl;

    printIndentation(indentLevel + 1);
    std::cout << "- right:" << std::endl;
    m_right->prettyPrint(indentLevel + 2);
}

void Identifier::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "Identifier:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- name:" << std::endl;
    printIndentation(indentLevel + 2);
    std::cout << m_name << std::endl;
}

void FunctionExpression::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "FunctionExpression:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- params:" << std::endl;
    for (const auto& param : m_params) {
        param->prettyPrint(indentLevel + 2);
    }
    printIndentation(indentLevel + 1);
    std::cout << "- body:" << std::endl;
    m_body->prettyPrint(indentLevel + 2);
}

void ReturnStatement::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "ReturnStatement:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- argument:" << std::endl;
    if (m_argument)
        m_argument->prettyPrint(indentLevel + 2);
}

void VariableDeclarator::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "VariableDeclarator:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- name:" << std::endl;
    m_name->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- init:" << std::endl;
    m_init->prettyPrint(indentLevel + 2);
}

void VariableDeclaration::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "VariableDeclaration:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- declarations:" << std::endl;
    for (const auto& declarator : m_declarators) {
        declarator->prettyPrint(indentLevel + 2);
    }
}

void CallExpression::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "CallExpression:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- name:" << std::endl;
    m_name->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- arguments:" << std::endl;
    for (const auto& param : m_arguments) {
        param->prettyPrint(indentLevel + 2);
    }
}

void PrintStatement::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "PrintStatement:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- argument:" << std::endl;
    m_argument->prettyPrint(indentLevel + 2);
}

void AssignmentExpression::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "AssignmentExpression:" << std::endl;

    printIndentation(indentLevel + 1);
    std::cout << "- op:" << std::endl;
    printIndentation(indentLevel + 2);
    std::cout << static_cast<int>(m_op) << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- left:" << std::endl;
    m_left->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- rigth:" << std::endl;
    m_right->prettyPrint(indentLevel + 2);
}

void LogicalExpression::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "LogicalExpression:" << std::endl;

    printIndentation(indentLevel + 1);
    std::cout << "- op:" << std::endl;
    printIndentation(indentLevel + 2);
    std::cout << static_cast<int>(m_op) << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- left:" << std::endl;
    m_left->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- rigth:" << std::endl;
    m_right->prettyPrint(indentLevel + 2);
}

void IfElseStatement::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "IfElseStatement:" << std::endl;

    printIndentation(indentLevel + 1);
    std::cout << "- condition:" << std::endl;
    m_condition->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- ifBranch:" << std::endl;
    m_ifBranch->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- elseBranch:" << std::endl;
    if (m_elseBranch)
        m_elseBranch->prettyPrint(indentLevel + 2);
}

void ForLoopStatement::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "ForLoopStatement:" << std::endl;

    printIndentation(indentLevel + 1);
    std::cout << "- init:" << std::endl;
    if (m_init)
        m_init->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- condition:" << std::endl;
    if (m_condition)
        m_condition->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- incremenet:" << std::endl;
    if (m_increment)
        m_increment->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- body:" << std::endl;
    m_body->prettyPrint(indentLevel + 2);
}

void ObjectProperty::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "ObjectProperty:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- name:" << std::endl;
    m_name->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- value:" << std::endl;
    m_value->prettyPrint(indentLevel + 2);
}

void ObjectExpression::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "ObjectExpression:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- properties:" << std::endl;
    for (const auto& property : m_properties) {
        property->prettyPrint(indentLevel + 2);
    }
}

void MemberExpression::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "MemberExpression:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- object:" << std::endl;
    m_object->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- property:" << std::endl;
    m_property->prettyPrint(indentLevel + 2);
}

void ArrayExpression::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "ArrayExpression:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- elements:" << std::endl;
    for (const auto& element : m_elements) {
        element->prettyPrint(indentLevel + 2);
    }
}

void ArrayMemberExpression::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "ArrayMememebrExpression:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- array:" << std::endl;
    m_array->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- index:" << std::endl;
    printIndentation(indentLevel + 2);
    std::cout << m_index << std::endl;
}

void ContinueStatement::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "ContinueExpression:" << std::endl;
}

void BreakStatement::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "BreakExpression:" << std::endl;
}

void WhileLoopStatement::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "WhileLoopStatement:" << std::endl;

    printIndentation(indentLevel + 1);
    std::cout << "- condition:" << std::endl;
    if (m_condition)
        m_condition->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- body:" << std::endl;
    m_body->prettyPrint(indentLevel + 2);
}

void DoWhileLoopStatement::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "DoWhileLoopStatement:" << std::endl;

    printIndentation(indentLevel + 1);
    std::cout << "- condition:" << std::endl;
    if (m_condition)
        m_condition->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- body:" << std::endl;
    m_body->prettyPrint(indentLevel + 2);
}

void UpdateExpression::prettyPrint(int32_t indentLevel) const
{
    printIndentation(indentLevel);
    std::cout << "UpdateExpression:" << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- argument:" << std::endl;
    m_argument->prettyPrint(indentLevel + 2);
    printIndentation(indentLevel + 1);
    std::cout << "- operator:" << std::endl;
    printIndentation(indentLevel + 2);
    std::cout << static_cast<int>(m_op) << std::endl;
    printIndentation(indentLevel + 1);
    std::cout << "- prefix:" << std::endl;
    printIndentation(indentLevel + 2);
    std::cout << m_prefix << std::endl;
}

std::optional<Value> Scope::execute(Interpreter& interpreter) const
{
    interpreter.stack().push_back({});
    for (auto& statement : m_body) {
        statement->execute(interpreter);
    }
    interpreter.stack().pop_back();

    return std::nullopt;
}

std::optional<Value> Scope::execute(Interpreter& interpreter,
    std::unordered_map<std::string, Value> environment) const
{
    interpreter.stack().push_back(environment);
    for (auto& statement : m_body) {
        statement->execute(interpreter);
    }
    interpreter.stack().pop_back();

    return std::nullopt;
}

std::optional<Value> ExpressionStatement::execute(Interpreter& interpreter) const
{
    m_expression->execute(interpreter);
    return std::nullopt;
}

std::optional<Value> Literal::execute(Interpreter&) const
{
    return m_value;
}

std::optional<Value> BinaryExpression::execute(Interpreter& interpreter) const
{
    Value left = m_left->execute(interpreter).value();
    Value right = m_right->execute(interpreter).value();
    switch (m_op) {
    case Operator::Addition:
        return left + right;
    case Operator::Subtraction:
        return left - right;
    case Operator::Multiplication:
        return left * right;
    case Operator::Division:
        return left / right;
    case Operator::Modulo:
        return left % right;
    case Operator::Equals:
        return left == right;
    case Operator::Inequals:
        return left != right;
    case Operator::GreaterThan:
        return left > right;
    case Operator::LessThan:
        return left < right;
    case Operator::GreaterThanEquals:
        return left >= right;
    case Operator::LessThanEquals:
        return left <= right;
    }

    assert(false);
}

std::optional<Value> UnaryExpression::execute(Interpreter& interpreter) const
{
    Value right = m_right->execute(interpreter).value();
    switch (m_op) {
    case Operator::Not:
        return Value(!right.toBoolean());
    case Operator::Plus:
        return Value(right.toNumber());
    case Operator::Minus:
        return Value(-right.toNumber());
    }
    return std::nullopt;
}

std::optional<Value> Identifier::execute(Interpreter& interpreter) const
{
    Value value = interpreter.getVariable(m_name);
    return value;
}

std::optional<Value> FunctionExpression::execute(Interpreter& interpreter) const
{
    std::vector<std::string> params = {};
    for (auto& param : m_params) {
        params.push_back(param->name());
    }

    auto function = interpreter.heap().allocate<Msl::Function>(m_body, params);
    return Value(function);
}

std::optional<Value> ReturnStatement::execute(Interpreter& interpreter) const
{
    Value ret;
    if (m_argument)
        ret = m_argument->execute(interpreter).value();
    interpreter.stack().pop_back();
    throw ReturnException(ret);

    return std::nullopt;
}

std::optional<Value> VariableDeclarator::execute(Interpreter& interpreter) const
{
    Value init = m_init->execute(interpreter).value();
    interpreter.declareVariable(m_name->name(), init);
    return std::nullopt;
}

std::optional<Value> VariableDeclaration::execute(Interpreter& interpreter) const
{
    for (auto& declarator : m_declarators) {
        declarator->execute(interpreter);
    }
    return std::nullopt;
}

std::optional<Value> CallExpression::execute(Interpreter& interpreter) const
{
    Value function = m_name->execute(interpreter).value();
    if (!function.isFunction()) {
        throw RuntimeException("Trying to call a non function value");
    }
    if (!function.function()->variadic()
        && function.function()->paramCount() != m_arguments.size()) {
        throw RuntimeException("Invalid number of parameters to function");
    }
    std::vector<Value> args = {};
    for (auto& argument : m_arguments) {
        args.push_back(argument->execute(interpreter).value());
    }
    Value ret = function.function()->execute(interpreter, args);
    return ret;
}

std::optional<Value> PrintStatement::execute(Interpreter& interpreter) const
{
    std::cout << m_argument->execute(interpreter).value() << std::endl;
    return std::nullopt;
}

std::optional<Value> AssignmentExpression::execute(Interpreter& interpreter) const
{
    Value value = m_right->execute(interpreter).value();

    if (m_left->isIdentifier()) {
        auto old = static_cast<Identifier*>(m_left)->execute(interpreter).value();
        std::string name = static_cast<Identifier*>(m_left)->name();
        switch (m_op) {
        case Operator::Equals:
            return interpreter.updateVariable(name, value);
        case Operator::PlusEquals:
            return interpreter.updateVariable(name, old + value);
        case Operator::MinusEquals:
            return interpreter.updateVariable(name, old - value);
        case Operator::AsteriskEquals:
            return interpreter.updateVariable(name, old * value);
        case Operator::SlashEquals:
            return interpreter.updateVariable(name, old / value);
        case Operator::ModuloEquals:
            return interpreter.updateVariable(name, old % value);
        }
    }

    if (m_left->isMemberExpression()) {
        Value object = static_cast<MemberExpression*>(m_left)->object()->execute(interpreter).value();
        std::string property = static_cast<MemberExpression*>(m_left)->property()->name();
        if (!object.isObject())
            throw RuntimeException("Assignment left expresion is not an object");
        Object* obj = object.object();
        auto old = obj->get(property);
        switch (m_op) {
        case Operator::Equals:
            return obj->set(property, value);
        case Operator::PlusEquals:
            return obj->set(property, old + value);
        case Operator::MinusEquals:
            return obj->set(property, old - value);
        case Operator::AsteriskEquals:
            return obj->set(property, old * value);
        case Operator::SlashEquals:
            return obj->set(property, old / value);
        case Operator::ModuloEquals:
            return obj->set(property, old % value);
        }
    }

    if (m_left->isArrayMemberExpression()) {
        Value array = static_cast<ArrayMemberExpression*>(m_left)->array()->execute(interpreter).value();
        Value indexValue = static_cast<ArrayMemberExpression*>(m_left)->index()->execute(interpreter).value();
        if (!array.isArray())
            throw RuntimeException("Assignment left expression is not an array");
        if (!indexValue.isNumber())
            throw RuntimeException("Can't use non number to access array element");
        Array* arr = array.array();
        size_t index = indexValue.number();
        auto old = arr->at(index);
        switch (m_op) {
        case Operator::Equals:
            return arr->at(index, value);
        case Operator::PlusEquals:
            return arr->at(index, old + value);
        case Operator::MinusEquals:
            return arr->at(index, old - value);
        case Operator::AsteriskEquals:
            return arr->at(index, old * value);
        case Operator::SlashEquals:
            return arr->at(index, old / value);
        case Operator::ModuloEquals:
            return arr->at(index, old % value);
        }
    }

    throw RuntimeException("Assignment left expression is not an identifier or object property or array");
}

std::optional<Value> LogicalExpression::execute(Interpreter& interpreter) const
{
    Value left = m_left->execute(interpreter).value();

    switch (m_op) {
    case Operator::And:
        if (!left.toBoolean())
            return left;
        break;
    case Operator::Or:
        if (left.toBoolean())
            return left;
        break;
    }

    return m_right->execute(interpreter).value();
}

std::optional<Value> IfElseStatement::execute(Interpreter& interpreter) const
{
    bool condition = m_condition->execute(interpreter).value().toBoolean();
    if (condition)
        m_ifBranch->execute(interpreter);
    else if (m_elseBranch)
        m_elseBranch->execute(interpreter);

    return std::nullopt;
}

std::optional<Value> ForLoopStatement::execute(Interpreter& interpreter) const
{
    interpreter.stack().push_back({});
    for (m_init ? m_init->execute(interpreter) : true;
         m_condition ? m_condition->execute(interpreter).value().toBoolean() : true;
         m_increment ? m_increment->execute(interpreter) : true) {
        try {
            m_body->execute(interpreter);
        } catch (ContinueException&) {
            continue;
        } catch (BreakException&) {
            break;
        }
    }
    interpreter.stack().pop_back();

    return std::nullopt;
}

std::optional<Value> ObjectProperty::execute(Interpreter&) const
{
    assert(false); // I should never ever run this
}

std::optional<Value> ObjectExpression::execute(Interpreter& interpreter) const
{
    auto object = interpreter.heap().allocate<Object>();
    for (auto& property : m_properties) {
        interpreter.heap().disableGC();
        object->set(property->name()->name(),
            property->value()->execute(interpreter).value());
        interpreter.heap().enableGC();
    }
    return object;
}

std::optional<Value> MemberExpression::execute(Interpreter& interpreter) const
{
    auto object = m_object->execute(interpreter).value();
    if (!object.isObject()) {
        return Value();
    }
    return object.object()->get(m_property->name());
}

std::optional<Value> ArrayExpression::execute(Interpreter& interpreter) const
{
    auto array = interpreter.heap().allocate<Array>();
    for (auto& element : m_elements) {
        interpreter.heap().disableGC();
        Value value = element->execute(interpreter).value();
        array->elements().push_back(value);
        interpreter.heap().enableGC();
    }
    return Value(array);
}

std::optional<Value> ArrayMemberExpression::execute(Interpreter& interpreter) const
{
    Value value = m_array->execute(interpreter).value();
    Value index = m_index->execute(interpreter).value();
    if (!value.isArray())
        throw RuntimeException("ArrayMemeberExpression on a non array value");
    if (!index.isNumber())
        throw RuntimeException("Can't use non number index on array");
    return value.array()->at(index.number());
}

std::optional<Value> ContinueStatement::execute(Interpreter&) const
{
    throw ContinueException();
}

std::optional<Value> BreakStatement::execute(Interpreter&) const
{
    throw BreakException();
}

std::optional<Value> WhileLoopStatement::execute(Interpreter& interpreter) const
{
    interpreter.stack().push_back({});
    while (m_condition ? m_condition->execute(interpreter).value().toBoolean() : true) {
        try {
            m_body->execute(interpreter);
        } catch (ContinueException&) {
            continue;
        } catch (BreakException&) {
            break;
        }
    }
    interpreter.stack().pop_back();

    return std::nullopt;
}

std::optional<Value> DoWhileLoopStatement::execute(Interpreter& interpreter) const
{
    do {
        try {
            m_body->execute(interpreter);
        } catch (ContinueException&) {
            continue;
        } catch (BreakException&) {
            break;
        }
    } while (m_condition ? m_condition->execute(interpreter).value().toBoolean() : true);

    return std::nullopt;
}

std::optional<Value> UpdateExpression::execute(Interpreter& interpreter) const
{
    Value oldVal = m_argument->execute(interpreter).value();
    if (!oldVal.isNumber())
        throw RuntimeException("Can't incremenet/decrement non numbre variables");
    Value newVal = m_op == Operation::Increment ? oldVal + Value(1) : oldVal - Value(1);

    if (m_argument->isIdentifier()) {
        interpreter.updateVariable(static_cast<Identifier*>(m_argument)->name(), newVal);
    } else if (m_argument->isMemberExpression()) {
        Value object = static_cast<MemberExpression*>(m_argument)->object()->execute(interpreter).value();
        std::string property = static_cast<MemberExpression*>(m_argument)->property()->name();
        if (!object.isObject())
            throw RuntimeException("Assignment left expresion is not an object");
        Object* obj = object.object();
        obj->set(property, newVal);
    } else if (m_argument->isArrayMemberExpression()) {
        Value array = static_cast<ArrayMemberExpression*>(m_argument)->array()->execute(interpreter).value();
        Value index = static_cast<ArrayMemberExpression*>(m_argument)->index()->execute(interpreter).value();
        if (index.isNumber())
            throw RuntimeException("Array index isn't a number");
        if (!array.isArray())
            throw RuntimeException("not an Array");
        Array* arr = array.array();
        arr->at(index.number(), newVal);
    } else {
        throw RuntimeException("Assignment left expression is not an object, array or identifier");
    }

    return m_prefix ? newVal : oldVal;
}

}
