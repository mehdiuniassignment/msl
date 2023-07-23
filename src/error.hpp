#pragma once

#include <string>

namespace Msl {

bool hadError();
void hadError(bool value);
void error(size_t line, size_t column, const std::string& message);

}
