cmake_minimum_required(VERSION 3.1.0)

project(mmlab01)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g3 -fsanitize=address,undefined -fno-omit-frame-pointer")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(INC
    inc/Monomial.h
)

set(TSRC
    inc/Node.hxx
)

set(SRC
    src/main.cc

    src/Monomial.cc
    src/Polynomial.cc
)

add_executable(mmlab01
    ${INC}
    ${TSRC}
    ${SRC}
)

target_include_directories(mmlab01
    PUBLIC
        "inc"
)

