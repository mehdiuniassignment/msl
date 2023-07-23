#pragma once

#include "object.hpp"
#include "value.hpp"

#include <vector>

namespace Msl {

class Array final : public Object {
public:
    Array();
    Value at(size_t i);
    Value at(size_t i, Value value);
    size_t size() const;
    std::vector<Value>& elements();

private:
    std::vector<Value> m_elements;
};

}
