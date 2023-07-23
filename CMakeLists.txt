cmake_minimum_required(VERSION 3.5)

project(msl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
endif ()

set(SOURCES
    src/main.cpp
    src/token.hpp
    src/token.cpp
    src/lexer.hpp
    src/lexer.cpp
    src/error.cpp
    src/error.hpp
    src/parser.hpp
    src/parser.cpp
    src/forward.hpp
    src/value.hpp src/value.cpp
    src/object.hpp src/object.cpp
    src/function.hpp src/function.cpp
    src/array.hpp src/array.cpp
    src/heap.cpp src/heap.hpp
    src/interpreter.cpp src/interpreter.hpp
    src/ast.cpp src/ast.hpp
    src/exceptions.hpp
)

add_executable(
    msl
    ${SOURCES}
)
