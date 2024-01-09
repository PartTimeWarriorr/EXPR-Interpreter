add_library(arithmetics)

target_sources(
    arithmetics
    PUBLIC
    ./arithmetics.cpp
)

add_library(expressions)

target_sources(
    expressions
    PUBLIC
    ./expressions.cpp
    ./saved-expressions.cpp
    ./interpreter.cpp
    ./arithmetics.cpp
)

add_library(saved-expressions)


target_sources(
    saved-expressions
    PUBLIC
    ./saved-expressions.cpp
    ./interpreter.cpp
)

add_library(interpreter)

target_sources(
    interpreter
    PUBLIC
    ./interpreter.cpp
)