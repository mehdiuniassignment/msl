#include "heap.hpp"
#include "array.hpp"
#include "function.hpp"
#include "interpreter.hpp"

namespace Msl {

Heap::Heap(Interpreter& interpreter)
    : m_interpreter(interpreter)
{
}

bool Heap::gcStatus() const
{
    return m_gcStatus;
}
void Heap::enableGC()
{
    m_gcStatus = true;
}
void Heap::disableGC()
{
    m_gcStatus = false;
}

void Heap::collectGarbage()
{
    getRoots();
    mark();
    sweep();
}

void Heap::getRoots()
{
    for (auto& environment : m_interpreter.stack()) {
        for (auto& variable : environment) {
            Value value = variable.second;
            if (value.isObject()) {
                m_grayObjects.push(value.object());
            } else if (value.isFunction()) {
                m_grayObjects.push(value.function());
            } else if (value.isArray()) {
                m_grayObjects.push(value.array());
            }
        }
    }
}

void Heap::mark()
{
    while (!m_grayObjects.empty()) {
        Object* obj = m_grayObjects.front();
        m_grayObjects.pop();

        if (!obj->marked()) {
            obj->marked(true);
            for (const auto& prop : obj->properties()) {
                auto val = prop.second;
                if (val.isObject()) {
                    m_grayObjects.push(val.object());
                } else if (val.isArray()) {
                    m_grayObjects.push(val.array());
                } else if (val.isFunction()) {
                    m_grayObjects.push(val.function());
                }
            }
            if (auto array = dynamic_cast<Array*>(obj)) {
                for (auto& element : array->elements()) {
                    if (element.isObject()) {
                        m_grayObjects.push(element.object());
                    } else if (element.isArray()) {
                        m_grayObjects.push(element.array());
                    } else if (element.isFunction()) {
                        m_grayObjects.push(element.function());
                    }
                }
            }
        }
    }
}

void Heap::sweep()
{
    for (auto it = m_liveObjects.begin(); it != m_liveObjects.end();) {
        auto obj = *it;
        if (!obj->marked()) {
            delete obj;
            it = m_liveObjects.erase(it);
        } else {
            obj->marked(false);
            ++it;
        }
    }
}

}
