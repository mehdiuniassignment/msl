#pragma once

#include "forward.hpp"

#include <string>
#include <variant>

namespace Msl {

class Value
{
public:
    enum class Type {
        Null,
        Boolean,
        Number,
        String,
        Function,
        Object,
        Array
    };

    Value();
    Value(bool boolean);
    Value(double number);
    Value(int32_t number);
    Value(const std::string& string);
    Value(const char* string);
    Value(Function* function);
    Value(Object* object);
    Value(Array* array);

    bool isNull() const;
    bool isBoolean() const;
    bool isNumber() const;
    bool isString() const;
    bool isFunction() const;
    bool isObject() const;
    bool isArray() const;

    bool boolean() const;
    double number() const;
    std::string string() const;
    Function* function();
    Object* object();
    Array* array();
    Type type() const;

    bool toBoolean() const;
    double toNumber() const;
    std::string toString() const;

    friend std::ostream& operator<<(std::ostream& os, const Value& value);
    Value operator+(const Value& right);
    Value operator-(const Value& right);
    Value operator*(const Value& right);
    Value operator/(const Value& right);
    Value operator%(const Value& right);
    Value operator==(Value& right);
    Value operator!=(Value& right);
    Value operator>(const Value& right);
    Value operator<(const Value& right);
    Value operator>=(const Value& right);
    Value operator<=(const Value& right);

private:
    Type m_type { Type::Null };
    std::variant<bool, double, std::string, Function*, Object*, Array*> m_value;
};

}
