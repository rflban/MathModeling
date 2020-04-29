cmake_minimum_required(VERSION 3.13)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g3 -fsanitize=address,undefined -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(SRC
    src/main.cc

    src/Picard.cc
    src/MTPicard.cc
    src/FFTPicard.cc
)

add_executable(mmlab01
    ${SRC}
)

target_include_directories(mmlab01
    "PUBLIC"
        inc
        3rd_party
)

target_link_libraries(mmlab01
    pthread
    mpfr
)

