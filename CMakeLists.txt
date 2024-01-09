cmake_minimum_required(VERSION 3.13.0)
project(EXPR_Interpreter LANGUAGES CXX)

add_executable(myinterpreter main.cpp)

include(CTest)
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
include_directories(include)

target_link_libraries(
    myinterpreter
    PUBLIC
    arithmetics
    expressions
    saved-expressions
    interpreter
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
