#include <iostream>

#include "error.hpp"

namespace Msl {

static bool m_hadError = false;

bool hadError()
{
    return m_hadError;
}

void hadError(bool value)
{
    m_hadError = value;
}

void error(size_t line, size_t column, const std::string& message)
{
    std::cerr << "Error " << line << ":" << column
              << " - " << message << std::endl;
    m_hadError = true;
}

}
