#pragma once

#include "value.hpp"

#include <unordered_map>

namespace Msl {

class Object
{
public:
    Object();
    virtual ~Object();
    Value get(const std::string& key) const;
    Value set(const std::string& key, Value value);
    const std::unordered_map<std::string, Value>& properties() const;
    bool isEmpty() const;
    bool marked() const;
    void marked(bool marked);

private:
    bool m_marked { false };
    std::unordered_map<std::string, Msl::Value> m_properties;
};

}
