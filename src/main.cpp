#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "error.hpp"
#include "exceptions.hpp"
#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"

namespace Msl {

static void run(const std::string& code)
{
    Lexer lexer(code);
    std::vector<Token> tokens = lexer.lexTokens();

    if (hadError()) {
        return;
    }

    Parser parser(tokens);
    try {
        Interpreter interpreter;
        auto program = parser.parse();
        // program->prettyPrint(0);
        interpreter.run(program);
        delete program;
    } catch (ParsingException& e) {
        error(e.token.line(), e.token.column(), e.message);
    } catch (RuntimeException& e) {
        std::cout << "RuntimeException: " << e.message << std::endl;
    }
}

static int runREPL()
{
    std::string line;

    for (;;) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (std::cin.fail())
            return 1;
        run(line);
        hadError(false);
    }

    return 0;
}

static int runFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open file " << path << std::endl;
        return 74;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    std::string code = ss.str();
    run(code);

    if (hadError())
        return 65;

    return 0;
}

}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        return Msl::runREPL();
    } else if (argc == 2) {
        return Msl::runFile(argv[1]);
    } else {
        std::cerr << "Usage: msl <script>" << std::endl;
        return 64;
    }

    return EXIT_SUCCESS;
}
