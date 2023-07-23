#include "value.hpp"

#include "array.hpp"
#include "object.hpp"

#include <cassert>
#include <cmath>
#include <limits>
#include <sstream>

namespace Msl {

Value::Value()
    : m_type(Type::Null)
{
}

Value::Value(bool boolean)
    : m_type(Type::Boolean)
    , m_value(boolean)
{
}

Value::Value(double number)
    : m_type(Type::Number)
    , m_value(number)
{
}

Value::Value(int32_t number)
    : m_type(Type::Number)
    , m_value(static_cast<double>(number))
{
}

Value::Value(const std::string& string)
    : m_type(Type::String)
    , m_value(string)
{
}

Value::Value(const char* string)
    : m_type(Type::String)
    , m_value(string)
{
}

Value::Value(Function* function)
    : m_type(Type::Function)
    , m_value(function)
{
}

Value::Value(Object* object)
    : m_type(Type::Object)
    , m_value(object)
{
}

Value::Value(Array* array)
    : m_type(Type::Array)
    , m_value(array)
{
}

bool Value::isNull() const
{
    return m_type == Type::Null;
}

bool Value::isBoolean() const
{
    return m_type == Type::Boolean;
}

bool Value::isNumber() const
{
    return m_type == Type::Number;
}

bool Value::isString() const
{
    return m_type == Type::String;
}

bool Value::isFunction() const
{
    return m_type == Type::Function;
}

bool Value::isObject() const
{
    return m_type == Type::Object;
}

bool Value::isArray() const
{
    return m_type == Type::Array;
}

bool Value::boolean() const
{
    assert(isBoolean());

    return std::get<bool>(m_value);
}

double Value::number() const
{
    assert(isNumber());

    return std::get<double>(m_value);
}

std::string Value::string() const
{
    assert(isString());

    return std::get<std::string>(m_value);
}

Function* Value::function()
{
    assert(isFunction());
    return std::get<Function*>(m_value);
}

Object* Value::object()
{
    assert(isObject());
    return std::get<Object*>(m_value);
}

Array* Value::array()
{
    assert(isArray());
    return std::get<Array*>(m_value);
}

Value::Type Value::type() const
{
    return m_type;
}

bool Value::toBoolean() const
{
    switch (m_type) {
    case Type::Null:
        return false;
    case Type::Boolean:
        return std::get<bool>(m_value);
    case Type::Number:
        return std::get<double>(m_value) ? true : false;
    case Type::String:
        return std::get<std::string>(m_value).size() ? true : false;
    case Type::Object:
        return std::get<Object*>(m_value)->isEmpty();
    case Type::Function:
        return true;
    case Type::Array:
        return std::get<Array*>(m_value)->size() ? true : false;
    }

    assert(false);
}

double Value::toNumber() const
{
    switch (m_type) {
    case Type::Null:
        return 0;
    case Type::Boolean:
        return std::get<bool>(m_value) ? 1 : 0;
    case Type::Number:
        return std::get<double>(m_value);
    case Type::String:
        double ret;
        try {
            ret = std::stod(std::get<std::string>(m_value));
        } catch (...) {
            ret = std::numeric_limits<double>::quiet_NaN();
        }
        return ret;
    case Type::Function:
        return std::numeric_limits<double>::quiet_NaN();
    case Type::Object:
        return std::numeric_limits<double>::quiet_NaN();
    case Type::Array:
        return std::numeric_limits<double>::quiet_NaN();
    }

    assert(false);
}

std::string Value::toString() const
{
    switch (m_type) {
    case Type::Null:
        return "null";
    case Type::Boolean:
        return std::get<bool>(m_value) ? "true" : "false";
    case Type::Number:
        return std::to_string(std::get<double>(m_value));
    case Type::String:
        return std::get<std::string>(m_value);
    case Type::Function:
        return "Function";
    case Type::Object: {
        std::ostringstream ss;
        ss << '{';
        const auto& m = std::get<Object*>(m_value)->properties();
        for (auto it = m.begin(); it != m.end(); it++) {
            ss << it->first;
            ss << ": ";
            ss << it->second;
            if (std::next(it) != m.end()) {
                ss << ", ";
            }
        }
        ss << '}';
        return ss.str();
    }
    case Type::Array: {
        std::ostringstream ss;
        ss << '[';
        const auto& array = std::get<Array*>(m_value);
        for (size_t i = 0; i < array->size(); i++) {
            ss << array->at(i);
            if (i != array->size() - 1) {
                ss << ", ";
            }
        }
        ss << ']';
        return ss.str();
    }
    }

    assert(false);
}

std::ostream& operator<<(std::ostream& os, const Value& value)
{
    os << value.toString();
    return os;
}

Value Value::operator+(const Value& right)
{
    if (isString() || right.isString()) {
        return Value(toString() + right.toString());
    }
    return Value(toNumber() + right.toNumber());
}

Value Value::operator-(const Value& right)
{
    return Value(toNumber() - right.toNumber());
}

Value Value::operator*(const Value& right)
{
    return Value(toNumber() * right.toNumber());
}

Value Value::operator/(const Value& right)
{
    return Value(toNumber() / right.toNumber());
}

Value Value::operator%(const Value& right)
{
    return Value(std::fmod(toNumber(), right.toNumber()));
}

Value Value::operator==(Value& right)
{
    if (isString() && right.isString()) {
        return Value(toString() != right.toString());
    }
    if (isObject() && right.isObject()) {
        return Value(object() == right.object());
    }
    return Value(toNumber() == right.toNumber());
}

Value Value::operator!=(Value& right)
{
    if (isString() && right.isString()) {
        return Value(toString() == right.toString());
    }
    if (isObject() && right.isObject()) {
        return Value(object() != right.object());
    }
    return Value(toNumber() != right.toNumber());
}

Value Value::operator>(const Value& right)
{
    if (isString() && right.isString()) {
        return Value(toString() > right.toString());
    }
    return Value(toNumber() > right.toNumber());
}

Value Value::operator<(const Value& right)
{
    if (isString() && right.isString()) {
        return Value(toString() < right.toString());
    }
    return Value(toNumber() < right.toNumber());
}

Value Value::operator>=(const Value& right)
{
    if (isString() && right.isString()) {
        return Value(toString() >= right.toString());
    }
    return Value(toNumber() >= right.toNumber());
}

Value Value::operator<=(const Value& right)
{
    if (isString() && right.isString()) {
        return Value(toString() <= right.toString());
    }
    return Value(toNumber() <= right.toNumber());
}

}
