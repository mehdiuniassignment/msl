#pragma once

#include "forward.hpp"
#include "object.hpp"

#include <list>
#include <memory>
#include <queue>

namespace Msl {

class Heap {
public:
    Heap(Interpreter& interpreter);

    template <typename T, typename... Args>
    T* allocate(Args&&... args)
    {
        if (m_gcStatus == true && m_liveObjects.size() > m_threshold) {
            collectGarbage();
        }
        T* object = new T(std::forward<Args>(args)...);
        m_liveObjects.emplace_front(object);
        return object;
    }

    bool gcStatus() const;
    void enableGC();
    void disableGC();

    void collectGarbage();
    void getRoots();
    void mark();
    void sweep();

private:
    bool m_gcStatus { true };
    size_t m_threshold { 20 };
    Interpreter& m_interpreter;
    std::queue<Object*> m_grayObjects;
    std::list<Object*> m_liveObjects;
};

};
