Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1
)

FetchContent_MakeAvailable(Catch2)

cmake_minimum_required(VERSION 3.13.0)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

add_executable(arithmeticsTests)
target_sources(
    arithmeticsTests
    PUBLIC
    arithmeticsTests.cpp
)

target_link_libraries(
    arithmeticsTests
    PUBLIC
    Catch2::Catch2WithMain
    arithmetics
)

add_test(NAME ArithmeticsTests COMMAND arithmeticsTests)

add_executable(expressionsTests)
target_sources(
    expressionsTests
    PUBLIC
    expressionsTests.cpp
)

target_link_libraries(
    expressionsTests
    PUBLIC
    Catch2::Catch2WithMain
    expressions
    saved-expressions
)

add_test(NAME ExpressionsTests COMMAND expressionsTests)

add_executable(saved-expressionsTests)

target_sources(
    saved-expressionsTests
    PUBLIC
    saved-expressionsTests.cpp
)

target_link_libraries(
    saved-expressionsTests
    PUBLIC
    Catch2::Catch2WithMain
    saved-expressions
    expressions
    interpreter
)

add_test(NAME SavedExpressionsTests COMMAND saved-expressionsTests)

add_executable(interpreterTests)

target_sources(
    interpreterTests
    PUBLIC
    interpreterTests.cpp
)

target_link_libraries(
    interpreterTests
    PUBLIC
    Catch2::Catch2WithMain
    interpreter
    saved-expressions
    expressions
    arithmetics
)

add_test(NAME InterpreterTests COMMAND interpreterTests)