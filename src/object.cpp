#include "object.hpp"

#include <iostream>
#include <stdexcept>

namespace Msl {

Object::Object()
    : m_marked(false)
{
}

Object::~Object()
{
}

Value Object::get(const std::string& key) const
{
    Value ret;
    try {
        ret = m_properties.at(key);
    } catch (const std::out_of_range& e) {
        ret = Value();
    }
    return ret;
}

Value Object::set(const std::string& key, Value value)
{
    m_properties.insert_or_assign(key, value);
    return value;
}

const std::unordered_map<std::string, Value>& Object::properties() const
{
    return m_properties;
}

bool Object::isEmpty() const
{
    return m_properties.size() ? true : false;
}

bool Object::marked() const
{
    return m_marked;
}

void Object::marked(bool marked)
{
    m_marked = marked;
}

}
