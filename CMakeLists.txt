cmake_minimum_required(VERSION 3.13.0)
project(EXPR_Interpreter LANGUAGES CXX)

add_executable(myinterpreter main.cpp)

add_subdirectory(src)
include_directories(include)

target_link_libraries(
    myinterpreter
    PUBLIC
    arithmetics
    expressions
    saved-expressions
    interpreter
)