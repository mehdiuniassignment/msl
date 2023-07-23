#pragma once

#include "value.hpp"

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace Msl {

// TODO: Add a Location struct to the AST Nodes so you can report the line and column
// where the error happened.

class Ast {
public:
    virtual std::optional<Value> execute(Interpreter& interpreter) const = 0;
    virtual void prettyPrint(int32_t indentLevel) const = 0;
    virtual bool isIdentifier() const;
    virtual bool isMemberExpression() const;
    virtual bool isArrayMemberExpression() const;

protected:
    Ast();
    virtual ~Ast();
};

class Statement : public virtual Ast {
public:
};

class Expression : public Ast {
public:
};

class Scope : public virtual Ast {
public:
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    std::optional<Value> execute(Interpreter& interpreter,
        std::unordered_map<std::string, Value> environment) const;
    virtual void prettyPrint(int32_t indentLevel) const override;
    void append(Statement* statement);
    virtual ~Scope();

protected:
    explicit Scope(std::vector<Statement*> body);

private:
    std::vector<Statement*> m_body;
};

class Program final : public Scope {
public:
    explicit Program(std::vector<Statement*> body);
    virtual void prettyPrint(int32_t indentLevel) const override;
};

class BlockStatement final : public Statement, public Scope {
public:
    explicit BlockStatement(std::vector<Statement*> body);
    virtual void prettyPrint(int32_t indentLevel) const override;
};

class ExpressionStatement final : public Statement {
public:
    explicit ExpressionStatement(Expression* expression);
    ~ExpressionStatement();
    std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Expression* m_expression;
};

class Literal final : public Expression {
public:
    explicit Literal(Value value);
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Value m_value;
};

class BinaryExpression final : public Expression {
public:
    enum class Operator {
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Modulo,
        Equals,
        Inequals,
        GreaterThan,
        LessThan,
        GreaterThanEquals,
        LessThanEquals
    };

    explicit BinaryExpression(Operator op, Expression* left, Expression* right);
    ~BinaryExpression();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Operator m_op;
    Expression* m_left;
    Expression* m_right;
};

class UnaryExpression final : public Expression {
public:
    enum class Operator {
        Not,
        Minus,
        Plus
    };

    explicit UnaryExpression(Operator op, Expression* right);
    ~UnaryExpression();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Operator m_op;
    Expression* m_right;
};

class Identifier final : public Expression {
public:
    explicit Identifier(const std::string& name);
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;
    const std::string& name() const;
    virtual bool isIdentifier() const override;

private:
    std::string m_name;
};

class FunctionExpression final : public Expression {
public:
    explicit FunctionExpression(std::vector<Identifier*> m_params,
        BlockStatement* m_body);
    ~FunctionExpression();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;
    const std::vector<Identifier*>& params() const;

private:
    std::vector<Identifier*> m_params;
    BlockStatement* m_body;
};

class ReturnStatement final : public Statement {
public:
    explicit ReturnStatement(Expression* argument);
    ~ReturnStatement();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Expression* m_argument;
};

class VariableDeclarator final : public Ast {
public:
    explicit VariableDeclarator(Identifier* name, Expression* init = new Literal(Value()));
    ~VariableDeclarator();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Identifier* m_name;
    Expression* m_init;
};

class VariableDeclaration final : public Statement {
public:
    explicit VariableDeclaration(std::vector<VariableDeclarator*> declarators);
    ~VariableDeclaration();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    std::vector<VariableDeclarator*> m_declarators;
};

class CallExpression final : public Expression {
public:
    explicit CallExpression(Expression* name, std::vector<Expression*> arguments);
    ~CallExpression();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Expression* m_name;
    std::vector<Expression*> m_arguments;
};

class PrintStatement final : public Statement {
public:
    explicit PrintStatement(Expression* argument);
    ~PrintStatement();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Expression* m_argument;
};

class AssignmentExpression final : public Expression {
public:
    enum class Operator {
        Equals,
        PlusEquals,
        MinusEquals,
        AsteriskEquals,
        SlashEquals,
        ModuloEquals
    };
    explicit AssignmentExpression(Operator op, Expression* left, Expression* right);
    ~AssignmentExpression();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Operator m_op;
    Expression* m_left;
    Expression* m_right;
};

class LogicalExpression final : public Expression {
public:
    enum class Operator {
        And,
        Or
    };

    explicit LogicalExpression(Operator op, Expression* left, Expression* right);
    ~LogicalExpression();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Operator m_op;
    Expression* m_left;
    Expression* m_right;
};

class IfElseStatement final : public Statement {
public:
    explicit IfElseStatement(Expression* condition, Statement* ifBranch,
        Statement* elseBranch);
    IfElseStatement(Expression* condition, Statement* ifBranch);
    ~IfElseStatement();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Expression* m_condition;
    Statement* m_ifBranch;
    Statement* m_elseBranch { nullptr };
};

class ForLoopStatement final : public Statement {
public:
    explicit ForLoopStatement(Statement* init, Expression* condition,
        Expression* increment, Statement* body);
    ~ForLoopStatement();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Statement* m_init;
    Expression* m_condition;
    Expression* m_increment;
    Statement* m_body;
};

class WhileLoopStatement final : public Statement {
public:
    explicit WhileLoopStatement(Expression* condition, Statement* body);
    ~WhileLoopStatement();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Expression* m_condition;
    Statement* m_body;
};

class DoWhileLoopStatement final : public Statement {
public:
    explicit DoWhileLoopStatement(Expression* condition, Statement* body);
    ~DoWhileLoopStatement();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Expression* m_condition;
    Statement* m_body;
};

class ObjectProperty final : public Ast {
public:
    explicit ObjectProperty(Identifier* name, Expression* value);
    ~ObjectProperty();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;
    Identifier* name();
    Expression* value();

private:
    Identifier* m_name;
    Expression* m_value;
};

class ObjectExpression final : public Expression {
public:
    explicit ObjectExpression(std::vector<ObjectProperty*> properties);
    ~ObjectExpression();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;
    const std::vector<ObjectProperty*>& properties() const;

private:
    std::vector<ObjectProperty*> m_properties;
};

class MemberExpression final : public Expression {
public:
    explicit MemberExpression(Expression* object, Identifier* property);
    ~MemberExpression();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;
    virtual bool isMemberExpression() const override;
    Expression* object();
    Identifier* property();

private:
    Expression* m_object;
    Identifier* m_property;
};

class ArrayExpression final : public Expression {
public:
    explicit ArrayExpression(std::vector<Expression*> elements);
    ~ArrayExpression();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    std::vector<Expression*> m_elements;
};

class ArrayMemberExpression final : public Expression {
public:
    explicit ArrayMemberExpression(Expression* array, Expression* index);
    ~ArrayMemberExpression();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;
    virtual bool isArrayMemberExpression() const override;
    Expression* array() const;
    Expression* index() const;

private:
    Expression* m_array;
    Expression* m_index;
};

class ContinueStatement final : public Statement {
public:
    explicit ContinueStatement();
    ~ContinueStatement();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;
};

class BreakStatement final : public Statement {
public:
    explicit BreakStatement();
    ~BreakStatement();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;
};

class UpdateExpression final : public Expression {
public:
    enum class Operation {
        Increment,
        Decrement
    };
    explicit UpdateExpression(Operation op, bool prefix, Expression* argument);
    ~UpdateExpression();
    virtual std::optional<Value> execute(Interpreter& interpreter) const override;
    virtual void prettyPrint(int32_t indentLevel) const override;

private:
    Operation m_op;
    bool m_prefix;
    Expression* m_argument;
};
}
