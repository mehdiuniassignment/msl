#include <iostream>
#include <memory>

#include "ast.hpp"
#include "exceptions.hpp"
#include "interpreter.hpp"

int main()
{
    Msl::Interpreter interpreter;

    auto program = new Msl::Program(
        { new Msl::VariableDeclaration(
              { new Msl::VariableDeclarator(
                  new Msl::Identifier("foo"),
                  new Msl::FunctionExpression(
                      { new Msl::Identifier("a") },
                      new Msl::BlockStatement(
                          { new Msl::ReturnStatement(
                              new Msl::BinaryExpression(
                                  Msl::BinaryExpression::Operator::GreaterThan,
                                  new Msl::Identifier("a"),
                                  new Msl::Literal(Msl::Value("my name is")))) }))) }),
            new Msl::VariableDeclaration(
                { new Msl::VariableDeclarator(
                    new Msl::Identifier("foooo"),
                    new Msl::FunctionExpression(
                        { new Msl::Identifier("a") },
                        new Msl::BlockStatement(
                            { new Msl::VariableDeclaration(
                                  { new Msl::VariableDeclarator(
                                      new Msl::Identifier("foo"),
                                      new Msl::FunctionExpression(
                                          { new Msl::Identifier("a") },
                                          new Msl::BlockStatement(
                                              { new Msl::ReturnStatement(
                                                  new Msl::BinaryExpression(
                                                      Msl::BinaryExpression::Operator::GreaterThan,
                                                      new Msl::Identifier("a"),
                                                      new Msl::Literal(Msl::Value("my name is")))) }))) }),
                                new Msl::ReturnStatement(
                                    new Msl::BinaryExpression(
                                        Msl::BinaryExpression::Operator::GreaterThan,
                                        new Msl::Identifier("a"),
                                        new Msl::Literal(Msl::Value("my name i")))) }))) }),

            new Msl::PrintStatement(
                new Msl::CallExpression(
                    new Msl::Identifier("foooo"),
                    { new Msl::Literal(Msl::Value("my name is")) })),
            new Msl::VariableDeclaration(
                { new Msl::VariableDeclarator(
                    new Msl::Identifier("nice_day")) }),
            new Msl::PrintStatement(
                new Msl::Identifier("nice_day")),
            new Msl::ExpressionStatement(
                new Msl::AssignmentExpression(
                    Msl::AssignmentExpression::Operator::Equals,
                    new Msl::Identifier("nice_day"),
                    new Msl::Literal(Msl::Value(true)))),
            new Msl::PrintStatement(
                new Msl::Identifier("nice_day")),
            new Msl::PrintStatement(
                new Msl::LogicalExpression(
                    Msl::LogicalExpression::Operator::And,
                    new Msl::Identifier("nice_day"),
                    new Msl::Literal(Msl::Value("")))),
            new Msl::IfElseStatement(
                new Msl::BinaryExpression(
                    Msl::BinaryExpression::Operator::GreaterThan,
                    new Msl::Literal(Msl::Value(3)),
                    new Msl::Literal(Msl::Value(5))),
                new Msl::PrintStatement(
                    new Msl::Literal(Msl::Value("yesssssss"))),
                new Msl::BlockStatement(
                    { new Msl::PrintStatement(new Msl::Literal(Msl::Value("yeaaaaaah"))),
                        new Msl::PrintStatement(new Msl::Literal(Msl::Value("we kinda made it"))) })),
            new Msl::VariableDeclaration(
                { new Msl::VariableDeclarator(
                    new Msl::Identifier("i"),
                    new Msl::Literal(Msl::Value(0))) }),
            new Msl::WhileStatement(
                new Msl::BinaryExpression(
                    Msl::BinaryExpression::Operator::LessThan,
                    new Msl::Identifier("i"),
                    new Msl::Literal(Msl::Value(5))),
                new Msl::BlockStatement(
                    { new Msl::PrintStatement(new Msl::Identifier("i")),
                        new Msl::ExpressionStatement(
                            new Msl::AssignmentExpression(
                                Msl::AssignmentExpression::Operator::PlusEquals,
                                new Msl::Identifier("i"),
                                new Msl::Literal(Msl::Value(1)))) })),
            new Msl::VariableDeclaration(
                { new Msl::VariableDeclarator(
                    new Msl::Identifier("arr"),
                    new Msl::ArrayExpression(
                        { new Msl::Literal(Msl::Value(34)),
                            new Msl::Literal(Msl::Value("nice")),
                            new Msl::Literal(Msl::Value(false)),
                            new Msl::Literal(Msl::Value()) })) }),
            new Msl::VariableDeclaration(
                { new Msl::VariableDeclarator(
                    new Msl::Identifier("obj"),
                    new Msl::ObjectExpression(
                        { new Msl::ObjectProperty(
                            new Msl::Identifier("foo"),
                            new Msl::Literal(34)) })) }),
            new Msl::PrintStatement(
                new Msl::Identifier("obj")),
            new Msl::PrintStatement(
                new Msl::MemberExpression(
                    new Msl::Identifier("obj"),
                    new Msl::Identifier("foo"))),
            new Msl::ExpressionStatement(
                new Msl::AssignmentExpression(
                    Msl::AssignmentExpression::Operator::PlusEquals,
                    new Msl::MemberExpression(
                        new Msl::Identifier("obj"),
                        new Msl::Identifier("foo")),
                    new Msl::Literal(Msl::Value(2)))),
            new Msl::PrintStatement(
                new Msl::MemberExpression(
                    new Msl::Identifier("obj"),
                    new Msl::Identifier("foo"))),

            new Msl::PrintStatement(
                new Msl::Identifier("arr")),

            new Msl::PrintStatement(
                new Msl::ArrayMemberExpression(
                    new Msl::Identifier("arr"),
                    1)),
            new Msl::ExpressionStatement(
                new Msl::AssignmentExpression(
                    Msl::AssignmentExpression::Operator::Equals,
                    new Msl::Identifier("i"),
                    new Msl::Literal(Msl::Value(0)))),
            new Msl::BreakStatement(),
            new Msl::WhileStatement(
                new Msl::BinaryExpression(
                    Msl::BinaryExpression::Operator::LessThan,
                    new Msl::Identifier("i"),
                    new Msl::Literal(Msl::Value(10))),
                new Msl::BlockStatement(

                    { new Msl::IfElseStatement(
                          new Msl::BinaryExpression(
                              Msl::BinaryExpression::Operator::Equals,
                              new Msl::Identifier("i"),
                              new Msl::Literal(Msl::Value(5))),
                          new Msl::BreakStatement()),
                        new Msl::PrintStatement(new Msl::Identifier("i")),
                        new Msl::ExpressionStatement(
                            new Msl::AssignmentExpression(
                                Msl::AssignmentExpression::Operator::PlusEquals,
                                new Msl::Identifier("i"),
                                new Msl::Literal(Msl::Value(1)))) })) });

    program->prettyPrint(0);

    try {
        interpreter.run(program);
    } catch (Msl::RuntimeException& e) {
        std::cout << "Runtime Error: " << e.message << std::endl;
    } catch (Msl::BreakException& e) {
        std::cout << "Runtime Error: break outside loop." << std::endl;
    } catch (Msl::ContinueException& e) {
        std::cout << "Runtime Error: continue outside loop." << std::endl;
    }

    delete program;

    return 0;
}
