#include "array.hpp"
#include "exceptions.hpp"

#include <iostream>

namespace Msl {

Array::Array()
{
}

Value Array::at(size_t i)
{
    if (i > m_elements.size()) {
        throw RuntimeException("Out of range index");
    }

    return m_elements[i];
}

Value Array::at(size_t i, Value value)
{
    if (i > m_elements.size()) {
        throw RuntimeException("Out of range index");
    }
    m_elements[i] = value;
    return value;
}

size_t Array::size() const
{
    return m_elements.size();
}

std::vector<Value>& Array::elements()
{
    return m_elements;
}
}
